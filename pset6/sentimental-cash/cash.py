from cs50 import get_float

while True:
    owed = get_float("Change owed: ")
    if owed >= 0:
        break

owed = owed * 100

coins = 0

while owed > 0:
    if owed >= 25:
        owed -= 25
        coins += 1

    elif owed >= 10:
        owed -= 10
        coins += 1

    elif owed >= 5:
        owed -= 5
        coins += 1

    elif owed >= 1:
        owed -= 1
        coins += 1

print(f"{coins}")
