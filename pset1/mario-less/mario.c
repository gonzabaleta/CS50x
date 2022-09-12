#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;

    // Get height from user
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    // Loop for every row
    for (int i = 0; i < height; i++)
    {
        // For every row, print height - row spaces
        for (int j = 0; j < height - 1 - i; j++)
        {
            printf(" ");
        }

        // For every row, print row hashes
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }

        // For every row, print a new line
        printf("\n");
    }

}