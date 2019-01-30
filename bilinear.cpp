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
#include <vector>

using namespace std;

//pad function
void padImage(vector<vector<unsigned char>> Imagedata, vector<vector<unsigned char>> &ImagedataPadded, int pad, int SizeH, int SizeW) {
  for (int i = 0; i < SizeH + 2 * pad; i++) {
    for (int j = 0; j < SizeW + 2 * pad; j++) {
      if (i < pad || i > SizeH + pad - 1 || j < pad || j > SizeW + pad - 1) {
        if (i < pad && j < pad) {
          ImagedataPadded[i][j] = Imagedata[2 * pad - 1 - i - pad][2 * pad - 1 - j - pad];
        }
        else if (i > SizeH + pad - 1 && j > SizeW + pad - 1) {
          ImagedataPadded[i][j] = Imagedata[i - 2 * pad + 1 - pad][j - 2 * pad + 1 - pad];
        }
        else if (i > SizeH + pad - 1 && j < pad) {
          ImagedataPadded[i][j] = Imagedata[i - 2 * pad + 1 - pad][2 * pad - 1 - j - pad];
        }
        else if (j > SizeH + pad - 1 && i < pad) {
          ImagedataPadded[i][j] = Imagedata[2 * pad - 1 - i - pad][j - 2 * pad + 1 - pad];
        }
        else{
          if (i < pad) {
            ImagedataPadded[i][j] = Imagedata[2 * pad - 1 - i - pad][j - pad];
          }
          else if (i > SizeH + pad - 1) {
            ImagedataPadded[i][j] = Imagedata[i - 2 * pad + 1 - pad][j - pad];
          }
          else if (j < pad) {
            ImagedataPadded[i][j] = Imagedata[i - pad][2 * pad - 1 - j - pad];
          }
          else if (j > SizeW + pad - 1) {
            ImagedataPadded[i][j] = Imagedata[i - pad][j - 2 * pad + 1 - pad];
          }
        }
      }
      else{
        ImagedataPadded[i][j] = Imagedata[i - pad][j - pad];
      }
    }
  }
}

//convolution function
float convolution(vector<vector<unsigned char>> ImageSection, vector<vector<float>> filter, int SizeF, int SizeH, int SizeW) {
  float convResult = 0.0;
  for (int i = 0; i < SizeF; i++) {
    for (int j = 0; j < SizeF; j++) {
      convResult += ImageSection[i][j] * filter[i][j];
    }
  }
  return convResult;
}

int main(int argc, char *argv[])
{
  // Define file pointer and variables
  FILE *file;
  int BytesPerPixelBayer = 1;
  int BytesPerPixelRGB = 3;
  int SizeW = 390;
  int SizeH = 300;
  int pad = 1;
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
    BytesPerPixelBayer = 1;
    // default is grey image
  }
  else
  {
    BytesPerPixelRGB = atoi(argv[3]);
    // Check if size is specified
    if (argc >= 5)
    {
      SizeW = atoi(argv[4]);
      SizeH = atoi(argv[5]);
    }
  }

  // Allocate image data array
  unsigned char Imagedata[SizeH][SizeW][BytesPerPixelBayer];
  // Read image (filename specified by first argument) into image data matrix
  if (!(file = fopen(argv[1], "rb")))
  {
    cout << "Cannot open file: " << argv[1] << endl;
    exit(1);
  }
  fread(Imagedata, sizeof(unsigned char), SizeH * SizeW * BytesPerPixelBayer, file);
  fclose(file);

  ///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
  //convert 2d array into vector
  vector<vector<unsigned char>> ImagedataVec;
  for (int i = 0; i < SizeH; i++) {
    vector<unsigned char> vec;
    for (int j = 0; j < SizeW; j++) {
      vec.push_back(Imagedata[i][j][0]);
    }
    ImagedataVec.push_back(vec);
  }

  //pad the image for the N values
  vector<vector<unsigned char>> ImagedataPadded(SizeH + 2 * pad, vector<unsigned char>(SizeW + 2 * pad, 0));
  padImage(ImagedataVec, ImagedataPadded, pad, SizeH, SizeW);

  //G at R filter
  vector<vector<float>> filterGR(2 * pad + 1, vector<float>(2 * pad + 1, 0.0));
  //G at B filter
  vector<vector<float>> filterGB(2 * pad + 1, vector<float>(2 * pad + 1, 0.0));
  //R at G, R row, B col filter
  vector<vector<float>> filterRGRB(2 * pad + 1, vector<float>(2 * pad + 1, 0.0));
  //R at G, B row, R col filter
  vector<vector<float>> filterRGBR(2 * pad + 1, vector<float>(2 * pad + 1, 0.0));
  //R at B, B row, B col
  vector<vector<float>> filterRBBB(2 * pad + 1, vector<float>(2 * pad + 1, 0.0));
  //B at G, B row, R col
  vector<vector<float>> filterBGBR(2 * pad + 1, vector<float>(2 * pad + 1, 0.0));
  //B at G, R row, B col
  vector<vector<float>> filterBGRB(2 * pad + 1, vector<float>(2 * pad + 1, 0.0));
  //B at R, R row, R col
  vector<vector<float>> filterBRRR(2 * pad + 1, vector<float>(2 * pad + 1, 0.0));
  //filter SizeF x SizeF
  filterGR[0][1] = 2.0 / 8;
  filterGR[1][0] = 2.0 / 8;
  filterGR[1][2] = 2.0 / 8;
  filterGR[2][1] = 2.0 / 8;

  filterGB[0][2] = 2.0 / 8;
  filterGB[1][0] = 2.0 / 8;
  filterGB[1][2] = 2.0 / 8;
  filterGB[2][0] = 2.0 / 8;

  filterRGRB[1][0] =  4.0 / 8;
  filterRGRB[1][2] =  4.0 / 8;

  filterRGBR[0][1] = 4.0 / 8;
  filterRGBR[2][1] = 4.0 / 8;

  filterRBBB[0][0] = 2.0 / 8;
  filterRBBB[0][2] = 2.0 / 8;
  filterRBBB[2][0] = 2.0 / 8;
  filterRBBB[2][2] = 2.0 / 8;

  filterBGRB[0][1] = 4.0 / 8;
  filterBGRB[2][1] = 4.0 / 8;

  filterBGBR[1][0] =  4.0 / 8;
  filterBGBR[1][2] =  4.0 / 8;

  filterBRRR[0][0] = 2.0 / 8;
  filterBRRR[0][2] = 2.0 / 8;
  filterBRRR[2][0] = 2.0 / 8;
  filterBRRR[2][2] = 2.0 / 8;

  //convolution to find all the channels
  unsigned char ImagedataRGB[SizeH][SizeW][BytesPerPixelRGB];
  for (int i = 0; i < SizeH; i++) {
    for (int j = 0; j < SizeW; j++) {
      vector<vector<unsigned char>> ImageSection(2 * pad + 1, vector<unsigned char>(2 * pad + 1, 0));
      //pull out image section
      for (int k = 0; k < 2 * pad + 1; k++) {
        for (int l = 0; l < 2 * pad + 1; l++) {
          ImageSection[k][l] = ImagedataPadded[i + k][j + l];
        }
      }
      //green pixel position
      if((i + pad + j + pad) % 2 == 0){
        ImagedataRGB[i][j][1] = ImagedataPadded[i + pad][j + pad];
        //red row, blue column
        if((i + pad) % 2 == 1){
          ImagedataRGB[i][j][0] = (unsigned char)convolution(ImageSection, filterRGRB, 2 * pad + 1, SizeH, SizeW);
          ImagedataRGB[i][j][2] = (unsigned char)convolution(ImageSection, filterBGRB, 2 * pad + 1, SizeH, SizeW);
        }
        //blue row, red column
        else{
          ImagedataRGB[i][j][2] = (unsigned char)convolution(ImageSection, filterBGBR, 2 * pad + 1, SizeH, SizeW);
          ImagedataRGB[i][j][0] = (unsigned char)convolution(ImageSection, filterRGBR, 2 * pad + 1, SizeH, SizeW);
        }
      }
      else{
        //blue pixel position
        if((i + pad) % 2 == 0){
          ImagedataRGB[i][j][2] = ImagedataPadded[i + pad][j + pad];
          ImagedataRGB[i][j][0] = (unsigned char)convolution(ImageSection, filterRBBB, 2 * pad + 1, SizeH, SizeW);
          ImagedataRGB[i][j][1] = (unsigned char)convolution(ImageSection, filterGB, 2 * pad + 1, SizeH, SizeW);
        }
        //red pixel position
        else{
          ImagedataRGB[i][j][0] = ImagedataPadded[i + pad][j + pad];
          ImagedataRGB[i][j][2] = (unsigned char)convolution(ImageSection, filterBRRR, 2 * pad + 1, SizeH, SizeW);
          ImagedataRGB[i][j][1] = (unsigned char)convolution(ImageSection, filterGR, 2 * pad + 1, SizeH, SizeW);
        }
      }
    }
  }
  // Write image data (filename specified by second argument) from image data matrix

  if (!(file = fopen(argv[2], "wb")))
  {
    cout << "Cannot open file: " << argv[2] << endl;
    exit(1);
  }

  fwrite(ImagedataRGB, sizeof(unsigned char), SizeH * SizeW * BytesPerPixelRGB, file);
  fclose(file);
  return 0;
}
