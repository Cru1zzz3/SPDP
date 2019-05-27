#ifndef PDP_UTIL_H
#define PDP_UTIL_H

#include <cmath>
#include <random>
#include <cstdlib>
#include <zconf.h>
#include <algorithm>
#include <vector>

using namespace std;

int getRandomNumber();

int getRandomNumber(int leftBound, int rightBound);

unsigned char *generateSecretKey(int size);

vector<unsigned char> mergeVectors(vector<vector<unsigned char>> datablocks, vector<int> indecies, int lim);


#endif //PDP_UTIL_H
