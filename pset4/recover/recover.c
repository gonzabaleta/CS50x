#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int is_jpeg_start(uint8_t buffer[]);

const int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("usage: ./recover filename\n");
        exit(1);
    }

    FILE *card = fopen(argv[1], "r");

    if (card == NULL)
    {
        printf("Could not open file\n");
        exit(1);
    }

    uint8_t buffer[BLOCK_SIZE];
    int jpeg_count = 0;
    char *filename = malloc(8);
    FILE *write_file;

    while (fread(buffer, 1, BLOCK_SIZE, card) == BLOCK_SIZE)
    {
        if (is_jpeg_start(buffer))
        {
            if (jpeg_count != 0)
            {
                fclose(write_file);
            }

            sprintf(filename, "%03i.jpg", jpeg_count);
            write_file = fopen(filename, "w");
            fwrite(buffer, 1, BLOCK_SIZE, write_file);
            jpeg_count++;

        }
        else if (jpeg_count > 0)
        {
            fwrite(buffer, 1, BLOCK_SIZE, write_file);
        }

    }

    fclose(write_file);
    fclose(card);

    free(filename);
}

int is_jpeg_start(uint8_t buffer[])
{
    if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
    {
        return 1;
    }
    return 0;
}