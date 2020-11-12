#include "Merge_Sort.hpp"

using std::vector;

struct timespec start_merge, finish_merge;

MergeSort::MergeSort() {}

MergeSort::~MergeSort() {}

static vector<int> mergeVectors(vector<int> vectorA, vector<int> vectorB){

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

static vector<int> mergeSort(vector<int> data) {

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


// Method to split the data into number of chunks that equals the nummber of threads 
static void Split_Data(vector<int> data, vector<int>* data_chunks, int chunk_size, int number_of_chunks) {

	int ending_index = chunk_size;
	int starting_index = 0;
    
	for(int i = 0; i < number_of_chunks; i++) {

		// If statement to handle the last chunk
		if(i == number_of_chunks - 1) {
			vector<int> chunk(data.begin() + starting_index,  data.end());
			data_chunks[i] = chunk;
			return;
		}

		vector<int> chunk(data.begin() + starting_index,  data.begin() + ending_index);
		data_chunks[i] = chunk;
		
		starting_index = ending_index;

		ending_index += chunk_size;

	}

}

vector<int> MergeSort::parallelMergeSort(vector<int> data) {
    
    clock_gettime(CLOCK_MONOTONIC,&start_merge);


    #pragma omp parallel default(none) shared(data, data_chunks)
    {

        int nthreads = omp_get_num_threads();
        if(nthreads > data.size()) {
            nthreads = (int)data.size();
        }
        int tid = omp_get_thread_num();

        #pragma omp single
        {
            data_chunks = new vector<int>[nthreads];
            int chunk_size = round(data.size() / nthreads);
            Split_Data(data, data_chunks, chunk_size, nthreads);

        }
        
        if(tid < (int)data.size() - 1) {
            
            data_chunks[tid] = mergeSort(data_chunks[tid]);
            
        }
        
        #pragma omp barrier

        #pragma omp single 
        {
            // Merging the sorted data chunks 
            for(int i = 1; i < nthreads; i++) {

                data_chunks[0] = mergeVectors(data_chunks[0], data_chunks[i]);

            }

        }

    }

    // mergeSort(data);
    clock_gettime(CLOCK_MONOTONIC,&finish_merge);

    unsigned long long elapsed_ns;  
    elapsed_ns = (finish_merge.tv_sec-start_merge.tv_sec)*1000000000 + (finish_merge.tv_nsec-start_merge.tv_nsec);
    printf("Elapsed (ns): %llu\n",elapsed_ns);

    vector<int> sorted_data = data_chunks[0];

    return sorted_data;

}