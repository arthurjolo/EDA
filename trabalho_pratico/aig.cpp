#include <iostream>
#include <bitset>

#include "aig.hh"

void AndInverterGraph::run()
{
    buildInputs();
    buildCircuit();
    test();
}

void AndInverterGraph::buildInputs()
{
    if(debug_) {
        std::cout << "Gerando inputs:" << std::endl;
        std::cout << "  nInputs: " << nInputs_ << std::endl;
    }
    for(int inputId = 0; inputId < nInputs_; inputId++) {
        GraphNode newInput = GraphNode(inputId, std::to_string(inputId));
        nodes_.push_back(newInput);
    }
    rootId_ = nodes_.size();
    GraphNode output = GraphNode(rootId_, "output");
    nodes_.push_back(output);
}

void AndInverterGraph::buildCircuit()
{
    if(debug_) {
        std::cout << "Gerando circuito:" << std::endl;
        std::cout << "Criando nodos AND: " << std::endl;
    }
    std::vector<Edge> andResults;
    for(std::string minTerm : minTerms_) {
        Edge prev = {-1, -1};
        if(debug_) {
            std::cout << "\nMin term " << minTerm << ": " << std::endl;
        }
        for(int input = 0; input < nInputs_; input++) {
            if(minTerm[input] == '-') {
                continue;
            }
            int weight = minTerm[input] - '0';
            if(debug_) {
                std::cout << "Prev: (" << prev.endPoint << ", " << prev.wight << ") " << std::endl;
                std::cout << "Input " << input << " tem peso: " << weight << std::endl;
            }
            Edge current = {input, weight};
            if(prev.endPoint != -1) {
                int newNodeID = nodes_.size();
                GraphNode currentNode = nodes_[input];
                GraphNode previousNode = nodes_[prev.endPoint];

                GraphNode newAnd = GraphNode(newNodeID, "AND("+ std::to_string(prev.endPoint) +", " + std::to_string(input) +") ");
                nodes_.push_back(newAnd);
                if(debug_) {
                    std::cout << "creating node" << newNodeID << ": " << newAnd.getName() << std::endl;
                }
                edges_[newNodeID].push_back(prev);
                edges_[newNodeID].push_back(current);
                current.endPoint = newNodeID;
                current.wight = 1;
            }
            prev = current;
        }
        andResults.push_back(prev);
    }

    if(debug_) {
        std::cout << "\nFinal nodes for the ands: " << std::endl;
        for(Edge edge : andResults) {
            std::cout << "Chegando no nodo " << edge.endPoint << ": " << nodes_[edge.endPoint].getName() << "; com peso: " << edge.wight << std::endl;
        }

        std::cout << "\nCriando nodos OR: " << std::endl;
    }

    Edge prev = {-1, -1};
    for(Edge edge : andResults) {
        if(prev.endPoint == -1) {
            prev = edge;
            continue;
        }
        int newNodeID = nodes_.size();
        GraphNode currentNode = nodes_[edge.endPoint];
        GraphNode previousNode = nodes_[prev.endPoint];

        GraphNode newOr = GraphNode(newNodeID, "OR("+ previousNode.getName()+", "+ currentNode.getName() +") ");
        nodes_.push_back(newOr);
        if(debug_) {
            std::cout << "creating node: " << newOr.getName() << std::endl;
        }
        edge.wight = (edge.wight + 1) % 2; 
        prev.wight = (prev.wight + 1) % 2;
        edges_[newNodeID].push_back(prev);
        edges_[newNodeID].push_back(edge);
        prev.endPoint = newNodeID;
        prev.wight = 0;
    }

    edges_[rootId_].push_back(prev);
}

void AndInverterGraph::resetNodesValue()
{
    for(int nodeID = 0; nodeID < nodes_.size(); nodeID++) {
        nodes_[nodeID].setCurrValue(1); 
    }
}

void AndInverterGraph::test()
{
    GraphNode root = nodes_[rootId_];
    for (int i = 0; i < (1 << nInputs_); i++) {
        std::bitset<32> b(i);
        testInput_ = b.to_string().substr(32-nInputs_);
        resetNodesValue();
        dfs(rootId_);
        if(nodes_[rootId_].getCurrValue()) {
            activatedTerms_.push_back(testInput_);
        }
        if(debug_) {
            std::cout << "Resultado para a entrada " <<  testInput_ << " = " << nodes_[rootId_].getCurrValue() << std::endl;
        }
    }
}

void AndInverterGraph::dfs(int nodeID)
{
    if(edges_[nodeID].size() == 0) {
        nodes_[nodeID].setCurrValue(testInput_[nodeID] - '0');
        return;
    }
    int nodeCurrValue = nodes_[nodeID].getCurrValue();
    std::vector<int> childs;
    for(Edge edge : edges_[nodeID]) {
        dfs(edge.endPoint);
        int childInput;
        if(!edge.wight) {
            childInput = (nodes_[edge.endPoint].getCurrValue() + 1) % 2;
        } else {
            childInput = nodes_[edge.endPoint].getCurrValue();
        }
        nodeCurrValue &= childInput;
    }
    if(debug_) {
        std::cout << "Node " << nodeID << ":" << std::endl;
        for(Edge edge : edges_[nodeID]) {
            std::cout << "  Filho: " << edge.endPoint << " value " << nodes_[edge.endPoint].getCurrValue() << ", weight " << edge.wight << std::endl;
        }
        std::cout << "  Ficou com: " << nodeCurrValue << std::endl;
    }
    nodes_[nodeID].setCurrValue(nodeCurrValue);
}
