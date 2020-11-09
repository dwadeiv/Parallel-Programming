#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <iostream>

class Array
{
    private:

    public:
        Array();
        ~Array();
        int* resize(int* arrayPtr, int &capacity);
};

#endif