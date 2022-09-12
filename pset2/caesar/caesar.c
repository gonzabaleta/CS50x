#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

bool validate_key(int argc, string argv[]);
void encypher_text(string text, int key);

int main(int argc, string argv[])
{
    // Validate key
    bool is_valid_key = validate_key(argc, argv);
    if (!is_valid_key)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // Convert key to int
    int key = atoi(argv[1]);

    // Prompt for text
    string text = get_string("plaintext: ");
    printf("ciphertext: ");

    // Encipher text
    encypher_text(text, key);
    printf("%s\n", text);
}






void encypher_text(string text, int key)
{

    for (int i = 0; text[i] != '\0'; i++)
    {
        char c = text[i];

        // Ignore non-alpha characters
        if (!isalpha(c))
        {
            continue;
        }

        // Shift uppercase letters
        if (isupper(c))
        {
            int index = (int) c - 'A';
            int new_index = (index + key) % 26 + 'A';
            text[i] = (char) new_index;
        }

        // Shift lowercase letters
        if (islower(c))
        {
            int index = (int) c - 'a';
            int new_index = (index + key) % 26 + 'a';
            text[i] = (char) new_index;
        }
    }
}

bool validate_key(int argc, string argv[])
{
    if (argc != 2)
    {
        return false;
    }

    string key = argv[1];

    for (int i = 0; key[i] != '\0'; i++)
    {
        bool is_digit = isdigit(key[i]);

        if (!is_digit)
        {
            return false;
        }
    }

    return true;
}