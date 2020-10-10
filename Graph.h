#include <bits/stdc++.h>
using namespace std;
class Graph
{

public:
    int v;
    vector<vector<int>> dist;

    Graph(int n);
    void addEdge(int u, int v, int w);
    int getEdgeWeight(int u, int v);
    void printGraph();
};
