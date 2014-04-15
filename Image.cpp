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
	Width = -1;
	Height = -1;
	ImagePath[0] = 0;
}

MyImage::~MyImage()
{
	if ( Data )
		delete Data;
	if(Hbuf)
		delete Hbuf;
	if(Sbuf)
		delete Sbuf;
	if(Vbuf)
		delete Vbuf;
}


// Copy constructor
MyImage::MyImage( MyImage *otherImage)
{
	Height = otherImage->Height;
	Width  = otherImage->Width;
	Data   = new char[Width*Height*3];
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
	Data   = new char[Width*Height*3];
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
		fprintf(stderr, "Usage is `Image.exe Imagefile w h`");
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
	char *Rbuf = new char[Height*Width]; 
	char *Gbuf = new char[Height*Width]; 
	char *Bbuf = new char[Height*Width]; 

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
	Data = new char[Width*Height*3];
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
	char *Rbuf = new char[Height*Width]; 
	char *Gbuf = new char[Height*Width]; 
	char *Bbuf = new char[Height*Width]; 

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

float Round(float Value) {

	 if( Value > 0.0 ) {
		return floor( Value  + 0.5);
	 } 
	 else
	 {
		return floor( Value - 0.5 );
	 }

}

float MAX(float x, float y)
{
    if (x >= y)
    {
        return x;
    }
    return y;
}

float MIN(float x, float y)
{
    if (x <= y)
    {
        return x;
    }
    return y;
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
		Bbuf[i] = Data[3*i];
		Gbuf[i] = Data[3*i+1];	
		Rbuf[i] = Data[3*i+2];
	}

	for (int i = 0; i < Height*Width; i++)
	{ 
    
		float R,G,B,H,S,V;
		float min, max, delta;

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
		if (R == max)		 H = fmod(((G - B) / delta), 6) * 60;
		if (G == max)		 H = (2 + (B - R) / delta) * 60;
		if (B == max)		 H = (4 + (R - G) / delta) * 60;
						                          
		if (H < 0)	 H += 360;
	

		//Saturation calculation//

		if( delta == 0 )   S = 0;
		else			   S = delta / max;

		//Value calculation//

		V = max;

		Hbuf[i] = Round(H);
		Sbuf[i] = Round(S*100);
		Vbuf[i] = Round(V*100);

		//TRACE( "R: %f\n", R);
		//TRACE( "G: %f\n", G);
		//TRACE( "B: %f\n", B);
		TRACE( "Red: %f\n", Rbuf[i]);
		TRACE( "Green: %f\n", Gbuf[i]);
		TRACE( "Blue: %f\n", Bbuf[i]);
		TRACE( "Hue: %d\n", Hbuf[i]);
		TRACE( "Sat: %d\n", Sbuf[i]);
		TRACE( "Val: %d\n", Vbuf[i]);

	}

	delete Rbuf;
	delete Gbuf;
	delete Bbuf;

	return true;
}