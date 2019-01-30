//NAME: Pulkit Pattnaik
//USC ID: 6879618446
//USC email: pattnaik@usc.edu
//Submission date: 22 January 2019
// This sample code reads in image data from a RAW image file and
// writes it into another file
// NOTE:	The code assumes that the image is of size 256 x 256 and is in the
//			RAW format. You will need to make corresponding changes to
//			accommodate images of different sizes and/or types
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

int main(int argc, char *argv[])
{
  // Define file pointer and variables
  FILE *file;
  int BytesPerPixel = 1;
  int SizeW = 400;
  int SizeH = 400;
  // Check for proper syntax
  if (argc < 3)
  {
    cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
    cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
    return 0;
  }

  // Check if image is grayscale or color
  if (argc < 4)
  {
    BytesPerPixel = 1;
    // default is grey image
  }
  else
  {
    BytesPerPixel = atoi(argv[7]);
    // Check if size is specified
    if (argc >= 9)
    {
      SizeW = atoi(argv[8]);
      SizeH = atoi(argv[9]);
    }
  }

  // Allocate image data array
  unsigned char Imagedata[SizeH][SizeW][BytesPerPixel];
  // Read image (filename specified by first argument) into image data matrix
  if (!(file = fopen(argv[1], "rb")))
  {
    cout << "Cannot open file: " << argv[1] << endl;
    exit(1);
  }
  fread(Imagedata, sizeof(unsigned char), SizeH * SizeW * BytesPerPixel, file);
  fclose(file);

  ///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
  //find histogram frequencies
  float hist[256] = {0.0};
  for (int i = 0; i < SizeH; i++) {
    for (int j = 0; j < SizeW; j++) {
      hist[Imagedata[i][j][0]] += 1.0;
    }
  }
  //write histogram values to a .txt file for input plotting
  if (!(file = fopen(argv[3], "wb")))
  {
    cout << "Cannot open file: " << argv[3] << endl;
    exit(1);
  }

  fwrite(hist, sizeof(float), 256, file);
  fclose(file);

  //normalize the frequencies
  for (int i = 0; i < 256; i++) {
    hist[i] = hist[i] / (SizeH * SizeW);
  }
  //calculate cdf
  float cdf[256] = {0.0};
  for (int i = 0; i < 256; i++) {
    for (int j = 0; j <= i; j++) {
      cdf[i] += hist[j];
    }
  }

  //write cdf values to a .txt file for transfer function plotting
  if (!(file = fopen(argv[4], "wb")))
  {
    cout << "Cannot open file: " << argv[4] << endl;
    exit(1);
  }

  fwrite(cdf, sizeof(float), 256, file);
  fclose(file);

  //mapping function
  unsigned char ImagedataEqualized[SizeH][SizeW][BytesPerPixel];
  for (int i = 0; i < SizeH; i++) {
    for (int j = 0; j < SizeW; j++) {
      ImagedataEqualized[i][j][0] = cdf[Imagedata[i][j][0]] * 255;
    }
  }

  //find histogram frequencies output
  float histEq[256] = {0.0};
  for (int i = 0; i < SizeH; i++) {
    for (int j = 0; j < SizeW; j++) {
      histEq[ImagedataEqualized[i][j][0]] += 1.0;
    }
  }

  //normalize the frequencies
  for (int i = 0; i < 256; i++) {
    histEq[i] = histEq[i] / (SizeH * SizeW);
  }
  //calculate cdf
  float cdfEq[256] = {0.0};
  for (int i = 0; i < 256; i++) {
    for (int j = 0; j <= i; j++) {
      cdfEq[i] += histEq[j];
    }
  }
  //write histogram values to a .mat file
  if (!(file = fopen(argv[5], "wb")))
  {
    cout << "Cannot open file: " << argv[5] << endl;
    exit(1);
  }

  fwrite(cdfEq, sizeof(float), 256, file);
  fclose(file);

  //write histogram values to a .mat file
  if (!(file = fopen(argv[6], "wb")))
  {
    cout << "Cannot open file: " << argv[6] << endl;
    exit(1);
  }

  fwrite(histEq, sizeof(float), 256, file);
  fclose(file);
  // Write image data (filename specified by second argument) from image data matrix

  if (!(file = fopen(argv[2], "wb")))
  {
    cout << "Cannot open file: " << argv[2] << endl;
    exit(1);
  }

  fwrite(ImagedataEqualized, sizeof(unsigned char), SizeH * SizeW * BytesPerPixel, file);
  fclose(file);

  return 0;
}
