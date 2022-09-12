#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

bool validate_key(int argc, string key);
void encypher_text(string text, string key);

int main(int argc, string argv[])
{
    // Validate key
    bool is_valid_key = validate_key(argc, argv[1]);
    if (!is_valid_key)
    {
        return 1;
    }

    // Prompt for text
    string text = get_string("plaintext: ");
    printf("ciphertext: ");

    // Encipher text
    encypher_text(text, argv[1]);
    printf("%s\n", text);
}








void encypher_text(string text, string key)
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
            text[i] = toupper(key[index]);
        }

        // Shift lowercase letterss
        else if (islower(c))
        {
            int index = (int) c - 'a';
            text[i] = tolower(key[index]);
        }
    }
}

bool validate_key(int argc, string key)
{
    // Validate only 2 argv
    if (argc != 2)
    {
        printf("Usage ./caesar KEY \n");
        return false;
    }

    // Validate keylength = 26
    if (strlen(key) != 26)
    {
        printf("Key must be 26 characters long\n");
        return false;
    }

    for (int i = 0; key[i] != '\0'; i++)
    {
        // Validate if there is a digit
        if (isdigit(key[i]))
        {
            printf("Key cannot contain digits\n");
            return false;
        }

        // Validate repeated characters
        for (int j = 0; key[j] != '\0'; j++)
        {
            if (j == i)
            {
                continue;
            }
            if (tolower(key[i]) == tolower(key[j]))
            {
                printf("Key cannot contain repeated characters\n");
                return false;
            }
        }
    }

    return true;
}