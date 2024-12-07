#include <map>
#include <vector>
#ifndef KGRAM_H
#define KGRAM_H
#include "KGramGenerator.hpp"
#endif

class WinnowingSelect
{
public:
    WinnowingSelect(int _w, int _k): w(_w), generator(KGramGenerator(_k))
    {};
    int getFileSize();
    std::map<int,int> selectFingerprints(std::string);
private:
    std::vector<int> createWindow();
    bool endOfFileReached();
    int getIndexOfSmallestInWindow(std::vector<int>);

    KGramGenerator generator;
    std::vector<int> hashes;
    int w;
    int currentHashPosition{0};
    int fSize;
};