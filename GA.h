#include <bits/stdc++.h>
using namespace std;
#include "Graph.h"

#define POP_SIZE 500
#define NUM_OF_GENERATIONS 1000
#define MUTATE_RATE 0.3
#define ELITE_FACTOR 0.4

struct Individual
{
    vector<int> path;
    int cost;

    Individual(int numberOfNodes = 0, int startNode = -1)
    {
        path = vector<int>(numberOfNodes + 1, -1);
        path[0] = path[numberOfNodes] = startNode;
    }
};

class GA
{
public:
    int numOfNodes;
    vector<vector<int>> dist; //2D distance matrix
    int startNode;            //start node for TSP
    int elite;

    Individual solution;           //solution inndividual
    vector<Individual> population; //POP_SIZE
    vector<double> fitness;        //POP_SIZE
    vector<double> cumFitness;     //POP_SIZE

    GA(Graph g, int startNode);
    bool solve();
    void initialisePopulation();
    int calcuateCost(Individual X);
    void calcuateFitness();
    void calcuateCumFitness();
    vector<Individual> selectElite();
    int rand_num(int start, int end);
    int rouletteWheelSelection();
    Individual cross(Individual X, Individual Y);
    Individual mutate(Individual X);
    Individual getSolution();
    void printPopulationDetails();
    void printIndividualDetails(Individual X, int index);
};
