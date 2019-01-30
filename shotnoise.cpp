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
#include <vector>
#include <cmath>

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
  int BytesPerPixel = 1;
  int SizeW = 256;
  int SizeH = 256;
  int SizeF = 3;
  int filterType = 1;
  float sigma = 1.0;
  float sigmaC = 10.0;
  float sigmaS = 100.0;
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
      SizeF = atoi(argv[7]);
      filterType = atoi(argv[8]);
    }
    if (argc >= 10) {
      sigma = atof(argv[9]);
      sigmaC = atof(argv[10]);
      sigmaS = atof(argv[11]);
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

  // Allocate image data array
  unsigned char ImagedataOrig[SizeH][SizeW][BytesPerPixel];
  // Read image (filename specified by first argument) into image data matrix
  if (!(file = fopen(argv[2], "rb")))
  {
    cout << "Cannot open file: " << argv[2] << endl;
    exit(1);
  }
  fread(ImagedataOrig, sizeof(unsigned char), SizeH * SizeW * BytesPerPixel, file);
  fclose(file);
  ///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
  //Anscombe transformation
  for (int i = 0; i < SizeH; i++) {
    for (int j = 0; j < SizeW; j++) {
      //Anscombe transformation
      Imagedata[i][j][0] = 2.0 * sqrt(Imagedata[i][j][0] + 3.0 / 8);
    }
  }

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
  vector<vector<unsigned char>> ImagedataPadded(SizeH + 2 * (SizeF / 2), vector<unsigned char>(SizeW + 2 * (SizeF / 2), 0));
  padImage(ImagedataVec, ImagedataPadded, (SizeF / 2), SizeH, SizeW);

  vector<vector<float>> filter(SizeF, vector<float>(SizeF, 0.0));
  //filter SizeF x SizeF
  for (int i = 0; i < SizeF; i++) {
    for (int j = 0; j < SizeF; j++) {
      if(filterType == 1) {
        //mean filter
        filter[i][j] = 1.0 / (SizeF * SizeF);
      }
      else{
        //gaussian and bilateral case: leave it for now. will be changed later on in the code
        filter[i][j] = 1.0;
      }
    }
  }

  unsigned char ImagedataConv[SizeH][SizeW][1];
  float sigmaMSE = 0.0;
  //conv with filter SizeF x SizeF
  for (int i = 0; i < SizeH; i++) {
    for (int j = 0; j < SizeW; j++) {
      vector<vector<unsigned char>> ImageSection(SizeF, vector<unsigned char>(SizeF, 0));
      //pull out image section
      for (int k = 0; k < SizeF; k++) {
        for (int l = 0; l < SizeF; l++) {
          ImageSection[k][l] = ImagedataPadded[i + k][j + l];
        }
      }
      //fill in the filter coefficients for gaussian filter
      if(filterType == 2) {
        float sigmaW = 0.0;
        for (int k = i; k < i + SizeF; k++) {
          for (int l = j; l < j + SizeF; l++) {
            sigmaW += 1.0 / (sqrt(2 * M_PI) * sigma) * exp(-(pow(k - i - SizeF / 2, 2) + pow(l - j - SizeF / 2, 2)) / (2 * pow(sigma, 2)));
          }
        }
        for (int k = i; k < i + SizeF; k++) {
          for (int l = j; l < j + SizeF; l++) {
            filter[k - i][l - j] = 1.0 / (sqrt(2 * M_PI) * sigma * sigmaW) * exp(-(pow(k - i - SizeF / 2, 2) + pow(l - j - SizeF / 2, 2)) / (2 * pow(sigma, 2)));
          }
        }
      }
      //fill in the filter coefficients for the bilateral filter
      else if (filterType == 3) {
        float sigmaW = 0.0;
        for (int k = i; k < i + SizeF; k++) {
          for (int l = j; l < j + SizeF; l++) {
            sigmaW += exp(- (pow(k - i - SizeF / 2, 2) + pow(l - j - SizeF / 2, 2)) / (2 * pow(sigmaC, 2)) - pow(ImageSection[SizeF / 2][SizeF / 2] - ImageSection[k - i][l - j], 2) / (2 * pow(sigmaS, 2)));
          }
        }
        for (int k = i; k < i + SizeF; k++) {
          for (int l = j; l < j + SizeF; l++) {
            filter[k - i][l - j] = 1 / sigmaW * exp(- (pow(k - i - SizeF / 2, 2) + pow(l - j - SizeF / 2, 2)) / (2 * pow(sigmaC, 2)) - pow(ImageSection[SizeF / 2][SizeF / 2] - ImageSection[k - i][l - j], 2) / (2 * pow(sigmaS, 2)));
          }
        }
      }
      ImagedataConv[i][j][0] = (unsigned char)convolution(ImageSection, filter, SizeF, SizeH, SizeW);
      //Inverse Anscombe transformation
      ImagedataConv[i][j][0] = pow(ImagedataConv[i][j][0] / 2.0, 2) - 3.0 / 8;
      sigmaMSE += 1.0 / (SizeH * SizeW) * pow(ImagedataConv[i][j][0] - ImagedataOrig[i][j][0], 2);
    }
  }
  float PSNR = 10.0 * log10(255 * 255 / sigmaMSE);
  cout << "PSNR is: " << PSNR << endl;
  // Write image data (filename specified by second argument) from image data matrix

  if (!(file = fopen(argv[3], "wb")))
  {
    cout << "Cannot open file: " << argv[3] << endl;
    exit(1);
  }

  fwrite(ImagedataConv, sizeof(unsigned char), SizeH * SizeW * BytesPerPixel, file);
  fclose(file);
  return 0;
}
