#include "helpers.h"
#include <math.h>
#include <stdio.h>

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

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
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

            // Calculate sepia colors
            int sepiaRed = round(.393 * red + .769 * green + .189 * blue);
            int sepiaGreen = round(.349 * red + .686 * green + .168 * blue);
            int sepiaBlue = round(.272 * red + .534 * green + .131 * blue);

            // Set the average
            image[i][j].rgbtRed = sepiaRed > 255 ? 255 : sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen > 255 ? 255 : sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue  > 255 ? 255 : sepiaBlue;
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
