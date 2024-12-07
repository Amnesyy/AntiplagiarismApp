#include "Comparator.hpp"
// #include <pair>
#include <sstream>
#include <filesystem>
#include "JsonBuilder.hpp"

#define MODULO 1
#define WINNOWING 2

int method = 0;

std::vector<int> extractHashes(std::map<int,int> fingerprints)
{
    std::vector<int> positions;
    std::transform(fingerprints.begin(), fingerprints.end(), back_inserter(positions),
                   [](const std::pair<int, int>& p)
                   {
                    return p.second;
                   });
    return positions;
}

std::vector<int> compareHashes(std::vector<int> hashesInFile, std::vector<int> hashesInComparativeFile)
{
    std::sort(hashesInFile.begin(), hashesInFile.end());
    std::sort(hashesInComparativeFile.begin(), hashesInComparativeFile.end());

    std::vector<int> commonHashes;

    std::set_intersection(hashesInFile.begin(), hashesInFile.end(),
                          hashesInComparativeFile.begin(), hashesInComparativeFile.end(),
                          std::back_inserter(commonHashes));

    return commonHashes;
}

std::vector<int> getPositionsByHashes(std::vector<int> hashes, std::map<int, int> fingerprints)
{
    std::vector<int> positions;
    if(hashes.size() > 0)
    {
      for(const auto& [pos, hash] : fingerprints)
      {
          if(std::count(hashes.begin(), hashes.end(), hash))
              positions.push_back(pos); 
      }
    }
    return positions;
}

int Comparator::calculateLengthOfCommonText(std::vector<int> filePositions)
{
  int i{0}, totalLength{0};
  while(i + 1 < filePositions.size())
  {
    int currentLength{0};
    while((i < filePositions.size() - 1) && (filePositions[i] + k > filePositions[i+1]))
    {
      currentLength += filePositions[i+1] - filePositions[i];
      i++;
    }
    totalLength += currentLength + k;
    i++;
  }
  return totalLength;
}

void Comparator::writeResultsToFile(std::vector<int> filePositions, 
                                    std::vector<int> comparativeFilePositions, 
                                    std::string fileName, 
                                    std::string comparativeFileName,
                                    double totalFileSize) 
{
    // Tworzenie katalogu Results, jeśli nie istnieje
    std::string directory = "../Results/" + std::string(std::string(method == MODULO ? "modulo" : "winnowing") + "/" + "W:" + std::to_string(w) + "K:" + std::to_string(k) + "Mod:" + std::to_string(modulo) +"/");
    std::filesystem::create_directories(directory);
    // Tworzymy nazwę pliku wynikowego dla tego porównania
    std::string resultsFileName{"../Results/" + directory + comparativeFileName.erase(comparativeFileName.length()-4).substr(20) + ".json"};

    // Obliczamy procent plagiatu w `File` z tego `ComparativeFile`
    int dl = calculateLengthOfCommonText(filePositions) + 1;
    double comparativePercentage = (static_cast<double>(dl) / totalFileSize) * 100;

    // ------------------------
    // ZAPIS DO PLIKU "Results"
    // ------------------------
    try {
        JsonBuilder resultsJson;

        // Dodajemy szczegóły pozycji z pliku File i ComparativeFile
        for (size_t i = 0; i < filePositions.size(); ++i) {
            resultsJson.addPlagiarismDetail("File Position " + std::to_string(i + 1), filePositions[i]);
            resultsJson.addPlagiarismDetail("ComparativeFile Position " + std::to_string(i + 1), comparativeFilePositions[i]);
        }

        // Ustawiamy procent plagiatu z tego pliku
        resultsJson.setTotalPlagiarism(comparativePercentage);

        // Zapisujemy plik JSON
        resultsJson.setParams(modulo, w, k, method);
        resultsJson.saveToFile(resultsFileName);

        std::cout << "Results saved to file: " << resultsFileName << "\n";
    } catch (const std::exception& ex) {
        std::cerr << "Error saving results: " << ex.what() << "\n";
    }

    // Aktualizujemy sumaryczne dane dla `SummaryResults`
    updateSummaryResults(filePositions, comparativePercentage);
}

void Comparator::updateSummaryResults(const std::vector<int>& filePositions, double comparativePercentage) 
{
    static std::vector<int> summaryFilePositions; // Przechowywanie pozycji z File
    static double totalPlagiarismPercentage = 0; // Całkowity procent plagiatu w File

    // Dodanie nowych pozycji do sumarycznych pozycji
    summaryFilePositions.insert(summaryFilePositions.end(), filePositions.begin(), filePositions.end());

    // Zwiększenie całkowitego procentu plagiatu
    totalPlagiarismPercentage += comparativePercentage;

    // ------------------------
    // ZAPIS DO PLIKU "SummaryResults"
    // ------------------------
    try {
        JsonBuilder summaryJson;

        // Dodanie unikalnych pozycji z File
        for (size_t i = 0; i < summaryFilePositions.size(); ++i) {
            summaryJson.addPlagiarismDetail("Position " + std::to_string(i + 1), summaryFilePositions[i]);
        }

        // Ustawienie całkowitego procentu plagiatu
        summaryJson.setTotalPlagiarism(totalPlagiarismPercentage);

        // Zapisujemy plik SummaryResults.json
        std::string resultsFile = "../Results/" + std::string(std::string(method == MODULO ? "modulo" : "winnowing") + "/" + "W:" + std::to_string(w) + "K:" + std::to_string(k) + "Mod:" + std::to_string(modulo) +"/") + "SummaryResults.json";
        summaryJson.setParams(modulo, w, k, method);
        summaryJson.saveToFile(resultsFile);
        std::cout << "Summary updated in file: ../Results/SummaryResults.json\n";
    } catch (const std::exception& ex) {
        std::cerr << "Error saving summary results: " << ex.what() << "\n";
    }
}



void Comparator::compareFilesWinnowingMethod(std::string fileName)
{
  method = WINNOWING;

  std::map<int, int> fileFingerprints = winnower.selectFingerprints(fileName);  

  int fileSize = winnower.getFileSize();

  std::vector<int> hashesInFile = extractHashes(fileFingerprints);

  std::string path = "../ComparativeFiles";
  for (const auto & entry : std::filesystem::directory_iterator(path))
  {
    std::cout<<"Comparing with: " << entry.path() <<std::endl;
    pref.preformatFile(entry.path());
    std::map<int, int> comparativeFingerprints = winnower.selectFingerprints(entry.path()); 
    std::vector<int> hashesInComparativeFile = extractHashes(comparativeFingerprints);  

    std::vector<int> commonHashes = compareHashes(hashesInFile, hashesInComparativeFile);

    std::vector<int> positionsOfHashesInFile = getPositionsByHashes(commonHashes, fileFingerprints);
    std::vector<int> positionsOfHashesInComparativeFile = getPositionsByHashes(commonHashes, comparativeFingerprints);  
    writeResultsToFile(positionsOfHashesInFile, positionsOfHashesInComparativeFile, fileName, entry.path(), static_cast<double>(fileSize));
  }

}

void Comparator::compareFilesModuloMethod(std::string fileName)
{
  method = MODULO;

  std::map<int, int> fileFingerprints = moduler.selectFingerprints(fileName);  

  std::vector<int> hashesInFile = extractHashes(fileFingerprints);  

  int fileSize = moduler.getFileSize();

  std::string path = "../ComparativeFiles";
  for (const auto & entry : std::filesystem::directory_iterator(path))
  {
    std::cout<<"Comparing with: " << entry.path() <<std::endl;
    pref.preformatFile(entry.path());
    std::map<int, int> comparativeFingerprints = moduler.selectFingerprints(entry.path()); 
    std::vector<int> hashesInComparativeFile = extractHashes(comparativeFingerprints);  

    std::vector<int> commonHashes = compareHashes(hashesInFile, hashesInComparativeFile);

    std::vector<int> positionsOfHashesInFile = getPositionsByHashes(commonHashes, fileFingerprints);
    std::vector<int> positionsOfHashesInComparativeFile = getPositionsByHashes(commonHashes, comparativeFingerprints);  
    writeResultsToFile(positionsOfHashesInFile, positionsOfHashesInComparativeFile, fileName, entry.path(), static_cast<double>(fileSize));
  }

  // plagiarisedContentInFile += ((double)commonHashes.size()/(double)hashesInFile.size());
}

void Comparator::compareFiles(int method, std::string file)
{
  if(method != MODULO && method != WINNOWING)
  {
    std::cout<<"No method specified\n";
    return;
  }
  if(method == MODULO)
    compareFilesModuloMethod(file);
  if(method == WINNOWING)
    compareFilesWinnowingMethod(file);
}