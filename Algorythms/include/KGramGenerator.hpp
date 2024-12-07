#include <vector>
#ifndef READER_H
#define READER_H
#include "Reader.hpp"
#endif

//!!!!!!!!!!!!!!!!!!!!!!
//Hashowanie w tej klasie bo w przyszlosci moga powstac inne algorytmy ktore beda wykorzystywac preformat pliku ale bez hashowania, jak nie powstana to hashowanie mozna wrzucic do FilePrepa
//!!!!!!!!!!!!!!!!!!!!!!!

class KGramGenerator
{
public:
    KGramGenerator(int _k): k(_k){};
    std::vector<int> getHashes(std::string);

private:
    std::string getKGram(int);
    std::vector<int> hash();
    bool endOfFileReached(int);

    int k;
    std::string kGram;
    std::string fileContent;
    Reader reader;
};