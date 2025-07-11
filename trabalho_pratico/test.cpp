#include <string>
#include <iostream>
#include <vector>
#include <map>

#include "QuineMcCluskey.hh"

int main()
{
    std::vector<std::string> minTerms = std::vector<std::string>();
    std::vector<std::string> dontCares = std::vector<std::string>();
    // Teste da video aula:
    /*minTerms.push_back("0100");  //  4
    minTerms.push_back("0101");  //  5
    minTerms.push_back("0110");  //  6
    minTerms.push_back("1000");  //  8
    minTerms.push_back("1001");  //  9
    minTerms.push_back("1010");  // 10
    minTerms.push_back("1101");  // 13

    dontCares.push_back("0000");  //  0
    dontCares.push_back("0111");  //  7
    dontCares.push_back("1111");  // 15*/

    // Teste da prof:
    minTerms.push_back("0010");  //  2  
    minTerms.push_back("0100");  //  4
    minTerms.push_back("0110");  //  6
    minTerms.push_back("1000");  //  8
    minTerms.push_back("1001");  //  9
    minTerms.push_back("1010");  // 10
    minTerms.push_back("1100");  // 12
    minTerms.push_back("1101");  // 13
    minTerms.push_back("1111");  // 15
    char teste = '1';
    int teste_int = minTerms[0][2] - '0';
    std::cout<<"Vamo ver se ele sabe: "<<std::atoi(&minTerms[0][2])<<std::endl;
    std::cout<<"Vamo ver se ele sabe: "<<teste_int<<std::endl;
    std::cout<<"estrnaho: "<<minTerms[0][0]<<std::endl;

    QuineMcCluskey ferramenta = QuineMcCluskey(4, minTerms, dontCares);
    ferramenta.run();
    return 0;
}