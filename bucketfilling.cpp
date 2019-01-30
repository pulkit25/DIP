//NAME: Pulkit Pattnaik
//22 January 2019
// This sample code reads in image data from a RAW image file and
// writes it into another file
// NOTE:	The code assumes that the image is of size 256 x 256 and is in the
//			RAW format. You will need to make corresponding changes to
//			accommodate images of different sizes and/or types
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include<vector>

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
    BytesPerPixel = atoi(argv[4]);
    // Check if size is specified
    if (argc >= 6)
    {
      SizeW = atoi(argv[5]);
      SizeH = atoi(argv[6]);
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
  int buckets = 256;
  unsigned char ImagedataEqualized[SizeH][SizeW][BytesPerPixel];
  int numPixelsPerBucket = (SizeH * SizeW) / buckets;
  vector<int> vec(SizeH * SizeW, 0);
  vector<int> indices(SizeH * SizeW, 0);
  for (int i = 0; i < SizeH; i++) {
    for (int j = 0; j < SizeW; j++) {
      vec[SizeW * i + j] = Imagedata[i][j][0];
    }
  }
  int n = 0;
  generate(indices.begin(), indices.end(), [&]{return n++;});
  sort(indices.begin(), indices.end(), [&](int i1, int i2){return vec[i1] < vec[i2];});
  for (int i = 0; i < SizeH * SizeW; i++) {
    ImagedataEqualized[indices[i] / SizeW][indices[i] % SizeW][1] = (i / numPixelsPerBucket);
  }

  //find histogram frequencies for plotting the output
  float histEq[256] = {0.0};
  for (int i = 0; i < SizeH; i++) {
    for (int j = 0; j < SizeW; j++) {
      histEq[ImagedataEqualized[i][j][0]] += 1.0;
    }
  }

  //write histogram values to a .mat file
  if (!(file = fopen(argv[3], "wb")))
  {
    cout << "Cannot open file: " << argv[3] << endl;
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
