#ifndef PDP_FILEWORKER_H
#define PDP_FILEWORKER_H

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class FileWorker {

public:
    vector<char> readFile(string filenamae);

    void writeFile(char *data, string filename);
};


#endif //PDP_FILEWORKER_H
