#include <iostream>
#include "Comparator/include/Comparator.hpp"
// #endif
void print_map(const std::map<int, int>& m)
{
    for (const auto& [key, value] : m)
        std::cout << '[' << key << "] = " << value << "\n ";
 
    std::cout << '\n';
}

int main(int argc, char* argv[])
{
    int K{1}, W{1}, modulo{1};
    bool use_winnowing{0};
      if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            std::string arg = argv[i];
            if (arg == "--K" && i + 1 < argc) {
                K = std::atoi(argv[++i]);
            } else if (arg == "--W" && i + 1 < argc) {
                W = std::atoi(argv[++i]);
            } else if (arg == "--modulo" && i + 1 < argc) {
                modulo = std::atoi(argv[++i]);
            } else if (arg == "--algorithm" && i + 1 < argc) {
                std::string algo = argv[++i];
                use_winnowing = (algo == "winnowing");
            } else {
                std::cerr << "Nieznany argument: " << arg << std::endl;
                return 1;
            }
        }
    }
    std::cout << "Parametry:\n";
    std::cout << "K = " << K << "\n";
    std::cout << "W = " << W << "\n";
    std::cout << "Modulo = " << modulo << "\n";
    std::cout << "Algorytm: " << (use_winnowing ? "Winnowing" : "Modulo") << "\n";

    Preformatter pref;
    pref.preformatFile("../File");
    // ModuloSelect selector(51);
    // std::map<int, int> fingerprints = selector.selectFingerprints();
    
    // print_map(fingerprints);
    Comparator comp(W, modulo, K);
    if(use_winnowing)
    {
        comp.compareFiles(2, "../File");
    }else{
        comp.compareFiles(1, "../File");
    }
}