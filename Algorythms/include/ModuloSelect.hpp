#include <vector>
#include <map>
#ifndef KGRAM_H
#define KGRAM_H
#include "KGramGenerator.hpp"
#endif
class ModuloSelect
{
public:
    ModuloSelect(int _modulo, int _k): modulo(_modulo), generator(KGramGenerator(_k))
    {};
    std::map<int,int> selectFingerprints(std::string);
    int getFileSize();
private:
    int modulo, fSize;
    KGramGenerator generator;
    std::vector<int> hashes;
};