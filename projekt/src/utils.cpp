#include "utils.h"
#include <exception>
#include <algorithm>
#include <sstream>

ArgMap processArguments(int nArgs, char **args)
{
    ArgMap result;

    for (int i = 1; i < nArgs; i++) {
        if (args[i] == nullptr)
            throw std::runtime_error("One of the arguments is null");

        std::string arg(args[i]);
        size_t delimeter = arg.find("=");

        if (delimeter == -1) {
            result.insert(std::make_pair(arg, ""));
        } else {
            result.insert(std::make_pair(arg.substr(0, delimeter), arg.substr(delimeter + 1)));
        }
    }

    return result;
}

static bool checkIntegerArgument(ArgMap &argMap, unsigned int &val, std::string arg, std::string abbrv = "")
{
    std::stringstream ss;
    
    if (argMap.find(arg) != argMap.end()) {
        ss << argMap[arg];
    } else if (argMap.find(abbrv) != argMap.end()) {
        ss << argMap[abbrv];
    } else {
        std::cerr << "Can't find " << arg << " argument\n";
        return true;
    }

    ss >> val;

    if (ss.fail()) {
        std::cerr << "Non-numeric value for argument: " << arg << std::endl;
        return true;
    }

    return false;
}

ArgList validateArguments(ArgMap &argMap)
{
    ArgList argList;
    bool err = false;

    err |= checkIntegerArgument(argMap, argList.nColors, "--color", "-c");
    err |= checkIntegerArgument(argMap, argList.nIterations, "--iterations", "-i");
    err |= checkIntegerArgument(argMap, argList.tabuSize, "--tabu", "-t");
    err |= checkIntegerArgument(argMap, argList.nNeighbours, "--neighbours", "-n");

    if (err)
        throw std::runtime_error("Error parsing arguments");

    if (argList.nColors < 2) {
        std::cerr << "Color value must be at least 2\n";
        err = true;
    }

    if (err)
        throw std::runtime_error("Arg values error");


    return argList;
}

Graph graphFromStream(std::istream &istr)
{
    std::string line;
    int n = -1, q = -1;
    std::vector<std::pair<int, int>> edges;

    while (std::getline(istr, line)) {
        if (line.front() == 'c')
            continue;

        if (line.front() == 'p') {
            std::stringstream ss(line.substr(2));

            std::string w;
            ss >> w;
            if (w != "edge")
                throw std::runtime_error("Parse error: expected \"edge\" word");

            ss >> n;
            if (ss.fail())
                throw std::runtime_error("Parse error: expected integer number of vertices");

            ss >> q;
            if (ss.fail())
                throw std::runtime_error("Parse error: expected integer number of vertices");


        } else if (line.front() == 'e') {
            std::stringstream ss(line.substr(2));
            int s, t;

            ss >> s;
            if (ss.fail())
                throw std::runtime_error("Parse error: expected integer source edge");

            ss >> t;
            if (ss.fail())
                throw std::runtime_error("Parse error: expected integer destination edge");

            edges.emplace_back(s, t);
        }
    }

    if (n == -1 || q == -1) {
        throw std::runtime_error("Parse error: unkown graph size");
    }

    if (edges.size() != q) {
        throw std::runtime_error("Parse error: wrong number of edges");
    }

    Graph g(n);

    for (auto p : edges)
        boost::add_edge(p.first - 1, p.second - 1, g);    

    return g;
}