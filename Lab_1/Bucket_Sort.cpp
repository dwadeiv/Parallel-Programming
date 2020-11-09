#include "Bucket_Sort.hpp"

using std::vector;
using std::map;

struct timespec start_bucket, finish_bucket;

BucketSort::BucketSort(int number_of_threads) {

    threads = (pthread_t*)(malloc(number_of_threads*sizeof(pthread_t)));
    pthread_barrier_init(&bar, NULL, number_of_threads);
	pthread_mutex_init(&lock, NULL);

}

BucketSort::~BucketSort() {

    free(threads);
    pthread_barrier_destroy(&bar);
	pthread_mutex_destroy(&lock);

}

// Each thread will execute this method individually 
static void* fork_bucketSort(void* args){

	// Extracting arguments from the passed struct
	bucketSort_args* inArgs = (bucketSort_args*)args;
	pthread_barrier_t *bar = inArgs->bar;
	pthread_mutex_t *lock = inArgs->lock;
	int tid = inArgs->tid;
	int* data = inArgs->data;
	int data_size = inArgs->data_size;
	int chunk_size = inArgs->chunk_size;
	vector< map<int,int> >* buckets = inArgs->buckets;
	int number_of_threads = inArgs->number_of_threads;
	int max_value = inArgs->max_value;

	pthread_barrier_wait(bar);

	// START CLOCK
	if(tid==1){
		clock_gettime(CLOCK_MONOTONIC,&start_bucket);
	}

	pthread_barrier_wait(bar);

	// If statememnt that handles the last chunk of data
	if(tid == number_of_threads) {
		for(int i = (tid - 1) * chunk_size; i < data_size; i++) {
			int bucket_index = floor((number_of_threads-1) * data[i] / max_value);
			// Locking critical section
			pthread_mutex_lock(lock);
			(*buckets)[bucket_index].insert({data[i], 1});
			pthread_mutex_unlock(lock);
		}
	}

	// Sorting chunks of the data, keeping track using the thread id
	else {
		for(int i = (tid - 1) * chunk_size; i < ((tid - 1) * chunk_size) + chunk_size; i++) {
			int bucket_index = floor((number_of_threads-1) * data[i] / max_value);
			// Locking critical section
			pthread_mutex_lock(lock);
			(*buckets)[bucket_index].insert({data[i], 1});
			pthread_mutex_unlock(lock);
		}	
	}

	// All threads have sorted their corresponding chunk 
	pthread_barrier_wait(bar);
	
	return 0;

}


void BucketSort::bucketSort(int data[], int size, int number_of_threads) {

	vector< map<int,int> > buckets;


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
		bkArgs->bar = &bar;
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
	masterArgs->bar = &bar;
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

	clock_gettime(CLOCK_MONOTONIC,&finish_bucket);

    unsigned long long elapsed_ns;  
    elapsed_ns = (finish_bucket.tv_sec-start_bucket.tv_sec)*1000000000 + (finish_bucket.tv_nsec-start_bucket.tv_nsec);
    printf("Elapsed (ns): %llu\n",elapsed_ns);

	delete masterArgs;

}









