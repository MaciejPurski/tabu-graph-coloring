#include <colorClass.h>

void ColorClass::addVertex(unsigned int v)
{
    vertices.push_back(v);
}

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
            if (boost::edge(*it, *it2, g).second)
                n++;
        }
    }

    nConflicts = n;
}

int ColorClass::getCost() const
{
    return cost;
}

int ColorClass::conflictsChangeAfterRemoval(unsigned int v) const
{
    int change = 0;

    for (auto it = vertices.begin(); it != vertices.end(); ++it) {
        if (*it == v)
            continue;

        if (boost::edge(*it, v, g).second)
            change--;
    }

    return change;
}

int ColorClass::conflictsChangeAfterAdding(unsigned int v) const
{
    int change = 0;

    for (auto it = vertices.begin(); it != vertices.end(); ++it) {
        if (boost::edge(*it, v, g).second)
            change++;
    }

    return change;
}

int ColorClass::costChangeAfterRemovalConflicts(int conflictsChange) const
{
    int newFirstTerm = -((vertices.size() - 1) * (vertices.size() - 1));
    int newSecondTerm = 2 *((vertices.size() - 1) * (nConflicts + conflictsChange));

    return newFirstTerm + newSecondTerm - cost;
}

int ColorClass::costChangeAfterAddingConflicts(int conflictsChange) const
{
    int newFirstTerm = -((vertices.size() + 1) * (vertices.size() + 1));
    int newSecondTerm = 2 *((vertices.size() + 1) * (nConflicts + conflictsChange));

    return newFirstTerm + newSecondTerm - cost;
}

int ColorClass::costChangeAfterRemoval(unsigned int v) const
{
    int conflictsChange = conflictsChangeAfterRemoval(v);

    return costChangeAfterRemovalConflicts(conflictsChange);
}

int ColorClass::costChangeAfterAdding(unsigned int v) const
{
    int conflictsChange = conflictsChangeAfterAdding(v);

    return costChangeAfterAddingConflicts(conflictsChange);
}

bool ColorClass::isEmpty() const
{
    return vertices.empty();
}

size_t ColorClass::nVertices() const
{
    return vertices.size();
}

std::list<unsigned int> ColorClass::getVertices() const
{
    return vertices;
}

unsigned int ColorClass::getVertex(unsigned int index) const
{
    auto it = vertices.begin();

    for (int i = 0; i < index; i++)
        it++;

    return *it;
}

void ColorClass::performMove(ColorClass &from, ColorClass &to, unsigned int v)
{
    int fromConflictsChange = from.conflictsChangeAfterRemoval(v);
    int toConflictsChange = to.conflictsChangeAfterAdding(v);

    int fromCostChange = from.costChangeAfterRemovalConflicts(fromConflictsChange);
    int toCostChange = to.costChangeAfterAddingConflicts(toConflictsChange);

    from.vertices.remove(v);
    to.vertices.push_back(v);

    from.nConflicts += fromConflictsChange;
    from.cost += fromCostChange;

    to.nConflicts += toConflictsChange;
    to.cost += toCostChange;
}

int ColorClass::getNConflicts() const
{
    return nConflicts;
}