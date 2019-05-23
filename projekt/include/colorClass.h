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
    /**
     * A list of vertices in a class
     */
    std::list<unsigned int> vertices;
    
    /**
     * The current number of conflicts in a class.
     */
    unsigned int nConflicts;
    
    /**
     * A reference to boost::graph object
     */
    const Graph &g;
    
    /**
     * Precalculated cost function value.
     */
    int cost;

    /**
     * @brief A helper method used to calculate cost change given
     * change in conflicts.
     */
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

    /**
     * @brief Recalculate number of conflicts in a class.
     */
    void updateConflicts();

    /**
     * @brief Recalculate current cost function value based
     * on the precalculated number of conflicts
     */
    void updateCost();

    /**
     * @brief Calculate change in number of conflicts in a class
     * if vertex v is removed without really removing it.
     * 
     * @param v A vertex to be removed
     * 
     * @return Change in number of conflicts
     */
    int conflictsChangeAfterRemoval(unsigned int v) const;

    /**
     * @brief Calculate change in number of conflicts in a class
     * if vertex v is added without really adding it.
     * 
     * @param v A vertex to be added.
     * 
     * @return Change in number of conflicts.
     */
    int conflictsChangeAfterAdding(unsigned int v) const;

    /**
     * @brief Calculate cost function value change for the class
     * if vertex v is removed without really removing it.
     * 
     * @param v A vertex to removed.
     * 
     * @return Change in cost function value
     */
    int costChangeAfterRemoval(unsigned int v) const;

    /**
     * @brief Calculate cost function value change for the class,
     * if vertex v is added without really adding it.
     * 
     * @param v Vertex to be added
     * 
     * @return Change in cost function value.
     */
    int costChangeAfterAdding(unsigned int v) const;

    /**
     * @brief Add a new vertex to the class.
     */
    void addVertex(unsigned int v);

    /**
     * @brief Get current cost based on a current number of conflicts
     *
     * @return Value of cost function.
     */
    int getCost() const;
    
    /**
     * @brief Get current number of conflicts in a class
     */
    int getNConflicts() const;
    
    /**
     * @brief Is class empty
     */
    bool isEmpty() const;
    
    /**
     * @brief Get number of vertices in a class
     * 
     * @return Number of vertices in a class.
     */
    size_t nVertices() const;

    /**
     * @brief Find vertex with index
     * 
     * @param index Index of a vertex to be returned
     * 
     * @return Vertex
     */
    unsigned int getVertex(unsigned int index) const;

    /**
     * @brief Obtain list of vertices belonging to the class
     * 
     * @return list of vertices belonging to the class
     */
    std::list<unsigned int> getVertices() const;

    /**
     * @brief Static method used to move one vertex from one class
     *        to another.
     * 
     * @param from ColorClass from which a vertex should be taken
     * @param to ColorClass to which a vertex should be moved
     * @param v vertex
     * 
     * This method updates number of conflicts in the 'from' and 'to' classes.
     * It also updates the cost function and performs the vertex move.
     */
    static void performMove(ColorClass &from, ColorClass &to, unsigned int v);
};


#endif