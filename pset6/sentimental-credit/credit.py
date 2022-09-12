from cs50 import get_int
import sys


def main():
    # Get card number
    card_num = get_int("Number: ")
    card_num = str(card_num)

    # Check number length
    length = len(card_num)
    if (length > 16 or length < 13):
        invalid_card()

    # Check Luhn's algorithm

    is_valid = luhn_verify(card_num)
    if (not is_valid):
        invalid_card()

    # Get card company
    company = get_company(card_num)

    # Print company
    print(company)
    sys.exit(0)

# Verify Luhn's algorithm


def luhn_verify(credit_num):
    # Initialize accumulator
    sum = 0

    # For every number, backwards
    for index, num in enumerate(reversed(credit_num)):
        num = int(num)

        if (index % 2 != 0):
            double_num = num * 2
            if (double_num >= 10):
                sum += 1 + double_num % 10
            else:
                sum += double_num
        else:
            sum += num

    # If final digit is 0, valid
    if (sum % 10 == 0):
        return True
    else:
        return False


def get_company(card_num):
    ab = card_num[0] + card_num[1]

    if ab in ["34", "37"]:
        return "AMEX"
    if ab in ['51', '52', '53', '54', '55']:
        return "MASTERCARD"
    if ab[0] == '4':
        return "VISA"

    invalid_card()


def invalid_card():
    print("INVALID")
    sys.exit(1)


main()
