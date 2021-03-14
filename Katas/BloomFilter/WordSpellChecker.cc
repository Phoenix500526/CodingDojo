#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <random>

#include "BloomFilter.h"

using namespace std;

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        cerr << "Arguments expected\n";
        return 1;
    }
    ifstream infile{argv[1]};
    if (!infile) {
        throw runtime_error("couldn't open source to reading\n");
    }
    BloomFilter_t<1'000'000'0> bf;
    string line;
    while (std::getline(infile, line)) {
        bf.insert(line);
    }
    assert(bf.get_insertCount() == 102401);
    random_device seed;
    default_random_engine generator(seed());
    uniform_int_distribution<char> char_distributor('a', 'z');
    uniform_int_distribution<int> word_len_distributor(2, 10);
    vector<string> word_list(128, "");
    word_list[0] = "hello";
    word_list[1] = "speed";
    word_list[2] = "world";
    word_list[3] = "word";
    word_list[4] = "thank";
    word_list[5] = "thing";
    word_list[6] = "think";
    word_list[7] = "bird";
    word_list[8] = "boy";
    word_list[9] = "girl";
    word_list[10] = "man";
    for (int i = 11; i < word_list.size(); ++i) {
        int len = word_len_distributor(generator);
        for (int j = 0; j < len; ++j) {
            word_list[i].push_back(char_distributor(generator));
        }
    }
    int cnt = 0;
    for (int i = 0; i < word_list.size(); ++i) {
        if (bf.query(word_list[i])) {
            cout << word_list[i] << '\n';
            ++cnt;
        }
    }
    cout << cnt << '\n';
    return 0;
}