import numpy as np
import math

def merge(data, iStart, iMiddle, iEnd):
    mergedData = []
    i1 = iStart
    i2 = iMiddle
    while i1 < iMiddle or i2 < iEnd:
        if i2 == iEnd or (i1 < iMiddle and data[i1] < data[i2]):
            mergedData.append(data[i1])
            i1 += 1
        else:
            mergedData.append(data[i2])
            i2 += 1

    data[iStart:iEnd] = mergedData

def mergeSort(data, iStart=None, iEnd=None):
    iStart = iStart if iStart else 0
    iEnd = iEnd if iEnd else len(data)
    if iEnd - iStart < 2:
        return data[0]
    else:
        iMiddle = math.ceil(iStart + ((iEnd - iStart) / 2))
        mergeSort(data, iStart, iMiddle)
        mergeSort(data, iMiddle, iEnd)
        merge(data, iStart, iMiddle, iEnd)


data = np.random.randint(10, size=10)
print(data)
mergeSort(data)
print(data)