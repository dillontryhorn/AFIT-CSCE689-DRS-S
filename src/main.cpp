#include "DivFinder.h"
#include "DivFinderSP.h"
#include "DivFinderT.h"

#include <chrono>
#include <iostream>
#include <string>

void displayHelp(const char* execname)
{
    std::cout << "Proper usage: " << execname << " <large number> <verbosity (0-3)> <number of threads (1-10000)> <run single-threaded (true/false)>" << std::endl;
    std::cout << "Example: " << execname << " 10000 3 4 false" << std::endl;
    std::cout << "Try: 3840973478624489797261566375423382807" << std::endl;
    exit(0);
}

int main(int argc, char* argv[])
{
    if(argc < 4)
        displayHelp(argv[0]);

    LARGEINT large_number = static_cast<LARGEINT>(argv[1]);

    int verbosity = atoi(argv[2]);
    if(verbosity < 0 || verbosity > 3) {
        std::cout << "Verbosity must be between 0-3" << std::endl;
        exit(0);
    }
    unsigned int num_threads = static_cast<unsigned int>(atoi(argv[3]));
        if(num_threads < 1 || num_threads > 10000) {
        std::cout << "Number of threads must be between 1-10000" << std::endl;
        exit(0);
    }

    bool use_singlethread;
    if(std::string(argv[4]) == "true" || std::string(argv[4]) == "True" || std::string(argv[4]) == "1")
        use_singlethread = true;
    else if((std::string(argv[4]) == "false" || std::string(argv[4]) == "False" || std::string(argv[4]) == "0"))
        use_singlethread = false;
    else {
        std::cout << "invalid choice for single_threaded\n\n";
        displayHelp(argv[0]); }
    

    if(use_singlethread)
    {
        DivFinderSP my_div_finder_sp(large_number);
        my_div_finder_sp.setVerbose(verbosity);

        auto start = std::chrono::system_clock::now();
        my_div_finder_sp.PolRho();
        std::chrono::duration<double, std::milli> elapsed = std::chrono::system_clock::now() - start;
        std::cout << "Time to complete: " << elapsed.count() << " ms" << std::endl;
    }
    else
    {
        DivFinderT my_div_finder_t(large_number, num_threads);
        my_div_finder_t.setVerbose(verbosity);

        auto start = std::chrono::system_clock::now();
        my_div_finder_t.PolRho();
        std::chrono::duration<double, std::milli> elapsed = std::chrono::system_clock::now() - start;
        std::cout << "Time to complete: " << elapsed.count() << " ms" << std::endl;
    }
    
    
    return 0;
}