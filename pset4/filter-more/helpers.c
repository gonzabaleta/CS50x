#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop for every row
    for (int i = 0; i < height; i++)
    {
        // Loop for every column
        for (int j = 0; j < width; j++)
        {
            // Store RGB in variables
            BYTE red = image[i][j].rgbtRed;
            BYTE green = image[i][j].rgbtGreen;
            BYTE blue = image[i][j].rgbtBlue;

            // Calculate average
            int average = round((float)(red + green + blue) / 3);

            // Set the average
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop for every row
    for (int i = 0; i < height; i++)
    {
        // Loop for every column until the middle of the row
        for (int j = 0; j < width / 2; j++)
        {
            // Swap RGB values
            RGBTRIPLE rgb = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = rgb;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    // Loop for every row
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int redSum = 0, greenSum = 0, blueSum = 0, pixels = 0;

            for (int k = (i - 1); k <= (i + 1); k++)
            {
                if (k < 0 || k >= height)
                {
                    continue;
                }

                for (int l = (j - 1); l <= (j + 1); l++)
                {
                    if (l < 0 || l >= width)
                    {
                        continue;
                    }
                    redSum += image[k][l].rgbtRed;
                    greenSum += image[k][l].rgbtGreen;
                    blueSum += image[k][l].rgbtBlue;
                    pixels++;
                }
            }

            int averageRed = round((float)redSum / pixels);
            int averageGreen = round((float)greenSum / pixels);
            int averageBlue = round((float)blueSum / pixels);

            copy[i][j].rgbtRed = averageRed;
            copy[i][j].rgbtGreen = averageGreen;
            copy[i][j].rgbtBlue = averageBlue;

        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = copy[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int Gx_red;
            int Gx_green;
            int Gx_blue;

            int Gy_red;
            int Gy_green;
            int Gy_blue;

            if (j == 0 && i == 0)
            {
                Gx_red = image[i][j + 1].rgbtRed*2 + image[i + 1][j + 1].rgbtRed;
                Gx_green = image[i][j + 1].rgbtGreen*2 + image[i + 1][j + 1].rgbtGreen;
                Gx_blue = image[i][j + 1].rgbtBlue*2 + image[i + 1][j + 1].rgbtBlue;

                Gy_red = image[i + 1][j].rgbtRed*2 + image[i + 1][j + 1].rgbtRed;
                Gy_green = image[i + 1][j].rgbtGreen*2 + image[i + 1][j + 1].rgbtGreen;
                Gy_blue = image[i + 1][j].rgbtBlue*2 + image[i + 1][j + 1].rgbtBlue;
            }

            else if (j == 0 && i == (height - 1))
            {
                Gx_red = image[i][j + 1].rgbtRed*2 + image[i-1][j + 1].rgbtRed;
                Gx_green = image[i][j + 1].rgbtGreen*2 + image[i-1][j + 1].rgbtGreen;
                Gx_blue = image[i][j + 1].rgbtBlue*2 + image[i-1][j + 1].rgbtBlue;

                Gy_red = image[i-1][j].rgbtRed*-2 + image[i-1][j + 1].rgbtRed*-1;
                Gy_green = image[i-1][j].rgbtGreen*-2 + image[i-1][j + 1].rgbtGreen*-1;
                Gy_blue = image[i-1][j].rgbtBlue*-2 + image[i-1][j + 1].rgbtBlue*-1;
            }

            else if (j == (width - 1) && i == 0)
            {
                Gx_red = image[i][j-1].rgbtRed*-2 + image[i + 1][j-1].rgbtRed*-1;
                Gx_green = image[i][j-1].rgbtGreen*-2 + image[i + 1][j-1].rgbtGreen*-1;
                Gx_blue = image[i][j-1].rgbtBlue*-2 + image[i + 1][j-1].rgbtBlue*-1;

                Gy_red = image[i + 1][j].rgbtRed*2 + image[i + 1][j-1].rgbtRed;
                Gy_green = image[i + 1][j].rgbtGreen*2 + image[i + 1][j-1].rgbtGreen;
                Gy_blue = image[i + 1][j].rgbtBlue*2 + image[i + 1][j-1].rgbtBlue;
            }

            else if (j == (width - 1) && i == (height - 1))
            {
                Gx_red = image[i][j-1].rgbtRed*-2 + image[i-1][j-1].rgbtRed*-1;
                Gx_green = image[i][j-1].rgbtGreen*-2 + image[i-1][j-1].rgbtGreen*-1;
                Gx_blue = image[i][j-1].rgbtBlue*-2 + image[i-1][j-1].rgbtBlue*-1;

                Gy_red = image[i-1][j].rgbtRed*-2 + image[i-1][j-1].rgbtRed*-1;
                Gy_green = image[i-1][j].rgbtGreen*-2 + image[i-1][j-1].rgbtGreen*-1;
                Gy_blue = image[i-1][j].rgbtBlue*-2 + image[i-1][j-1].rgbtBlue*-1;
            }

            else if (j == 0)
            {
                Gx_red = image[i][j + 1].rgbtRed*2 + image[i-1][j + 1].rgbtRed + image[i + 1][j + 1].rgbtRed;
                Gx_green = image[i][j + 1].rgbtGreen*2 + image[i-1][j + 1].rgbtGreen + image[i + 1][j + 1].rgbtGreen;
                Gx_blue = image[i][j + 1].rgbtBlue*2 + image[i-1][j + 1].rgbtBlue + image[i + 1][j + 1].rgbtBlue;

                Gy_red = image[i-1][j].rgbtRed*-2 + image[i-1][j + 1].rgbtRed*-1 + image[i + 1][j].rgbtRed*2 + image[i + 1][j + 1].rgbtRed;
                Gy_green = image[i-1][j].rgbtGreen*-2 + image[i-1][j + 1].rgbtGreen*-1 + image[i + 1][j].rgbtGreen*2 + image[i + 1][j + 1].rgbtGreen;
                Gy_blue = image[i-1][j].rgbtBlue*-2 + image[i-1][j + 1].rgbtBlue*-1 + image[i + 1][j].rgbtBlue*2 + image[i + 1][j + 1].rgbtBlue;
            }

            else if (j == (width - 1))
            {
                Gx_red = image[i][j-1].rgbtRed*-2 + image[i-1][j-1].rgbtRed*-1 + image[i + 1][j-1].rgbtRed*-1;
                Gx_green = image[i][j-1].rgbtGreen*-2 + image[i-1][j-1].rgbtGreen*-1 + image[i + 1][j-1].rgbtGreen*-1;
                Gx_blue = image[i][j-1].rgbtBlue*-2 + image[i-1][j-1].rgbtBlue*-1 + image[i + 1][j-1].rgbtBlue*-1;

                Gy_red = image[i-1][j].rgbtRed*-2 + image[i-1][j-1].rgbtRed*-1 + image[i + 1][j].rgbtRed*2 + image[i + 1][j-1].rgbtRed;
                Gy_green = image[i-1][j].rgbtGreen*-2 + image[i-1][j-1].rgbtGreen*-1 + image[i + 1][j].rgbtGreen*2 + image[i + 1][j-1].rgbtGreen;
                Gy_blue = image[i-1][j].rgbtBlue*-2 + image[i-1][j-1].rgbtBlue*-1 + image[i + 1][j].rgbtBlue*2 + image[i + 1][j-1].rgbtBlue;
            }

            else if (i == 0)
            {
                Gx_red = image[i][j + 1].rgbtRed*2 + image[i][j-1].rgbtRed*-2 + image[i + 1][j + 1].rgbtRed + image[i + 1][j-1].rgbtRed*-1;
                Gx_green = image[i][j + 1].rgbtGreen*2 + image[i][j-1].rgbtGreen*-2 + image[i + 1][j + 1].rgbtGreen + image[i + 1][j-1].rgbtGreen*-1;
                Gx_blue = image[i][j + 1].rgbtBlue*2 + image[i][j-1].rgbtBlue*-2 + image[i + 1][j + 1].rgbtBlue + image[i + 1][j-1].rgbtBlue*-1;

                Gy_red = image[i + 1][j].rgbtRed*2 + image[i + 1][j-1].rgbtRed + image[i + 1][j + 1].rgbtRed;
                Gy_green = image[i + 1][j].rgbtGreen*2 + image[i + 1][j-1].rgbtGreen + image[i + 1][j + 1].rgbtGreen;
                Gy_blue = image[i + 1][j].rgbtBlue*2 + image[i + 1][j-1].rgbtBlue + image[i + 1][j + 1].rgbtBlue;
            }

            else if (i == (height - 1))
            {
                Gx_red = image[i][j + 1].rgbtRed*2 + image[i][j-1].rgbtRed*-2 + image[i-1][j + 1].rgbtRed + image[i-1][j-1].rgbtRed*-1;
                Gx_green = image[i][j + 1].rgbtGreen*2 + image[i][j-1].rgbtGreen*-2 + image[i-1][j + 1].rgbtGreen + image[i-1][j-1].rgbtGreen*-1;
                Gx_blue = image[i][j + 1].rgbtBlue*2 + image[i][j-1].rgbtBlue*-2 + image[i-1][j + 1].rgbtBlue + image[i-1][j-1].rgbtBlue*-1;

                Gy_red = image[i-1][j].rgbtRed*-2 + image[i-1][j-1].rgbtRed*-1 + image[i-1][j + 1].rgbtRed*-1;
                Gy_green = image[i-1][j].rgbtGreen*-2 + image[i-1][j-1].rgbtGreen*-1 + image[i-1][j + 1].rgbtGreen*-1;
                Gy_blue = image[i-1][j].rgbtBlue*-2 + image[i-1][j-1].rgbtBlue*-1 + image[i-1][j + 1].rgbtBlue*-1;
            }

            else
            {
                Gx_red = image[i][j + 1].rgbtRed*2 + image[i][j-1].rgbtRed*-2 + image[i + 1][j + 1].rgbtRed + image[i + 1][j-1].rgbtRed*-1 + image[i-1][j + 1].rgbtRed + image[i-1][j-1].rgbtRed*-1;
                Gx_green = image[i][j + 1].rgbtGreen*2 + image[i][j-1].rgbtGreen*-2 + image[i + 1][j + 1].rgbtGreen + image[i + 1][j-1].rgbtGreen*-1 + image[i-1][j + 1].rgbtGreen + image[i-1][j-1].rgbtGreen*-1;
                Gx_blue = image[i][j + 1].rgbtBlue*2 + image[i][j-1].rgbtBlue*-2 + image[i + 1][j + 1].rgbtBlue + image[i + 1][j-1].rgbtBlue*-1 + image[i-1][j + 1].rgbtBlue + image[i-1][j-1].rgbtBlue*-1;

                Gy_red = image[i + 1][j].rgbtRed*2 + image[i + 1][j + 1].rgbtRed + image[i + 1][j-1].rgbtRed + image[i-1][j].rgbtRed*-2 + image[i-1][j-1].rgbtRed*-1 + image[i-1][j + 1].rgbtRed*-1;
                Gy_green = image[i + 1][j].rgbtGreen*2 + image[i + 1][j + 1].rgbtGreen + image[i + 1][j-1].rgbtGreen + image[i-1][j].rgbtGreen*-2 + image[i-1][j-1].rgbtGreen*-1 + image[i-1][j + 1].rgbtGreen*-1;
                Gy_blue = image[i + 1][j].rgbtBlue*2 + image[i + 1][j + 1].rgbtBlue + image[i + 1][j-1].rgbtBlue + image[i-1][j].rgbtBlue*-2 + image[i-1][j-1].rgbtBlue*-1 + image[i-1][j + 1].rgbtBlue*-1;
            }

            float tmp_r = sqrt(pow(Gx_red, 2) + pow(Gy_red, 2));
            if (tmp_r > 255)
            {
                tmp_r = 255;
            }
            tmp_r = round(tmp_r);
            copy[i][j].rgbtRed = tmp_r;

            float tmp_g = sqrt(pow(Gx_green, 2) + pow(Gy_green, 2));
            if (tmp_g > 255)
            {
                tmp_g = 255;
            }
            tmp_g = round(tmp_g);
            copy[i][j].rgbtGreen = tmp_g;

            float tmp_b = sqrt(pow(Gx_blue, 2) + pow(Gy_blue, 2));
            if (tmp_b > 255)
            {
                tmp_b = 255;
            }
            tmp_b = round(tmp_b);
            copy[i][j].rgbtBlue = tmp_b;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = copy[i][j];
        }
    }
    return;
}

