#include "../include/Agent.h"

using namespace std;

//Class Agent
Agent::Agent() : isActiveVirus(false) { }

bool Agent::isAgentActiveVirus() {
    return isActiveVirus;
}

void Agent::setIsActiveVirus(bool isActiveVirusStatus) {
    isActiveVirus = isActiveVirusStatus;
}

//The variable isActiveVirus is mainly used by the virus to declare their status:
//True - means that the virus hasn't finished it's role and still has more possible ways to act
//False - means that the virus finished  it's act and there is nothing left for him to do
bool Agent::getIsActiveVirus() const {
    return isActiveVirus;
}

Agent::~Agent() = default;

//Class ContactTracer
//ContactTracer by Default is not an active virus
ContactTracer::ContactTracer() : Agent() { }

Agent* ContactTracer::clone() {
    return new ContactTracer(*this);
}

void ContactTracer::act(Session &session) {
    if (!session.isQueueEmpty()) {
        int infectedNodeInd = session.dequeueInfected();
        Graph* sessionGraph = session.getGraph();
        Tree *BFSTree = sessionGraph->BFS(session, infectedNodeInd);
        int nodeToRemove = BFSTree->traceTree();
        sessionGraph->removeNodeFromGraph(nodeToRemove);
        delete BFSTree;
    }
}

//Class Virus
Virus::Virus(int nodeInd) : nodeInd(nodeInd) {
    setIsActiveVirus(true);
}

Agent* Virus::clone() {
    return new Virus(*this);
}

void Virus::act(Session &session) {
    if(isActiveVirus) {
        //Infecting the current node
        Graph* sessionGraph = session.getGraph();
        if (sessionGraph->isOccupied(nodeInd)) {
            session.enqueueInfected(nodeInd);
        }
        //Infecting one of the neighbors node
        vector<int>* virusNeighbors = sessionGraph->nodeNeighbors(nodeInd);
        bool isActFinished = false;
        int numberOfVirusNeighbors = virusNeighbors->size();
        for (int i = 0; i < numberOfVirusNeighbors && !isActFinished; i++) {
            if ((virusNeighbors->at(i) == 1) && (!sessionGraph->isInfected(i)) && (!sessionGraph->isOccupied(i))) {
                sessionGraph->occupyNode(i);
                session.addAgent(Virus(i));
                isActFinished = true;
            }
        }
        if (!isActFinished) {
            isActiveVirus = false;
        }
    }
}

int Virus::getNodeInd() const {
    return nodeInd;
}