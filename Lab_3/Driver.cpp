#include "Driver.hpp"

using std::string;
using std::ifstream;
using std::ofstream;

int main(int argc, char* argv[]){
	
	// parse args
	if(argc != 2 && argc != 6){
		
		printf("Error\nUsage: mysort [--name] [sourcefile.txt] [-o outfile.txt] [-t NUM_THREADS] [--alg=<fjmerge,lkbucket>]\n");
		
	}

	// Variable declarations
	int NUM_THREADS;
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
	int capacity = 1000;
	int* data;	
	data = new int[capacity];
	int index = 0;
    string line;

	Array array;

    // Parsing through input file and inserting data into a vector

    if(inFile.is_open()) {
        while(getline(inFile, line)){

            int number = stoi(line);
			// Array needs to be resized
			if(index == capacity) {

				data = array.resize(data, capacity);

			}

            data[index] = number;
			index++;

        }

        inFile.close();
    }

	//////////// Merge Sort //////////// 
	if(sortType.compare("fjmerge") == 0) {

		MergeSort mergeSort(NUM_THREADS);

		vector<int> data_vector(data, data + index);

		vector<int> sorted_data = mergeSort.parallelMergeSort(data_vector, NUM_THREADS);

		// Adding sorted data into output file
    	outFile.open(outputFile);
		for(int i = 0; i < sorted_data.size(); i++){

			outFile << sorted_data[i] << endl;

		}
		outFile.close();

	}

	//////////// Bucket Sort //////////// 
	else {

		BucketSort bucketSort(NUM_THREADS);

		bucketSort.bucketSort(data, index, NUM_THREADS);

		// Adding sorted data into output file
    	outFile.open(outputFile);
		for(int i = 0; i < index; i++){

			outFile << data[i] << endl;

		}

		outFile.close();

	}

	return 0;

}








