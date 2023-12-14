#include "encoding.hpp"
#include "HCTree.hpp"

using namespace std;

// for debugging purposes
void printOccurrences(unordered_map<unsigned char, int> occurrences) {
    for (auto ch: occurrences) {
        cout << ch.first << ": " << ch.second << endl;
    }
    cout << "PRINTED OCCURRENCES" << endl << endl;
}

int main(int argc, char* argv[]) {
    if (!parseArgs(argc, argv)) { // check validity of inputs
        exit(-1);
    }

    FancyInputStream fis(argv[1]);
    FancyOutputStream fos(argv[2]);
    int sz = fis.filesize();
    unordered_map<unsigned char, int> occurrences = countOccurrences(fis, sz);
    HCTree huffTree;

    if (occurrences.size() == 0) { // if given empty file, return (output file will be empty)
        return 0;
    }

    writeHeader(fos, occurrences);

    huffTree.build(occurrences);

    for (int i = 0; i < sz; i++) { // encode each character in input file and write to output file
        huffTree.encode(fis.read_byte(), fos);
    }
    fos.flush();
    
    return 0;
}

unordered_map<unsigned char, int> countOccurrences(FancyInputStream & fis, int sz) {
    unordered_map<unsigned char, int> occurrences;
    int curr;

    for (int i = 0; i < sz; i++) {
        curr = fis.read_byte();
        if (occurrences.find(curr) == occurrences.end()) {
            occurrences.insert({curr, 1});
        } else {
            occurrences.at(curr) = occurrences.at(curr) + 1;
        }
    }

    fis.reset();
    return occurrences;
}

void writeHeader(FancyOutputStream & fos, unordered_map<unsigned char, int> occurrences) {
    for (int i = 0; i < 256; i++) {
        if (occurrences.find(i) == occurrences.end()) { // if char doesn't exist in occurrences
            fos.write_shorter_int(0);
        } else {
            fos.write_shorter_int(occurrences.at(i));
        }
    }

    fos.flush();
}
