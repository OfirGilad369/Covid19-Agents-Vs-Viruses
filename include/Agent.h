#ifndef AGENT_H_
#define AGENT_H_

#include "Session.h"
#include <vector>

class Agent{
public:
    Agent();
    virtual Agent* clone()=0;
    virtual void act(Session& session)=0;
    virtual bool isAgentActiveVirus();
    void setIsActiveVirus(bool isActiveVirusStatus);
    bool getIsActiveVirus() const;
    virtual ~Agent();
protected:
    bool isActiveVirus;
};

class ContactTracer: public Agent{
public:
    ContactTracer();
    virtual Agent* clone();
    virtual void act(Session& session);
};


class Virus: public Agent{
public:
    Virus(int nodeInd);
    virtual Agent* clone();
    virtual void act(Session& session);
    int getNodeInd() const;
private:
    const int nodeInd;
};

#endif