#ifndef SUFFIX_TREE_H
#define SUFFIX_TREE_H

#include <string>
#include <vector>
#include <map>

class SuffixTree {
private:
    struct Node {
        std::map<char, int> children;
        int start;
        int* end;
        int suffixLink;

        Node(int start, int* end);
    };

    std::string str;
    std::vector<Node> tree;
    int size;
    int* globalEnd;

    // Aktívne premenné
    int activeNode;
    int activeEdge;
    int activeLength;
    int remainingSuffixCount;
    int lastNewNode;

    bool walkDown(int nextNode);
    void extendSuffixTree(int pos);
    void preorder(int node) const;
    std::string edgeString(int node) const;

public:
    SuffixTree();
    ~SuffixTree();

    void clear();
    void build(const std::string& s);
    bool find(const std::string& s) const;
    void print() const;
};

#endif // SUFFIX_TREE_H
