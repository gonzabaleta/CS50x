from cs50 import get_string


def main():
    # Get text
    text = get_string("Text: ")

    # Calculate letters, words, and sentences
    letters, words, sentences = count_text_data(text)

    # Calculate index
    index = calculate_index(letters, words, sentences)

    # Print index
    print_grade(index)


# Calculate letters, words, and sentences


def count_text_data(text):
    letters = 0
    words = 1
    sentences = 0

    for c in text:
        if c == " ":
            words += 1
        elif c in ['.', '?', '!']:
            sentences += 1
        elif c.isalpha():
            letters += 1

    return [letters, words, sentences]


# Calculate index


def calculate_index(letters, words, sentences):

    L = letters / words * 100
    S = sentences / words * 100

    return 0.0588 * L - 0.296 * S - 15.8

# Print grade


def print_grade(index):
    if index > 16:
        print("Grade 16+")
    elif index < 1:
        print("Before Grade 1")
    else:
        print("Grade ", round(index))


main()