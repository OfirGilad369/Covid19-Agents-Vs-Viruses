#ifndef TREE_H_
#define TREE_H_

#include <vector>

using namespace std;

class Session;

class Tree{
public:
    Tree(int rootLabel);
    void addChild(Tree* child);
    void addChild(const Tree& child);
    Tree* getChildAtIndex(int childIndex);
    static Tree* createTree(const Session& session, int rootLabel);
    virtual int traceTree()=0;
    static Tree* cloneTree(Tree* other);

    //Setters
    void setRootLabel(const int &myNode);

    //Getters
    vector<Tree*> getChildren() const;
    int getRootLabel() const;

    //Rule of 5
    virtual ~Tree();
    void clear();
    static Tree* createTree(Tree* other);
    virtual vector<Tree*> copyChildren() const;
    Tree(const Tree& other);
    Tree(Tree&& other) noexcept;
    Tree& operator=(const Tree& other);
    Tree& operator=(Tree&& other) noexcept;

    //destructor
    //copy construct
    //move construct
    //assignment construct
    //construct assignment construct


private:
    int node;
    std::vector<Tree*> children;

};

class CycleTree: public Tree{
public:
    CycleTree(int rootLabel, int currCycle);
    virtual int traceTree();
    int getCurrCycle() const;

    //Rule of 5 Overloading
    CycleTree(const CycleTree& other);
    CycleTree(CycleTree&& other) noexcept;
    CycleTree& operator=(const CycleTree& other);
    CycleTree& operator=(CycleTree&& other) noexcept;
private:
    int currCycle;
};

class MaxRankTree: public Tree{
public:
    MaxRankTree(int rootLabel);
    virtual int traceTree();
    Tree* getMaxRankTreeNode(Tree* TreePointer);
};

class RootTree: public Tree{
public:
    RootTree(int rootLabel);
    virtual int traceTree();
};

#endif