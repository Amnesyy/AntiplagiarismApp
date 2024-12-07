#include "KGramGenerator.hpp"
#include <sstream>

bool KGramGenerator::endOfFileReached(int kGramNumber)
{
    int endOfRange = kGramNumber + k;
    return endOfRange > fileContent.length();
}

std::vector<int> KGramGenerator::getHashes(std::string fileName)
{
    std::cout<<"Generating KGrams...\n";
    std::stringstream ss;
    reader.openFile(fileName.c_str()); //lokalizacja plikow zawsze wzgledem builda!!

    fileContent = reader.get();

    std::vector<int> hashes = hash();
    // for(size_t i = 0; i < hashes.size(); ++i)
    // {
    //   if(i != 0)
    //     ss << ",";
    //   ss << hashes[i];
    // }
    // fileContent = ss.str();

    // reader.put(fileContent.c_str(), fileContent.length());
    reader.closeFile();
    // reader.openFile("../File"); //lokalizacja plikow zawsze wzgledem builda!!
// 
    // fileContent = reader.get();
// 
    // std::vector<int> hashes = hash();
    return hashes;
}


std::string KGramGenerator::getKGram(int kGramNumber)
{
    return fileContent.substr(kGramNumber, k); 
}

std::vector<int> KGramGenerator::hash()
{
    std::cout<<"Hashing KGrams...\n";
    std::vector<int> hashes;
    int kGramNumber = 0;
    while(!endOfFileReached(kGramNumber))
    {
        hashes.push_back(std::hash<std::string>{}(getKGram(kGramNumber)));
        kGramNumber++;
    }
    return hashes;
}