from cs50 import get_int

n = get_int('how long do you want the pyramid :')
x = n
for i in range(n):
    for k in range(x):
        print("  ",end="")
    y = n-x
    x = x-1
    for j in range(y):
        print("__",end='')
    print('  ',end='')
    for l in range(y):
        print("__",end='')
    print()