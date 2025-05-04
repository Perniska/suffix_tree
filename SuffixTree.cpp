#include "SuffixTree.h"
#include <iostream>

using namespace std;

SuffixTree::Node::Node(int start, int* end)
    : start(start), end(end), suffixLink(0) {}

SuffixTree::SuffixTree() : size(0), globalEnd(nullptr) {
    clear();
}

SuffixTree::~SuffixTree() {
    clear();
}

void SuffixTree::clear() {
    for (auto& node : tree)
        if (node.end != nullptr && node.end != globalEnd)
            delete node.end;

    tree.clear();
    globalEnd = new int(-1);
    tree.emplace_back(-1, nullptr); // Root
    size = 0;

    activeNode = 0;
    activeEdge = -1;
    activeLength = 0;
    remainingSuffixCount = 0;
    lastNewNode = -1;
}

void SuffixTree::build(const string& s) {
    clear();
    str = s + "$";
    size = str.size();

    for (int i = 0; i < size; i++) {
        extendSuffixTree(i);
    }
}

void SuffixTree::extendSuffixTree(int pos) {
    (*globalEnd)++;
    remainingSuffixCount++;
    lastNewNode = -1;

    while (remainingSuffixCount > 0) {
        if (activeLength == 0)
            activeEdge = pos;

        char currentChar = str[activeEdge];
        auto it = tree[activeNode].children.find(currentChar);

        if (it == tree[activeNode].children.end()) {
            tree.emplace_back(pos, globalEnd);
            tree[activeNode].children[currentChar] = tree.size() - 1;

            if (lastNewNode != -1) {
                tree[lastNewNode].suffixLink = activeNode;
                lastNewNode = -1;
            }
        } else {
            int next = it->second;
            if (walkDown(next)) continue;

            if (str[tree[next].start + activeLength] == str[pos]) {
                if (lastNewNode != -1 && activeNode != 0) {
                    tree[lastNewNode].suffixLink = activeNode;
                    lastNewNode = -1;
                }
                activeLength++;
                break;
            }

            int* splitEnd = new int(tree[next].start + activeLength - 1);
            tree.emplace_back(tree[next].start, splitEnd);
            int split = tree.size() - 1;

            tree[activeNode].children[currentChar] = split;
            tree.emplace_back(pos, globalEnd);
            tree[split].children[str[pos]] = tree.size() - 1;

            tree[next].start += activeLength;
            tree[split].children[str[tree[next].start]] = next;

            if (lastNewNode != -1)
                tree[lastNewNode].suffixLink = split;
            lastNewNode = split;
        }

        remainingSuffixCount--;

        if (activeNode == 0 && activeLength > 0) {
            activeLength--;
            activeEdge = pos - remainingSuffixCount + 1;
        } else if (activeNode != 0) {
            activeNode = tree[activeNode].suffixLink;
        }
    }
}

bool SuffixTree::walkDown(int nextNode) {
    int edgeLen = *(tree[nextNode].end) - tree[nextNode].start + 1;
    if (activeLength >= edgeLen) {
        activeEdge += edgeLen;
        activeLength -= edgeLen;
        activeNode = nextNode;
        return true;
    }
    return false;
}

bool SuffixTree::find(const string& s) const {
    int current = 0;
    int i = 0;

    while (i < s.size()) {
        auto it = tree[current].children.find(s[i]);
        if (it == tree[current].children.end()) return false;

        int next = it->second;
        int edgeLen = *(tree[next].end) - tree[next].start + 1;
        int j = 0;

        while (j < edgeLen && i < s.size()) {
            if (str[tree[next].start + j] != s[i]) return false;
            j++; i++;
        }

        current = next;
    }

    return true;
}

void SuffixTree::print() const {
    preorder(0);
}

void SuffixTree::preorder(int node) const {
    for (const auto& [ch, child] : tree[node].children) {
        cout << edgeString(child) << endl;
        preorder(child);
    }
}

string SuffixTree::edgeString(int node) const {
    return str.substr(tree[node].start, *(tree[node].end) - tree[node].start + 1);
}
