from cs50 import get_int
from math import floor
no = get_int("enter your credit card no. : ")

sum1,sum2 = 0,0
if floor(no/10**17)==0:
    for i in range(15,0,-2):
        sum11 = floor(no/(10**i))
        sum1 = sum1 + (2*(sum11//10))

    for j in range(16,-1,-2):
        sum22 = floor(no/(10**i))
        sum2 = sum2 + (sum22//10)
else:
    for i in range(14,1,-2):
        sum11 = floor(no/(10**i))
        sum1 = sum1 + (2*(sum11//10))

    for j in range(15,0,-2):
        sum22 = floor(no/(10**i))
        sum2 = sum2 + (sum22//10)

total = sum1 + sum2

if((total//10)==0):
    print('valid credit card')

else:
    print('invalid')