#include "tabu.h"
#include <limits>


void ColorClass::updateCost()
{
    cost = -(vertices.size() * vertices.size()) + 2 * (vertices.size() * nConflicts);
}

void ColorClass::updateConflicts()
{
    int n = 0;

    for (auto it = vertices.begin(); it != vertices.end(); it++) {
        auto it2 = it;
        it2++;
        for (; it2 != vertices.end(); it2++) {
            if (boost::edge(*it, *it2, g).second) {
                n++;
            }

        }
    }

    nConflicts = n;
}

int ColorClass::costChangeAfterRemoval(unsigned int v)
{
    int newConflicts = 0;

    for (std::list<int>::const_iterator it = vertices.begin(); it != vertices.end(); ++it) {
        if (*it == v)
            continue;

        if (boost::edge(*it, v, g).second)
            newConflicts++;
    }

    return -((vertices.size() - 1) * (vertices.size() - 1)) + 2 *((vertices.size() - 1) * (nConflicts - newConflicts))
           -cost;
}

int ColorClass::costChangeAfterAdding(unsigned int v)
{
    int newConflicts = 0;

    for (auto it = vertices.begin(); it != vertices.end(); ++it) {
        if (boost::edge(*it, v, g).second)
            newConflicts++;
    }
    return -((vertices.size() + 1) * (vertices.size() + 1)) + 2 *((vertices.size() + 1) * (nConflicts + newConflicts))
           -cost;

}

TabuSearch::TabuSearch(int nIterations, int tabuSize, size_t kColors, int nNeighbours, const Graph &ng) : tabuList(tabuSize),
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
    
    std::random_shuffle(v.begin(), v.begin() + kColors);
    for (int i = 0; i < kColors; i++) {
        workPoint[i].vertices.push_back(v[i]);
    }

    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(0, kColors - 1);

    for (auto it = v.begin() + kColors; it != v.end(); it++) {
        workPoint[uni(rng)].vertices.push_back(*it);
    }

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
        cost += c.cost;
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
        
        /* Generate random non empty class */
        do {
            cOld = uniColor(rng);
        } while (workPoint[cOld].vertices.size() == 0);

        /* Generate destination class different from the old one */
        do {
            cNew = uniColor(rng);
        } while (cNew == cOld);

        std::uniform_int_distribution<unsigned int> uniV(0, workPoint[cOld].vertices.size() - 1);
        unsigned int index = uniV(rng);

        unsigned int v;
        auto it = workPoint[cOld].vertices.begin();

        for (int i = 0; i < index; i++)
            it++;

        v = *it;

        neighbours.push_back({cOld, cNew, v});
    }

    return neighbours;
}

Solution TabuSearch::optimize(bool verbose)
{
    for (int i = 0; i < nIterations; i++) {
        std::vector<Move> neighbours = getNeighbours();

        int bestMoveCost = std::numeric_limits<int>::max();
        Move bestMove;

        /* Find best neighbour move */
        for (Move &m : neighbours) {
            int cost;

            cost = workPoint[m[0]].costChangeAfterRemoval(m[2]) +
                   workPoint[m[1]].costChangeAfterAdding(m[2]);

            if (cost < bestMoveCost &&
                std::find(tabuList.begin(), tabuList.end(), std::make_pair(m[1], m[2])) == tabuList.end()) {
                bestMoveCost = cost;
                bestMove = m;
            }
        }

        tabuList.push_back({bestMove[0], bestMove[2]});

        /* Perform move */
        // TODO optimize
        workPoint[bestMove.at(0)].vertices.remove(bestMove.at(2));
        workPoint[bestMove.at(1)].vertices.push_back(bestMove.at(2));

        workPoint[bestMove.at(0)].updateConflicts();
        workPoint[bestMove.at(1)].updateConflicts();

        currentCost = evaluateSolution(workPoint);

        if (verbose)
            std::cout << "Current cost: " << currentCost << std::endl;

        if (currentCost < bestCost) {
            bestSolution = workPoint;
            bestCost = currentCost;
        }
    }


    return bestSolution;
}