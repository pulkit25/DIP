#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include<math.h>
#include<string.h>

using namespace std;

//pad function
void padImage(unsigned char *Imagedata, unsigned char *ImagedataPadded, int pad, int SizeH, int SizeW)
{
  for (int i = 0; i < SizeH + 2 * pad; i++)
  {
    for (int j = 0; j < SizeW + 2 * pad; j++)
    {
      if (i < pad || i > SizeH + pad - 1 || j < pad || j > SizeW + pad - 1)
      {
        if (i < pad && j < pad)
        {
          *(ImagedataPadded + (SizeW + 2 * pad) * i + j) = *(Imagedata + SizeW * (2 * pad - 1 - i - pad) + (2 * pad - 1 - j - pad));
        }
        else if (i > SizeH + pad - 1 && j > SizeW + pad - 1)
        {
          *(ImagedataPadded + (SizeW + 2 * pad) * i + j) = *(Imagedata + SizeW * (i - 2 * pad + 1 - pad) + (j - 2 * pad + 1 - pad));
        }
        else if (i > SizeH + pad - 1 && j < pad)
        {
          *(ImagedataPadded + (SizeW + 2 * pad) * i + j) = *(Imagedata + SizeW * (i - 2 * pad + 1 - pad) + (2 * pad - 1 - j - pad));
        }
        else if (j > SizeH + pad - 1 && i < pad)
        {
          *(ImagedataPadded + (SizeW + 2 * pad) * i + j) = *(Imagedata + SizeW * (2 * pad - 1 - i - pad) + (j - 2 * pad + 1 - pad));
        }
        else
        {
          if (i < pad)
          {
            *(ImagedataPadded + (SizeW + 2 * pad) * i + j) = *(Imagedata + SizeW * (2 * pad - 1 - i - pad) + (j - pad));
          }
          else if (i > SizeH + pad - 1)
          {
            *(ImagedataPadded + (SizeW + 2 * pad) * i + j) = *(Imagedata + SizeW * (i - 2 * pad + 1 - pad) + (j - pad));
          }
          else if (j < pad)
          {
            *(ImagedataPadded + (SizeW + 2 * pad) * i + j) = *(Imagedata + SizeW * (i - pad) + (2 * pad - 1 - j - pad));
          }
          else if (j > SizeW + pad - 1)
          {
            *(ImagedataPadded + (SizeW + 2 * pad) * i + j) = *(Imagedata + SizeW * (i - pad) + (j - 2 * pad + 1 - pad));
          }
        }
      }
      else
      {
        *(ImagedataPadded + (SizeW + 2 * pad) * i + j) = *(Imagedata + SizeW * (i - pad) + (j - pad));
      }
    }
  }
}

//convolution function
float convolution(unsigned char *ImageSection, float *filter, int SizeF)
{
  float convResult = 0.0;
  for (int i = 0; i < SizeF; i++)
  {
    for (int j = 0; j < SizeF; j++)
    {
      convResult += *(ImageSection + SizeF * i + j) * (*(filter + SizeF * i + j));
    }
  }
  return convResult;
}

int calculate_coefficient(int SizeI, int i, int j)
{
    if(SizeI / 2 >= 2)
    {
        if ((j % SizeI) / (SizeI / 2) == 0 && (i % SizeI) / (SizeI / 2) == 0)
        {
            return 4 * calculate_coefficient(SizeI / 2, i, j) + 1;
        }
        else if ((j % SizeI) / (SizeI / 2) == 1 && (i % SizeI) / (SizeI / 2) == 0)
        {
            return 4 * calculate_coefficient(SizeI / 2, i, j) + 2;
        }
        else if ((j % SizeI) / (SizeI / 2) == 0 && (i % SizeI) / (SizeI / 2) == 1)
        {
            return 4 * calculate_coefficient(SizeI / 2, i, j) + 3;
        }
        else
        {
            return 4 * calculate_coefficient(SizeI / 2, i, j);
        }
    }
    else
    {
        if ((i % SizeI) / (SizeI / 2) == 0 && (j % SizeI) / (SizeI / 2) == 0)
        {
            return 1;
        }
        else if ((i % SizeI) / (SizeI / 2) == 0 && (j % SizeI) / (SizeI / 2) == 1)
        {
            return 2;
        }
        else if ((i % SizeI) / (SizeI / 2) == 1 && (j % SizeI) / (SizeI / 2) == 0)
        {
            return 3;
        }
        else
        {
            return 0;
        }
    }
}

void calculate_index_matrix(int *IndexMatrix, int SizeI)
{
    for(int i = 0; i < SizeI; i++)
    {
        for(int j = 0; j < SizeI; j++)
        {
            *(IndexMatrix + i * SizeI + j) = calculate_coefficient(SizeI, i, j);
        }
    }
}

void error_diffusion(float *ImageResdata, float error, int i, int j, int p, int SizeH, int SizeW, int method)
{
    if (method == 0)
    {
        if(i % 2 == 0)
        {
            if (j + 1 < SizeW)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * i + j + 1) += 7.0 / 16 * error;
            }
            if (i + 1 < SizeH && j + 1 < SizeW)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 1) + j + 1) += 1.0 / 16 * error;
            }
            if (i + 1 < SizeH)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 1) + j) += 5.0 / 16 * error;
            }
            if (i + 1 < SizeH && j - 1 >= 0)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 1) + j - 1) += 3.0 / 16 * error;
            }
        }
        else 
        {
            if (j - 1 >= 0)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * i + j - 1) += 7.0 / 16 * error;
            }
            if (i + 1 < SizeH && j - 1 >= 0)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 1) + j - 1) += 1.0 / 16 * error;
            }
            if (i + 1 < SizeH)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 1) + j) += 5.0 / 16 * error;
            }
            if (i + 1 < SizeH && j + 1 < SizeW)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 1) + j + 1) += 3.0 / 16 * error;
            }
        }
    }
    else if (method == 1)
    {
        if(i % 2 == 0)
        {
            if (j + 1 < SizeW)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * i + j + 1) += 7.0 / 48 * error;
            }
            if (j + 2 < SizeW)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * i + j + 2) += 5.0 / 48 * error;
            }
            if (i + 1 < SizeH && j + 2 < SizeW)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 1) + j + 2) += 3.0 / 48 * error;
            }
            if (i + 1 < SizeH && j + 1 < SizeW)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 1) + j + 1) += 5.0 / 48 * error;
            }
            if (i + 1 < SizeH)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 1) + j) += 7.0 / 48 * error;
            }
            if (i + 1 < SizeH && j - 1 >= 0)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 1) + j - 1) += 5.0 / 48 * error;
            }
            if (i + 1 < SizeH && j - 2 >= 0)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 1) + j - 2) += 3.0 / 48 * error;
            }
            if (i + 2 < SizeH && j - 2 >= 0)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 2) + j - 2) += 1.0 / 48 * error;
            }
            if (i + 2 < SizeH && j - 1 >= 0)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 2) + j - 1) += 3.0 / 48 * error;
            }
            if (i + 2 < SizeH)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 2) + j) += 5.0 / 48 * error;
            }
            if (i + 2 < SizeH && j + 1 < SizeW)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 2) + j + 1) += 3.0 / 48 * error;
            }
            if (i + 2 < SizeH && j + 2 < SizeW)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 2) + j + 2) += 1.0 / 48 * error;
            }
        }
        else 
        {
            if (j - 1 >= 0)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * i + j - 1) += 7.0 / 48 * error;
            }
            if (j - 2 >= 0)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * i + j - 2) += 5.0 / 48 * error;
            }
            if (i + 1 < SizeH && j - 2 >= 0)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 1) + j - 2) += 3.0 / 48 * error;
            }
            if (i + 1 < SizeH && j - 1 >= 0)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 1) + j - 1) += 5.0 / 48 * error;
            }
            if (i + 1 < SizeH)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 1) + j) += 7.0 / 48 * error;
            }
            if (i + 1 < SizeH && j + 1 < SizeW)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 1) + j + 1) += 5.0 / 48 * error;
            }
            if (i + 1 < SizeH && j + 2 < SizeW)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 1) + j + 2) += 3.0 / 48 * error;
            }
            if (i + 2 < SizeH && j + 2 < SizeW)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 2) + j + 2) += 1.0 / 48 * error;
            }
            if (i + 2 < SizeH && j + 1 < SizeW)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 2) + j + 1) += 3.0 / 48 * error;
            }
            if (i + 2 < SizeH)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 2) + j) += 5.0 / 48 * error;
            }
            if (i + 2 < SizeH && j - 1 >= 0)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 2) + j - 1) += 3.0 / 48 * error;
            }
            if (i + 2 < SizeH && j - 2 >= 0)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 2) + j - 2) += 1.0 / 48 * error;
            }
        }
    }
    else if (method == 2)
    {
        if(i % 2 == 0)
        {
            if (j + 1 < SizeW)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * i + j + 1) += 8.0 / 42 * error;
            }
            if (j + 2 < SizeW)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * i + j + 2) += 4.0 / 42 * error;
            }
            if (i + 1 < SizeH && j + 2 < SizeW)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 1) + j + 2) += 2.0 / 42 * error;
            }
            if (i + 1 < SizeH && j + 1 < SizeW)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 1) + j + 1) += 4.0 / 42 * error;
            }
            if (i + 1 < SizeH)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 1) + j) += 8.0 / 42 * error;
            }
            if (i + 1 < SizeH && j - 1 >= 0)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 1) + j - 1) += 4.0 / 42 * error;
            }
            if (i + 1 < SizeH && j - 2 >= 0)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 1) + j - 2) += 2.0 / 42 * error;
            }
            if (i + 2 < SizeH && j - 2 >= 0)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 2) + j - 2) += 1.0 / 42 * error;
            }
            if (i + 2 < SizeH && j - 1 >= 0)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 2) + j - 1) += 2.0 / 42 * error;
            }
            if (i + 2 < SizeH)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 2) + j) += 4.0 / 42 * error;
            }
            if (i + 2 < SizeH && j + 1 < SizeW)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 2) + j + 1) += 2.0 / 42 * error;
            }
            if (i + 2 < SizeH && j + 2 < SizeW)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 2) + j + 2) += 1.0 / 42 * error;
            }
        }
        else 
        {
            if (j - 1 >= 0)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * i + j - 1) += 8.0 / 42 * error;
            }
            if (j - 2 >= 0)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * i + j - 2) += 4.0 / 42 * error;
            }
            if (i + 1 < SizeH && j - 2 >= 0)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 1) + j - 2) += 2.0 / 42 * error;
            }
            if (i + 1 < SizeH && j - 1 >= 0)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 1) + j - 1) += 4.0 / 42 * error;
            }
            if (i + 1 < SizeH)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 1) + j) += 8.0 / 42 * error;
            }
            if (i + 1 < SizeH && j + 1 < SizeW)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 1) + j + 1) += 4.0 / 42 * error;
            }
            if (i + 1 < SizeH && j + 2 < SizeW)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 1) + j + 2) += 2.0 / 42 * error;
            }
            if (i + 2 < SizeH && j + 2 < SizeW)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 2) + j + 2) += 1.0 / 42 * error;
            }
            if (i + 2 < SizeH && j + 1 < SizeW)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 2) + j + 1) += 2.0 / 42 * error;
            }
            if (i + 2 < SizeH)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 2) + j) += 4.0 / 42 * error;
            }
            if (i + 2 < SizeH && j - 1 >= 0)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 2) + j - 1) += 2.0 / 42 * error;
            }
            if (i + 2 < SizeH && j - 2 >= 0)
            {
                *(ImageResdata + (SizeH * SizeW) * p + SizeW * (i + 2) + j - 2) += 1.0 / 42 * error;
            }
        }
    }
}

string pyramidMBVQ(float R, float G, float B)
{
    //order for return values- RGBWCMYK(11111111)
    if(R + G > 255.0)
    {
        if (G + B > 255.0)
        {
            if (R + G + B > 510.0)
            {
                return  "CMYW";
            }
            else
            {
                return "MYGC";
            }
        }
        else
        {
            return "RGMY";
        }
    }
    else
    {
        if (G + B <= 255.0)
        {
            if (R + G + B <= 255.0)
            {
                return "KRGB";
            }
            else
            {
                return "RGBM";
            }
        }
        else
        {
            return "CMGB";
        }
    }
}

string findNearestEdge(float R, float G, float B, string mbvq)
{
    string vertex = "W";
    if (mbvq.compare("CMYW") == 0)
    {
        vertex = "W";
        if (B < 127.0)
        {
            if (B <= R)
            {
                if (B <= G)
                {
                    vertex = "Y";
                }
            }
        }
        if (G < 127.0)
        {
            if (G <= B)
            {
                if (G <= R)
                {
                    vertex = "M";
                }
            }
        }
        if (R < 127.0)
        {
            if (R <= B)
            {
                if (R <= G)
                {
                    vertex = "C";
                }
            }
        }
    }

    if (mbvq.compare("MYGC") == 0)
    {
        vertex = "M";
        if (G >= B)
        {
            if (R >= B)
            {
                if (R >= 127.0)
                {
                    vertex = "Y";
                }
                else
                {
                    vertex = "G";
                }
            }
        }
        if (G >= R)
        {
            if (B >= R)
            {
                if (B >= 127.0)
                {
                    vertex = "C";
                }
                else
                {
                    vertex = "G";
                }
            }
        }
    }

    if (mbvq.compare("RGMY") == 0)
    {
        if (B > 127.0)
        {
            if (R > 127.0)
            {
                if (B >= G)
                {
                    vertex = "M";
                }
                else
                {
                    vertex = "Y";
                }
            }
            else
            {
                if (G > B + R)
                {
                    vertex = "G";
                }
                else
                {
                    vertex = "M";
                }
            }
        }
        else
        {
            if (R >= 127.0)
            {
                if (G >= 127.0)
                {
                    vertex = "Y";
                }
                else
                {
                    vertex = "R";
                }
            }
            else
            {
                if (R >= G)
                {
                    vertex = "R";
                }
                else
                {
                    vertex = "G";
                }
            }
        }
    }

    if (mbvq.compare("KRGB") == 0)
    {
        vertex = "K";
        if (B > 127.0)
        {
            if (B >= R)
            {
                if (B >= G)
                {
                    vertex = "B";
                }
            }
        }
        if (G > 127.0)
        {
            if (G >= B)
            {
                if (G >= R)
                {
                    vertex = "G";
                }
            }
        }
        if (R > 127.0)
        {
            if (R >= B)
            {
                if (R >= G)
                {
                    vertex = "R";
                }
            }
        }
    }

    if (mbvq.compare("RGBM") == 0)
    {
        vertex = "G";
        if (R > G)
        {
            if (R >= B)
            {
                if (B < 127.0)
                {
                    vertex = "R";
                }
                else
                {
                    vertex = "M";
                }
            }
        }
        if (B > G)
        {
            if (B >= R)
            {
                if (R < 127.0)
                {
                    vertex = "B";
                }
                else
                {
                    vertex = "M";
                }
            }
        }
    }

    if (mbvq.compare("CMGB") == 0)
    {
        if (B > 127.0)
        {
            if ( R > 127.0)
            {
                if (G >= R)
                {
                    vertex = "C";
                }
                else
                {
                    vertex = "M";
                }
            }
            else
            {
                if (G > 127.0)
                {
                    vertex = "C";
                }
                else
                {
                    vertex = "B";
                }
            }
        }
        else
        {
            if ( R > 127.0)
            {
                if (R - G + B >= 127.0)
                {
                    vertex = "M";
                }
                else
                {
                    vertex = "G";
                }
            }
            else
            {
                if (G >= B)
                {
                    vertex = "G";
                }
                else
                {
                    vertex = "B";
                }
            }
        }
    }

    return vertex;
}

int main(int argc, char *argv[])
{
    // Define file pointer and variables
    FILE *file;
    int BytesPerPixel = 1;
    int SizeW = 481;
    int SizeH = 321;
    int prob = 1;
    int SizeI = 2;
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
        BytesPerPixel = atoi(argv[3]);
        // Check if size is specified
        if (argc >= 5)
        {
            SizeW = atoi(argv[4]);
            SizeH = atoi(argv[5]);
            prob = atoi(argv[6]);
        }
        if (argc >= 8)
        {
            SizeI = atoi(argv[7]);
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
    unsigned char ImageResultdata[SizeH][SizeW][BytesPerPixel];
    //Random Thresholding
    if (prob == 0)
    {
        for(int i = 0; i < SizeH; i++)
        {
            for(int j = 0; j < SizeW; j++)
            {
                unsigned char n  = rand() % 256;
                if (Imagedata[i][j][0] <= n)
                {
                    ImageResultdata[i][j][0] = 0;
                }
                else
                {
                    ImageResultdata[i][j][0] = 255;
                }
            }
        }
    }
    //Dithering matrix
    else if (prob == 1)
    {
        int *IndexMatrix = (int *)malloc(SizeI * SizeI * sizeof(int));
        calculate_index_matrix(IndexMatrix, SizeI);
        for(int i = 0; i < SizeH; i++)
        {
            for(int j = 0; j < SizeW; j++)
            {
                int thresh_val = 0;
                if((int)((*(IndexMatrix + SizeI * (i % SizeI) + (j % SizeI)) + 0.5) / (SizeI * SizeI) * 255.0) > 255)
                {
                    thresh_val = 255;
                }
                else
                {
                    thresh_val = (int)((*(IndexMatrix + SizeI * (i % SizeI) + (j % SizeI)) + 0.5) / (SizeI * SizeI) * 255.0);
                }
                
                if (Imagedata[i][j][0] <= thresh_val)
                {
                    ImageResultdata[i][j][0] = 0;
                }
                else
                {
                    ImageResultdata[i][j][0] = 255;
                }
            }
        }
    }
    //Floyd-Steinberg, Jarvis, Stucki
    else if (prob == 2 || prob == 3 || prob == 4)
    {
        int thresh = 127;
        float *ImageResdata = (float *)malloc(SizeH * SizeW * sizeof(float));
        for(int i = 0; i < SizeH; i++)
        {
            for(int j = 0; j < SizeW; j++)
            {
                *(ImageResdata + SizeW * i + j) = Imagedata[i][j][0];
            }
        }
        for(int i = 0; i < SizeH; i++)
        {
            for(int j = 0; j < SizeW; j++)
            {
                if (*(ImageResdata + SizeW * i + j) <= thresh)
                {
                    ImageResultdata[i][j][0] = 0;
                }
                else
                {
                    ImageResultdata[i][j][0] = 255;
                }
                float e = *(ImageResdata + SizeW * i + j) - ImageResultdata[i][j][0];
                error_diffusion(ImageResdata, e, i, j, 0, SizeH, SizeW, prob - 2);
            }
        }
        
    }
    //color error diffusion
    else if (prob == 5)
    {
        int thresh = 0.5;
        float *ImageResdata = (float *)malloc(SizeH * SizeW * BytesPerPixel * sizeof(float));
        for(int i = 0; i < SizeH; i++)
        {
            for(int j = 0; j < SizeW; j++)
            {
                for(int p = 0; p < BytesPerPixel; p++)
                {
                    //convert to CMYK
                    *(ImageResdata + (SizeH * SizeW) * p + SizeW * i + j) = 1.0 - Imagedata[i][j][p] / 255.0;
                }
            }
        }
        for(int i = 0; i < SizeH; i++)
        {
            for(int j = 0; j < SizeW; j++)
            {
                for(int p = 0; p < BytesPerPixel; p++)
                {
                    if (*(ImageResdata + (SizeH * SizeW) * p + SizeW * i + j) <= thresh)
                    {
                        ImageResultdata[i][j][p] = 0;
                    }
                    else
                    {
                        ImageResultdata[i][j][p] = 255;
                    }
                    float e = *(ImageResdata + (SizeH * SizeW) * p + SizeW * i + j) - ImageResultdata[i][j][p] / 255.0;
                    error_diffusion(ImageResdata, e, i, j, p, SizeH, SizeW, 0);
                    ImageResultdata[i][j][p] = 255 - ImageResultdata[i][j][p];
                }
            }
        }
    }
    //MBVQ error diffusion
    else if (prob == 6)
    {
        float *ImageResdata = (float *)malloc(SizeH * SizeW * BytesPerPixel * sizeof(float));
        for(int i = 0; i < SizeH; i++)
        {
            for(int j = 0; j < SizeW; j++)
            {
                for(int p = 0; p < BytesPerPixel; p++)
                {
                    *(ImageResdata + (SizeH * SizeW) * p + SizeW * i + j) = Imagedata[i][j][p];
                }
            }
        }
        for(int i = 0; i < SizeH; i++)
        {
            for(int j = 0; j < SizeW; j++)
            {
                //find the quad the point belongs to
                string MBVQ = pyramidMBVQ(Imagedata[i][j][0], Imagedata[i][j][1], Imagedata[i][j][2]);
                //find nearest edge to the point
                string v = findNearestEdge(*(ImageResdata + (SizeH * SizeW) * 0 + SizeW * i + j), *(ImageResdata + (SizeH * SizeW) * 1 + SizeW * i + j), *(ImageResdata + (SizeH * SizeW) * 2 + SizeW * i + j), MBVQ);
                float Rerror = 0;
                float Gerror = 0;
                float Berror = 0;
                if (v.compare("C") == 0)
                {
                    Rerror = *(ImageResdata + (SizeH * SizeW) * 0 + SizeW * i + j);
                    Gerror = *(ImageResdata + (SizeH * SizeW) * 1 + SizeW * i + j) - 255.0;
                    Berror = *(ImageResdata + (SizeH * SizeW) * 2 + SizeW * i + j) - 255.0;
                    ImageResultdata[i][j][0] = 0;
                    ImageResultdata[i][j][1] = 255;
                    ImageResultdata[i][j][2] = 255;
                }
                else if (v.compare("M") == 0)
                {
                    Rerror = *(ImageResdata + (SizeH * SizeW) * 0 + SizeW * i + j) - 255.0;
                    Gerror = *(ImageResdata + (SizeH * SizeW) * 1 + SizeW * i + j);
                    Berror = *(ImageResdata + (SizeH * SizeW) * 2 + SizeW * i + j) - 255.0;
                    ImageResultdata[i][j][0] = 255;
                    ImageResultdata[i][j][1] = 0;
                    ImageResultdata[i][j][2] = 255;
                }
                else if (v.compare("Y") == 0)
                {
                    Rerror = *(ImageResdata + (SizeH * SizeW) * 0 + SizeW * i + j) - 255.0;
                    Gerror = *(ImageResdata + (SizeH * SizeW) * 1 + SizeW * i + j) - 255.0;
                    Berror = *(ImageResdata + (SizeH * SizeW) * 2 + SizeW * i + j);
                    ImageResultdata[i][j][0] = 255;
                    ImageResultdata[i][j][1] = 255;
                    ImageResultdata[i][j][2] = 0;
                }
                else if (v.compare("R") == 0)
                {
                    Rerror = *(ImageResdata + (SizeH * SizeW) * 0 + SizeW * i + j) - 255.0;
                    Gerror = *(ImageResdata + (SizeH * SizeW) * 1 + SizeW * i + j);
                    Berror = *(ImageResdata + (SizeH * SizeW) * 2 + SizeW * i + j);
                    ImageResultdata[i][j][0] = 255;
                    ImageResultdata[i][j][1] = 0;
                    ImageResultdata[i][j][2] = 0;
                }
                else if (v.compare("G") == 0)
                {
                    Rerror = *(ImageResdata + (SizeH * SizeW) * 0 + SizeW * i + j);
                    Gerror = *(ImageResdata + (SizeH * SizeW) * 1 + SizeW * i + j) - 255.0;
                    Berror = *(ImageResdata + (SizeH * SizeW) * 2 + SizeW * i + j);
                    ImageResultdata[i][j][0] = 0;
                    ImageResultdata[i][j][1] = 255;
                    ImageResultdata[i][j][2] = 0;
                }
                else if (v.compare("B") == 0)
                {
                    Rerror = *(ImageResdata + (SizeH * SizeW) * 0 + SizeW * i + j);
                    Gerror = *(ImageResdata + (SizeH * SizeW) * 1 + SizeW * i + j);
                    Berror = *(ImageResdata + (SizeH * SizeW) * 2 + SizeW * i + j) - 255.0;
                    ImageResultdata[i][j][0] = 0;
                    ImageResultdata[i][j][1] = 0;
                    ImageResultdata[i][j][2] = 255;
                }
                else if (v.compare("W") == 0)
                {
                    Rerror = *(ImageResdata + (SizeH * SizeW) * 0 + SizeW * i + j) - 255.0;
                    Gerror = *(ImageResdata + (SizeH * SizeW) * 1 + SizeW * i + j) - 255.0;
                    Berror = *(ImageResdata + (SizeH * SizeW) * 2 + SizeW * i + j) - 255.0;
                    ImageResultdata[i][j][0] = 255;
                    ImageResultdata[i][j][1] = 255;
                    ImageResultdata[i][j][2] = 255;
                }
                else if (v.compare("K") == 0)
                {
                    Rerror = *(ImageResdata + (SizeH * SizeW) * 0 + SizeW * i + j);
                    Gerror = *(ImageResdata + (SizeH * SizeW) * 1 + SizeW * i + j);
                    Berror = *(ImageResdata + (SizeH * SizeW) * 2 + SizeW * i + j);
                    ImageResultdata[i][j][0] = 0;
                    ImageResultdata[i][j][1] = 0;
                    ImageResultdata[i][j][2] = 0;
                }
                error_diffusion(ImageResdata, Rerror, i, j, 0, SizeH, SizeW, 0);
                error_diffusion(ImageResdata, Gerror, i, j, 1, SizeH, SizeW, 0);
                error_diffusion(ImageResdata, Berror, i, j, 2, SizeH, SizeW, 0);
            }
        }
    }
    //edge detection - Sobel
    else if (prob == 7)
    {
        unsigned char *ImageBW = (unsigned char *)malloc(SizeH * SizeW * sizeof(unsigned char));
        int SizeF = 3;
        //convert to grayscale
        for(int i = 0; i < SizeH; i++)
        {
            for(int j = 0; j < SizeW; j++)
            {
                *(ImageBW + SizeW * i + j) = ((float)Imagedata[i][j][0] + Imagedata[i][j][1] + Imagedata[i][j][2]) / 3;
            }
        }

        //pad image for convolution
        unsigned char *ImageBWPadded = (unsigned char *)malloc((SizeH + 2 * (SizeF / 2)) * (SizeW + 2 * (SizeF / 2)) * sizeof(unsigned char));
        padImage(ImageBW, ImageBWPadded, SizeF / 2, SizeH, SizeW);

        int *ImageResultdataHorz = (int *)malloc(SizeH * SizeW * sizeof(int));
        int *ImageResultdataVert = (int *)malloc(SizeH * SizeW * sizeof(int));
        int *ImageResultdataComb = (int *)malloc(SizeH * SizeW * sizeof(int));
        unsigned char *ImageResultdataOut = (unsigned char *)malloc(SizeH * SizeW * sizeof(unsigned char));
        //convolve
        int maxHorz = 0;
        int maxVert = 0;
        int maxComb = 0;
        int minHorz = 0;
        int minVert = 0;
        int minComb = 0;
        for (int i = 0; i < SizeH; i++)
        {
            for (int j = 0; j < SizeW; j++)
            {
                unsigned char *ImageSection = (unsigned char *)malloc(SizeF * SizeF * sizeof(unsigned char));
                //pull out image section
                for (int k = 0; k < SizeF; k++)
                {
                    for (int l = 0; l < SizeF; l++)
                    {
                        *(ImageSection + SizeF * k + l) = *(ImageBWPadded + (SizeW + 2 * (SizeF / 2)) * (i + k) + j + l);
                    }
                }
                //define filter
                float *filterhorz = (float *)malloc(SizeF * SizeF * sizeof(float));
                float *filtervert = (float *)malloc(SizeF * SizeF * sizeof(float));
                *(filtervert) = -1.0;
                *(filterhorz) = 1.0;
                *(filtervert + 1) = 0;
                *(filterhorz + 1) = 2.0;
                *(filtervert + 2) = 1.0;
                *(filterhorz + 2) = 1.0;
                *(filtervert + 3) = -2.0;
                *(filterhorz + 3) = 0;
                *(filtervert + 4) = 0;
                *(filterhorz + 4) = 0;
                *(filtervert + 5) = 2.0;
                *(filterhorz + 5) = 0;
                *(filtervert + 6) = -1.0;
                *(filterhorz + 6) = -1.0;
                *(filtervert + 7) = 0;
                *(filterhorz + 7) = -2.0;
                *(filtervert + 8) = 1.0;
                *(filterhorz + 8) = -1.0;
                *(ImageResultdataHorz + SizeW * i + j) = convolution(ImageSection, filterhorz, SizeF);
                if (*(ImageResultdataHorz + SizeW * i + j) > maxHorz)
                {
                    maxHorz = *(ImageResultdataHorz + SizeW * i + j);
                }
                if (*(ImageResultdataHorz + SizeW * i + j) < minHorz)
                {
                    minHorz = *(ImageResultdataHorz + SizeW * i + j);
                }
                *(ImageResultdataVert + SizeW * i + j) = convolution(ImageSection, filtervert, SizeF);
                if (*(ImageResultdataVert + SizeW * i + j) > maxVert)
                {
                    maxVert = *(ImageResultdataVert + SizeW * i + j);
                }
                if (*(ImageResultdataVert + SizeW * i + j) < minVert)
                {
                    minVert = *(ImageResultdataVert + SizeW * i + j);
                }
                *(ImageResultdataComb + SizeW * i + j) = sqrt(pow(*(ImageResultdataHorz + SizeW * i + j), 2) + pow(*(ImageResultdataVert + SizeW * i + j), 2));
                if (*(ImageResultdataComb + SizeW * i + j) > maxComb)
                {
                    maxComb = *(ImageResultdataComb + SizeW * i + j);
                }
                if (*(ImageResultdataComb + SizeW * i + j) < minComb)
                {
                    minComb = *(ImageResultdataComb + SizeW * i + j);
                }
            }
        }

        for (int i = 0; i < SizeH; i++)
        {
            for (int j = 0; j < SizeW; j++)
            {
                *(ImageResultdataHorz + SizeW * i + j) = ((float)*(ImageResultdataHorz + SizeW * i + j) - minHorz) / (maxHorz - minHorz) * 255;
                *(ImageResultdataVert + SizeW * i + j) = ((float)*(ImageResultdataVert + SizeW * i + j) - minVert) / (maxVert - minVert) * 255;
                *(ImageResultdataComb + SizeW * i + j) = ((float)*(ImageResultdataComb + SizeW * i + j) - minComb) / (maxComb - minComb) * 255;
            }
        }
        
        if (!(file = fopen("sobel_pig_horz.raw", "wb")))
        {
            cout << "Cannot open file: " << "sobel_pig_horz.raw" << endl;
            exit(1);
        }
        for(int i = 0; i < SizeH; i++)
        {
            for(int j = 0; j < SizeW; j++)
            {
                *(ImageResultdataOut + SizeW * i + j) = (unsigned char)*(ImageResultdataHorz + SizeW * i + j);
            }
        }
        fwrite(ImageResultdataOut, sizeof(unsigned char), SizeH * SizeW, file);
        fclose(file);

        if (!(file = fopen("sobel_pig_vert.raw", "wb")))
        {
            cout << "Cannot open file: " << "sobel_pig_vert.raw" << endl;
            exit(1);
        }

        for(int i = 0; i < SizeH; i++)
        {
            for(int j = 0; j < SizeW; j++)
            {
                *(ImageResultdataOut + SizeW * i + j) = (unsigned char)*(ImageResultdataVert + SizeW * i + j);
            }
        }
        fwrite(ImageResultdataOut, sizeof(unsigned char), SizeH * SizeW, file);
        fclose(file);

        if (!(file = fopen("sobel_pig_comb.raw", "wb")))
        {
            cout << "Cannot open file: " << "sobel_pig_comb.raw" << endl;
            exit(1);
        }
        for(int i = 0; i < SizeH; i++)
        {
            for(int j = 0; j < SizeW; j++)
            {
                *(ImageResultdataOut + SizeW * i + j) = (unsigned char)*(ImageResultdataComb + SizeW * i + j);
            }
        }
        fwrite(ImageResultdataOut, sizeof(unsigned char), SizeH * SizeW, file);
        fclose(file);

        //find histogram frequencies
        float hist[256] = {0.0};
        for (int i = 0; i < SizeH; i++)
        {
            for (int j = 0; j < SizeW; j++)
            {
                hist[*(ImageResultdataComb + SizeW * i + j)] += 1.0;
            }
        }

        //normalize the frequencies
        for (int i = 0; i < 256; i++)
        {
            hist[i] = hist[i] / (SizeH * SizeW);
        }
        //calculate cdf
        float cdf[256] = {0.0};
        for (int i = 0; i < 256; i++)
        {
            for (int j = 0; j <= i; j++)
            {
                cdf[i] += hist[j];
            }
        }

        unsigned char thresh = 0;
        for(int i = 0; i < 256; i++)
        {
            if (cdf[i] >= 0.85)
            {
                thresh = i;
                break;
            }
        }
        for(int i = 0; i < SizeH; i++)
        {
            for(int j = 0; j < SizeW; j++)
            {
                if(*(ImageResultdataComb + SizeW * i + j) > thresh)
                {
                    *(ImageResultdataOut + SizeW * i + j) = 0;
                }
                else
                {
                    *(ImageResultdataOut + SizeW * i + j) = 255;
                }
            }
        }
        if (!(file = fopen("sobel_pig_thresh.raw", "wb")))
        {
            cout << "Cannot open file: " << "sobel_pig_thresh.raw" << endl;
            exit(1);
        }
        fwrite(ImageResultdataOut, sizeof(unsigned char), SizeH * SizeW, file);
        fclose(file);

        return 0;
    }
    // Write image data (filename specified by second argument) from image data matrix

    if (!(file = fopen(argv[2], "wb")))
    {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }

    fwrite(ImageResultdata, sizeof(unsigned char), SizeH * SizeW * BytesPerPixel, file);
    fclose(file);
    return 0;
}