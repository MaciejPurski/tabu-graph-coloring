#ifndef TABU_H
#define TABU_H

#include <boost/circular_buffer.hpp>
#include <list>
#include <vector>
#include <utils.h>
#include <algorithm>
#include <random>
#include <colorClass.h>

typedef std::vector<ColorClass> Solution;
typedef std::array<unsigned int, 3> Move;

class TabuSearch {
private:
    boost::circular_buffer<std::pair<unsigned int, unsigned int>> tabuList;
    Solution workPoint;
    Solution bestSolution;
    int currentCost;
    int bestCost;
    
    const unsigned int nIterations;
    const unsigned int nNeighbours;
    const Graph& g;

    std::vector<Move> getNeighbours();
    
    int evaluateSolution(Solution &s);
public:
    int getCost() const;
    unsigned int getChromaticNumber() const;
    unsigned int getNumberOfConflicts() const;
    Solution getSolution() const;
    TabuSearch(unsigned int nIterations, unsigned int tabuSize, size_t kColors, unsigned int nNeighbours, const Graph &ng);
    void optimize(bool verbose);
};

unsigned int chromaticNumber(const Solution &s);
unsigned int numberOfConflicts(const Solution &s);



#endif