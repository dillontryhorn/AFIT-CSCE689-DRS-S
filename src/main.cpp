#include "DivFinder.h"
#include "DivFinderSP.h"
#include "DivFinderT.h"
#include <iostream>
#include <chrono>

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

    auto start = std::chrono::system_clock::now();
    my_div_finder_sp.PolRho();
    std::chrono::duration<double, std::milli> elapsed = std::chrono::system_clock::now() - start;
    std::cout << "Time to complete: " << elapsed.count() << " ms" << std::endl;
    
    return 0;
}