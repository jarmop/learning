import numpy as np

def calc_i_left_child(i):
    return 2 * i + 1

def calc_i_right_child(i):
    return calc_i_left_child(i) + 1

# swap parent with the biggest child
def sift_down(data, iParent, iEnd):
    i_left_child = calc_i_left_child(iParent)
    i_right_child = calc_i_right_child(iParent)

    i_biggest_child = None
    if i_left_child < iEnd and (i_right_child >= iEnd or data[i_left_child] > data[i_right_child]):
        i_biggest_child = i_left_child
    elif i_right_child < iEnd:
        i_biggest_child = i_right_child

    if i_biggest_child and data[iParent] < data[i_biggest_child]:
        temp = data[iParent]
        data[iParent] = data[i_biggest_child]
        data[i_biggest_child] = temp
        sift_down(data, i_biggest_child, iEnd)

def heapify(data, iParent, iEnd):
    if (iParent < iEnd):
        heapify(data, calc_i_left_child(iParent), iEnd)
        heapify(data, calc_i_right_child(iParent), iEnd)
        sift_down(data, iParent, iEnd)

def heapSort(data):
    i_end_unsorted = len(data)
    heapify(data, 0, i_end_unsorted)
    while i_end_unsorted > 0 and data[0] > data[i_end_unsorted - 1]:
        temp = data[0]
        data[0] = data[i_end_unsorted - 1]
        data[i_end_unsorted - 1] = temp
        i_end_unsorted -= 1
        heapify(data, 0, i_end_unsorted)

data = np.random.randint(10, size=10)
print(data)
heapSort(data)
print(data)