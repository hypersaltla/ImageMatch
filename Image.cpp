//*****************************************************************************
//
// Image.cpp : Defines the class operations on images
//
// Author - Parag Havaldar
// Code used by students as starter code to display and modify images
//
//*****************************************************************************

#include "Image.h"


// Constructor and Desctructors
MyImage::MyImage() 
{
	Data = NULL;
	Alpha = NULL;
	Width = -1;
	Height = -1;
	ImagePath[0] = 0;
    Hbuf = NULL;
    Sbuf = NULL;
    Vbuf = NULL;
	Ybuf = NULL;
}

MyImage::~MyImage()
{
	if ( Data )
		delete Data;
	if ( Alpha )
		delete Alpha;
	if(Hbuf)
		delete Hbuf;
	if(Sbuf)
		delete Sbuf;
	if(Vbuf)
		delete Vbuf;
	if(Ybuf)
		delete Ybuf;
}


// Copy constructor
MyImage::MyImage( MyImage *otherImage)
{
	Height = otherImage->Height;
	Width  = otherImage->Width;
	Data   = new unsigned char[Width*Height*3];
	strcpy(otherImage->ImagePath, ImagePath );

	for ( int i=0; i<(Height*Width*3); i++ )
	{
		Data[i]	= otherImage->Data[i];
	}


}



// = operator overload
MyImage & MyImage::operator= (const MyImage &otherImage)
{
	Height = otherImage.Height;
	Width  = otherImage.Width;
	Data   = new unsigned char[Width*Height*3];
	strcpy( (char *)otherImage.ImagePath, ImagePath );

	for ( int i=0; i<(Height*Width*3); i++ )
	{
		Data[i]	= otherImage.Data[i];
	}
	
	return *this;

}


// MyImage::ReadImage
// Function to read the image given a path
bool MyImage::ReadImage()
{

	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		//fprintf(stderr, "Usage is `Image.exe Imagefile w h`");
		return false;
	}
	
	// Create a valid output file pointer
	FILE *IN_FILE;
	IN_FILE = fopen(ImagePath, "rb");
	if ( IN_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Reading");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	unsigned char *Rbuf = new unsigned char[Height*Width]; 
	unsigned char *Gbuf = new unsigned char[Height*Width]; 
	unsigned char *Bbuf = new unsigned char[Height*Width]; 

	for (i = 0; i < Width*Height; i ++)
	{
		Rbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		Gbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		Bbuf[i] = fgetc(IN_FILE);
	}
	
	// Allocate Data structure and copy
	Data = new unsigned char[Width*Height*3];
	for (i = 0; i < Height*Width; i++)
	{
		Data[3*i]	= Bbuf[i];
		Data[3*i+1]	= Gbuf[i];
		Data[3*i+2]	= Rbuf[i];
	}

	// Clean up and return
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
	fclose(IN_FILE);

	return true;

}

bool MyImage::ReadAlphas(const char *alpha_file)
{
	// Verify ImagePath
	if (alpha_file == 0 || alpha_file[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		//fprintf(stderr, "Usage is `Image.exe Imagefile w h`");
		return false;
	}

	// Create a valid output file pointer
	FILE *IN_FILE;
	IN_FILE = fopen(alpha_file, "rb");
	if ( IN_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening Alpha File for Reading");
		return false;
	}

	int total = Width * Height;
	Alpha = new unsigned char[total];
	for(int i = 0; i < total; i++) {
		Alpha[i] = fgetc(IN_FILE);
	}

	fclose(IN_FILE);
	return true;
}


// MyImage functions defined here
bool MyImage::WriteImage()
{
	// Verify ImagePath
	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		return false;
	}
	
	// Create a valid output file pointer
	FILE *OUT_FILE;
	OUT_FILE = fopen(ImagePath, "wb");
	if ( OUT_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Writing");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	unsigned char *Rbuf = new unsigned char[Height*Width]; 
	unsigned char *Gbuf = new unsigned char[Height*Width]; 
	unsigned char *Bbuf = new unsigned char[Height*Width]; 

	for (i = 0; i < Height*Width; i++)
	{
		Bbuf[i] = Data[3*i];
		Gbuf[i] = Data[3*i+1];
		Rbuf[i] = Data[3*i+2];
	}

	
	// Write data to file
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Rbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Gbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Bbuf[i], OUT_FILE);
	}
	
	// Clean up and return
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
	fclose(OUT_FILE);

	return true;

}

// Here is where you would place your code to modify an image
// eg Filtering, Transformation, Cropping, etc.
bool MyImage::Modify()
{

	// TO DO by student

	for ( int i=0; i<Width*Height; i++ )
	{
		Data[3*i] = 0;
		Data[3*i+1] = 0;

	}

	return false;
}

bool MyImage::DrawBox(int start_r, int start_c, int end_r, int end_c)
{
	//omit error input checking
	for(int col = start_c; col < end_c; col++) {
		int index = start_r * Width + col;
		Data[index * 3] = 0;
		Data[index * 3 + 1] = 255;
		Data[index * 3 + 2] = 0;
		
	}

	for(int row = start_r; row < end_r; row++) {
		int index = row * Width + end_c - 1;
		Data[index * 3] = 0;
		Data[index * 3 + 1] = 255;
		Data[index * 3 + 2] = 0;
	}

	for(int col = start_c; col < end_c; col++) {
		int index = (end_r - 1) * Width + col;
		Data[index * 3] = 0;
		Data[index * 3 + 1] = 255;
		Data[index * 3 + 2] = 0;
	}

	for(int row = start_r; row < end_r; row++) {
		int index = row * Width + start_c;
		Data[index * 3] = 0;
		Data[index * 3 + 1] = 255;
		Data[index * 3 + 2] = 0;
	}

	return true;
}

float Round(float Value) {
		return floor( Value  + 0.5);
}

float MAX(float x, float y, float z)
{
	float max = x;
    if (y > max)
    {
        max = y;
    }
	if (z > max)
	{
		max = z;
	}
	return max;
}

float MIN(float x, float y, float z)
{
	float min = x;
    if (y < min)
    {
        min = y;
    }
	if (z < min)
	{
		min = z;
	}
	return min;
}

void rgbToHSV( float r, float g, float b, float *h, float *s, float *v )
{
	
		float min, max, delta;
/*
		R = Rbuf[i]/255;
		G = Gbuf[i]/255;
		B = Bbuf[i]/255;

		max = MAX(MAX(R, G), B);
	    min = MIN(MIN(R, G), B);
		delta = max - min;
		
		//TRACE( "max: %f\n", max);
		//TRACE( "min: %f\n", min);
		//TRACE( "delta: %f\n", delta);
	
		// Hue calculation//
		if( delta == .0) {
			H = 0;
		}
		else {
			if (R == max)		 H = fmod(((G - B) / delta), 6) * 60;
			if (G == max)		 H = (2 + (B - R) / delta) * 60;
			if (B == max)		 H = (4 + (R - G) / delta) * 60;
		}			       
		
		if (H < 0)	 H += 360;
	

		//Saturation calculation//

		if( delta == 0 )   S = 0;
		else			   S = delta / max;

		//Value calculation//

		V = max;
	*/
	min = MIN( r, g, b );
	max = MAX( r, g, b );
	*v = max;	
	delta = max - min;
	if( delta != 0 ) {
		*s = delta / max;	
	}
	else {
		*s = 0;
		*h = -1;
		return;
	}
	if( r == max )
		*h = ( g - b ) / delta;		
	else if( g == max )
		*h = 2 + ( b - r ) / delta;
	else
		*h = 4 + ( r - g ) / delta;	
	*h *= 60;	
	if( *h < 0 ) {
		*h += 360;
	}
}

bool MyImage::RGBtoHSV()
{
	if(!Data) return false;
	if(Hbuf) delete Hbuf;
	if(Sbuf) delete Sbuf;
	if(Vbuf) delete Vbuf;

	float *Rbuf = new float[Height*Width]; 
	float *Gbuf = new float[Height*Width]; 
	float *Bbuf = new float[Height*Width];

	Hbuf = new int[Height*Width]; 
	Sbuf = new int[Height*Width]; 
	Vbuf = new int[Height*Width]; 

	for (int i = 0; i < Height*Width; i++)
	{
		Bbuf[i] =  Data[3*i];
		Gbuf[i] =  Data[3*i+1];
		Rbuf[i] =  Data[3*i+2];
	}
	for (int i = 0; i < Height*Width; i++)
	{ 
		float R = Rbuf[i]/255,
			  G = Gbuf[i]/255,
			  B = Bbuf[i]/255,
			  H,S,V;

		rgbToHSV(R, G, B, &H, &S, &V);
		
		Hbuf[i] = H;
		Sbuf[i] = S*100;
		Vbuf[i] = V*100;
		//TRACE("r: %f, g: %f, b: %f, h: %d, s: %d, v: %d\n", R, G, B, Hbuf[i], Sbuf[i], Vbuf[i]);
		//TRACE( "R: %f\n", R);
		//TRACE( "G: %f\n", G);
		//TRACE( "B: %f\n", B);
		//if(Hbuf[i] < 0 || Hbuf[i] >= 360) TRACE("%d\n", Hbuf[i]);
        if (Hbuf[i] < 0 || Hbuf[i] >= 360) {
		//	TRACE( "Hue: %d\n", Hbuf[i]);
			/*
			TRACE( "Red: %f\n", Rbuf[i]);
			TRACE( "Green: %f\n", Gbuf[i]);
		TRACE( "Blue: %f\n", Bbuf[i]);
		TRACE( "Hue: %d\n", Hbuf[i]);
		TRACE( "Sat: %d\n", Sbuf[i]);
		TRACE( "Val: %d\n", Vbuf[i]);
		*/
         /*   
		printf( "Red: %f\n", Rbuf[i]);
		printf( "Green: %f\n", Gbuf[i]);
		printf( "Blue: %f\n", Bbuf[i]);
		printf( "Hue: %d\n", Hbuf[i]);
		printf( "Sat: %d\n", Sbuf[i]);
		printf( "Val: %d\n", Vbuf[i]);
		*/
        }
	}
    
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;

	return true;
}

bool MyImage::RGBtoGray()
{
	if(Ybuf)
		delete Ybuf;
	Ybuf = new unsigned char[Width * Height];
	for(int i = 0; i < Width * Height; i++) {
		Ybuf[i] = (unsigned char) (0.2126 * Data[i + 2] + 0.7152 * Data[i + 1] + 0.0722 * Data[i]);
		//TRACE("i: %d\n", Ybuf[i]);
	}

	return true;
}