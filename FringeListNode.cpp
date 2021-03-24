#include "FringeListNode.h"

FringeListNode ::FringeListNode()
{
    this->fValue = INT_MAX;
    this->gValue = INT_MAX;
    this->nodeId = -1;
    this->path = {};
    this->visitedNodes = {};
}

FringeListNode ::FringeListNode(int nodeId = 0, int fValue = 0, int gValue = 0, vector<int> path = {}, unordered_set<int> visitesNodes = {})
{
    this->fValue = fValue;
    this->gValue = gValue;
    this->nodeId = nodeId;
    this->path = path;
    this->visitedNodes = visitedNodes;
}

int FringeListNode ::getFValue()
{
    return this->fValue;
}