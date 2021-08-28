#include "../include/Tree.h"
#include "../include/Session.h"
#include <vector>

using namespace std;

//Class Tree
Tree::Tree(int rootLabel) : node(rootLabel), children() { }

void Tree::addChild(Tree *child) {
    children.push_back(child);
}

//This function is used mainly for the implementation of Rule of 5
void Tree::addChild(const Tree& child) {
    Tree* childClone = cloneTree((Tree*) (&child));
    children.push_back(childClone);
}

Tree* Tree::createTree(const Session& session, int rootLabel) {
    switch (session.getTreeType()) {
        case Cycle:
            return new CycleTree(rootLabel, session.getCycleNumber());
        case MaxRank:
            return new MaxRankTree(rootLabel);
        default:
            return new RootTree(rootLabel);
    }
}

void Tree::setRootLabel(const int &myNode) {
    node = myNode;
}

vector<Tree*> Tree::getChildren() const {
    return children;
}

int Tree::getRootLabel() const {
    return node;
}

Tree *Tree::getChildAtIndex(int childIndex) {
    return children[childIndex];
}

Tree::~Tree() {
    clear();
}

void Tree::clear() {
    for (Tree *child : children) {
        delete child;
    }
}

vector<Tree*> Tree::copyChildren() const {
    vector<Tree*> childrenCopy;
    for (Tree *child : children) {
        Tree *child_copy = createTree(child);
        childrenCopy.push_back(child_copy);
        child_copy->children = child_copy->copyChildren();
    }
    return childrenCopy;
}

Tree* Tree::createTree(Tree* other) {
    if (typeid(other)==typeid(Cycle))
        return new CycleTree(other->getRootLabel(), ((CycleTree*)other)->getCurrCycle());
    if (typeid(other)==typeid(MaxRankTree))
        return new MaxRankTree(other->getRootLabel());
    else
        return new RootTree(other->getRootLabel());
}

Tree::Tree(const Tree& other) : node() , children(other.copyChildren()) {
    node = other.node;
}

Tree::Tree(Tree&& other) noexcept : node() , children() {
    children = other.children;
    node = other.node;
    other.children = vector<Tree*>();
}

Tree& Tree::operator=(const Tree& other) {
    // check for "self assignment" and do nothing in that case
    if (this == &other) {
        return *this;
    }
    clear();
    children = other.copyChildren();
    node=other.node;
    return *this;
}

Tree& Tree::operator=(Tree&& other) noexcept {
    // check for "self assignment" and do nothing in that case
    if (this != &other)
    {
        clear();
        children = other.children;
        node = other.node;
        other.children=vector<Tree*>();
    }
    return *this;
}

Tree *Tree::cloneTree(Tree* other) {
    Tree* cloneTree = createTree(other);
    cloneTree->children = other->copyChildren();
    return cloneTree;
}

//Class CycleTree
CycleTree::CycleTree(int rootLabel, int currCycle) : Tree(rootLabel) , currCycle(currCycle) { }

int CycleTree::traceTree() {
    if(getChildren().empty() || getCurrCycle() == 0)
        return getRootLabel();
    else {
        int currentCycle = 1;
        Tree* nextCycleNode = getChildren()[0];
        while (!nextCycleNode->getChildren().empty() && currentCycle != currCycle) {
            nextCycleNode = nextCycleNode->getChildAtIndex(0);
            currentCycle = currentCycle + 1;
        }
        return nextCycleNode->getRootLabel();
    }
}

int CycleTree::getCurrCycle() const {
    return currCycle;
}

CycleTree::CycleTree(const CycleTree& other) : Tree(other), currCycle() {
    currCycle = other.currCycle;
}
CycleTree::CycleTree(CycleTree&& other) noexcept : Tree(other.getRootLabel()), currCycle() {
    getChildren() = other.getChildren();
    setRootLabel(other.getRootLabel());
    currCycle = other.currCycle;
    other.getChildren() = vector<Tree*>();
}

CycleTree& CycleTree::operator=(const CycleTree& other) {
    // check for "self assignment" and do nothing in that case
    if (this == &other) {
        return *this;
    }
    clear();
    getChildren() = other.copyChildren();
    setRootLabel(other.getRootLabel());
    return *this;
}

CycleTree& CycleTree::operator=(CycleTree&& other) noexcept {
    // check for "self assignment" and do nothing in that case
    if (this != &other)
    {
        clear();
        getChildren() = other.getChildren();
        setRootLabel(other.getRootLabel());
        other.getChildren()=vector<Tree*>();
    }
    return *this;
}

//Class MaxRankTree
MaxRankTree::MaxRankTree(int rootLabel) : Tree(rootLabel) { }

int MaxRankTree::traceTree() {
    Tree* maxRankTreePointer = getMaxRankTreeNode(this);
    return maxRankTreePointer->getRootLabel();
}

Tree* MaxRankTree::getMaxRankTreeNode(Tree* treePointer) {
    //If the tree has no children it returns himself
    if (treePointer->getChildren().empty())
        return treePointer;
    else {
        //The tree checks which one of his children has the highest rank
        //If this tree has the highest rank it return itself
        Tree* maxRankTreePointer = treePointer;
        int numberOfNodes = maxRankTreePointer->getChildren().size();
        for (int i = 0; i < numberOfNodes; i++) {
            Tree* childTreePointer = treePointer->getChildAtIndex(i);
            Tree* childTreeMaxRankTreePointer = getMaxRankTreeNode(childTreePointer);
            if (maxRankTreePointer->getChildren().size() < childTreeMaxRankTreePointer->getChildren().size()) {
                maxRankTreePointer = childTreeMaxRankTreePointer;
            }
        }
        return maxRankTreePointer;
    }
}

//Class RootTree
RootTree::RootTree(int rootLabel) : Tree(rootLabel) { }

int RootTree::traceTree() {
    return getRootLabel();
}