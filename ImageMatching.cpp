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
}

//retain the majority color in the histogram with "percent" of total pixels
void retainMajority(int *histogram, double percent)
{
}

//normalize the histogram, get the new histogram ranging from 0 - 1
double *normalize(int *his_logo, int size)
{
}

//filter the histogram of destination picture by using histogram of logo picture
void filter(int *his_logo, int *his_pic, int size)
{
}

//calculate the difference between two normalized histogram, can use sum of squres or sum of absolute values
double differ(double *his_logo, double *his_pic, int size)
{
}

//the basic version of main algorithm
bool compareImage_basic(const MyImage &img_logo, const MyImage &img_pic)
{
	int *his_logo = getHistogram_H(img_logo);
	int *his_pic = getHistogram_H(img_pic);
	retainMajority(his_logo, 0.9);
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