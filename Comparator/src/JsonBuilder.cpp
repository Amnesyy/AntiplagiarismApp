#include "JsonBuilder.hpp"

using json = nlohmann::json;

    void JsonBuilder::addPlagiarismDetail(const std::string& fileName, double percentage)
    {
        plagiarismDetails[fileName] = percentage;
    }

    void JsonBuilder::setTotalPlagiarism(double total)
    {
        totalPlagiarism = total;
    }

    void JsonBuilder::setParams(int modulo, int w, int k, int method)
    {
        _modulo = modulo;
        _w = w;
        _k = k;
        _method = method;
    }

    json JsonBuilder::buildJson() const
    {
        json result;
        result["total_plagiarism"] = totalPlagiarism;
        result["details"] = plagiarismDetails;
        result["params"]["w"] = _w;
        result["params"]["k"] = _k;
        result["params"]["modulo"] = _modulo;
        result["params"]["method"] = _method == 1 ? "modulo" : "winnowing";
        return result;
    }

    void JsonBuilder::saveToFile(const std::string& filePath) const
    {
        std::ofstream outFile(filePath);
        if (!outFile) {
            throw std::runtime_error("Nie można otworzyć pliku do zapisu: " + filePath);
        }
        outFile << buildJson().dump(4);
        outFile.close();
    }

    void JsonBuilder::printJson() const
    {
        std::cout << buildJson().dump(4) << std::endl;
    }
