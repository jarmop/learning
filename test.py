# calculation operators
# ** calculates power
# 2**3 = 8

# types
# int, float as usual

# decimal

# fraction

# STRINGS

# print('str\ning')
# print(r'str\ning')
#
# print("""\
# Usage: thingy [OPTIONS]
#      -h                        Display this usage message
#      -H hostname               Hostname to connect to
# """)

# LISTS

# a = [1,2,3,4]

# a = []
# for i in range(10):
#     a.append([1,2,3,4])
# print(a)
#
# print(a[:-4])

# ARRAY
# from array import array
# arr = array([1,2,3,4]);
# print(arr)

# NumPy ARRAYS

import numpy as np

# print(np.ndarray((3,3)))
# print(np.ones((3,3)))
# print(np.arange())
# print(np.array([[1,2,3],[4,5,6],[7,8,9]]))

# myArray=np.array([[1,2,3],[4,5,6],[7,8,9]])
# print(myArray)
# print(myArray[1,2])
# print(myArray[0:2,0:2])

# print([1,2,3],[4,5,6])
# print(zip([1,2,3],[4,5,6]))
# print(list(zip([1,2,3],[4,5,6])))

print(np.ones((3,3)))
print(np.ones((3)))
# print(list(np.ones((3,3)), np.ones((3))))
print(list(zip(np.ones((3,3)), np.ones((3)))))

# arr = np.array([])
# for i in range(10):
#     arr.append(i);

# lista = [1, 2, 3]
# lista2 = [1, 2, 3]
# print(lista)
# print(list(lista))
# zipped_list = zip(lista, lista2) # iterator to first element
# # print(*zipped_list)
# print(zipped_list)
# unzipped_list = list(zipped_list) # unzip
# print(unzipped_list)

# print(lista())

# TUPLE (immutable list)
# print(tuple([1,2]))
# print((1,2))
