#ifndef COLORCLASS_H
#define COLORCLASS_H

#include <list>
#include <utils.h>

/**
 *	@class ColorClass
 *
 *	@brief This class represents one color or class
 *         of vertices - e.t. it keeps a set of vertices
 *         belonging to the same color. It might be empty.
 */
class ColorClass {
private:
    std::list<unsigned int> vertices;
    unsigned int nConflicts;
    const Graph &g;
    int cost;

    int costChangeAfterRemovalConflicts(int conflictsChange) const;
    int costChangeAfterAddingConflicts(int conflictsChange) const;

public:
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
    int conflictsChangeAfterRemoval(unsigned int v) const;
    int conflictsChangeAfterAdding(unsigned int v) const;
    int costChangeAfterRemoval(unsigned int v) const;
    int costChangeAfterAdding(unsigned int v) const;

    void addVertex(unsigned int v);
    int getCost() const;
    int getNConflicts() const;
    bool isEmpty() const;
    size_t nVertices() const;
    unsigned int getVertex(unsigned int v) const;
    std::list<unsigned int> getVertices() const;

    static void performMove(ColorClass &from, ColorClass &to, unsigned int v);
};


#endif