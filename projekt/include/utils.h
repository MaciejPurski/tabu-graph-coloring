#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <unordered_map>
#include <string>
#include <boost/graph/adjacency_matrix.hpp>

typedef boost::adjacency_matrix<boost::undirectedS> Graph;

typedef std::unordered_map<std::string, std::string> ArgMap;

struct ArgList {
    unsigned int nColors;
    unsigned int nIterations;
    unsigned int tabuSize;
};

ArgMap processArguments(int nArgs, char **args);
ArgList validateArguments(ArgMap &argMap);
Graph graphFromStream(std::istream &istr);

static const std::string help = "Following arguments are obligatory:\n"
                                "\t-c, --color\n\t\tNumber of initial colors (positive integer)\n\n"
                                "\t-i, --iterations\n\t\tNumber of algorithm iterations (positive integer)\n\n"
                                "\t-n, --neighbours\n\t\tNumber of neighbours generated on each iteration (positive integer)\n\n"
                                "\t-t, --tabu\n\t\tSize of the tabu list (non-negative integer)\n\n"
                                "By default the program prints found solution, its cost and number of colors."
                                "Following arguments are used to control output:\n"
                                "\t--show-historical-cost\n\n"
                                "\t--show-only-colors\n\n";

#endif