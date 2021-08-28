#ifndef GRAPH_H_
#define GRAPH_H_

#include "Tree.h"
#include <vector>

using namespace std;

class Graph{
public:
    Graph();
    Graph(std::vector<std::vector<int>> matrix);
    void infectNode(int nodeInd);
    bool isInfected(int nodeInd);
    vector<int>* nodeNeighbors(int nodeLabel) const;
    void occupyNode(int nodeInd);
    bool isOccupied(int nodeInd);
    Tree* BFS(const Session& session, int rootLabel);
    void removeNodeFromGraph(int nodeInd);
    virtual ~Graph();
    //Setters
    void setEdges(const vector<vector<int>>& myEdges);
    void setInfectedList(const vector<int>& myInfectedList);

    //Getters
    vector<vector<int>>* getEdges() const;
    vector<int>* getInfectedList() const;
private:
    std::vector<std::vector<int>> edges;
    std::vector<int> infectedList;
};

#endif
