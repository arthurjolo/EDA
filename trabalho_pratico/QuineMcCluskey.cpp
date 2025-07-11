#include <iostream>
#include <algorithm>

#include "QuineMcCluskey.hh"

void Implicant::removeMinTerm(int minTerm) 
{
    auto remove = std::find(minTerms_.begin(), minTerms_.end(), minTerm);
    if(remove != minTerms_.end()) {
        minTerms_.erase(remove);
    }
}

void QuineMcCluskey::run()
{
    createTable();
    
    return;
}

Implicant QuineMcCluskey::bestImplicante()
{
    int bestCoverage = 0;
    int bestImplicant = -1;
    for(int i = 0; i < implicantePrimos_.size(); i++) {
        Implicant imp = implicantePrimos_[i];
        int coverage = imp.getCoverage();
        if(bestCoverage < coverage) {
            bestCoverage = coverage;
            bestImplicant = i;
        }
    }
    if(bestImplicant == -1) {
        std::cout << "não achou o melhor" << std::endl;
        Implicant* quebraaa = &implicantePrimos_[100];
        auto t = quebraaa->getExpression();
    }
    return implicantePrimos_[bestImplicant];
}

void QuineMcCluskey::createTable()
{
    if(debug_) {
        std::cout << "Minterms: " << std::endl;
    }
    for(std::string minTerm : minTerms_) {
        int cont = 0;
        for(auto c : minTerm) {
            if(c == '1') {
                cont += 1;
            }
        }
        int n_minterm = std::stoi(minTerm, nullptr, 2);
        if(debug_) {
            std::cout << "Min term: " << n_minterm << std::endl;
        }
        Implicant implicante = Implicant(minTerm);
        implicante.addMinTerm(n_minterm);
        table_[cont].push_back(implicante);
    }

    for(std::string dontCare : dontCares_) {
        int cont = 0;
        for(auto c : dontCare) {
            if(c == '1') {
                cont += 1;
            }
        }
        Implicant implicante = Implicant(dontCare);
        table_[cont].push_back(implicante);
    }

    std::map<int, std::vector<Implicant>> testTempTable;
    std::vector<std::string> mergedExp;
    std::vector<std::string> reachedExps;
    while (!table_.empty()) {
        for(auto it = table_.begin(); it != table_.end(); it++) {
            if(debug_) {
                std::cout << std::endl;
                std::cout << "Número de 1s: " << it->first << std::endl;
            }
            auto next = std::next(it);
            std::vector<Implicant> comp_next;
            if(next == table_.end()) {
                comp_next = std::vector<Implicant>();
            } else {
                comp_next = next->second;
            }
            for(Implicant implicante : it->second) {
                int n_merges = 0;
                std::string exp = implicante.getExpression();
                if(debug_) {
                    std::cout << "Analizando exp " << exp << std::endl;
                }
                for(Implicant implicante2 : comp_next) {
                    int diff = 0;
                    int last_diff = 0;
                    std::string comp_exp = implicante2.getExpression();
                    for(int i = 0; i < exp.size(); i++) {
                        if(exp[i] != comp_exp[i]) {
                            diff += 1;
                            last_diff = i;
                        }
                    }
                    if(diff == 1) {
                        n_merges += 1;
                        std::string new_exp = exp;
                        new_exp[last_diff] = '-';
                        mergedExp.push_back(exp);
                        mergedExp.push_back(comp_exp);
                        if (std::find(reachedExps.begin(), reachedExps.end(), new_exp) != reachedExps.end()) {
                            continue;
                        }
                        reachedExps.push_back(new_exp);
                        Implicant newImplicat = Implicant(new_exp);

                        newImplicat.addMinTerms(implicante.getMinTerms());
                        newImplicat.addMinTerms(implicante2.getMinTerms());
                        testTempTable[it->first].push_back(newImplicat);
                        if(debug_) {
                            std::cout << "Mege expressions: " << exp << " and " << comp_exp << std::endl;
                            std::cout << "  result: " << new_exp << std::endl;
                        }
                    }
                }

                if((!n_merges) && (std::find(mergedExp.begin(), mergedExp.end(), exp) == mergedExp.end())) {
                    if(debug_) {
                        std::cout << exp << " é implicante" << std::endl;
                    }
                    for(int minTerm : implicante.getMinTerms()) {
                        minTerm2Implicante_[minTerm].push_back(implicantePrimos_.size());
                    }
                    implicantePrimos_.push_back(implicante);
                }
            }
        }
        if(debug_) {
            std::cout << std::endl;
        }
        table_.clear();
        table_ = testTempTable;
        testTempTable.clear();
        mergedExp.clear();
        reachedExps.clear();
    }

    if(debug_) {
        std::cout << "Finalizou" << std::endl;
        for(auto el : table_) {
            std::cout << el.second.size() << " tem " << el.first << " 1s" << std::endl; 
            for(auto implicant : el.second) {
                std::cout << "expressão: " << implicant.getExpression() << " contempla os minterms: " << std::endl;
                std::cout << "  [";
                for(auto ms: implicant.getMinTerms()) {
                    std::cout << ms << ", ";
                }
                std::cout << "]" << std::endl;
            }
        }

        std::cout << "Tamanho dos implicantes: " << implicantePrimos_.size() << std::endl << std::endl;
        for(auto implicante : implicantePrimos_) {
            std::cout << "Implicante: " << implicante.getExpression() << std::endl;
            std::cout << " contempla os minterms: [";
            for(auto minterm : implicante.getMinTerms()) {
                    std::cout << minterm << ", ";
            }
            std::cout << "]" << std::endl;
        }
        std::cout << std::endl;
    }
    for(auto el : minTerm2Implicante_) {
        if(debug_) {
            std::cout << el.first << " contemplado pelos implicantes : [";
            for(auto implicante: el.second) {
                std::cout << implicante << ", ";
            }
            std::cout << "]" << std::endl;
        }
        if(el.second.size() == 1) {
            Implicant implicante = implicantePrimos_[el.second[0]];
            result_.push_back(implicante.getExpression());
            for(int minTerm : implicante.getMinTerms()) {
                minTermContemplados_.insert(minTerm);
            }
        }
    }

    for(int minTerm : minTermContemplados_) {
        for(int imp : minTerm2Implicante_[minTerm]) {
            implicantePrimos_[imp].removeMinTerm(minTerm);
        }
    }
    if(debug_) {
        std::cout << std::endl;
        std::cout << "Implicantes primos essenciais contemplou " << minTermContemplados_.size() << " min terms" << std::endl;
        if(minTermContemplados_.size() != minTerms_.size()) {
            std::cout << "Iniciando busca gulosa por melhores implicantes: " << std::endl;
        }
    }
    while (minTermContemplados_.size() != minTerms_.size()) {
        Implicant escolhido = bestImplicante();
        if(debug_) {
            std::cout << "  Escolhido foi: " << escolhido.getExpression() << " contemplando: " << escolhido.getCoverage() << " minterms" << std::endl;
        }
        result_.push_back(escolhido.getExpression());
        for(int minTerm : escolhido.getMinTerms()) {
            minTermContemplados_.insert(minTerm);
            for(int imp : minTerm2Implicante_[minTerm]) {
                implicantePrimos_[imp].removeMinTerm(minTerm);
            }
        }
    }
    if(debug_) {
        std::cout << std::endl;
        std::cout << "result: [";
        for(std::string escolhido :result_) {
            std::cout << escolhido << ", ";
        }
        std::cout << "]" << std::endl;
    }

    return;
}