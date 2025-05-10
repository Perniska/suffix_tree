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

        Node(int start, int* end) : start(start), end(end), suffixLink(0) {}
    };

    std::string text;
    std::vector<Node> tree;
    int n; // Dĺžka textu
    int* globalEnd;

    // Premenné pre Ukkonenov algoritmus
    int activeNode;
    int activeEdge;
    int activeLength;
    int remainingSuffixCount;
    int lastNewNode;

    bool walkDown(int nextNode);
    void extendSuffixTree(int pos);
    void preorder(int node, int depth = 0) const;
    std::string edgeString(int node) const;

public:
    SuffixTree();
    ~SuffixTree();
    void clear();
    void build(const std::string& str);
    bool find(const std::string& str) const;
    void print() const;
};

#endif // SUFFIX_TREE_H