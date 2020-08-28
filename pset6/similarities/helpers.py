from enum import Enum


class Operation(Enum):
    """Operations"""
    DELETED = 1
    INSERTED = 2
    SUBSTITUTED = 3

    def __str__(self):
        return str(self.name.lower())


def distances(a, b):
    """Calculate edit distance from a to b"""
    cost = [[(0,None) for x in range((len(b)+1))] for y in range((len(a)+1))]

    for i in range(1,len(a)+1):
        cost[i][0] = (i,Operation.DELETED)

    for j in range(1,len(b)+1):
        cost[0][j] = (j,Operation.INSERTED)

    for k in range(1,len(a)+1):

        for l in range(1,len(b)+1):

            delet, _ = cost[k-1][l]
            inssert, _ = cost[k][l-1]
            sub, _ = cost[k-1][l-1]

            delet +=1

            inssert +=1

            if a[k-1]!=b[l-1]:
                sub += 1

            if inssert<=delet and inssert<=sub:
                cost[k][l] = (inssert,Operation.INSERTED)

            elif delet<=inssert and delet<=sub:
                cost[k][l] = (delet,Operation.DELETED)
            else :
                cost[k][l] = (sub,Operation.SUBSTITUTED)

    return cost
