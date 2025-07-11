#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

#include "QuineMcCluskey.hh"
#include "aig.hh"

int readFiles(int& nInputs, std::vector<std::string>& minTerms, std::vector<std::string>& dontCares) {
    std::string minTermsFileName;
    std::string dontCaresFileName;
    std::string linha;

    std::cout << "Path to min terms file: ";
    std::getline(std::cin, minTermsFileName);

    std::ifstream minTermsFile(minTermsFileName);
    if (!minTermsFile.is_open()) {
        std::cerr << "Error when opening min terms file." << std::endl;
        return 1;
    }

    nInputs = 0; 
    while (std::getline(minTermsFile, linha)) {
        if (!linha.empty()) {
            if(!nInputs) {
                nInputs = linha.size();
            } else if(nInputs != linha.size()) {
                std::cerr << "Error: min terms don't have the same number of inputs!." << std::endl;
                return 1;
            }
            minTerms.push_back(linha);
        }
    }

    if(!nInputs) {
        std::cerr << "Error: no min terms were read!." << std::endl;
        return 1;
    }
    minTermsFile.close();

    std::cout << "Path to dont cares file (if there is none press [ENTER]): ";
    std::getline(std::cin, dontCaresFileName);

    if(!dontCaresFileName.empty()) {
        std::ifstream dontCaresFile(dontCaresFileName);
        if (!dontCaresFile.is_open()) {
            std::cerr << "Error when opening dont cares file." << std::endl;
            return 1;
        }

        while (std::getline(dontCaresFile, linha)) {
            if (!linha.empty()) {
                dontCares.push_back(linha);
            }
        }
        dontCaresFile.close();
    }
    return 0;
}

bool testResults(std::vector<std::string> minTerms,
                 std::vector<std::string> dontCares,
                 std::vector<std::string> results)
{
    bool pass = true;
    for (const std::string& binario : results) {
        if (std::find(minTerms.begin(), minTerms.end(), binario) == minTerms.end()) {
            if (std::find(dontCares.begin(), dontCares.end(), binario) == dontCares.end()) {
                pass = false;
            }
        }
    }
    if(!pass) {
        std::cout << "Are activacted but are not in min terms or dont cares list." << std::endl;
    }
    return pass;
}

int main() {
    std::vector<std::string> minTerms;
    std::vector<std::string> dontCares;

    int nInputs;
    if(readFiles(nInputs, minTerms, dontCares)) {
        return 1;
    }

    std::cout << "\nInput logic: ";
    bool first = true;
    for(const std::string& binario : minTerms) {
        if(first) {
            std::cout << binario;
            first = false;
        } else {
            std::cout << " + " << binario;
        }
    }

    std::cout << "\n\nRuning QuineMcCluskey algorithm ..." << std::endl;
    QuineMcCluskey ferramenta = QuineMcCluskey(nInputs, minTerms, dontCares);
    ferramenta.run();
    std::cout << "  Optimized logic: ";
    first = true;
    for(const std::string& binario : ferramenta.getRestuls()) {
        if(first) {
            std::cout << binario;
            first = false;
        } else {
            std::cout << " + " << binario;
        }
    }
    std::cout << std::endl;

    std::cout << "\nBuilding AIG ..." << std::endl;
    AndInverterGraph aig = AndInverterGraph(nInputs, ferramenta.getRestuls());
    aig.run();

    std::vector<std::string> testsRestuls = aig.getActivatedTerms();

    std::cout << "\nTesting QuineMcCluskey results ..." << std::endl;
    if(testResults(minTerms, dontCares, testsRestuls)) {
        std::cout << "QuineMcCluskey logic is equivalent to the input!" << std::endl;
    } else {
        std::cerr << "ERROR: QuineMcCluskey logic is NOT equivalent to the input!" << std::endl;
        return 1;
    }
    return 0;
}