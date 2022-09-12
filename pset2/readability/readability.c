#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>

float calculate_grade(string text);

int main(void)
{
    string text = get_string("Text: ");

    float grade = calculate_grade(text);

    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        int rounded_grade = round(grade);
        printf("Grade %i\n", rounded_grade);
    }
}

float calculate_grade(string text)
{
    // Initialize letters words and sentences
    int letters = 0;
    int words = 0;
    int sentences = 0;

    int i = 0;
    while (text[i] != '\0')
    {
        char c = text[i];

        // If it's a letter, add to letters
        if (isalpha(c))
        {
            letters++;
        }

        // If it's a space, add to words
        if (c == ' ')
        {
            words++;
        }

        // If its a . ! or ?, add to sentences.
        if (c == '.' || c == '!' || c == '?')
        {
            sentences++;
        }
        i++;
    }

    // We add one more word per the logic
    words++;

    // Calculate formula's variables
    float letter_avg = (float) letters / words * 100;
    float sentence_avg = (float) sentences / words * 100;

    float grade = 0.0588 * letter_avg - 0.296 * sentence_avg - 15.8;

    return grade;
}