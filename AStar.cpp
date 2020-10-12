#include "AStar.h"

AStar::AStar(Graph g, int startNode)
{
    this->dist = g.dist;
    this->startNode = startNode;
    this->numOfNodes = g.v;
    solution = FringeListNode();
    cost = 0;
}

void AStar::calculateCost()
{
    vector<int> path = solution.path;
    int n = path.size();
    for (int i = 1; i < path.size(); i++)
    {
        cost += dist[path[i - 1]][path[i]];
    }
    cost += dist[path[n - 1]][startNode];
}

vector<int> AStar::getAdjacentNodes(FringeListNode curr)
{
    vector<int> successorNodes;
    unordered_set<int> visited = curr.visitedNodes;
    for (int i = 0; i < numOfNodes; i++)
    {
        if (i != curr.nodeId && visited.find(i) == visited.end())
        {
            successorNodes.push_back(i);
        }
    }
    return successorNodes;
}

int AStar::calculateHeuristic(FringeListNode curr)
{
    // We will consider only unvisited nodes (except current) for calcuating MST
    // h = min cost edge from curr node to any node in thr MST + MST + min cost edge from any node in the MST to startNode
    cout << "\tcalculating heuristic" << endl;
    if (curr.path.size() == numOfNodes - 1)
    {
        cout << "\tLeaf Node" << endl;
        return 0;
    }
    unordered_set<int> unvisitedNodes; //nodes to be considered for MST
    cout << "\t\t";
    for (int i = 0; i < numOfNodes; i++)
    {
        if (i != curr.nodeId && curr.visitedNodes.find(i) == curr.visitedNodes.end())
        {
            unvisitedNodes.insert(i);
            cout << i << " ";
        }
    }
    cout << endl;

    int MSTCost = 0;
    if (unvisitedNodes.size() > 0)
    {
        MSTCost = getMSTCost(unvisitedNodes);
    }

    int currToMST = INT_MAX;
    int MSTToStart = INT_MAX;
    for (int node : unvisitedNodes)
    {
        currToMST = min(currToMST, dist[curr.nodeId][node]);
        MSTToStart = min(MSTToStart, dist[node][startNode]);
    }
    cout << "\t\theuristic calcuated => curr_to_MST=" << currToMST << ", MST=" << MSTCost << ", MST_to_Start=" << MSTToStart << ", Total=" << currToMST + MSTCost + MSTToStart << endl;
    return currToMST + MSTCost + MSTToStart;
}

bool AStar::solve()
{
    //initial state i.e startNode
    fringeList.push(FringeListNode(startNode, 0, 0, {}, {}));
    cout << "initiated" << endl;

    // run while we have states in the fringelist or until we find the optimal solution
    while (!fringeList.empty())
    {

        // select node with least f cost
        FringeListNode curr = fringeList.top();
        fringeList.pop();
        cout << "best node = " << curr.nodeId << endl;

        //Goal test
        if (isGoalState(curr))
        {
            solution = curr;
            solution.path.push_back(curr.nodeId);
            calculateCost();
            cout << "GOAL FOUND!!" << endl;
            return true;
        }
        cout << "Not Goal" << endl;

        //get all adjacent nodes of current node i.e all - {already visited, itself}
        vector<int> successorsNodes = getAdjacentNodes(curr);
        cout << "successors -> ";
        for (int x : successorsNodes)
            cout << x << " ";
        cout << endl;

        // for each successor node insert corresponding state in the fringeList
        for (int successor : successorsNodes)
        {
            cout << "\t" << successor << endl;
            FringeListNode successorState;
            successorState.nodeId = successor;
            successorState.gValue = curr.gValue + dist[curr.nodeId][successor];
            successorState.path = curr.path;
            successorState.path.push_back(curr.nodeId); //add current node id to the path of successor
            successorState.visitedNodes = curr.visitedNodes;
            successorState.visitedNodes.insert(curr.nodeId);   //similarly add current node id to the visited set
            int h = calculateHeuristic(successorState);        // MST heuristic
            successorState.fValue = successorState.gValue + h; // f = g + h
            fringeList.push(successorState);
            cout << "\t\t g=" << successorState.gValue << ", f=" << h << ", Total=" << successorState.fValue << endl;
        }
    }
    return false;
}

bool AStar::isGoalState(FringeListNode curr)
{
    //TODO:
    if (curr.path.size() == numOfNodes - 1)
        return true;
    return false;
}

int AStar::getMSTCost(unordered_set<int> nodeList)
{
    //TODO:
    if (nodeList.size() == 1)
    {
        cout << "\tsingle node MST" << endl;
        return 0;
    }
    // cout << "\tcalculating MST" << endl;
    int key[numOfNodes];
    int visited[numOfNodes];
    for (int i = 0; i < numOfNodes; i++)
    {
        key[i] = INT_MAX;
        visited[i] = 0;
    }
    int src = *nodeList.begin();
    key[src] = 0;
    int cost = 0;
    for (int i = 0; i < nodeList.size(); i++)
    {
        int u = minKey(key, visited, nodeList);
        cost += key[u];
        visited[u] = 1;
        for (int j = 0; j < numOfNodes; j++)
        {
            if (nodeList.find(j) != nodeList.end() && !visited[j] && dist[u][j] < INT_MAX)
            {
                key[j] = min(key[j], dist[u][j]);
            }
        }
    }
    cout << "\t\tMST calculated" << endl;
    return cost;
}

int AStar::minKey(int key[], int visited[], unordered_set<int> nodeList)
{
    //TODO:
    int mn = INT_MAX;
    int min_index;
    // cout << "\tcalculating min key" << endl;
    for (int i = 0; i < numOfNodes; i++)
    {
        if (nodeList.count(i) && visited[i] == 0 && key[i] < mn)
        {
            mn = key[i];
            min_index = i;
        }
    }
    // cout << "\tcalculated min key" << endl;
    return min_index;
}
