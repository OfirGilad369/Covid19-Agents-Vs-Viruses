#ifndef SESSION_H_
#define SESSION_H_

#include "Graph.h"
#include <vector>
#include <string>
#include <queue>

class Agent;

enum TreeType{
    Cycle,
    MaxRank,
    Root
};

class Session{
public:
    Session(const std::string& path);
    void simulate();
    void addAgent(const Agent& agent);
    void enqueueInfected(int);
    int dequeueInfected();
    void outPutResult();
    bool isQueueEmpty();

    //Setters
    void setGraph(const Graph& graph);
    void setInfectedTreesQueue(vector<int>& myInfectedTreesQueue);
    void setInfectedTressFinalResult(vector<int>& myIetInfectedTressFinalResult);

    //Getters
    Graph* getGraph() const;
    TreeType getTreeType() const;
    vector<Agent*>* getAgents() const;
    int getCycleNumber() const;
    vector<int>* getInfectedTreesQueue();
    vector<int>* getInfectedTressFinalResult();

    //Rule of 5
    virtual ~Session();
    void clearAgents();
    Session(const Session& other);
    Session(Session&& other) noexcept;
    Session& operator=(const Session& other);
    Session& operator=(Session&& other) noexcept;

    //destructor
    //copy construct
    //move construct
    //assignment construct
    //construct assignment construct

private:
    Graph g;
    TreeType treeType;
    std::vector<Agent*> agents;
    int cycleNumber = 0;
    int virusIndex = 0;
    vector<int>infectedTreesQueue;
    vector<int>infectedTressFinalResult;
};

#endif