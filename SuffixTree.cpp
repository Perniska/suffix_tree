#include "SuffixTree.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

// Konštruktor triedy SuffixTree
SuffixTree::SuffixTree() : n(0), globalEnd(nullptr) {
    clear();  // Zavolá metódu clear(), ktorá resetuje strom
}

// Deštruktor triedy SuffixTree
SuffixTree::~SuffixTree() {
    clear();  // Zavolá metódu clear() na uvoľnenie alokovanej pamäte
}

// Metóda clear() vyčistí strom a pripraví ho na nové použitie
void SuffixTree::clear() {
    for (auto& node : tree) {  // Pre každý uzol v strome
        if (node.end != nullptr && node.end != globalEnd) {  // Ak existuje koncový ukazovateľ
            delete node.end;  // Uvoľni pamäť pre tento koncový ukazovateľ
        }
    }
    tree.clear();  // Vymaže všetky uzly v strome
    globalEnd = new int(-1);  // Vytvorí nový koncový ukazovateľ na hodnotu -1
    tree.emplace_back(-1, nullptr);  // Pridá koreňový uzol do stromu
    n = 0;  // Počet uzlov je 0
    activeNode = 0;  // Aktívny uzol je nastavený na koreň
    activeEdge = -1;  // Aktívna hrana je neplatná
    activeLength = 0;  // Dĺžka aktívnej hrany je 0
    remainingSuffixCount = 0;  // Počet zostávajúcich sufixov je 0
    lastNewNode = -1;  // Posledný nový uzol je neplatný
}

// Metóda build() postaví sufixový strom z daného reťazca
void SuffixTree::build(const string& str) {
    clear();  // Vyčistí strom pred začiatkom
    text = str + '$';  // Pridá '$' na koniec reťazca ako terminátor
    n = text.length();  // Nastaví dĺžku reťazca
    for (int i = 0; i < n; ++i) {  // Pre každý znak v reťazci
        extendSuffixTree(i);  // Rozšíri strom o znak na pozícii i
    }
}

// Metóda extendSuffixTree() pridá nový sufix do stromu
void SuffixTree::extendSuffixTree(int pos) {
    (*globalEnd)++;  // Zväčší hodnotu globalEnd
    remainingSuffixCount++;  // Zvýši počet zostávajúcich sufixov
    lastNewNode = -1;  // Nastaví posledný nový uzol na neplatný

    while (remainingSuffixCount > 0) {  // Pokračuje, kým sú nejaké zostávajúce sufixy
        if (activeLength == 0) {  // Ak je dĺžka aktívnej hrany 0
            activeEdge = pos;  // Nastaví aktívnu hranu na aktuálnu pozíciu
        }

        char current_char = text[activeEdge];  // Získa aktuálny znak na aktívnej hrane
        auto it = tree[activeNode].children.find(current_char);  // Skontroluje, či uzol má dieťa pre tento znak

        if (it == tree[activeNode].children.end()) {  // Ak neexistuje dieťa pre tento znak
            tree.emplace_back(pos, globalEnd);  // Vytvorí nový uzol
            tree[activeNode].children[current_char] = tree.size() - 1;  // Pridá dieťa do aktívneho uzla

            if (lastNewNode != -1) {  // Ak existuje posledný nový uzol
                tree[lastNewNode].suffixLink = activeNode;  // Nastaví odkaz na suffix pre posledný nový uzol
                lastNewNode = -1;  // Nastaví posledný nový uzol na neplatný
            }
        } else {  // Ak dieťa pre tento znak existuje
            int next_node = it->second;  // Získa index nasledujúceho uzla
            if (walkDown(next_node)) {  // Skúša prejsť po hranách
                continue;  // Ak sa podarí prejsť po hranách, pokračuje na ďalší cyklus
            }

            if (text[tree[next_node].start + activeLength] == text[pos]) {  // Ak sa znak na aktívnej hrane zhoduje s novým znakom
                if (lastNewNode != -1 && activeNode != 0) {  // Ak existuje posledný nový uzol a aktívny uzol nie je koreň
                    tree[lastNewNode].suffixLink = activeNode;  // Nastaví odkaz na suffix pre posledný nový uzol
                    lastNewNode = -1;  // Nastaví posledný nový uzol na neplatný
                }
                activeLength++;  // Zvýši dĺžku aktívnej hrany
                break;  // Ukončí rozširovanie
            }

            int* splitEnd = new int(tree[next_node].start + activeLength - 1);  // Vytvorí koncový ukazovateľ pre rozdelenie
            tree.emplace_back(tree[next_node].start, splitEnd);  // Vytvorí nový uzol pre rozdelenie
            int split_node = tree.size() - 1;  // Získa index nového uzla

            tree[activeNode].children[current_char] = split_node;  // Pripojí nový uzol k aktívnemu uzlu
            tree.emplace_back(pos, globalEnd);  // Vytvorí nový uzol pre nový znak
            tree[split_node].children[text[pos]] = tree.size() - 1;  // Pridá dieťa pre nový znak do rozdeleného uzla

            tree[next_node].start += activeLength;  // Posunie začiatok pôvodného uzla
            tree[split_node].children[text[tree[next_node].start]] = next_node;  // Aktualizuje deti rozdeleného uzla

            if (lastNewNode != -1) {  // Ak existuje posledný nový uzol
                tree[lastNewNode].suffixLink = split_node;  // Nastaví odkaz na suffix pre posledný nový uzol
            }
            lastNewNode = split_node;  // Nastaví posledný nový uzol na nový rozdelený uzol
        }

        remainingSuffixCount--;  // Zníži počet zostávajúcich sufixov
        if (activeNode == 0 && activeLength > 0) {  // Ak je aktívny uzol koreň a dĺžka aktívnej hrany je väčšia ako 0
            activeLength--;  // Zníži dĺžku aktívnej hrany
            activeEdge = pos - remainingSuffixCount + 1;  // Nastaví aktívnu hranu na novú pozíciu
        } else if (activeNode != 0) {  // Ak aktívny uzol nie je koreň
            activeNode = tree[activeNode].suffixLink;  // Prejde na suffix link
        }
    }
}

// Metóda walkDown() sa používa na prechod po hranách stromu
bool SuffixTree::walkDown(int nextNode) {
    int edgeLength = *(tree[nextNode].end) - tree[nextNode].start + 1;  // Získa dĺžku hrany
    if (activeLength >= edgeLength) {  // Ak je dĺžka aktívnej hrany väčšia alebo rovná dĺžke hrany
        activeEdge += edgeLength;  // Posunie aktívnu hranu
        activeLength -= edgeLength;  // Zníži dĺžku aktívnej hrany
        activeNode = nextNode;  // Nastaví aktívny uzol na nasledujúci uzol
        return true;  // Prechod bol úspešný
    }
    return false;  // Prechod nebol možný
}
