#include <bits/stdc++.h>
using namespace std;
class FringeListNode
{

public:
    int nodeId;
    vector<int> path;
    int fValue, gValue;
    unordered_set<int> visitedNodes;

    FringeListNode();
    FringeListNode(int nodeId, int fValue, int gValue, vector<int> path, unordered_set<int> visitesNodes);
    int getFValue();
};

struct CompareFLNode
{
    bool operator()(FringeListNode &n1, FringeListNode &n2)
    {
        return n1.getFValue() > n2.getFValue();
    }
};