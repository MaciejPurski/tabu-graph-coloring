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

/**
 * @brief A class, which implements the tabu search algorithm
 */
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
    TabuSearch(unsigned int nIterations, unsigned int tabuSize, size_t kColors, unsigned int nNeighbours, const Graph &ng);
    /**
     * @brief Get current best cost function value
     */
    int getCost() const;

    /**
     * @brief Get curent best chromatic number found
     */
    unsigned int getChromaticNumber() const;
    
    /**
     * @brief Get number of conflicts in a currently best solution
     */
    unsigned int getNumberOfConflicts() const;
    
    /**
     * @brief Get currently best solution
     * 
     * @return Best solution found - a vector of ColorClass
     */
    Solution getSolution() const;

    /**
     * @brief This method performs tabu search algorithm
     * 
     * @param verbose If set to true it prints all cost function values
     */
    void optimize(bool verbose);
};

unsigned int chromaticNumber(const Solution &s);
unsigned int numberOfConflicts(const Solution &s);

#endif