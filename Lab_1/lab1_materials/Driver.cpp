#include "Driver.hpp"

using namespace std;

struct timespec start, finish;

// Struct for arguments passed to the fork_mergeSort method
struct mergeSort_args {

	int tid;
	vector<int>* data_chunks;

};

// Method to split the data into number of chunks that equals the nummber of threads 
void Split_Data(vector<int> data, vector<int>* data_chunks, int chunk_size) {

	int ending_index = chunk_size;
	int starting_index = 0;

	for(int i = 0; i < NUM_THREADS; i++) {

		// If statement to handle the last chunk
		if(i == NUM_THREADS - 1) {
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

void global_init(){
	
	threads = (pthread_t*)(malloc(NUM_THREADS*sizeof(pthread_t)));
	pthread_barrier_init(&bar, NULL, NUM_THREADS);
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

}

void global_cleanup(){

	free(threads);
	pthread_barrier_destroy(&bar);
	pthread_mutex_destroy(&lock);

}

// Each thread will execute this method individually 
void* fork_mergeSort(void* args){

	// Extrracting arguments from the passed struct
	mergeSort_args* inArgs = (mergeSort_args*)args;
	int tid = inArgs->tid;
	vector<int>* data_chunks = inArgs->data_chunks;
	
	pthread_barrier_wait(&bar);

	// START CLOCK
	if(tid==1){
		clock_gettime(CLOCK_MONOTONIC,&start);
	}

	pthread_barrier_wait(&bar);
	
	// Calling mergeSort for each data chunk 
	data_chunks[tid-1] = mergeSort(data_chunks[tid-1]);
	
	pthread_barrier_wait(&bar);

	// END CLOCK
	if(tid==1){
		clock_gettime(CLOCK_MONOTONIC,&finish);
	}
	
	return 0;

}




int main(int argc, char* argv[]){
	
	// parse args
	if(argc != 2 && argc != 6){
		
		printf("Error\nUsage: mysort [--name] [sourcefile.txt] [-o outfile.txt] [-t NUM_THREADS] [--alg=<fjmerge,lkbucket>]\n");
		
	}

	// Variable declarations
	int c;
	string sortType;
	string inputFile;
	string outputFile;

	ifstream inFile;
	ofstream outFile;

	// First option is the name of the input file.
	inputFile = argv[1];
	
    while(1) {

        // Creating the long options
        static struct option long_options[] = {
            {"name",   no_argument,         0,  'n' },
            {"alg",   required_argument,   0,  'a' },
            {0, 0, 0, 0}
        };

        // Parsing through the command line options
        int option_index = 0;
        c = getopt_long(argc, argv, "na:o:t:", long_options, &option_index);

        // When all the options have been read
        if (c == -1) {
            break;
        }

        switch(c) {

            // Name option. Program will terminate after printing name.
            case 'n': {

                cout << "\nDavid Milton Wade IV\n" << endl;
                return 0;

            }

            // Chosen algorithm option
            case 'a': {

                sortType = optarg;

                // If invalid input is entered
                if(sortType.compare("fjmerge") != 0 && sortType.compare("lkbucket") != 0) {
                    cout << "\nInvalid sort type\n" << "Usage: mysort [--name] [sourcefile.txt] [-o outfile.txt] [-t NUM_THREADS] [--alg=<fjmerge,lkbucket>]\n" << endl;
                    return 1;
                }
				break;

            }

            // Name of outputfile
            case 'o': {

                outputFile = optarg;
				break;

            }

			// Number of threads
            case 't': {

                // Chosen name for the output file
                NUM_THREADS = atoi(optarg);
				if(NUM_THREADS > 150){
					printf("ERROR; too many threads\n");
					exit(-1);
				}
				
				break;

            }
        }

    }
	
	inFile.open(inputFile);
    string line;

    // Parsing through input file and inserting data into a vector
    if(inFile.is_open()) {
        while(getline(inFile, line)){

            int number = stoi(line);
            interest_data.push_back(number);

        }

        inFile.close();
    }

	global_init();

	//////////// Merge Sort //////////// 
	if(sortType.compare("fjmerge") == 0) {

		vector<int> data_chunks[NUM_THREADS];
		int chunk_size = round(interest_data.size() / NUM_THREADS);

		Split_Data(interest_data, data_chunks, chunk_size);

		// launch threads
		int ret; size_t i;
		for(i=1; i < NUM_THREADS; i++){

			// Launch thread
			mergeSort_args* msArgs = new mergeSort_args;
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
		masterArgs->data_chunks = data_chunks;
		masterArgs->tid = 1;
		fork_mergeSort((void*)masterArgs); // master also calls thread_main

		// join threads
		for(size_t i=1; i<NUM_THREADS; i++){

			ret = pthread_join(threads[i],NULL);

			if(ret){

				printf("ERROR; pthread_join: %d\n", ret);
				exit(-1);

			}
		}

		// Merging the sorted data chunks 
		for(i = 1; i < NUM_THREADS; i++) {

			data_chunks[0] = mergeVectors(data_chunks[0], data_chunks[i]);

		}

		vector<int> sorted_data = data_chunks[0];

		// Adding sorted data into output file
    	outFile.open(outputFile);
		for(int i = 0; i < sorted_data.size(); i++){

			outFile << sorted_data[i] << endl;

		}
		outFile.close();

		
		global_cleanup();
		
		unsigned long long elapsed_ns;
		elapsed_ns = (finish.tv_sec-start.tv_sec)*1000000000 + (finish.tv_nsec-start.tv_nsec);
		printf("Elapsed (ns): %llu\n",elapsed_ns);

	}

	//////////// Bucket Sort //////////// 
	else {

		int* interest_data_array = &interest_data[0];
		int size = interest_data.size();

		int samples[NUM_THREADS-1];

		for(int i = 0; i < NUM_THREADS-1; i++) {

			int random_index = rand() % size;
			samples[i] = interest_data_array[random_index];

		}

		// START CLOCK
		clock_gettime(CLOCK_MONOTONIC,&start);

		bucketSort(interest_data_array, size, NUM_THREADS, threads, &bar, &lock);

		// END CLOCK
		clock_gettime(CLOCK_MONOTONIC,&finish);

		// Adding sorted data into output file
    	outFile.open(outputFile);
		for(int i = 0; i < size; i++){

			outFile << interest_data_array[i] << endl;

		}

		outFile.close();

		
		global_cleanup();
		
		unsigned long long elapsed_ns;
		elapsed_ns = (finish.tv_sec-start.tv_sec)*1000000000 + (finish.tv_nsec-start.tv_nsec);
		printf("Elapsed (ns): %llu\n",elapsed_ns);

	}

	return 0;

}







