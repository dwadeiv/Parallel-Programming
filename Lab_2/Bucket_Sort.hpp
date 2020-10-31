#ifndef BS_HPP
#define BS_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <functional>
#include <iterator>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cmath>
#include "Locks.hpp"
#include "Barriers.hpp"


using std::vector;
using std::map;

struct bucketSort_args {
	
	Locks *lock;
	Barriers *barrier;
	int tid;
	int *data;
	int data_size;
	int chunk_size;
	vector< map<int,int> >* buckets;
	int number_of_threads;
	int max_value;

};

class BucketSort
{
    private:
		pthread_t* threads;

    public:
        BucketSort(int number_of_threads);
        ~BucketSort();
        void bucketSort(int data[], int size, int number_of_threads, string lockType, string barrierType);
};

#endif