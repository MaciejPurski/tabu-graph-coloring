#ifndef TABU_H
#define TABU_H
#include <boost/circular_buffer.hpp>


class TabuSearch {
private:
    boost::circular_buffer<int> tabuList;
    

public:
    TabuSearch(int tabuSize) : tabuList(tabuSize) { }

};




#endif