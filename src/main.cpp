#include "DivFinder.h"
#include "DivFinderSP.h"
#include <iostream>

void displayHelp(const char* execname)
{
    std::cout << "Proper usage: " << execname << " <large number> <verbosity>" << std::endl;
    exit(1);
}

int main(int argc, char* argv[])
{
    if(argc < 3 || argc > 3)
        displayHelp(argv[0]);

    DivFinderSP my_div_finder_sp((LARGEINT)argv[1]);
    my_div_finder_sp.setVerbose(atoi(argv[2]));
    my_div_finder_sp.PolRho();
    return 0;
}