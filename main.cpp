#include <iostream>
#include "SuffixTree.h"

int main() {
    SuffixTree tree;
    std::string input = "banana";
    tree.build(input);

    std::cout << "Vytvorenie sufixoveho stromu pre '" << input << "':\n";
    tree.print();

    std::cout << "\nHladanie 'ana': " << (tree.find("ana") ? "Najdene" : "Nenaslo sa") << std::endl;
    std::cout << "Hladanie 'nana': " << (tree.find("nana") ? "Najdene" : "Nenaslo sa") << std::endl;
    std::cout << "Hladanie 'apple': " << (tree.find("apple") ? "Najdene" : "Nenaslo sa") << std::endl;

    std::cout << "Stlacte lubovolnu klavesu pre ukoncenie...\n";
    std::cin.get(); // Počká na stlačenie Enter
    
    return 0;
}
