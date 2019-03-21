//NAME: Pulkit Pattnaik
//12 February 2019

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include<math.h>
#include<string.h>

using namespace std;

unsigned char applyConditionalMaskPatterns(char method, unsigned char *ImageSection, int SizeF)
{
    unsigned char conditionalFilter[66][9] = 
    {
        //S
        {0,0,1,0,1,0,0,0,0},
        {1,0,0,0,1,0,0,0,0},
        {0,0,0,0,1,0,1,0,0},
        {0,0,0,0,1,0,0,0,1},
        //S
        {0,0,0,0,1,1,0,0,0},
        {0,1,0,0,1,0,0,0,0},
        {0,0,0,1,1,0,0,0,0},
        {0,0,0,0,1,0,0,1,0},
        //S
        {0,0,1,0,1,1,0,0,0},
        {0,1,1,0,1,0,0,0,0},
        {1,1,0,0,1,0,0,0,0},
        {1,0,0,1,1,0,0,0,0},
        {0,0,0,1,1,0,1,0,0},
        {0,0,0,0,1,0,1,1,0},
        {0,0,0,0,1,0,0,1,1},
        {0,0,0,0,1,1,0,0,1},
        //TK
        {0,1,0,0,1,1,0,0,0},
        {0,1,0,1,1,0,0,0,0},
        {0,0,0,1,1,0,0,1,0},
        {0,0,0,0,1,1,0,1,0},
        //STK
        {0,0,1,0,1,1,0,0,1},
        {1,1,1,0,1,0,0,0,0},
        {1,0,0,1,1,0,1,0,0},
        {0,0,0,0,1,0,1,1,1},
        //ST
        {1,1,0,0,1,1,0,0,0},
        {0,1,0,0,1,1,0,0,1},
        {0,1,1,1,1,0,0,0,0},
        {0,0,1,0,1,1,0,1,0},
        //ST
        {0,1,1,0,1,1,0,0,0},
        {1,1,0,1,1,0,0,0,0},
        {0,0,0,1,1,0,1,1,0},
        {0,0,0,0,1,1,0,1,1},
        //ST
        {1,1,0,0,1,1,0,0,1},
        {0,1,1,1,1,0,1,0,0},
        //STK
        {1,1,1,0,1,1,0,0,0},
        {0,1,1,0,1,1,0,0,1},
        {1,1,1,1,1,0,0,0,0},
        {1,1,0,1,1,0,1,0,0},
        {1,0,0,1,1,0,1,1,0},
        {0,0,0,1,1,0,1,1,1},
        {0,0,0,0,1,1,1,1,1},
        {0,0,1,0,1,1,0,1,1},
        //STK
        {1,1,1,0,1,1,0,0,1},
        {1,1,1,1,1,0,1,0,0},
        {1,0,0,1,1,0,1,1,1},
        {0,0,1,0,1,1,1,1,1},
        //STK
        {0,1,1,0,1,1,0,1,1},
        {1,1,1,1,1,1,0,0,0},
        {1,1,0,1,1,0,1,1,0},
        {0,0,0,1,1,1,1,1,1},
        //STK
        {1,1,1,0,1,1,0,1,1},
        {0,1,1,0,1,1,1,1,1},
        {1,1,1,1,1,1,1,0,0},
        {1,1,1,1,1,1,0,0,1},
        {1,1,1,1,1,0,1,1,0},
        {1,1,0,1,1,0,1,1,1},
        {1,0,0,1,1,1,1,1,1},
        {0,0,1,1,1,1,1,1,1},
        //STK
        {1,1,1,0,1,1,1,1,1},
        {1,1,1,1,1,1,1,0,1},
        {1,1,1,1,1,0,1,1,1},
        {1,0,1,1,1,1,1,1,1},
        //K
        {1,1,1,1,1,1,0,1,1},
        {1,1,1,1,1,1,1,1,0},
        {1,1,0,1,1,1,1,1,1},
        {0,1,1,1,1,1,1,1,1}
    };
    unsigned char shrink[58] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61};
    int shrinkLen = 58;
    unsigned char thin[46] = {16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61};
    int thinLen = 46;
    unsigned char skel[40] = {16,17,18,19,20,21,22,23,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65};
    int skelLen = 40;

    if(method == 0 || method == 3)
    {
        //iterate over the mask patterns
        for(int k = 0; k < shrinkLen; k++)
        {
            int flag = 0;
            for(int i = 0; i < SizeF * SizeF; i++)
            {
                //if any element mismatch break from loop and move to next mask
                if(*(ImageSection + i) != conditionalFilter[shrink[k]][i])
                {
                    flag = 1;
                    break;
                }
            }
            if(flag == 0)
            {
                return 1;
            }
        }
        return 0;
    }
    else if(method == 1)
    {
        //iterate over the mask patterns
        for(int k = 0; k < thinLen; k++)
        {
            int flag = 0;
            for(int i = 0; i < SizeF * SizeF; i++)
            {
                //if any element mismatch break from loop and move to next mask
                if(*(ImageSection + i) != conditionalFilter[thin[k]][i])
                {
                    flag = 1;
                    break;
                }
            }
            if(flag == 0)
            {
                return 1;
            }
        }
        return 0;
    }
    else if(method == 2)
    {
        //iterate over the mask patterns
        for(int k = 0; k < skelLen; k++)
        {
            int flag = 0;
            for(int i = 0; i < SizeF * SizeF; i++)
            {
                //if any element mismatch break from loop and move to next mask
                if(*(ImageSection + i) != conditionalFilter[skel[k]][i])
                {
                    flag = 1;
                    break;
                }
            }
            if(flag == 0)
            {
                return 1;
            }
        }
        return 0;
    }
}

unsigned char applyUnconditionalMaskPatterns(char method, unsigned char *MSection, int SizeF)
{
    unsigned char unconditionalFilter[119][9] = 
    {
        //Spur
        {0,0,1,0,1,0,0,0,0},
        {1,0,0,0,1,0,0,0,0},
        //Single 4 connection
        {0,0,0,0,1,0,0,1,0},
        {0,0,0,0,1,1,0,0,0},
        //L cluster
        {0,0,1,0,1,1,0,0,0},
        {0,1,1,0,1,0,0,0,0},
        {1,1,0,0,1,0,0,0,0},
        {1,0,0,1,1,0,0,0,0},
        {0,0,0,1,1,0,1,0,0},
        {0,0,0,0,1,0,1,1,0},
        {0,0,0,0,1,0,0,1,1},
        {0,0,0,0,1,1,0,0,1},
        //4 connected offset
        {0,1,1,1,1,0,0,0,0},
        {1,1,0,0,1,1,0,0,0},
        {0,1,0,0,1,1,0,0,1},
        {0,0,1,0,1,1,0,1,0},
        //Spur corner cluster
        {0,1,1,0,1,0,1,0,0},
        {0,0,1,0,1,1,1,0,0},
        {0,1,1,0,1,1,1,0,0},
        {1,1,0,0,1,0,0,0,1},
        {1,0,0,1,1,0,0,0,1},
        {1,1,0,1,1,0,0,0,1},
        {0,0,1,1,1,0,1,0,0},
        {0,0,1,0,1,0,1,1,0},
        {0,0,1,1,1,0,1,1,0},
        {1,0,0,0,1,1,0,0,1},
        {1,0,0,0,1,0,0,1,1},
        {1,0,0,0,1,1,0,1,1},
        //Corner cluster
        {1,1,2,1,1,2,2,2,2},
        //Tee branch
        {2,1,0,1,1,1,2,0,0},
        {0,1,2,1,1,1,0,0,2},
        {0,0,2,1,1,1,0,1,2},
        {2,0,0,1,1,1,2,1,0},
        {2,1,2,1,1,0,0,1,0},
        {0,1,0,1,1,0,2,1,2},
        {0,1,0,0,1,1,2,1,2},
        {2,1,2,0,1,1,0,1,0},
        //Vee branch
        {1,2,1,2,1,2,1,0,0},
        {1,2,1,2,1,2,0,1,0},
        {1,2,1,2,1,2,0,0,1},
        {1,2,1,2,1,2,0,1,1},
        {1,2,1,2,1,2,1,1,0},
        {1,2,1,2,1,2,1,0,1},
        {1,2,1,2,1,2,1,1,1},
        {1,2,1,2,1,0,1,2,0},
        {1,2,0,2,1,1,1,2,0},
        {1,2,0,2,1,0,1,2,1},
        {1,2,0,2,1,1,1,2,1},
        {1,2,1,2,1,0,1,2,1},
        {1,2,1,2,1,1,1,2,0},
        {1,2,1,2,1,1,1,2,1},
        {1,0,0,2,1,2,1,2,1},
        {0,1,0,2,1,2,1,2,1},
        {0,0,1,2,1,2,1,2,1},
        {0,1,1,2,1,2,1,2,1},
        {1,0,1,2,1,2,1,2,1},
        {1,1,0,2,1,2,1,2,1},
        {1,1,1,2,1,2,1,2,1},
        {1,2,1,0,1,2,0,2,1},
        {0,2,1,1,1,2,0,2,1},
        {0,2,1,0,1,2,1,2,1},
        {0,2,1,1,1,2,1,2,1},
        {1,2,1,0,1,2,1,2,1},
        {1,2,1,1,1,2,0,2,1},
        {1,2,1,1,1,2,1,2,1},
        //Diagonal branch
        {2,1,0,0,1,1,1,0,2},
        {0,1,2,1,1,0,2,0,1},
        {2,0,1,1,1,0,0,1,2},
        {1,0,2,0,1,1,2,1,0},
        //Skel Spur
        {0,0,0,0,1,0,0,0,1},
        {0,0,0,0,1,0,1,0,0},
        {0,0,1,0,1,0,0,0,0},
        {1,0,0,0,1,0,0,0,0},
        //skel single 4 connection
        {0,0,0,0,1,0,0,1,0},
        {0,0,0,0,1,1,0,0,0},
        {0,0,0,1,1,0,0,0,0},
        {0,1,0,0,1,0,0,0,0},
        //skel L corner
        {0,1,0,0,1,1,0,0,0},
        {0,1,0,1,1,0,0,0,0},
        {0,0,0,0,1,1,0,1,0},
        {0,0,0,1,1,0,0,1,0},
        //skel corner cluster
        {1,1,2,1,1,2,2,2,2},
        {2,2,2,2,1,1,2,1,1},
        //skel tee branch
        {2,1,2,1,1,1,2,2,2},
        {2,1,2,1,1,2,2,1,2},
        {2,2,2,1,1,1,2,1,2},
        {2,1,2,2,1,1,2,1,2},
        //skel vee branch
        {1,2,1,2,1,2,1,0,0},
        {1,2,1,2,1,2,0,1,0},
        {1,2,1,2,1,2,0,0,1},
        {1,2,1,2,1,2,0,1,1},
        {1,2,1,2,1,2,1,0,1},
        {1,2,1,2,1,2,1,1,0},
        {1,2,1,2,1,2,1,1,1},
        {1,2,1,2,1,0,1,2,0},
        {1,2,0,2,1,1,1,2,0},
        {1,2,0,2,1,0,1,2,1},
        {1,2,0,2,1,1,1,2,1},
        {1,2,1,2,1,0,1,2,1},
        {1,2,1,2,1,1,1,2,0},
        {1,2,1,2,1,1,1,2,1},
        {1,0,0,2,1,2,1,2,1},
        {0,1,0,2,1,2,1,2,1},
        {0,0,1,2,1,2,1,2,1},
        {0,1,1,2,1,2,1,2,1},
        {1,0,1,2,1,2,1,2,1},
        {1,1,0,2,1,2,1,2,1},
        {1,1,1,2,1,2,1,2,1},
        {1,2,1,0,1,2,0,2,1},
        {0,2,1,1,1,2,0,2,1},
        {0,2,1,0,1,2,1,2,1},
        {0,2,1,1,1,2,1,2,1},
        {1,2,1,0,1,2,1,2,1},
        {1,2,1,1,1,2,0,2,1},
        {1,2,1,1,1,2,1,2,1},
        //skel diag branch
        {2,1,0,0,1,1,1,0,2},
        {0,1,2,1,1,0,2,0,1},
        {2,0,1,1,1,0,0,1,2},
        {1,0,2,0,1,1,2,1,0}
    };

    if(method == 0 || method == 1 || method == 3)
    {
        //iterate over the mask patterns
        for(int k = 0; k < 69; k++)
        {
            int flag = 0;                
                
            for(int i = 0; i < SizeF * SizeF; i++)
            {
                //if any element mismatch break from loop and move to next mask
                if(unconditionalFilter[k][i] == 2)
                {
                    continue;
                }
                if(*(MSection + i) != unconditionalFilter[k][i])
                {
                    flag = 1;
                    break;
                }
            }
            if(flag == 0)
            {
                return 1;
            }
        }
        return 0;
    }
    else if(method == 2)
    {
        //iterate over the mask patterns
        for(int k = 69; k < 119; k++)
        {
            int flag = 0;

            for(int i = 0; i < SizeF * SizeF; i++)
            {
                if(unconditionalFilter[k][i] == 2)
                {
                    continue;
                }
                //if any element mismatch break from loop and move to next mask
                if(*(MSection + i) != unconditionalFilter[k][i])
                {
                    flag = 1;
                    break;
                }
            }
            if(flag == 0)
            {
                return 1;
            }
        }
        return 0;
    }
}

unsigned char skelWithBridging(unsigned char *ImageSection)
{
    unsigned char L1 = (1 - *(ImageSection + 4)) && (1 - *(ImageSection + 5)) && *(ImageSection + 2) && (1 - *(ImageSection + 1)) && *(ImageSection) && (1 - *(ImageSection + 3)) && (1 - *(ImageSection + 6)) && (1 - *(ImageSection + 7)) && (1 - *(ImageSection + 8));
    unsigned char L2 = (1 - *(ImageSection + 4)) && (1 - *(ImageSection + 5)) && (1 - *(ImageSection + 2)) && (1 - *(ImageSection + 1)) && *(ImageSection) && (1 - *(ImageSection + 3)) && *(ImageSection + 6) && (1 - *(ImageSection + 7)) && (1 - *(ImageSection + 8));
    unsigned char L3 = (1 - *(ImageSection + 4)) && (1 - *(ImageSection + 5)) && (1 - *(ImageSection + 2)) && (1 - *(ImageSection + 1)) && (1 - *(ImageSection)) && (1 - *(ImageSection + 3)) && *(ImageSection + 6) && (1 - *(ImageSection + 7)) && *(ImageSection + 8);
    unsigned char L4 = (1 - *(ImageSection + 4)) && (1 - *(ImageSection + 5)) && *(ImageSection + 2) && (1 - *(ImageSection + 1)) && (1 - *(ImageSection)) && (1 - *(ImageSection + 3)) && (1 - *(ImageSection + 6)) && (1 - *(ImageSection + 7)) && *(ImageSection + 8);

    unsigned char PQ = L1 || L2 || L3 || L4;

    unsigned char P1 = (1 - *(ImageSection + 1)) && (1 - *(ImageSection + 7)) && (*(ImageSection) || *(ImageSection + 3) || *(ImageSection + 6)) && (*(ImageSection + 5) || *(ImageSection + 2) || *(ImageSection + 8)) && (1 - PQ);
    unsigned char P2 = (1 - *(ImageSection + 5)) && (1 - *(ImageSection + 3)) && (*(ImageSection + 2) || *(ImageSection + 1) || *(ImageSection)) && (*(ImageSection + 6) || *(ImageSection + 7) || *(ImageSection + 8)) && (1 - PQ);
    unsigned char P3 = (1 - *(ImageSection + 5)) && (1 - *(ImageSection + 7)) && *(ImageSection + 8) && (*(ImageSection + 1) || *(ImageSection) || *(ImageSection + 3));
    unsigned char P4 = (1 - *(ImageSection + 5)) && (1 - *(ImageSection + 1)) && *(ImageSection + 2) && (*(ImageSection + 3) || *(ImageSection + 6) || *(ImageSection + 7));
    unsigned char P5 = (1 - *(ImageSection + 1)) && (1 - *(ImageSection + 3)) && *(ImageSection) && (*(ImageSection + 5) || *(ImageSection + 7) || *(ImageSection + 8));
    unsigned char P6 = (1 - *(ImageSection + 3)) && (1 - *(ImageSection + 7)) && *(ImageSection + 6) && (*(ImageSection + 5) || *(ImageSection + 2) || *(ImageSection + 1));

    return P1 || P2 || P3 || P4 || P5 || P6;
}

int main(int argc, char *argv[])
{
    // Define file pointer and variables
    FILE *file;
    int BytesPerPixel = 3;
    int SizeW = 481;
    int SizeH = 321;
    int SizeF = 3;
    int prob = 0;
    int thresh = 127;
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
            thresh = atoi(argv[7]);
        }
    }

    // Allocate image data array
    unsigned char *Imagedata = (unsigned char *)malloc(SizeH * SizeW * BytesPerPixel * sizeof(unsigned char));
    // Read image (filename specified by first argument) into image data matrix
    if (!(file = fopen(argv[1], "rb")))
    {
        cout << "Cannot open file: " << argv[1] << endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), SizeH * SizeW * BytesPerPixel, file);
    fclose(file);

    ///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
    unsigned char *ImageGray = (unsigned char *)malloc(SizeH * SizeW * sizeof(unsigned char));
    unsigned char *ImageBW = (unsigned char *)malloc(SizeH * SizeW * sizeof(unsigned char));
    //normalise pixel values
    if(prob != 4)
    {
        for(int i = 0; i < SizeH; i++)
        {
            for(int j = 0; j < SizeW; j++)
            {
                *(ImageBW + i * SizeW + j) = *(Imagedata + i * SizeW + j) / 255;
            }
        }
    }
    else
    {
        //grayscale
        for(int i = 0; i < SizeH; i++)
        {
            for(int j = 0; j < SizeW; j++)
            {
                *(ImageGray + SizeW * i + j) = ((float)*(Imagedata + 0 * (SizeH * SizeW) + i * SizeW + j) + *(Imagedata + 1 * (SizeH * SizeW) + i * SizeW + j) + *(Imagedata + 2 * (SizeH * SizeW) + i * SizeW + j)) / 3;
            }
        }

        if (!(file = fopen("gray.raw", "wb")))
        {
            cout << "Cannot open file: " << "gray.raw" << endl;
            exit(1);
        }

        fwrite(ImageGray, sizeof(unsigned char), SizeH * SizeW, file);
        fclose(file);

        //threshold
        for(int i = 0; i < SizeH; i++)
        {
            for(int j = 0; j < SizeW; j++)
            {
                if(*(ImageGray + i * SizeW + j) > thresh)
                {
                    *(ImageBW + i * SizeW + j) = 255;
                }
                else
                {
                    *(ImageBW + i * SizeW + j) = 0;
                }
            }
        }
        if (!(file = fopen("bw.raw", "wb")))
        {
            cout << "Cannot open file: " << "bw.raw" << endl;
            exit(1);
        }

        fwrite(ImageBW, sizeof(unsigned char), SizeH * SizeW, file);
        fclose(file);
    }     

    if(prob == 3)
    {
        for(int i = 0; i < SizeH; i++)
        {
            for(int j = 0; j < SizeW; j++)
            {
                *(ImageBW + i * SizeW + j) = 1 - *(ImageBW + i * SizeW + j);
            }
        }
    }   
    
    unsigned char *M = (unsigned char *)malloc(SizeH * SizeW * sizeof(unsigned char));
    unsigned char *G = (unsigned char *)malloc(SizeH * SizeW * sizeof(unsigned char));
    unsigned char *Gdash = (unsigned char *)malloc(SizeH * SizeW * sizeof(unsigned char));

    for(int i = 0; i < SizeH; i++)
    {
        for(int j = 0; j < SizeW; j++)
        {
            *(Gdash + i * SizeW + j) = *(ImageBW + i * SizeW + j);
            *(G + i * SizeW + j) = *(ImageBW + i * SizeW + j);
        }
    }
    free(ImageBW);

    //except object problem
    if(prob != 4)
    {
        int change = 1;
        int ctr = 0;
        while(change)
        {
            ctr++;
            //conditional
            for(int i = 0; i < SizeH - 2; i++)
            {
                for(int j = 0; j < SizeW - 2; j++)
                {
                    unsigned char *ImageSection = (unsigned char *)malloc(SizeF * SizeF * sizeof(unsigned char));
                    for (int k = 0; k < SizeF; k++)
                    {
                        for (int l = 0; l < SizeF; l++)
                        {
                            *(ImageSection + k * SizeF + l) = *(Gdash + (i + k) * SizeW + j + l);
                        }
                    }
                    *(M + (i + 1) * SizeW + j + 1) = applyConditionalMaskPatterns(prob, ImageSection, SizeF);
                    free(ImageSection);
                }
            }

            //unconditional
            for(int i = 0; i < SizeH - 2; i++)
            {
                for(int j = 0; j < SizeW - 2; j++)
                {
                    unsigned char *ImageSection = (unsigned char *)malloc(SizeF * SizeF * sizeof(unsigned char));
                    for (int k = 0; k < SizeF; k++)
                    {
                        for (int l = 0; l < SizeF; l++)
                        {
                            *(ImageSection + k * SizeF + l) = *(M + (i + k) * SizeW + j + l);
                        }
                    }
                    *(Gdash + (i + 1) * SizeW + j + 1) = *(Gdash + (i + 1) * SizeW + j + 1) && ((1 - *(M + (i + 1) * SizeW + j + 1)) || applyUnconditionalMaskPatterns(prob, ImageSection, SizeF));
                    free(ImageSection);
                }
            }

            //check for change
            int flag = 0;
            for(int i = 0; i < SizeH * SizeW; i++)
            {
                if(*(Gdash + (i / SizeW) * SizeW + i % SizeW) != *(G + (i / SizeW) * SizeW + i % SizeW) / 255)
                {
                    flag = 1;
                    break;
                }
            }

            //assign the gdash to g
            for(int i = 0; i < SizeH * SizeW; i++)
            {
                *(G + (i / SizeW) * SizeW + i % SizeW) = 255 * (*(Gdash + (i / SizeW) * SizeW + i % SizeW));
            }
            
            //break if no change
            if(flag != 1)
            {
                change = 0;
            }
            //print iteration number
            cout << ctr;
        }
    }

    //extra bridging part for skel prob
    if(prob == 2)
    {
        //scale up pixels
        for(int i = 0; i < SizeH * SizeW; i++)
        {
            *(Gdash + (i / SizeW) * SizeW + i % SizeW) = 255 * (*(Gdash + (i / SizeW) * SizeW + i % SizeW));
        }

        if (!(file = fopen("unbridged.raw", "wb")))
        {
            cout << "Cannot open file: " << "unbridged.raw" << endl;
            exit(1);
        }

        fwrite(Gdash, sizeof(unsigned char), SizeH * SizeW * BytesPerPixel, file);
        fclose(file);

        //scale back down
        for(int i = 0; i < SizeH * SizeW; i++)
        {
            *(Gdash + (i / SizeW) * SizeW + i % SizeW) = *(Gdash + (i / SizeW) * SizeW + i % SizeW) / 255;
        }

        //apply bridging
        for(int i = 0; i < SizeH - 2; i++)
        {
            for(int j = 0; j < SizeW - 2; j++)
            {
                unsigned char *ImageSection = (unsigned char *)malloc(SizeF * SizeF * sizeof(unsigned char));
                for (int k = 0; k < SizeF; k++)
                {
                    for (int l = 0; l < SizeF; l++)
                    {
                        *(ImageSection + k * SizeF + l) = *(Gdash + (i + k) * SizeW + j + l);
                    }
                }
                *(Gdash + (i + 1) * SizeW + j + 1) = *(Gdash + (i + 1) * SizeW + j + 1) || skelWithBridging(ImageSection);
                free(ImageSection);
            }
        }
    }
    // Write image data (filename specified by second argument) from image data matrix

    //write fresh image values into the final image array
    for(int i = 0; i < SizeH * SizeW; i++)
    {
        *(G + (i / SizeW) * SizeW + i % SizeW) = 255 * (*(Gdash + (i / SizeW) * SizeW + i % SizeW));
    }
    free(Gdash);
    //correction for deer- remove the first 2 white pixels and then or with orig image
    if(prob == 3)
    {
        int cnt = 0;
        for(int i = 0; i < SizeH - 2; i++)
        {
            for(int j = 0; j < SizeW - 2; j++)
            {
                if((*(G + (i) * SizeW + j) == 0 && *(G + (i) * SizeW + j + 1) == 0 && *(G + (i) * SizeW + j + 2) == 0 && *(G + (i + 1) * SizeW + j) == 0 && *(G + (i + 1) * SizeW + j + 1) == 255 && *(G + (i + 1) * SizeW + j + 2) == 0 && *(G + (i + 2) * SizeW + j) == 0 && *(G + (i + 2) * SizeW + j + 1) == 0 && *(G + (i + 2) * SizeW + j + 2) == 0) == 255)
                {
                    cnt++;
                    if(cnt <= 2)
                    {
                        cout << "ind:" << i << j << " ";
                        *(G + (i + 1) * SizeW + j + 1) = 0;
                        continue;
                    }
                    *(Imagedata + (i + 1) * SizeW + j + 1) = 255;
                }   
            }
        }

        if (!(file = fopen("CorrectedDeer.raw", "wb")))
        {
            cout << "Cannot open file: " << "CorrectedDeer.raw" << endl;
            exit(1);
        }
        fwrite(Imagedata, sizeof(unsigned char), SizeH * SizeW * BytesPerPixel, file);
        fclose(file);
    }

    if (!(file = fopen(argv[2], "wb")))
    {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(G, sizeof(unsigned char), SizeH * SizeW * BytesPerPixel, file);
    fclose(file);
    free(G);
    free(M);
    free(ImageGray);
    free(Imagedata);
    return 0;
}