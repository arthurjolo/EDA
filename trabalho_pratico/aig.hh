#include <vector>
#include <map>
#include <string>

struct Edge
{
    int endPoint;
    int wight;
};


class GraphNode
{
    public:
    GraphNode(int id, std::string name) : id_(id), name_(name) {};
    ~GraphNode() = default;
    std::string getName() { return name_; };

    void setCurrValue(int value) { currValue_ = value; };
    int getCurrValue() { return currValue_; };
    int getId() { return id_; };

    private:
    int id_;
    std::string name_;
    int currValue_;
    std::vector<int> children_;
};


class AndInverterGraph 
{
    public:
    AndInverterGraph(int nInputs, std::vector<std::string> minTerms) :
                nInputs_(nInputs),
                minTerms_(minTerms) {};
    ~AndInverterGraph() = default;

    void run();
    void buildInputs();
    void buildCircuit();
    void resetNodesValue();
    void test();
    void dfs(int nodeID);
    void setDebug(bool debug) { debug_ = debug; };
    std::vector<std::string> getActivatedTerms() { return activatedTerms_; };

    private:
    int nInputs_;
    int rootId_;
    bool debug_ = false;
    std::string testInput_ = "0100";
    std::vector<std::string> minTerms_;
    std::vector<std::string> activatedTerms_;
    std::vector<GraphNode> nodes_;
    std::map<int, std::vector<Edge>> edges_;


};