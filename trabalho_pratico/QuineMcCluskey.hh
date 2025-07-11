#include <string>
#include <vector>
#include <map>
#include <set>

class Implicant
{
    public:
    Implicant(std::string expression) : exp_(expression) {};
    ~Implicant() = default;

    std::string getExpression() {return exp_; };
    void setExpression(std::string expression) { exp_ = expression; };

    void addMinTerms(std::vector<int> minTerms) { minTerms_.insert(minTerms_.end(), minTerms.begin(), minTerms.end()); };
    void addMinTerm(int minTerm) { minTerms_.push_back(minTerm); };
    void removeMinTerm(int minTerm);
    std::vector<int> getMinTerms() {return minTerms_; }

    int getCoverage() { return minTerms_.size(); };

    private:
    std::string exp_;
    std::vector<int> minTerms_;
};

class QuineMcCluskey 
{
    public:
    QuineMcCluskey(int nInputs,
                   std::vector<std::string> minTerms,
                   std::vector<std::string> dontCares) :
                     nInputs_(nInputs),
                     minTerms_(minTerms),
                     dontCares_(dontCares) {};
    ~QuineMcCluskey() = default;

    void run();
    Implicant bestImplicante();
    void createTable();
    void setDebuf(bool debug) { debug_ = debug; };
    std::vector<std::string> getRestuls() { return result_; };
    


    private:
    int nInputs_;
    bool debug_ = false;

    std::vector<std::string> minTerms_;
    std::vector<std::string> dontCares_;
    std::map<int, std::vector<Implicant>> table_;
    std::vector<Implicant> implicantePrimos_;
    std::map<int, std::vector<int>> minTerm2Implicante_;
    std::set<int> minTermContemplados_;
    std::vector<std::string> result_;
};