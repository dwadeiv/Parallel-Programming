    #include "Bucket_Sort.hpp"

using std::vector;
using std::map;

BucketSort::BucketSort(int number_of_threads) {

	threads = (pthread_t*)(malloc(number_of_threads*sizeof(pthread_t)));

}

BucketSort::~BucketSort() {
	
	free(threads);

}

// Each thread will execute this method individually 
void* fork_bucketSort(void* args){

	// Extracting arguments from the passed struct
	bucketSort_args* inArgs = (bucketSort_args*)args;
	Barriers *bar = inArgs->barrier;
	Locks *lock = inArgs->lock;
	int tid = inArgs->tid;
	int* data = inArgs->data;
	int data_size = inArgs->data_size;
	int chunk_size = inArgs->chunk_size;
	vector< map<int,int> >* buckets = inArgs->buckets;
	int number_of_threads = inArgs->number_of_threads;
	int max_value = inArgs->max_value;

	bar->wait();

	// If statememnt that handles the last chunk of data
	if(tid == number_of_threads) {
		for(int i = (tid - 1) * chunk_size; i < data_size; i++) {
			int bucket_index = floor((number_of_threads-1) * data[i] / max_value);
			// Locking critical section
			lock->acquire();
			(*buckets)[bucket_index].insert({data[i], 1});
			lock->release();
		}
	}

	// Sorting chunks of the data, keeping track using the thread id
	else {
		for(int i = (tid - 1) * chunk_size; i < ((tid - 1) * chunk_size) + chunk_size; i++) {
			int bucket_index = floor((number_of_threads-1) * data[i] / max_value);
			// Locking critical section
			lock->acquire();
			(*buckets)[bucket_index].insert({data[i], 1});
			lock->release();
		}	
	}

	// All threads have sorted their corresponding chunk 
	bar->wait();
	

}


void BucketSort::bucketSort(int data[], int size, int number_of_threads, string lockType, string barrierType) {

	vector< map<int,int> > buckets;
	Locks lock(lockType);
	Barriers barrier(barrierType, number_of_threads);


	int max = data[0];

    for(int i = 1; i < size; i++) {
		
		if(data[i] > max) {
			max = data[i];
		}

    }

	for(int i = 0; i < number_of_threads; i++) {
		
		buckets.push_back(map<int,int>());

    }

	// Size of data chunks each thread is responsible for 
	int chunk_size = round(size / number_of_threads);


	// launch threads
	int ret; size_t i;
	for(i=1; i < number_of_threads; i++){

		bucketSort_args* bkArgs = new bucketSort_args;
		bkArgs->barrier = &barrier;
		bkArgs->lock = &lock;
		bkArgs->data = data;
		bkArgs->data_size = size;
		bkArgs->chunk_size = chunk_size;
		bkArgs->buckets = &buckets;
		bkArgs->max_value = max;
		bkArgs->number_of_threads = number_of_threads;
		bkArgs->tid = i+1;
		
		ret = pthread_create(&threads[i], NULL, &fork_bucketSort, (void*)bkArgs);

		if(ret){

			printf("ERROR; pthread_create: %d\n", ret);
			exit(-1);

		}
	}

	bucketSort_args* masterArgs = new bucketSort_args;
	masterArgs->barrier = &barrier;
	masterArgs->lock = &lock;
	masterArgs->data = data;
	masterArgs->data_size = size;
	masterArgs->chunk_size = chunk_size;
	masterArgs->buckets = &buckets;
	masterArgs->max_value = max;
	masterArgs->number_of_threads = number_of_threads;
	masterArgs->tid = 1;
	fork_bucketSort((void*)masterArgs); // master also calls thread_main

	// join threads
	for(size_t i=1; i<number_of_threads; i++){

		ret = pthread_join(threads[i],NULL);

		if(ret){

			printf("ERROR; pthread_join: %d\n", ret);
			exit(-1);

		}
	}


	// Concatinating the sorted buckets into the original data array
	int index = 0;
	for(int i = 0; i < (number_of_threads); i++) {
		for(auto itr = buckets[i].begin(); itr != buckets[i].end(); itr++) {
			data[index] = itr->first;
			index++;
		}
	}

	delete masterArgs;
// 	delete bkArgs;

}











