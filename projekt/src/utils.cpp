#include "utils.h"
#include <exception>
#include <algorithm>
#include <sstream>

TabuMethod stringToTabuMethod(const std::string &str)
{
    if (str == "random")
        return TabuMethod::RAND;
    else if (str == "fifo")
        return TabuMethod::RAND;
    else if (str == "priority")
        return TabuMethod::PRIORITY;
    else return TabuMethod::UNKNOWN;
}

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

    if (argMap.find("-m") != argMap.end()) {
        argList.tabuMethod = stringToTabuMethod(argMap["-m"]);
    } else if (argMap.find("--method") != argMap.end()) {
        argList.tabuMethod = stringToTabuMethod(argMap["--method"]);
    } else {
        std::cerr << "Can't find --method argument\n";
        err = true;
    }

    if (err)
        throw std::runtime_error("Error parsing arguments");

    if (argList.tabuMethod == TabuMethod::UNKNOWN) {
        std::cerr << "Unkown value of method argument\n";
        err = true;
    }

    if (argList.nColors < 2) {
        std::cerr << "Color value must be at least 2\n";
        err = true;
    }

    if (err)
        throw std::runtime_error("Arg values error");


    return argList;
}