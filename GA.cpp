#include "GA.h"

GA::GA(Graph g, int startNode)
{
    this->dist = g.dist;
    this->startNode = startNode;
    this->numOfNodes = g.v;

    this->elite = POP_SIZE * ELITE_FACTOR;
    this->population = vector<Individual>(POP_SIZE);
    this->fitness = vector<double>(POP_SIZE, -1);
    this->cumFitness = vector<double>(POP_SIZE, -1);
}

int GA::rand_num(int start, int end)
{
    int r = end - start;
    int rnum = start + rand() % r;
    return rnum;
}

void GA::calcuateFitness()
{
    double sum = 0;
    for (int i = 0; i < POP_SIZE; i++)
    {
        fitness[i] = (double)1 / (double)population[i].cost;
    }
    calcuateCumFitness();
}

int GA::calcuateCost(Individual X)
{
    int sum = 0;
    for (int i = 0; i < numOfNodes; i++)
    {
        sum += dist[X.path[i]][X.path[i + 1]];
    }
    return sum;
}

void GA::calcuateCumFitness()
{
    double sum = 0;
    for (int i = 0; i < POP_SIZE; i++)
        sum += fitness[i];
    cumFitness[0] = fitness[0] / sum;
    for (int i = 1; i < POP_SIZE; i++)
    {
        cumFitness[i] = cumFitness[i - 1] + fitness[i] / sum;
    }
}

void GA::initialisePopulation()
{
    for (int i = 0; i < POP_SIZE; i++)
    {
        Individual tmp = Individual(numOfNodes, startNode);
        int j = 1;
        while (j < numOfNodes)
        {
            int x = rand_num(0, numOfNodes);
            bool valid = true;
            for (int k = 0; k < j; k++)
            {
                if (tmp.path[k] == x)
                {
                    valid = false;
                    break;
                }
            }
            if (!valid)
                continue;
            tmp.path[j] = x;
            j++;
        }
        tmp.cost = calcuateCost(tmp);
        population[i] = tmp;
    }
}

int GA::rouletteWheelSelection()
{
    double x = (double)rand() / (double)RAND_MAX;
    for (int i = 0; i < POP_SIZE; i++)
    {
        if (x <= cumFitness[i])
        {
            return i;
        }
    }
    return POP_SIZE - 1;
}
Individual GA::cross(Individual X, Individual Y)
{
    vector<int> chrm1 = X.path;
    vector<int> chrm2 = Y.path;

    int n = chrm1.size();

    Individual child(numOfNodes, startNode);

    int upper = n - 2;
    int lower = 1;

    int pt1 = (rand() % (upper - lower + 1)) + lower;
    int pt2 = (rand() % (upper - lower + 1)) + lower;

    int startGene = min(pt1, pt2);
    int endGene = max(pt1, pt2);

    unordered_set<int> present;
    for (int i = startGene; i <= endGene; i++)
    {
        child.path[i] = chrm1[i];
        present.insert(chrm1[i]);
    }
    int i = 1;
    for (int j = 1; j < n - 1; j++)
    {
        if (i == startGene)
        {
            i = endGene + 1;
        }
        if (present.count(chrm2[j]))
            continue;
        else
        {
            child.path[i] = chrm2[j];
            i++;
        }
    }
    child.cost = calcuateCost(child);
    return child;
}

void GA::printIndividualDetails(Individual X, int index)
{
    for (int i = 0; i < X.path.size(); i++)
    {
        cout << X.path[i] << " ";
    }
    cout << " -> " << X.cost << "\t" << fitness[index] << "\t" << cumFitness[index] << endl;
}

void GA::printPopulationDetails()
{
    static int generation = 0;
    int sum = 0, best = INT_MAX, index = -1;
    // cout << "----------------Generation----------------" << endl;
    for (int i = 0; i < POP_SIZE; i++)
    {
        // printIndividualDetails(population[i], i);
        sum += population[i].cost;
        if (population[i].cost < best)
        {
            index = i;
            best = population[i].cost;
        }
    }
    solution = population[index];
    cout << "Gen# " << generation++ << " : Avg cost = " << (double)sum / double(POP_SIZE) << ", Best= " << best << endl;
    cout << "------------------------------------------" << endl;
}

Individual GA::mutate(Individual X)
{
    double x = (double)rand() / (double)RAND_MAX;
    int n = X.path.size();
    if (x < MUTATE_RATE)
    {
        int upper = n - 2;
        int lower = 1;
        int pt1 = (rand() % (upper - lower + 1)) + lower;
        int pt2 = (rand() % (upper - lower + 1)) + lower;

        int temp = X.path[pt1];
        X.path[pt1] = X.path[pt2];
        X.path[pt2] = temp;
    }
    return X;
}

vector<Individual> GA::selectElite()
{
    vector<pair<int, int>> arr;
    for (int i = 0; i < POP_SIZE; i++)
    {
        arr.push_back({population[i].cost, i});
    }
    sort(arr.begin(), arr.end());
    vector<Individual> selected;
    for (int i = 0; i < elite; i++)
    {
        selected.push_back(population[arr[i].second]);
    }
    return selected;
}

Individual GA::getSolution()
{
    int best = INT_MAX, index = -1;
    for (int i = 0; i < POP_SIZE; i++)
    {
        if (population[i].cost < best)
        {
            index = i;
            best = population[i].cost;
        }
    }
    return population[index];
}

bool GA::solve()
{

    initialisePopulation(); //initiate population
    calcuateFitness();      //calculate fitness and cumFitness for the population
    printPopulationDetails();

    srand((time(0))); //seed for random number generator

    int g = NUM_OF_GENERATIONS;
    while (g--) //repeat
    {
        vector<Individual> new_population = selectElite(); //fittest individuals will be retained in the new generations
        for (int i = 0; i < POP_SIZE - elite; i++)         // for i 1 to POP_SIZE-elite
        {
            //randomly select two parents (selection prob is directly prop to the fitness)
            Individual X = population[rouletteWheelSelection()];
            Individual Y = population[rouletteWheelSelection()];
            Individual child = cross(X, Y);  //child = cross(X,Y)
            child = mutate(child);           //mutate child
            new_population.push_back(child); //add child to new_population
        }

        population = new_population; //new generation = population
        calcuateFitness();           //calculate fitness and cumFitness
        printPopulationDetails();
    }
    solution = getSolution();
    return true;
}