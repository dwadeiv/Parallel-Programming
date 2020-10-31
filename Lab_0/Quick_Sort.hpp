#ifndef QS_HPP
#define QS_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

void swap(vector<int> &data, int index1, int index2);
int partition(vector<int> &data, int low, int high);
void quickSort(vector<int> &data, int low, int high);

#endif