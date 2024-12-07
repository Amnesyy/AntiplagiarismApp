#include "ModuloSelect.hpp"

int ModuloSelect::getFileSize()
{
    return fSize;
}

std::map<int,int> ModuloSelect::selectFingerprints(std::string fileName)
{
    std::cout<<"Hashes selection method: modulo\n";
    hashes = generator.getHashes(fileName);
    fSize = hashes.size();
    std::map<int, int> fingerprints{};
    for(int i=0; i<hashes.size(); i++)
    {
        if(!(hashes[i]%modulo))
        {
            fingerprints.insert(std::pair{i, hashes[i]});
        }
    }
    return fingerprints;
}