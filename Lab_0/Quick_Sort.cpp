#include "Quick_Sort.hpp"

using namespace std;

// Partition function that places pivot in sorted position. All values to the left will be smaller and values to the right
// will be larger
int partition(vector<int> &data, int low, int high) {

    int pivot = data[low];

    vector<int> temp;
    temp.push_back(pivot);
    // Counter to keep track of final pivot position
    int k = 0;

    for(int i = low + 1; i <= high; i++){

        // Placing all larger than or equal pivot elements to the right of pivot
        if(data[i] >= pivot){
            temp.push_back(data[i]);
        }
        // Placing all smaller than pivot elements to the left of pivot
        else {
            temp.insert(temp.begin(),data[i]);
            k++;
        }

    }

    // Placing sorted elements from temp vector back into original vector
    for(int i = 0; i <= high - low; i++) {
        data[i+low] = temp[i];
    }

    // The returning partition position for the following iterations
    return low + k;

}

void quickSort(vector<int> &data, int low, int high) {
    
    if(low < high) {
        int particion = partition(data, low, high);
        // Sort items left of partiion first
        quickSort(data, low, particion);
        // Then sort items right of partition
        quickSort(data, particion + 1, high);
    }

}