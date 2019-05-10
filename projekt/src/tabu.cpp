#include "tabu.h"
#include <limits>

TabuSearch::TabuSearch(unsigned int nIterations, unsigned int tabuSize, size_t kColors, unsigned int nNeighbours, const Graph &ng) : tabuList(tabuSize),
                                                                                         workPoint(kColors, ColorClass(ng)),
                                                                                         g(ng), nIterations(nIterations),
                                                                                         nNeighbours(nNeighbours)
{
    if (kColors > boost::num_vertices(g))
        throw std::runtime_error("Number of classes can't be higher than number of vertices");

    /* Randomly initialize classess */
    std::random_device rd;
    std::vector<int> v(boost::num_vertices(g));

    for (int i = 0; i < v.size(); i++)
        v[i] = i;
    
    /* First k randomly chosen vertices are assigned
       to k subsequent classes */ 
    std::random_shuffle(v.begin(), v.begin() + kColors);
    for (int i = 0; i < kColors; i++)
        workPoint[i].addVertex(v[i]);

    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(0, kColors - 1);

    /* Each of the e - k vertices is assigned to one of the
       k classes */
    for (auto it = v.begin() + kColors; it != v.end(); it++)
        workPoint[uni(rng)].addVertex(*it);

    bestSolution = workPoint;
    currentCost = evaluateSolution(workPoint);
    bestCost = currentCost;
}

int TabuSearch::evaluateSolution(Solution &s)
{
    int cost = 0;

    for (ColorClass &c : s) {
        c.updateConflicts();
        c.updateCost();
        cost += c.getCost();
    }

    return cost;
}

std::vector<Move> TabuSearch::getNeighbours()
{
    std::vector<Move> neighbours;
    std::random_device rd;
    std::mt19937 rng(rd());

    std::uniform_int_distribution<unsigned int> uniColor(0, workPoint.size() - 1);

    while (neighbours.size() != nNeighbours) {
        unsigned int cOld;
        unsigned int cNew;
        
        /* Draw random non empty class */
        do {
            cOld = uniColor(rng);
        } while (workPoint[cOld].isEmpty());

        /* Generate destination class different from the old one */
        do {
            cNew = uniColor(rng);
        } while (cNew == cOld);

        /* Draw a vertex from the old class, which is a candidate
           to be moved to the new class */
        std::uniform_int_distribution<unsigned int> uniV(0, workPoint[cOld].nVertices() - 1);
        unsigned int index = uniV(rng);
        unsigned int v = workPoint[cOld].getVertex(index);

        neighbours.push_back({cOld, cNew, v});
    }

    return neighbours;
}

void TabuSearch::optimize(bool verbose)
{
    for (int i = 0; i < nIterations; i++) {
        int bestMoveCost = std::numeric_limits<int>::max();
        Move bestMove;

        std::vector<Move> neighbours = getNeighbours();

        /* Find best neighbour by comparing cost change*/
        for (Move &m : neighbours) {
            int costChange;

            costChange = workPoint[m[0]].costChangeAfterRemoval(m[2]) +
                         workPoint[m[1]].costChangeAfterAdding(m[2]);

            /* A candidate move m is chosen as best if its cost change is
               smaller than bestMoveCost and it is not a tabu move. However,
               if move m is on tabu it still can be chosen if it is better
               than the global optimum */
            if (costChange < bestMoveCost &&
                (std::find(tabuList.begin(),
                           tabuList.end(),
                           std::make_pair(m[1], m[2])) == tabuList.end() ||
                currentCost + costChange < bestCost)) {
                bestMoveCost = costChange;
                bestMove = m;
            }
        }
        tabuList.push_back({bestMove[0], bestMove[2]});

        ColorClass::performMove(workPoint[bestMove.at(0)],
                                workPoint[bestMove.at(1)],
                                bestMove.at(2));

        currentCost += bestMoveCost;;

        if (verbose)
            std::cout << "Current cost: " << currentCost << std::endl;

        if (currentCost < bestCost) {
            bestSolution = workPoint;
            bestCost = currentCost;
        }
    }
}

int TabuSearch::getCost() const
{
    return bestCost;
}

unsigned int TabuSearch::getChromaticNumber() const
{
    return chromaticNumber(bestSolution);
}

unsigned int TabuSearch::getNumberOfConflicts() const
{
    return numberOfConflicts(bestSolution);
}

Solution TabuSearch::getSolution() const
{
    return bestSolution;
}

unsigned int chromaticNumber(const Solution &s)
{
    return std::count_if(s.begin(), s.end(), [](const ColorClass &x) -> bool {return x.nVertices() != 0;});
}

unsigned int numberOfConflicts(const Solution &s)
{
    return std::accumulate(s.begin(), s.end(), 0, [](int acc, const ColorClass &x) { return acc + x.getNConflicts();});
}