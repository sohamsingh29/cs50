from helpers import distances
from sys import argv

s1 = argv[1]
s2 = argv[2]
cost = distances(s1,s2)
print(cost[len(s1)][len(s2)])