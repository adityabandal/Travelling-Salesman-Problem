#include "Graph.h"

Graph::Graph(int n)
{
    v = n;
    dist = vector<vector<int>>(n, vector<int>(n, INT_MAX));
    for (int i = 0; i < n; i++)
        dist[i][i] = 0;
}

void Graph::addEdge(int u, int v, int w)
{
    dist[u][v] = w;
    dist[v][u] = w;
}

void Graph::printGraph()
{
    for (int i = 0; i < v; i++)
    {
        for (int j = 0; j < v; j++)
        {
            cout << dist[i][j] << "\t";
        }
        cout << endl;
    }
}

int Graph::getEdgeWeight(int u, int v)
{
    return dist[u][v];
}
