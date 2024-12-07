#include "WinnowingSelect.hpp"

int WinnowingSelect::getIndexOfSmallestInWindow(std::vector<int> window)
{
    int tmpMin = window[0];
    int minIndex = 0;
    for(int i=0; i<window.size(); i++)
    {
        if(window[i]<tmpMin)
        {
            tmpMin = window[i];
            minIndex = i;
        }
    }
    return minIndex;
}

bool WinnowingSelect::endOfFileReached()
{
    int endOfRange = currentHashPosition + w;
    return endOfRange > hashes.size();
}

std::vector<int> WinnowingSelect::createWindow()
{
    std::vector<int> window;
    for(int i = 0; i < w; i++)
    {
        window.push_back(hashes[currentHashPosition + i]);
    }
    currentHashPosition++;
    return window;
}

int WinnowingSelect::getFileSize()
{
    return fSize;
}

std::map<int, int> WinnowingSelect::selectFingerprints(std::string fileName)
{
    std::cout<<"Hashes selection method: winnowing\n";
    int lastInsertedPosition = 0;
    hashes = generator.getHashes(fileName);
    fSize = hashes.size();
    std::cout<<"hases size: "<<hashes.size()<<std::endl;
    std::map<int, int> fingerprints{};
    fingerprints.insert(std::pair{0, hashes[0]});
    while(!endOfFileReached())
    {
        std::vector<int> window = createWindow();
        int index = getIndexOfSmallestInWindow(window);
        int positionInFile = currentHashPosition + index;
        if(positionInFile > lastInsertedPosition)
        {
            lastInsertedPosition = positionInFile;
            fingerprints.insert(std::pair{positionInFile, window[index]});
        }
    }
    currentHashPosition = 0;
    return fingerprints;
}