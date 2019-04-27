#include "utils.h"
#include "tabu.h"


int main(int argc, char *argv[])
{
    ArgMap args = processArguments(argc, argv);

    if (args.find("--help") != args.end()) {
        std::cout << help << std::endl;
    }

    ArgList argList;

    try {
        argList = validateArguments(args);
    } catch (std::exception &e) {
        std::cout << help << std::endl;
        return -1;
    }

    Graph g = graphFromStream(std::cin);

    TabuSearch t(argList.nIterations, argList.tabuSize, argList.nColors, argList.nNeighbours, g);

    t.optimize();


    return 0;
}