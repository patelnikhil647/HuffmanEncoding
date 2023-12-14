#include "HCTree.hpp"
#include "Helper.hpp"
#include <stack>

using namespace std;

void clearTree(HCNode* reference);

// for debugging purposes
void printTree(HCNode* reference) {
    cout << reference->symbol << ": " << reference->count << endl;
    if (reference->c0 != nullptr) {
        printTree(reference->c0);
    }
    if (reference->c1 != nullptr) {
        printTree(reference->c1);
    }    
}

// for debugging purposes
void printLeaves(vector<HCNode*> leaves) {
    for (unsigned int i = 0; i < leaves.size(); i++) {
        if (leaves[i] != nullptr) {
            cout << "(" << i << ") " << leaves[i]->symbol << ": " << leaves[i]->count << endl;
        }
    }

    cout << "PRINTED LEAVES" << endl << endl;
}

HCTree::~HCTree() {
    if (this->root == nullptr) { // if tree has no elements
        return;
    }
    clearTree(this->root); // delete each node recursively
}

void HCTree::build(const unordered_map<unsigned char, int> & freqs) {
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> forest;

    for (auto ch: freqs) { // create priority queue with all the leaf nodes of future Huffman Tree
        HCNode* tree = new HCNode(ch.second, ch.first);
        forest.push(tree);
        this->leaves[ch.first] = tree;
    }

    while (forest.size() > 1) {
        // pop and assign two highest priority (lowest frequency) HCNodes
        HCNode* left = forest.top();
        forest.pop();
        HCNode* right = forest.top();
        forest.pop();
        // create new node to be pushed to queue and assign children/parents
        // note symbol always has left child symbol to keep consistency
        HCNode* newNode = new HCNode(left->count + right->count, left->symbol);
        newNode->c0 = left;
        newNode->c1 = right;
        left->p = newNode;
        right->p = newNode;

        forest.push(newNode);
    }

    this->root = forest.top();
}

void HCTree::encode(unsigned char symbol, FancyOutputStream & out) const {
    HCNode* curr = this->leaves[symbol];
    HCNode* parent;
    stack<bool> code;

    while (curr->p != nullptr) { // while parent of current node exists
        parent = curr->p;
        (curr->symbol == parent->symbol) ? code.push(0) : code.push(1); // if current node is parent's left child, push 0, else push 1
        curr = parent;
    }

    while(code.size() > 0) { // write top from stack and pop until stack is empty
        out.write_bit(code.top());
        code.pop();
    }
}

unsigned char HCTree::decode(FancyInputStream & fis) const {
    HCNode* curr = this->root;
    int bit;

    while ((bit = fis.read_bit()) != -1) { // simultaneously assign bit to next bit in file and assert bit isn't eof
        (bit == 0) ? curr = curr->c0 : curr = curr->c1; // if bit is 0, traverse left, else traverse right
        if (curr->c0 == nullptr) { // if curr has no children
            return curr->symbol;
        }
    }

    return 0; // in case fis.read_bit() returns eof
}

void clearTree(HCNode* reference) {
    if (reference->c0 != nullptr) {
        clearTree(reference->c0);
        reference->c0 = nullptr;
    }
    if (reference->c1 != nullptr) {
        clearTree(reference->c1);
        reference->c1 = nullptr;
    }
    reference->p = nullptr;
    delete reference;
}
