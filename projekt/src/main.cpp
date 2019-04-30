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

    Solution s = t.optimize(args.find("--show-historical-cost") != args.end());

    if (args.find("--show-best-cost") != args.end()) {
        std::cout << "Best cost: " << std::accumulate(s.begin(), s.end(), 0, [](int acc, ColorClass &x) { return acc + x.cost;}) << std::endl;;
        std::cout << std::endl;
    }

    std::cout << "Chromatic number: ";
    std::cout << TabuSearch::chromaticNumber(s) << std::endl;
    std::cout << "Number of conflicts: " << TabuSearch::numberOfConflicts(s) << std::endl;
    std::cout << std::endl;

    if (args.find("--print-coloring") != args.end()) {
        for (int i = 0; i < s.size(); i++) {
            if (s[i].vertices.empty())
                continue;
            std::cout << "Class: " << i << "\nNumber of conflicts: " << s[i].nConflicts <<  std::endl;
            std::cout << "Number of vertices: " << s[i].vertices.size() << std::endl << "\t";
            for (auto v : s[i].vertices)
                std::cout << v << " ";
            std::cout << std::endl;
        }
    }


    return 0;
}