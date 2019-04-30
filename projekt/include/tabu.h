#ifndef TABU_H
#define TABU_H
#include <boost/circular_buffer.hpp>
#include <list>
#include <vector>
#include <utils.h>
#include <algorithm>
#include <random>


struct ColorClass {
    std::list<int> vertices;
    int nConflicts;
    const Graph &g;
    int cost;

    ColorClass(const Graph &_g) : g(_g) {
        nConflicts = 0;
        cost = 0;
    }

    ColorClass(const ColorClass &nC) : g(nC.g)
    {
        vertices = nC.vertices;
        nConflicts = nC.nConflicts;
        cost = nC.cost;
    }

    ColorClass &operator=(const ColorClass &c) {
        vertices = c.vertices;
        nConflicts = c.nConflicts;
        cost = c.cost;

        return *this;
    }


    void updateConflicts();
    void updateCost();
    int costChangeAfterRemoval(unsigned int v);
    int costChangeAfterAdding(unsigned int v);
};

typedef std::vector<ColorClass> Solution;
typedef std::array<unsigned int, 3> Move;

class TabuSearch {
private:
    boost::circular_buffer<std::pair<unsigned int, unsigned int>> tabuList;
    Solution workPoint;
    Solution bestSolution;
    int currentCost;
    int bestCost;
    
    const int nIterations;
    const int nNeighbours;
    const Graph& g;

    std::vector<Move> getNeighbours();
    
    int evaluateSolution(Solution &s);
public:
    TabuSearch(int nIterations, int tabuSize, size_t kColors, int nNeighbours, const Graph &ng);
    Solution optimize(bool verbose);
    static int chromaticNumber(Solution &s);
    static int numberOfConflicts(Solution &s);
};




#endif