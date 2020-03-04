#include "DivFinder.h"
#include "DivFinderSP.h"
#include <iostream>

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        std::cout << "Proper usage: ./divfinder <large number>" << std::endl;
        exit(1);
    }
    DivFinderSP my_div_finder_sp(atoll(argv[1]));
    my_div_finder_sp.setVerbose(3);
    my_div_finder_sp.PolRho();
    return 0;
}