#include <cs50.h>
#include <stdio.h>
#include <string.h>

bool is_valid(string num);

int main(void)
{
    long num = get_long("Number: ");
    char num_str[256];
    sprintf(num_str, "%ld", num);
    bool valid = is_valid(num_str);
    if (valid == false)
    {
        printf("INVALID\n");
        return 0;
    }

    int len = strlen(num_str);
    char d1 = num_str[0];
    char d2 = num_str[1];

    if (len == 15 && d1 == '3' && (d2 == '7' || d2 == '4'))
    {
        printf("AMEX\n");
    }
    else if (len == 16 && d1 == '5' && (d2 == '1' || d2 == '2' || d2 == '3' || d2 == '4' || d2 == '5'))
    {
        printf("MASTERCARD\n");
    }
    else if ((len == 13 || len == 16) && num_str[0] == '4')
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }

    return 0;
}

bool is_valid(string num)
{
    int length = strlen(num) - 1;

    // Initialize sum variables
    int double_sum = 0;
    int normal_sum = 0;

    // We iterate for each character of the number
    for (int i = 0; i <= length; i++)
    {
        // We transform the character to an int
        int digit = num[length - i] - '0';

        if (i % 2 == 0)
        {
            // If it's an even position, we just add it to normal_sum
            normal_sum += digit;
        }
        else
        {
            // If it's an odd position, we multiply by 2, add both digits, and add them it to double_sum
            int to_sum = digit * 2;
            int first_digit = to_sum / 10;
            int last_digit = to_sum % 10;
            double_sum += (first_digit + last_digit);
        }
    }

    // We initialize result
    int result = double_sum + normal_sum;

    // If it's multiple of 10 (ends with 0), return true
    if (result % 10 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}