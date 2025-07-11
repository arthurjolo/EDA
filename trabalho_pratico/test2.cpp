#include <iostream>
#include <string>

int main() {
    std::string entrada;

    std::cout << "Digite algo (pode estar vazio): ";
    std::getline(std::cin, entrada);

    if (entrada.empty()) {
        std::cout << "Você digitou uma linha vazia." << std::endl;
    } else {
        std::cout << "Você digitou: " << entrada << std::endl;
    }

    return 0;
}