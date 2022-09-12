from cs50 import get_int

# Keep repromptin till getting integer between 1 - 8
while True:
    h = get_int("Height: ")
    if h <= 8 and h >= 1:
        break

z = 1
s = h-1
for x in range(h):
    # Print spaces
    for k in range(s):
        print(" ", end="")
    # Print hashes
    for y in range(z):
        print("#", end="")
    # Print newline
    print("")
    z += 1
    s -= 1