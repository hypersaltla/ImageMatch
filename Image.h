//*****************************************************************************
//
// Image.h : Defines the class operations on images
//
// Author - Parag Havaldar
// Main Image class structure 
//
//*****************************************************************************

#ifndef IMAGE_DISPLAY
#define IMAGE_DISPLAY

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "afxwin.h"

// C RunTime Header Files
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>


// Class structure of Image 
// Use to encapsulate an RGB image
class MyImage 
{

private:
	int		Width;					// Width of Image
	int		Height;					// Height of Image
	char	ImagePath[_MAX_PATH];	// Image location
	unsigned char   *Data;					// RGB data of the image
	unsigned char   *Alpha;                   // alpha frame
	int     *Hbuf;                  // Hue buffer
	int     *Sbuf;                  // Saturation buffer
	int     *Vbuf;                  // Value buffer
	unsigned char   *Ybuf;          // Y buffer

public:
	// Constructor
	MyImage();
	// Copy Constructor
	MyImage::MyImage( MyImage *otherImage);
	// Destructor
	~MyImage();

	// operator overload
	MyImage & operator= (const MyImage & otherImage);

	// Reader & Writer functions
	void	setWidth( const int w)  { Width = w; }; 
	void	setHeight(const int h) { Height = h; }; 
	void	setImageData( const unsigned char *img ) { Data = (unsigned char *)img; };
	void	setImagePath( const char *path) { strcpy(ImagePath, path); }
	bool    RGBtoHSV(); //RGB convert to HSV
	bool    RGBtoGray(); //RGB conver to gray scale
	int		getWidth() const { return Width; };
	int		getHeight() const { return Height; };
	unsigned char*	getImageData() const { return Data; };
	unsigned char*  getAlpha() const { return Alpha; };
	//char*	getImagePath() const { return ImagePath; }
	int*	getHbuf() const { return Hbuf; }
	int*	getSbuf() const { return Sbuf; }
	int*	getVbuf() const { return Vbuf; }
	unsigned char *getYbuf() const { return Ybuf; }


	// Input Output operations
	bool	ReadImage();
	bool	WriteImage();
	bool    ReadAlphas(const char *alpha_file);

	// Modifications
	bool	Modify();
	bool    DrawBox(int start_r, int start_c, int end_r, int end_c);
	

};

void print_arr(int *array, int size);
int *getHistogram_H(const MyImage &img);
void retainMajority(int *histogram, double percent, const int size, int total);
double *normalize(int *histogram, int size, int total);
void filter(int *his_logo, int *his_pic, int size);
double differ(double *his_logo, double *his_pic, int size);
bool compareImage_basic(const MyImage &img_logo, const MyImage &img_pic);
bool compareImage_v2(MyImage &img_logo, MyImage &img_pic);

#endif //IMAGE_DISPLAY
