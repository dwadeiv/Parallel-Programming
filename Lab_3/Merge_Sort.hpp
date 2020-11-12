#ifndef MS_HPP
#define MS_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <omp.h>
#include <cmath>


using std::vector;


class MergeSort
{
    private:

        vector<int> *data_chunks;

    public:
        MergeSort();
        ~MergeSort();
        vector<int> parallelMergeSort(vector<int> data);

};

#endif