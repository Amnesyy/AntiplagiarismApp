#include <nlohmann/json.hpp>
#include <string>
#include <map>
#include <iostream>
#include <fstream>

using json = nlohmann::json;

class JsonBuilder {
public:
    void addPlagiarismDetail(const std::string& fileName, double percentage);
    void setTotalPlagiarism(double total);
    void setParams(int, int, int, int);
    json buildJson() const;
    void saveToFile(const std::string& filePath) const;
    void printJson() const;

private:
    double totalPlagiarism = 0.0; 
    std::map<std::string, double> plagiarismDetails; 
    int _w, _k, _modulo, _method;
};
