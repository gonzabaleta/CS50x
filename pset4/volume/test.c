// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "r");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file
    uint8_t* headerp = malloc(HEADER_SIZE);
    fread(headerp, sizeof(uint8_t), HEADER_SIZE, output);
    for (int i = 0; i < HEADER_SIZE; i++)
    {
        printf("%i\n", headerp[i]);
    }

    // TODO: Read samples from input file and write updated data to output file

    // Close files
    fclose(input);
    fclose(output);

    // free(headerp);
}
