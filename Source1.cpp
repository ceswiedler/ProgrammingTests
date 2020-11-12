#include <stdio.h>

// Implement a function which turns on a series of bits in a byte array, 
// given an offset and length, as if drawing a horizontal line on a screen.

void turnOnBits(char* array, size_t bitOffset, size_t bitLength)
{
}

int main() 
{
    return 0;
}

// Given a collection of words like:
// {"cat", "dog"}
// and an input string, we want to determine if the input string can be constructed by 
// sticking words from the collection together without spaces.
// For example :
//   "catdog" should return True
//   single words are okay, so "cat" should return True
//   duplicates are fine, so "catcat" should return True
// But if there are any extra junk characters, that is not good, so "xcat" should return False, 
// because there is no way to make that *just* by combining words from the collection without spaces