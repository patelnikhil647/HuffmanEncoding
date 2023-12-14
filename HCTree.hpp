#ifndef HCTREE_HPP
#define HCTREE_HPP
#include "Helper.hpp"
#include <queue>
#include <vector>
#include <unordered_map>
#include <fstream>
using namespace std;

/**
 * A Huffman Code Tree class
 */
class HCTree {
    private:
        HCNode* root;
        vector<HCNode*> leaves;

    public:
        /**
         * Constructor, which initializes everything to null pointers
         */
        HCTree() : root(nullptr) {
            leaves = vector<HCNode*>(256, nullptr);
        }

        ~HCTree();

        /**
         * Use the Huffman algorithm to build a Huffman coding tree.
         * PRECONDITION: freqs is a hash map containing the {symbol, frequency} elements.
         * POSTCONDITION: root points to the root of the trie, and leaves[i] points to the leaf node containing byte i.
         */
        void build(const unordered_map<unsigned char, int> & freqs);

        /**
         * Write to the given FancyOutputStream the sequence of bits coding the given symbol.
         * PRECONDITION: build() has been called, to create the coding tree, and initialize root pointer and leaves vector.
         */
        void encode(unsigned char symbol, FancyOutputStream & out) const;

        /**
         * Return symbol coded in the next sequence of bits from the stream.
         * PRECONDITION: build() has been called, to create the coding tree, and initialize root pointer and leaves vector.
         */
        unsigned char decode(FancyInputStream & fis) const;
};
#endif // HCTREE_HPP
