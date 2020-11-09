#ifndef MS_HPP
#define MS_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <pthread.h>
#include <cmath>


using namespace std;

// Struct for arguments passed to the fork_mergeSort method
struct mergeSort_args {

	int tid;
	vector<int>* data_chunks;
	pthread_barrier_t *bar;

};

class MergeSort
{
    private:
        pthread_t* threads;
        pthread_barrier_t bar;

    public:
        MergeSort(int number_of_threads);
        ~MergeSort();
        vector<int> parallelMergeSort(vector<int> data, int number_of_threads);
};

#endif