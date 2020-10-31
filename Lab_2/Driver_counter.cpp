#include "Driver.hpp"
using std::string;
using std::ifstream;
using std::ofstream;

int main(int argc, char* argv[]){
	
	// parse args
	if(argc != 2 && argc != 8){
        std::cout << argc;
		
		printf("Error\nUsage: counter [--name] [-t NUM_THREADS] [-i NUM_ITERATIONS] [--bar=<sense,pthread>] [--lock=<tas,ttas,ticket,pthread>] [-o outfile.txt]\n\n");
		return 1;
	}

	// Variable declarations
	int c;
	size_t NUM_THREADS;
    size_t NUM_ITERATIONS;
	string barrierType = "none";
	string lockType = "none";
    string counter_type;
	string outputFile;

	ofstream outFile;

	
    while(1) {

        // Creating the long options
        static struct option long_options[] = {
            {"name",   no_argument,         0,  'n' },
			{"bar",   required_argument,   0,  'b' },
			{"lock",   required_argument,   0,  'l' },
            {0, 0, 0, 0}
        };

        // Parsing through the command line options
        int option_index = 0;
        c = getopt_long(argc, argv, "nb:l:t:i:o:", long_options, &option_index);

        // When all the options have been read
        if (c == -1) {
            break;
        }

        switch(c) {

            // Name option. Program will terminate after printing name.
            case 'n': {

                printf("\nDavid Milton Wade IV\n");
                return 0;

            }

            // Chosen barrier type
            case 'b': {

                barrierType = optarg;

                // If invalid barrier type input is entered
                if(barrierType.compare("sense") != 0 && barrierType.compare("pthread") != 0) {
                    printf("\nInvalid lock type\nUsage: counter [--name] [-t NUM_THREADS] [-i NUM_ITERATIONS] [--bar=<sense,pthread>] [--lock=<tas,ttas,ticket,pthread>] [-o outfile.txt]\n\n");
                    return 1;
                }
				break;

            }

            // Chosen lock type
            case 'l': {

                lockType = optarg;

                // If invalid lock type input is entered
                if(lockType.compare("tas") != 0 && lockType.compare("ttas") != 0 && lockType.compare("ticket") != 0 && lockType.compare("pthread") != 0) {
                    printf("\nInvalid lock type\nUsage: counter [--name] [-t NUM_THREADS] [-i NUM_ITERATIONS] [--bar=<sense,pthread>] [--lock=<tas,ttas,ticket,pthread>] [-o outfile.txt]\n\n");
                    return 1;
                }
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

			// Number of iterations
            case 'i': {

                // Chosen name for the output file
                NUM_ITERATIONS = atoi(optarg);
				break;

            }

            // Name of outputfile
            case 'o': {

                outputFile = optarg;
				break;

            }

        }

    }

    // Checking for invalid input
    if(barrierType.compare("none") != 0 && lockType.compare("none") !=0) {
        printf("\nERROR\nCannot input lock AND barrier argument. Must choose one.\n\n");
        return 1;
    }
    // Setting which counter to use
    else if(barrierType.compare("none") != 0) {
        counter_type = "barrier";
        lockType = "pthread";
    }
    else {
        counter_type = "lock";
        barrierType = "pthread";
    }

//////////// Counter //////////// 

    int cntr;
    Counters counter(counter_type, NUM_THREADS, NUM_ITERATIONS);

    counter.count(&cntr, lockType, barrierType);

	// Adding counter value into output file
	outFile.open(outputFile);
    outFile << cntr << std::endl;
	outFile.close();

	return 0;

}











