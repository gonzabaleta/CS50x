// Implements a dictionary's functionality
// ./speller dictionaries/small texts/cat.txt

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int ASCII_CHARS = 26;
const unsigned int N = ASCII_CHARS * ASCII_CHARS;

// Hash table
node *table[N];

// Amount of words in the dictionary
int wordAmount = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Get hash from the word
    int hashNum = hash(word);

    // For every node in the index:
    for (node *temp = table[hashNum]; temp != NULL; temp = temp->next)
    {
        // If word is found, return true
        if (strcasecmp(word, temp->word) == 0)
        {
            return true;
        }
    }

    // If not found, return false
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{

    // If the word is 1 character long, has it as if the character was repeated twice
    if (strlen(word) == 1)
    {
        unsigned int ascii = tolower(word[0]) - 'a' + 1;
        return ascii * ASCII_CHARS - 1;
    }

    unsigned int ascii1 = tolower(word[0]) - 'a' + 1;
    unsigned int ascii2 = tolower(word[1]) - 'a' + 1;

    return ascii1 * ascii2 - 1;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary
    FILE *dict = fopen(dictionary, "r");

    // If error, return
    if (dict == NULL)
    {
        return false;
    }

    // Define word buffer
    char word[LENGTH + 1];

    // Read all words
    while (fscanf(dict, "%s", word) != EOF)
    {
        // Hash word
        unsigned int hashNum = hash(word);

        // Allocate space for new node
        node *newNode = malloc(sizeof(node));
        // Copy the word to the node's word field
        strcpy(newNode->word, word);

        // If first node of the hash:
        if (table[hashNum] == NULL)
        {
            // Set next pointer to NULL
            newNode->next = NULL;
            // Set table's pointer to the new node
            table[hashNum] = newNode;
        }
        // If NOT the first node in the hash
        else
        {
            // Define temp node that points to the first node
            node *temp = table[hashNum];
            // Set the table to point to the new node
            table[hashNum] = newNode;
            // Set the new node to point to temp
            newNode->next = temp;
        }

        // Increase word amount by 1
        wordAmount++;
    }

    // Close dictionary and return true
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return wordAmount;

}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // For each hash:
    for (int i = 0; i < N; i++)
    {
        // Definte temp node
        node *tmp = NULL;

        // For each node:
        for (node *cursor = table[i]; cursor != NULL; cursor = cursor->next)
        {
            // First free temp, and then set temp to the cursor.
            free(tmp);
            tmp = cursor;
        }


        // Free temp when cursor == NULL
        free(tmp);
    }
    return true;
}