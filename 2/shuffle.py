#!/usr/bin/python3

shuffle_map = {
    0: 2,
    1: 14,
    2: 26,
    3: 9,
    4: 21,
    5: 1,
    6: 13,
    7: 25,
    8: 5,
    9: 17,
    10: 29,
    11: 0,
    12: 12,
    13: 24,
    14: 4,
    15: 16,
    16: 28,
    17: 8,
    18: 20,
    19: 3,
    20: 15,
    21: 27,
    22: 7,
    23: 19,
    24: 31,
    25: 11,
    26: 23,
    27: 6,
    28: 18,
    29: 30,
    30: 10,
    31: 22
}

number = "89349536319392163324855876422573"
shuffled = [number[shuffle_index] for shuffle_index in shuffle_map.values()]
print("".join(shuffled))

# 31415926535897932384626433832795