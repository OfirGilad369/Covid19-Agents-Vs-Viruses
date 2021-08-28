#include "../include/Graph.h"
#include "../include/Session.h"
#include <queue>
#include <utility>
#include <vector>

using namespace std;

//infectedList list value:
//0 = Virus Free
//1 = Infected
//2 = Occupied

Graph::Graph() : edges(vector<vector<int>>()) , infectedList(vector<int>()) { }

Graph::Graph(vector<vector<int>> matrix) : edges(), infectedList() {
    edges = std::move(matrix);
    infectedList = vector<int>(edges.size(),0);
}

void Graph::infectNode(int nodeInd) {
    infectedList[nodeInd] = 1;
}

bool Graph::isInfected(int nodeInd) {
    if (infectedList[nodeInd] == 1)
        return true;
    else return false;
}

vector<int>* Graph::nodeNeighbors(int nodeLabel) const {
    return (vector<int>*) (&edges[nodeLabel]);
}

void Graph::occupyNode(int nodeInd) {
    infectedList[nodeInd] = 2;
}

bool Graph::isOccupied(int nodeInd) {
    if (infectedList[nodeInd] == 2)
        return true;
    else return false;
}

Tree* Graph::BFS(const Session &session, int rootLabel) {
    //node order to be checked
    queue<Tree*> treeLine;

    //Setting Up
    int numberOfNodes = edges.size();
    vector<bool> availableNodes = vector<bool>(numberOfNodes, true);

    //Creating Root
    Tree* root = Tree::createTree(session, rootLabel);
    treeLine.push(root);
    availableNodes[root->getRootLabel()] = false;

    //running BFS algorithm
    while (!treeLine.empty()) {

        //Dequeue the next node
        Tree* currentNode = treeLine.front();
        treeLine.pop();
        vector<int> nodeNeighbors = edges[currentNode->getRootLabel()];

        //Checking currentNode Neighbors and creating them
        for (int i = 0; i < numberOfNodes; i++) {
            if ((nodeNeighbors[i] == 1) && availableNodes[i]) {

                //Creating tree node
                availableNodes[i] = false;
                Tree* nextNode = Tree::createTree(session, i);

                //Enqueue nodes to the next BFS iteration
                currentNode->addChild(nextNode);
                treeLine.push(nextNode);
            }
        }
    }
    return root;
}

void Graph::removeNodeFromGraph(int nodeInd) {
    edges[nodeInd] = vector<int>(edges.size(), 0);
    for (vector<int> & nodeEdges : edges) {
        nodeEdges[nodeInd] = 0;
    }
}

void Graph::setEdges(const vector<vector<int>>& myEdges) {
    edges = myEdges;
}

void Graph::setInfectedList(const vector<int>& myInfectedList) {
    infectedList = myInfectedList;
}

vector<vector<int>>* Graph::getEdges() const {
    return (vector<vector<int>>*) (&edges);
}

vector<int>* Graph::getInfectedList() const{
    return (vector<int>*) (&infectedList);
}

Graph::~Graph() {
    edges.clear();
    infectedList.clear();
}