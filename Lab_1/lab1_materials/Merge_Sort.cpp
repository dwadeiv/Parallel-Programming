#include "Merge_Sort.hpp"

using namespace std;

vector<int> mergeVectors(vector<int> vectorA, vector<int> vectorB){

    vector<int> merged;

    // Both vectors have elements inside of them
    while(!vectorA.empty() && !vectorB.empty()) {

        // Finding smallest element and adding to new merged vector. Once element is added, it is removed from
        // origin vector
        // This while loop repeats until one of the vectors is empty

        if(vectorA.front() < vectorB.front()) {
            merged.push_back(vectorA.front());
            vectorA.erase(vectorA.begin());
        }

        else {
            merged.push_back(vectorB.front());
            vectorB.erase(vectorB.begin());
        }

    }

    // Vector A still has elemnts in it, adding remaing elements.
    while(!vectorA.empty()) {

        merged.push_back(vectorA.front());
        vectorA.erase(vectorA.begin());

    }

    // Vector B still has elemnts in it, adding remaing elements.
    while(!vectorB.empty()) {

        merged.push_back(vectorB.front());
        vectorB.erase(vectorB.begin());

    }

    return merged;

}

vector<int> mergeSort(vector<int> data) {

    // Conditional for when the vector has been completly broken up
    if(data.size() <= 1) {
        return data;
    }

    // Finding middle most index
    int middleIndex = (data.size() + 1) / 2;
    vector<int> vectorA, vectorB;

    // First half of data vector
    for(int i = 0; i < middleIndex; i++) {

        vectorA.push_back(data[i]);

    }

    // Second half of data vector
    for(int i = middleIndex; i < data.size(); i++) {

        vectorB.push_back(data[i]);

    } 

    // Recursively go through left side first
    vectorA = mergeSort(vectorA);
    // Recursively go through right side second
    vectorB = mergeSort(vectorB);

    return mergeVectors(vectorA, vectorB);

}