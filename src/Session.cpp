#include "../include/Session.h"
#include "../include/json.hpp"
#include "../include/Agent.h"
#include <fstream>
#include <iostream>
#include <utility>

using namespace std;
using json = nlohmann::json;

Session::Session(const std::string& path) : g() , treeType(), agents(), infectedTreesQueue(), infectedTressFinalResult() {
    //Importing json file data to the program
    ifstream sessionStreamInput(path);
    json jsonFile;
    sessionStreamInput >> jsonFile;
    string jsonTreeType = jsonFile.at("tree");
    vector<vector<int>> sessionMatrix = jsonFile.at("graph");
    vector<pair<string,int>> sessionAgents = jsonFile.at("agents");
    setGraph(Graph(sessionMatrix));
    //Checking the tree type frim the json file
    if (jsonTreeType == "C") {
        treeType = Cycle;
    }
    else if (jsonTreeType == "M") {
        treeType = MaxRank;
    }
    else {
        treeType = Root;
    }
    //Inserting the agents from the json file into the agents vector
    for (const pair<string,int>& sessionAgent : sessionAgents) {
        if (sessionAgent.first == "V") {
            Agent* virusAgent = new Virus(sessionAgent.second);
            agents.push_back(virusAgent);
            g.occupyNode(sessionAgent.second);
        }
        else {
            Agent* contactTracerAgent = new ContactTracer();
            agents.push_back(contactTracerAgent);
        }
    }
}

void Session::simulate() {
    //Running simulation
    bool isSessionActive = true;
    while (isSessionActive) {
        int numberOfActiveViruses = 0;
        //Checking if there are active viruses left
        for (Agent * agent : agents) {
            if (agent->getIsActiveVirus()) {
                numberOfActiveViruses = numberOfActiveViruses + 1;
            }
        }
        if (numberOfActiveViruses == 0) {
            isSessionActive = false;
        }
        else {
            //Running the current cycle
            int currentNumberOfAgents = agents.size();
            for (int i = 0; i < currentNumberOfAgents; i++) {
                agents[i]->act(*this);
            }
        }
        cycleNumber = cycleNumber + 1;
    }
    //Calling export results function
    outPutResult();
}

void Session::addAgent(const Agent &agent) {
    auto* cloneAgent = (Agent*) (&agent);
    cloneAgent = cloneAgent->clone();
    agents.push_back(cloneAgent);
}

void Session::enqueueInfected(int nodeInd) {
    g.infectNode(nodeInd);
    infectedTreesQueue.push_back(nodeInd);
    infectedTressFinalResult.push_back(nodeInd);
}

int Session::dequeueInfected() {
    int nextInfectedTree = infectedTreesQueue[virusIndex];
    virusIndex = virusIndex + 1;
    return nextInfectedTree;
}

void Session::outPutResult() {
    //Exporting program data to output json file
    json outputJsonFile;
    outputJsonFile["graph"] = *g.getEdges();
    outputJsonFile["infected"] = infectedTressFinalResult;
    ofstream sessionStreamOutput("./output.json");
    sessionStreamOutput << outputJsonFile;
}

bool Session::isQueueEmpty() {
    int queueSize = infectedTreesQueue.size();
    return virusIndex >= queueSize;
}

void Session::setGraph(const Graph &graph) {
    g = graph;
}

void Session::setInfectedTreesQueue(vector<int>& myInfectedTreesQueue) {
    infectedTreesQueue = myInfectedTreesQueue;
}

void Session::setInfectedTressFinalResult(vector<int>& myIetInfectedTressFinalResult) {
    infectedTressFinalResult = myIetInfectedTressFinalResult;
}

Graph* Session::getGraph() const {
    return (Graph*) (&g);
}

TreeType Session::getTreeType() const {
    return treeType;
}

vector<Agent*>* Session::getAgents() const {
    return (vector<Agent*>*) (&agents);
}

int Session::getCycleNumber() const {
    return cycleNumber;
}

vector<int>* (Session::getInfectedTreesQueue()) {
    return (vector<int>*) &infectedTreesQueue;
}

vector<int>* Session::getInfectedTressFinalResult() {
    return (vector<int>*) (&infectedTressFinalResult);
}

Session::~Session() {
    clearAgents();
    infectedTreesQueue.clear();
    infectedTressFinalResult.clear();
}

void Session::clearAgents() {
    for (Agent* agent : agents) {
        delete agent;
    }
    agents.clear();
}

Session::Session(const Session& other) : g(), treeType(), agents(), infectedTreesQueue(), infectedTressFinalResult() {
    g = other.g;
    treeType = other.treeType;
    for (Agent* agent : other.agents) {
        Agent* newAgent = agent->clone();
        agents.push_back(newAgent);
    }
    cycleNumber = other.cycleNumber;
    virusIndex = other.virusIndex;
    infectedTreesQueue = other.infectedTreesQueue;
    infectedTressFinalResult = other.infectedTressFinalResult;
}

Session::Session(Session&& other) noexcept : g(), treeType(), agents(), infectedTreesQueue(), infectedTressFinalResult()  {
    g = other.g;
    treeType = other.treeType;
    for (Agent* agent : other.agents) {
        agents.push_back(agent);
    }
    cycleNumber = other.cycleNumber;
    virusIndex = other.virusIndex;
    infectedTreesQueue = other.infectedTreesQueue;
    infectedTressFinalResult = other.infectedTressFinalResult;
    other.agents.clear();
}

Session &Session::operator=(const Session& other) {
    if (this == &other) {
        return *this;
    }
    clearAgents();
    g = other.g;
    treeType = other.treeType;
    for (Agent* agent : other.agents) {
        Agent* cloneAgent = agent->clone();
        agents.push_back(cloneAgent);
    }
    cycleNumber = other.cycleNumber;
    virusIndex = other.virusIndex;
    infectedTreesQueue = other.infectedTreesQueue;
    infectedTressFinalResult = other.infectedTressFinalResult;
    return *this;
}

Session &Session::operator=(Session&& other) noexcept {
    clearAgents();
    g = other.g;
    treeType = other.treeType;
    for (Agent *agent : other.agents) {
        Agent *cloneAgent = agent->clone();
        agents.push_back(cloneAgent);
    }
    cycleNumber = other.cycleNumber;
    virusIndex = other.virusIndex;
    infectedTreesQueue = other.infectedTreesQueue;
    infectedTressFinalResult = other.infectedTressFinalResult;
    other.agents.clear();
    return *this;
}