#include "WinnowingSelect.hpp"
#include "ModuloSelect.hpp"
#include <map>
// #ifndef READER_H
// #define READER_H
// #include "Reader.hpp"
// #endif
#ifndef PREF_H
#define PREF_H
#include "Preformatter.hpp"
#endif

class Comparator
{
public:
    Comparator(int _w, int _modulo, int _k): winnower(WinnowingSelect(_w, _k)), moduler(ModuloSelect(_modulo, _k)), modulo(_modulo), w(_w), k(_k)
    {}; 
    void compareFilesWinnowingMethod(std::string);
    void compareFilesModuloMethod(std::string);
    void compareFiles(int, std::string);
    void writeResultsToFile(std::vector<int>, std::vector<int>, std::string, std::string, double);
    void updateSummaryResults(const std::vector<int>&, double);
    int calculateLengthOfCommonText(std::vector<int>);
private:
    int modulo, w, k;
    WinnowingSelect winnower;
    ModuloSelect moduler;
    Reader reader;
    Preformatter pref;
    double plagiarisedContentInFile{0.0};
};