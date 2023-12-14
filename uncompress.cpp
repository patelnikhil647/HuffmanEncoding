#include "encoding.hpp"
#include "HCTree.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    if (!parseArgs(argc, argv)) { // check validity of inputs
        exit(-1);
    }

    FancyInputStream fis(argv[1]);
    FancyOutputStream fos(argv[2]);
    unordered_map<unsigned char, int> occurrences = readHeader(fis);
    unordered_map<unsigned char, int> counts;
    HCTree huffTree;
    unsigned char ch;

    if (occurrences.size() == 0) {
        return 0;
    }

    counts = assignCounts(occurrences);
    huffTree.build(occurrences);

    ch = huffTree.decode(fis);
    while (fis.good()) {
        counts.at(ch) = counts.at(ch) + 1;

        if (counts.at(ch) > occurrences.at(ch)) {
            break;
        }

        fos.write_byte(ch);
        ch = huffTree.decode(fis);
    }
    fos.flush();

    return 0;
}

unordered_map<unsigned char, int> readHeader(FancyInputStream & fis) {
    unordered_map<unsigned char, int> occurrences;
    int count;

    if (fis.filesize() == 0) {
        return occurrences;
    }
    
    for (int i = 0; i < 256; i++) {
        count = fis.read_shorter_int();
        if (count != 0) {
            occurrences.insert({(unsigned char) i, count});
        }
    }

    return occurrences;
}

unordered_map<unsigned char, int> assignCounts(unordered_map<unsigned char, int> occurrences) {
    unordered_map<unsigned char, int> counts;

    for (auto ch: occurrences) {
        counts.insert({ch.first, 0});
    }

    return counts;
}
