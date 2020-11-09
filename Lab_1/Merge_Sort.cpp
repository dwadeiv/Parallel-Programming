#include "Merge_Sort.hpp"

using std::vector;

struct timespec start_merge, finish_merge;

MergeSort::MergeSort(int number_of_threads) {

    threads = (pthread_t*)(malloc(number_of_threads*sizeof(pthread_t)));
    pthread_barrier_init(&bar, NULL, number_of_threads);

}

MergeSort::~MergeSort() {

    free(threads);
    pthread_barrier_destroy(&bar);

}

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

// Each thread will execute this method individually 
static void* fork_mergeSort(void* args){

	// Extrracting arguments from the passed struct
	mergeSort_args* inArgs = (mergeSort_args*)args;
	int tid = inArgs->tid;
	vector<int>* data_chunks = inArgs->data_chunks;
	pthread_barrier_t *bar = inArgs->bar;
	
	pthread_barrier_wait(bar);

	// START CLOCK
	if(tid==1){
		clock_gettime(CLOCK_MONOTONIC,&start_merge);
	}

	pthread_barrier_wait(bar);
	
	// Calling mergeSort for each data chunk 
	data_chunks[tid-1] = mergeSort(data_chunks[tid-1]);
	
	pthread_barrier_wait(bar);
	
	return 0;

}

// Method to split the data into number of chunks that equals the nummber of threads 
static void Split_Data(vector<int> data, vector<int>* data_chunks, int chunk_size, int number_of_threads) {

	int ending_index = chunk_size;
	int starting_index = 0;

	for(int i = 0; i < number_of_threads; i++) {

		// If statement to handle the last chunk
		if(i == number_of_threads - 1) {
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

vector<int> MergeSort::parallelMergeSort(vector<int> data, int number_of_threads) {

    vector<int> data_chunks[number_of_threads];
    int chunk_size = round(data.size() / number_of_threads);

    Split_Data(data, data_chunks, chunk_size, number_of_threads);

    // launch threads
    int ret; size_t i;
    for(i=1; i < number_of_threads; i++){

        // Launch thread
        mergeSort_args* msArgs = new mergeSort_args;
		msArgs->bar = &bar;
        msArgs->data_chunks = data_chunks;
        msArgs->tid = i+1;

        ret = pthread_create(&threads[i], NULL, &fork_mergeSort, (void*)msArgs);

        if(ret){

            printf("ERROR; pthread_create: %d\n", ret);
            exit(-1);

        }
    }

    // Executing master thread 
    mergeSort_args* masterArgs = new mergeSort_args;
	masterArgs->bar = &bar;
    masterArgs->data_chunks = data_chunks;
    masterArgs->tid = 1;
    fork_mergeSort((void*)masterArgs); // master also calls thread_main

    // join threads
    for(size_t i = 1; i < number_of_threads; i++){

        ret = pthread_join(threads[i],NULL);

        if(ret){

            printf("ERROR; pthread_join: %d\n", ret);
            exit(-1);

        }
    }

    // Merging the sorted data chunks 
    for(i = 1; i < number_of_threads; i++) {

        data_chunks[0] = mergeVectors(data_chunks[0], data_chunks[i]);

    }

    clock_gettime(CLOCK_MONOTONIC,&finish_merge);

    unsigned long long elapsed_ns;  
    elapsed_ns = (finish_merge.tv_sec-start_merge.tv_sec)*1000000000 + (finish_merge.tv_nsec-start_merge.tv_nsec);
    printf("Elapsed (ns): %llu\n",elapsed_ns);

    vector<int> sorted_data = data_chunks[0];

    return sorted_data;

}