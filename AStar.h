#include <bits/stdc++.h>
using namespace std;
#include "Graph.h"
#include "FringeListNode.h"
class AStar
{
public:
    int numOfNodes;
    vector<vector<int>> dist;
    priority_queue<FringeListNode, vector<FringeListNode>, CompareFLNode> fringeList;
    int startNode;
    FringeListNode solution;
    int cost;

    AStar(Graph g, int startNode);
    bool solve();
    vector<int> getAdjacentNodes(FringeListNode curr);
    bool isGoalState(FringeListNode curr);
    int calculateHeuristic(FringeListNode succ);
    int getMSTCost(unordered_set<int> unvisitedNodes);
    int minKey(int key[], int visited[], unordered_set<int> nodeList);
    void calculateCost();
};
