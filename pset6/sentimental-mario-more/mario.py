from cs50 import get_int

# Get height
height = 0
while (height <= 0 or height > 8):
    height = get_int("Height: ")

# For each row
for i in range(height):
    # Print first pyramid
    for j in range(height):
        if (j + 1 > height - (i + 1)):
            print("#", end="")
        else:
            print(" ", end="")

    # Print middle space
    print("  ", end="")

    # Print second pyramid
    for j in range(height):
        if (j + 1 > height - (i + 1)):
            print("#", end="")
    print()