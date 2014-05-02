/*
 * ImageMatching.cpp defines the main algorithms for the image searching project
 */

#include "Image.h"

#define H_N 360
#define S_N 100
#define V_N 100
#define THRESHOLD 10

//get the histogram of Hue from the MyImage class, return an array
int *getHistogram_H(const MyImage &img)
{
    int *histogram = new int[H_N];
    memset(histogram, 0, sizeof(int) * H_N);
    int total = img.Width * img.Height;
    for(int i = 0; i < total; i++) {
        histogram[img.Hbuf[i]]++;
    }
    return histogram;
}

//retain the majority color in the histogram with "percent" of total pixels
void retainMajority(int *histogram, double percent, int size, int total)
{
    int tmp[size][2];
    for(int i = 0; i < size; i++) {
        tmp[i][0] = histogram[i];
        tmp[i][1] = i;
    }
    //sort
    for(int i = 1; i < size; i++) {
        int cnt = tmp[i][0];
        int index = tmp[i][1];
        int j = i - 1;
        for(; j >= 0 && tmp[j][0] < cnt; j--) {
            tmp[j + 1][0] = tmp[j][0];
            tmp[j + 1][1] = tmp[j][1];
        }
        tmp[j + 1][0] = cnt;
        tmp[j + 1][1] = index;
    }
    
    int bound = total * percent;
    int sum = 0, pix_i = size - 1;
    for(; pix_i >= 0; pix_i--) {
        sum += tmp[pix_i][0];
        if(sum >= bound) break;
    }
    for(int i = 0; i < pix_i; i++) {
        histogram[tmp[i][1]] = 0;
    }
}

//normalize the histogram, get the new histogram ranging from 0 - 1
double *normalize(int *histogram, int size, int total)
{
    double *norm_his = new double[size];
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

//the basic version of main algorithm
bool compareImage_basic(const MyImage &img_logo, const MyImage &img_pic)
{
	int *his_logo = getHistogram_H(img_logo);
	int *his_pic = getHistogram_H(img_pic);
	retainMajority(his_logo, 0.9, H_N);
	filter(his_logo, his_pic, H_N);
	double *norm_his_logo = normalize(his_logo, H_N);
	double *norm_his_pic = normalize(his_pic, H_N);
	double diff = differ(norm_his_logo, norm_his_pic);
	delete his_logo;
	delete his_pic;
	delete norm_his_logo;
	delete norm_his_pic;
	return diff <= THRESHOLD;
}
