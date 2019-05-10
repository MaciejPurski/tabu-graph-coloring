#include "utils.h"
#include "tabu.h"


int main(int argc, char *argv[])
{
    ArgMap args = processArguments(argc, argv);

    if (args.find("--help") != args.end()) {
        std::cout << help << std::endl;

        return 0;
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

    t.optimize(args.find("--show-historical-cost") != args.end());

    if (args.find("--show-best-cost") != args.end()) {
        std::cout << "Best cost: " << t.getCost() << std::endl;;
        std::cout << std::endl;
    }

    std::cout << "Chromatic number: ";
    std::cout << t.getChromaticNumber() << std::endl;
    std::cout << "Number of conflicts: " << t.getNumberOfConflicts() << std::endl;
    std::cout << std::endl;

    if (args.find("--print-coloring") != args.end()) {
        Solution s = t.getSolution();
        for (int i = 0; i < s.size(); i++) {
            if (s[i].isEmpty())
                continue;
            std::cout << "Class: " << i << "\nNumber of conflicts: " << s[i].getNConflicts() <<  std::endl;
            std::cout << "Number of vertices: " << s[i].nVertices() << std::endl << "\t";
            for (auto v : s[i].getVertices())
                std::cout << v << " ";
            std::cout << std::endl;
        }
    }


    return 0;
}