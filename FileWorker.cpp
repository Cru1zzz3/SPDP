#include "FileWorker.h"

vector<char> FileWorker::readFile(string filename) {
    ifstream file;
    file.open(filename);

    if (!file.is_open()) {
        cout << "Error opening file" << endl;
    }

    file.seekg(0, ios::end);
    auto size = file.tellg();
    file.seekg(0, ios::beg);

    auto data = vector<char>();
    char ch;

    for (int i = 0; i < size; i++) {
        file.get(ch);
        data.push_back(ch);
    }

    return data;
}

void FileWorker::writeFile(char *data, string filename) {
    std::ofstream file(filename, std::ios::binary);

    if (file) {
        file << data;
    } else {
        std::cout << "Error writing file." << std::endl;
    }
}
