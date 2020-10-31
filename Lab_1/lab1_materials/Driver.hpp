#ifndef DRIVER_HPP
#define DRIVER_HPP

#include <getopt.h>
#include <string>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "Merge_Sort.hpp"
#include "Bucket_Sort.hpp"

using namespace std;

// Global Variables
pthread_t* threads;
size_t NUM_THREADS;
pthread_barrier_t bar;
pthread_mutex_t lock;

vector<int> interest_data;

#endif