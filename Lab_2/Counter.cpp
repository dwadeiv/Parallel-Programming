#include "Counter.hpp"

using std::string;
struct timespec start, finish;

Counters::Counters(string counter_type, int number_of_threads,int number_of_iterations) {

    threads = (pthread_t*)(malloc(number_of_threads*sizeof(pthread_t)));

    NUM_THREADS = number_of_threads;
    NUM_ITERATIONS = number_of_iterations;

    // Determing which counter method to use
    if(counter_type.compare("lock") == 0) {
        counter = lock_counter;
    }
    else if(counter_type.compare("barrier") == 0) {
        counter = barrier_counter;
    }
    else {
        assert(false);
    }

}

Counters::~Counters() {

    free(threads);

}

// Barrier counter
static void* fork_barrierCounter(void* args){

    counter_args* inArgs = (counter_args*)args;
    int tid = inArgs->tid;
    Barriers *bar = inArgs->barrier;
    int *cntr = inArgs->cntr;
    int NUM_ITERATIONS = inArgs->NUM_ITERATIONS;
    int NUM_THREADS = inArgs->NUM_THREADS;
    
    bar->wait();
    
    // START CLOCK
    if(tid==1){
		clock_gettime(CLOCK_MONOTONIC,&start);
	}

    for(int i = 0; i < NUM_ITERATIONS*NUM_THREADS; i++) {
        if(i%NUM_THREADS+1 == tid) {
            (*cntr)++;
        }
        bar->wait();
    }

    // END CLOCK
    if(tid==1){
		clock_gettime(CLOCK_MONOTONIC,&finish);
	}

}

// Lock counter
static void* fork_lockCounter(void* args){
    
    counter_args* inArgs = (counter_args*)args;
    int tid = inArgs->tid;
    Locks *lock = inArgs->lock;
    Barriers *bar = inArgs->barrier;
    int *cntr = inArgs->cntr;
    int NUM_ITERATIONS = inArgs->NUM_ITERATIONS;
    int NUM_THREADS = inArgs->NUM_THREADS;

    bar->wait();
    
    // START CLOCK
    if(tid==1){
		clock_gettime(CLOCK_MONOTONIC,&start);
	}
    
    for(int i = 0; i < NUM_ITERATIONS*NUM_THREADS; i++) {
        if(i%NUM_THREADS+1 == tid) {
            lock->acquire();
            (*cntr)++;
            lock->release();
        }
    }

    // END CLOCK
    if(tid==1){
		clock_gettime(CLOCK_MONOTONIC,&finish);
	}

}

// Counter algorithm handler function
void Counters::count(int *return_counter, string lockType, string barrierType) {

	Locks lock(lockType);
	Barriers barrier(barrierType, NUM_THREADS);

    int cntr = 0;

	// launch threads
	int ret; size_t i;
	for(i=1; i < NUM_THREADS; i++){

		counter_args* bkArgs = new counter_args;
		bkArgs->barrier = &barrier;
		bkArgs->lock = &lock;
        bkArgs->cntr = &cntr;
        bkArgs->NUM_ITERATIONS = NUM_ITERATIONS;
        bkArgs->NUM_THREADS = NUM_THREADS;
		bkArgs->tid = i+1;
	    switch(counter) {

            case(lock_counter):{
                ret = pthread_create(&threads[i], NULL, &fork_lockCounter, (void*)bkArgs);
                break;
            }

            case(barrier_counter):{         
                ret = pthread_create(&threads[i], NULL, &fork_barrierCounter, (void*)bkArgs);
                break;
            }

            default:{
                assert(false);
                break;
            }
        }

		if(ret){

			printf("ERROR; pthread_create: %d\n", ret);
			exit(-1);

		}
	}

    counter_args* masterArgs = new counter_args;
	masterArgs->barrier = &barrier;
	masterArgs->lock = &lock;
    masterArgs->cntr = &cntr;
    masterArgs->NUM_ITERATIONS = NUM_ITERATIONS;
    masterArgs->NUM_THREADS = NUM_THREADS;
	masterArgs->tid = 1;

    // Master also calls thread_main
    switch(counter) {

        case(lock_counter):{
            fork_lockCounter((void*)masterArgs);
            break;
        }

        case(barrier_counter):{    
            fork_barrierCounter((void*)masterArgs);
            break;
        }

        default:{
            assert(false);
            break;
        }
    }

	unsigned long long elapsed_ns;
	elapsed_ns = (finish.tv_sec-start.tv_sec)*1000000000 + (finish.tv_nsec-start.tv_nsec);
	printf("Elapsed (ns): %llu\n",elapsed_ns);

	// join threads
	for(size_t i=1; i<NUM_THREADS; i++){

		ret = pthread_join(threads[i],NULL);

		if(ret){

			printf("ERROR; pthread_join: %d\n", ret);
			exit(-1);

		}
	}

    // Retruning counter
    *return_counter = cntr;

}