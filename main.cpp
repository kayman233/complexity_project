#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

size_t overlap(std::string& s1, std::string& s2) {
    size_t last = s1.length() - 1;
    size_t length_s2 = s2.length();
    size_t overlap = 0;

    for (size_t i = last, j = 1; i > 0 && j < length_s2; --i, ++j) {
        std::string suffix = s1.substr(i);
        std::string prefix = s2.substr(0, j);
        if (suffix == prefix) {
            overlap = j;
        }
    }

    return overlap;
}

std::string merge(std::string& s1, std::string& s2, size_t length) {
    s2 = s2.substr(length);
    return s1 + s2;
}

std::string createSuperString(std::vector<std::string>& strings) {
    while (strings.size() > 1) {
        int maxoverlap = -1;
        int firstInd;
        int secondInd;

        for (int i = 0; i < strings.size(); ++i) {
            for (int j = 0; j < strings.size(); ++j) {
                if (i == j) {
                    continue;
                }

                int curoverlap = (int)overlap(strings[i], strings[j]);

                if (curoverlap > maxoverlap) {
                    maxoverlap = curoverlap;
                    firstInd = i;
                    secondInd = j;
                }
            }
        }
        std::string first = strings[firstInd];
        std::string second = strings[secondInd];

        strings.erase(strings.begin() + firstInd);
        if (firstInd < secondInd) {
            secondInd--;
        }
        strings.erase(strings.begin() + secondInd);

        strings.push_back(merge(first, second, maxoverlap));
    }

    return *strings.begin();
}

std::string makeSuperstring(std::string first, std::string second) {
    return merge(first, second, overlap(first, second));
}

std::string bruteForce(std::vector<std::string>& strings, std::vector<std::vector<int>>& permutations) {
    std::vector<std::string> givenWords = strings;
    std::vector<std::string> results;

    //create superstrings
    std::string partialWord;
    for(const std::vector<int >&p: permutations) {
        for(int i = 0; i < p.size(); ++i) {
            if (i == 0) {
                partialWord = givenWords[p[i]];
                continue;
            }
            if (partialWord.find(givenWords[p[i]]) == std::string::npos) {
                partialWord = makeSuperstring(partialWord, givenWords[p[i]]);
            }
        }
        results.push_back(partialWord);
    }

    //find shortest
    if (results.empty()) {
        return "";
    }
    std::string shortest = results[0];
    for (const std::string& s: results) {
        if (s.length() < shortest.length()) {
            shortest = s;
        }
    }
    return shortest;
}

int main() {
    size_t tests_size = 0;
    size_t words_q = 0;
    std::cin >> tests_size;
    std::cin >> words_q;
    std::vector<std::vector<int>> permutations;

    std::vector<int> indexes;
    indexes.reserve(words_q);
    for (int i = 0; i < words_q; ++i) indexes.push_back(i);
    do {
        permutations.push_back(indexes);
    } while (next_permutation(indexes.begin(), indexes.end()));

    for (int t = 0; t < tests_size; ++t) {
        std::vector<std::string> strings;
        std::string str;

        for (size_t i = 0; i < words_q; ++i) {
            std::cin >> str;
            strings.push_back(str);
        }

        std::string brute = bruteForce(strings, permutations);
        std::string greedy = createSuperString(strings);
        std::cout << brute.length() << "\t" << greedy.length() << std::endl;
    }

    return 0;
}