#include "Driver.hpp"

using std::string;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;

int main(int argc, char* argv[]){
	
	// parse args
	if(argc != 2 && argc != 4){
		
		printf("Error\nUsage: mysort [--name] [sourcefile.txt] [-o outfile.txt]\n");
		
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
            {0, 0, 0, 0}
        };

        // Parsing through the command line options
        int option_index = 0;
        c = getopt_long(argc, argv, "no:", long_options, &option_index);

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

            // Name of outputfile
            case 'o': {

                outputFile = optarg;
				break;

            }
        }

    }
	
    inFile.open(inputFile);

    vector<int> data;
    string line;

    // Parsing through input file and inserting data into a vector
    if(inFile.is_open()) {
        while(getline(inFile, line)){

            int number = stoi(line);
            data.push_back(number);

        }

        inFile.close();
    }


	MergeSort mergeSort;

	vector<int> sorted_data = mergeSort.parallelMergeSort(data);

	// Adding sorted data into output file
	outFile.open(outputFile);
	for(int i = 0; i < data.size(); i++){

		outFile << sorted_data[i] << endl;

	}
	outFile.close();

	return 0;

}










