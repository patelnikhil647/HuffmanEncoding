#ifndef ENCODING_H
#define ENCODING_H
#include "Helper.hpp"
#include <unordered_map>

using namespace std;

bool parseArgs(unsigned int argc, char* argv[]) {
    string arg1;
    string arg2;

    // parse command line args and determine whether inputs are correct
    try{
        if (argc != 3) { // check if number of arguments is 2
            throw invalid_argument("Incorrect number of arguments (should be 2)");
        }
        arg1 = argv[1];
        arg2 = argv[2];
        // check if arg1 is a file
        FancyInputStream fis1(argv[1]);
        if (!fis1.good()) {
            throw invalid_argument(arg1 + " cannot be found");
        }
    } catch (invalid_argument & e) {
        cerr << "ERROR: " << e.what() << endl;
        return false;
    }

    return true;
}

// counts the frequency of each character in input file and return hash map of {symbol, frequency} elements
unordered_map<unsigned char, int> countOccurrences(FancyInputStream & fis, int sz);

// writes header to output file
void writeHeader(FancyOutputStream & fos, unordered_map<unsigned char, int> occurrences);

// reads header from input file to find frequency of each character in uncompressed file
unordered_map<unsigned char, int> readHeader(FancyInputStream & fis);

// set each key in counts hash map with value of 0
unordered_map<unsigned char, int> assignCounts(unordered_map<unsigned char, int> occurrences);

#endif
