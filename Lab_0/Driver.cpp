#include "Driver.hpp"

using namespace std;

int main(int argc, char **argv) {

    int c;
    int oFlag = 0;
    string sortType;
    string inputFile;
    string outputFile;

    ifstream inFile;
    ofstream outFile;

    while(1) {

        // Creating the long options
        static struct option long_options[] = {
            {"name",   no_argument,         0,  'a' },
            {"alg=",   required_argument,   0,  'b' },
            {0, 0, 0, 0}
        };

        // Parsing through the command line options
        int option_index = 0;
        c = getopt_long(argc, argv, "o:", long_options, &option_index);

        // When all the options have been read
        if (c == -1) {
            break;
        }

        switch(c) {

            // Name option. Program will terminate after printing name.
            case 'a': {

                cout << "\nDavid Milton Wade IV\n" << endl;
                return 0;

            }
            // Chosen algorithm option
            case 'b': {

                sortType = optarg;
                // If invalid input is entered
                if(sortType.compare("merge") != 0 && sortType.compare("quick") != 0) {
                    cout << "\nInvalid sort type\n" << "Usage: mysort [--name] [sourcefile.txt] [-o outfile.txt] [--alg=<merge,quick>]\n" << endl;
                    return 1;
                }

            }
            // Name of outputfile
            case 'o': {
                
                // Flag is used in order to avoid reading 'o' option twice
                if(oFlag == 1) {
                    break;
                }

                // Chosen name for the output file
                outputFile = optarg;
                oFlag ++;

            }
        }

        // First option is the name of the input file.
        inputFile = argv[1];
        break;

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

    // Implementing chosen sorting algorithm
    if(sortType.compare("merge") == 0) {
        data = mergeSort(data);
    }
    else {
        quickSort(data, 0, data.size() - 1);
    }

    // Adding sorted data into output file
    outFile.open(outputFile);
    for(int i = 0; i < data.size(); i++){

        outFile << data[i] << endl;

    }
    outFile.close();

    return 0;
}