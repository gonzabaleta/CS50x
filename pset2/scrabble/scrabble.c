#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    // printf("%i", (int) 'A');
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int compute_score(string word)
{
    // Initialize score variable
    int score = 0;

    // Loop through every character of word
    int i = 0;
    while (word[i] != '\0')
    {
        // If it's lowercase, subtract 32 to ASCII value
        if (islower(word[i]))
        {
            int char_index = (int) word[i] - 'A' - 32;
            if (char_index <= 25 && char_index >= 0)
            {
                int char_score = POINTS[char_index];
                score += char_score;
            }
        }
        // Else, subtract just A
        else
        {
            int char_index = (int) word[i] - 'A';
            if (char_index <= 25 && char_index >= 0)
            {
                int char_score = POINTS[char_index];
                score += char_score;
            }
        }
        i++;
    }

    return score;
}
