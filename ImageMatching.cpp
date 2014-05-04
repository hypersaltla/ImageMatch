/*
 * ImageMatching.cpp defines the main algorithms for the image searching project
 */

#include <vector>
#include <queue>
#include "Image.h"

#define H_N 15
#define BUCKET 24
#define S_N 100
#define V_N 100
#define THRESHOLD 0.07
#define BLOCK_SIZE 32

void print_arr(int *arr, int size)
{
    for (int i = 0; i < size; i++) {
       // printf("%d ", array[i]);
		TRACE("%d ", arr[i]);
    }
    TRACE("\n");
}
void print_arr_d(double *arr, int size)
{
    for (int i = 0; i < size; i++) {
       // printf("%d ", array[i]);
		TRACE("%lf ", arr[i]);
    }
    TRACE("\n");
}
//get the histogram of Hue from the MyImage class, return an array, exclusive
int *getHistogram_H(const MyImage &img, int start_r, int start_c, int end_r, int end_c)
{
	//omit error checking...
    int *histogram = new int[H_N];
    //memset(histogram, 0, H_N);
    for (int i = 0; i < H_N; i++) {
        histogram[i] = 0;
    }

    int *Hbuf = img.getHbuf();
	int *Sbuf = img.getSbuf();
	int *Vbuf = img.getVbuf();
    //print_arr(histogram, 360);
    //printf("%d\n", total);
	unsigned char *alpha_frame = img.getAlpha();
	int non_sense = 0;
    for(int i = start_r; i < end_r; i++) {
		for(int j = start_c; j < end_c; j++) {
			//printf("%d\n", Hbuf[i]);
			int index = i * img.getWidth() + j;
			if(alpha_frame && !alpha_frame[index]) {
				//TRACE("%d\n", Hbuf[i]);
				continue;
			}
			//TRACE("%d\n", Hbuf[i]);
			//TRACE("h: %d, s: %d, v: %d\n", Hbuf[index], Sbuf[index], Vbuf[index]);
			
			if(Hbuf[index] >= 0)
				histogram[Hbuf[index] / BUCKET]++;
		}
    }

    return histogram;
}

//retain the majority color in the histogram with "percent" of total pixels
void retainMajority(int *histogram, double percent, const int size)
{
	int total = 0;
    int tmp[H_N][2];
    for(int i = 0; i < size; i++) {
        tmp[i][0] = histogram[i];
        tmp[i][1] = i;
		total += tmp[i][0];
    }
    //sort
    for(int i = 1; i < size; i++) {
        int cnt = tmp[i][0];
        int index = tmp[i][1];
        int j = i - 1;
        for(; j >= 0 && tmp[j][0] > cnt; j--) {
            tmp[j + 1][0] = tmp[j][0];
            tmp[j + 1][1] = tmp[j][1];
        }
        tmp[j + 1][0] = cnt;
        tmp[j + 1][1] = index;
    }
    //if(arr_sum == total) TRACE("%d\n", arr_sum);
    int bound = total * percent;
    int sum = 0, pix_i = size - 1;
    for(; pix_i >= 0; pix_i--) {
        sum += tmp[pix_i][0];
        if(sum >= bound) break;
    }
	//TRACE("%d\n", pix_i);
	//TRACE("%d\n", tmp[pix_i][0]);
    for(int i = 0; i < pix_i; i++) {
        histogram[tmp[i][1]] = 0;
    }
}

//normalize the histogram, get the new histogram ranging from 0 - 1
double *normalize(int *histogram, int size)
{
    double *norm_his = new double[size];
	int total = 0;
	for(int i = 0; i < size; i++) {
		total += histogram[i];
	}
    for(int i = 0; i < size; i++) {
        norm_his[i] = histogram[i] / (double)total;
    }
    return norm_his;
}

//filter the histogram of destination picture by using histogram of logo picture
void filter(int *his_logo, int *his_pic, int size)
{
    for(int i = 0; i < size; i++) {
        if(!his_logo[i]) {
            his_pic[i] = 0;
        }
    }
}

//calculate the difference between two normalized histogram, can use sum of squres or sum of absolute values
double differ(double *his_logo, double *his_pic, int size)
{
    double rss = .0;
    for(int i = 0; i < size; i++) {
        double diff = his_logo[i] - his_pic[i];
        rss += diff * diff;
    }
    return rss;
}

typedef struct Box
{
	int row, col, len;
	double diff;
};

class CompareBox
{
public:
	bool operator() (Box &a, Box &b)
	{
		return a.diff < b.diff;
	}
};

//the basic version of main algorithm
bool compareImage_basic(const MyImage &img_logo, const MyImage &img_pic)
{
	int *his_logo = getHistogram_H(img_logo, 0, 0, img_logo.getHeight(), img_logo.getWidth());
	int *his_pic = getHistogram_H(img_pic, 0, 0, img_pic.getHeight(), img_pic.getWidth());
	print_arr(his_logo, H_N);
    print_arr(his_pic, H_N);
    int total_pixel = img_logo.getWidth() * img_logo.getHeight();
	retainMajority(his_logo, 0.9, H_N);
	TRACE("After retaining majority\n");
	print_arr(his_logo, H_N);
	filter(his_logo, his_pic, H_N);
	TRACE("After filtering\n");
	print_arr(his_pic, H_N);
	double *norm_his_logo = normalize(his_logo, H_N);
	double *norm_his_pic = normalize(his_pic, H_N);
	double diff = differ(norm_his_logo, norm_his_pic, H_N);
    TRACE("ecu diff: %lf\n", diff);
    delete his_logo;
    delete his_pic;
    delete norm_his_logo;
    delete norm_his_pic;

	return diff <= THRESHOLD;
}

bool compareImage_v2(const MyImage &img_logo, MyImage &img_pic)
{
	int row_n = img_pic.getHeight() / BLOCK_SIZE;
	int col_n = img_pic.getWidth() / BLOCK_SIZE;
	
	int *his_logo = getHistogram_H(img_logo, 0, 0, img_logo.getHeight(), img_logo.getWidth());
	int *his_pic = getHistogram_H(img_pic, 0, 0, img_pic.getHeight(), img_pic.getWidth());
	print_arr(his_logo, H_N);
	print_arr(his_pic, H_N);
	delete his_pic;
	double *norm_logo = normalize(his_logo, H_N);
	int **block_histos = new int*[row_n * col_n];

	int total_blc = 0;
	for(int i = 0; i < img_pic.getHeight(); i += BLOCK_SIZE) {
		for(int j = 0; j < img_pic.getWidth(); j += BLOCK_SIZE) {
			//TRACE("i: %d, j: %d\n", i, j);
			block_histos[total_blc++] = getHistogram_H(img_pic, i, j, i + BLOCK_SIZE, j + BLOCK_SIZE);
		//	print_arr(block_histos[total_blc - 1], H_N);
		}
	}

	int window_size = min(row_n, col_n);
	//double min_diff = 1000.0;
	//int min_x = -1, min_y = -1, min_size = -1;
	std::priority_queue<Box, std::vector<Box>, CompareBox> best_boxes;
	int max_heap_size = 5;
	while(window_size > 0) {
		for(int row = 0; row <= row_n - window_size; row++) {
			for(int col = 0; col <= col_n - window_size; col++) {
				
				int *local_histo = new int[H_N];
				for(int x = 0; x < H_N; x++) local_histo[x] = 0;
				for(int x = row; x < row + window_size; x++) {
					for(int y = col; y < col + window_size; y++) {
						int block_index = x * col_n + y;
						for(int z = 0; z < H_N; z++)
							local_histo[z] += block_histos[block_index][z];
					}
				}
				
				double *norm_local = normalize(local_histo, H_N);
				//print_arr_d(norm_local, H_N);
				//print_arr_d(norm_logo, H_N);
				double diff = differ(norm_local, norm_logo, H_N);
				//TRACE("row: %d, col: %d, size: %d, diff: %lf\n", row, col, window_size, diff);
				if(best_boxes.size() == max_heap_size && best_boxes.top().diff > diff) {
					best_boxes.pop();
				}
				if(best_boxes.size() < max_heap_size) {
					Box new_box = {row, col, window_size, diff};
					//TRACE("r: %d, c: %d, size: %d, diff: %lf\n", new_box.row, new_box.col, new_box.len, new_box.diff);
					best_boxes.push(new_box);
				}
				delete local_histo;
				delete norm_local;
			}
		}
		window_size--;
	}
	//TRACE("row: %d, col: %d, size: %d, diff: %lf\n", min_y, min_x, min_size, min_diff);
	//int length = min_size * BLOCK_SIZE;
	while(!best_boxes.empty()) {
		Box best_box = best_boxes.top();
		TRACE("row: %d, col: %d, size: %d, diff: %lf\n", best_box.row, best_box.col, best_box.len, best_box.diff);
		img_pic.DrawBox(best_box.row * BLOCK_SIZE, best_box.col * BLOCK_SIZE, (best_box.row  + best_box.len)* BLOCK_SIZE, (best_box.col + best_box.len) * BLOCK_SIZE);
		best_boxes.pop();
	}
	delete his_logo;
	delete norm_logo;
	
	//delete [] block_histos;
	for(int i = 0; i < total_blc; i++) {
		delete block_histos[i];
	}
	delete block_histos;
	return false;
}
