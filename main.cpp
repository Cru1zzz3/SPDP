#include <iostream>
#include <cmath>
#include <string>
#include <openssl/aes.h>
#include <openssl/sha.h>
#include <cstring>

#include "util.h"
#include "FileWorker.h"

using namespace std;

void printHelp() {
    cout << "Enter parameters d, t, k" << endl;
    cout << "d - number of blocks divided database" << endl;
    cout << "t - number of request to storage server" << endl;
    cout << "k - length of secret keys" << endl;
    cout << "h - print help" << endl;
}

// -d n -t n -k n
int main(int argc, char *argv[]) {

    string filename;
    int d = 0, t = 0, k = 0, r = 0, L = 0;

    if (argc < 1) return 0;

    for (int i = 0; i < argc; i++) {
        if (!strncmp(argv[i], "-d", 3))
            d = atoi(argv[i + 1]);
        else if (!strncmp(argv[i], "-t", 3))
            t = atoi(argv[i + 1]);
        else if (!strncmp(argv[i], "-k", 3))
            k = atoi(argv[i + 1]);
        else if (!strncmp(argv[i], "-f", 50))
            filename = argv[i + 1];
        else if (!strncmp(argv[i], "-r", 3))
            r = atoi(argv[i + 1]);
        else if (!strncmp(argv[i], "-L", 3))
            L = atoi(argv[i + 1]);
        else if (!strncmp(argv[i], "-h", 3))
            printHelp();
    }

    auto c = log(t);
    auto l = log(d);

    // generating secret keys
    AES_KEY *s;
    auto W = generateSecretKey(128);
    auto K = generateSecretKey(128);
    auto Z = generateSecretKey(128);

    // reading file
    FileWorker fileWorker;
    auto D = fileWorker.readFile(filename);

    // allocating memory to split file to blocks
    vector<vector<unsigned char>> datablocks(d);

    // coping
    for (int i = 0; i < datablocks.size(); i++)
        for (int j = 0; j < D.size() / d; j++)
            datablocks[i].push_back(D.at(i * d + j));

    //indecies
    auto indices = vector<int>();
    for (int i = 0; i < d; i++)
        indices.push_back(i);

    // permutation key
    auto permKey = vector<unsigned char>(t);
    // challenge nonce
    auto chal = vector<unsigned char>(t);
    // tokens
    auto tokens = vector<unsigned char *>(t);
    // authinticated tokens
    auto auth = vector<unsigned char *>(t);

    auto hash = new unsigned char[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha;
    SHA256_Init(&sha);

    for (int i = 0; i < t; i++) {
        // begin round i
        AES_set_encrypt_key(W, L, s);                   // L = 128
        AES_encrypt(reinterpret_cast<const unsigned char *>(i), reinterpret_cast<unsigned char *>(permKey[i]), s);

        AES_set_encrypt_key(Z, L, s);
        AES_encrypt(reinterpret_cast<const unsigned char *>(i), reinterpret_cast<unsigned char *>(chal[i]), s);

        // random permutation
        mt19937 mt(permKey[i]);
        shuffle(indices.begin(), indices.end(), mt);

        auto temp = mergeVectors(datablocks, indices, r);
        SHA256_Update(&sha, &temp, temp.size());                // тут я не уверен в указателе
        SHA256_Final(hash, &sha);
        tokens[i] = hash;

        AES_set_encrypt_key(K, L, s);
        AES_ofb128_encrypt(tokens[i], auth[i], L, s, 0, 0);         // тут я не уверен насчет нулей, понять зачем в статье тут стоит i

    }

    // отправка на сервер (D, {i, auth[i]}, 1 <= i <= t)

    return 0;
}