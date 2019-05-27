#include "util.h"

int getRandomNumber() {
    srand(time(nullptr));
    int r = rand();
    return r;
}

int getRandomNumber(int leftBound, int rightBound) {
    int r = getRandomNumber();
    while (r >= leftBound && r <= rightBound) {
        r = getRandomNumber();
    }
    return r;
}

unsigned char *generateSecretKey(int size) {
    sleep(1);
    srand(time(NULL));
    auto *r = new unsigned char[size / 8];
    for (int i = 0; i < size /  8; i++) {
        r[i] = static_cast<unsigned char>(rand() % 255);
    }
    return r;
}

/* indecies -- vector of randomly permutated indecies
 * lim -- limit of vectors to be merged
 * datablocks -- source data*/
vector<unsigned char> mergeVectors(vector<vector<unsigned char>> datablocks, vector<int> indecies, int lim) {
    vector<unsigned char> r;
    for (int i = 0; i < lim; i++) {
        for (int j = 0; j < datablocks[i].size(); j++) {
            r.push_back(datablocks[indecies[i]].at(j));
        }
    }
    return r;
}
