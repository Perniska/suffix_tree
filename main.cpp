#include <iostream>
#include "SuffixTree.h"
#include <chrono>
#include <vector>
#include <string>
#include <random>
#include <fstream>

using namespace std;
using namespace std::chrono;

// Funkcia na generovanie náhodného reťazca danej dĺžky a abecedy
string generateRandomString(int length, const string& alphabet) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, alphabet.length() - 1);
    string result = "";
    for (int i = 0; i < length; ++i) {
        result += alphabet[distrib(gen)];
    }
    return result;
}

int main() {
    SuffixTree tree;
    string input = "banana";
    tree.build(input);

    cout << "Vytvorenie sufixoveho stromu pre '" << input << "':\n";
    tree.print();

    cout << "\nHladanie 'ana': " << (tree.find("ana") ? "Najdene" : "Nenaslo sa") << endl;
    cout << "Hladanie 'nana': " << (tree.find("nana") ? "Najdene" : "Nenaslo sa") << endl;
    cout << "Hladanie 'apple': " << (tree.find("apple") ? "Nenaslo sa" : "Najdene") << endl;

    cout << "\nSpustam experimenty...\n";

    vector<int> alphabet_sizes = {4, 26, 62};
    vector<int> string_lengths = {100, 500, 1000, 2000, 4000, 8000};
    vector<string> alphabets = {
        "ACGT", // Veľkosť 4
        "abcdefghijklmnopqrstuvwxyz", // Veľkosť 26
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789" // Veľkosť 62
    };

    ofstream outfile("experiment_data.csv");
    outfile << "alphabet_size,string_length,build_time_ms\n";

    for (size_t i = 0; i < alphabet_sizes.size(); ++i) {
        for (int length : string_lengths) {
            string random_string = generateRandomString(length, alphabets[i]);
            auto start_time = high_resolution_clock::now();
            tree.build(random_string);
            auto end_time = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(end_time - start_time);

            cout << "Abeceda: " << alphabet_sizes[i] << ", Dlzka: " << length << ", Cas budovania: " << duration.count() << " ms\n";
            outfile << alphabet_sizes[i] << "," << length << "," << duration.count() << "\n";
        }
    }

    outfile.close();
    cout << "\nExperimentalne data boli ulozene do 'experiment_data.csv'.\n";

    cout << "\nStlacte lubovolnu klavesu pre ukoncenie...\n";
    cin.get();

    return 0;
}