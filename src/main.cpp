#include "DivFinder.h"
#include "DivFinderSP.h"
#include "DivFinderT.h"
#include <iostream>
#include <chrono>
#include <unistd.h>

void displayHelp(const char* execname)
{
    std::cout << "Proper usage: " << execname << " <large number> " << std::endl;
    std::cout << "   v: verbosity level (0-3)\n";
    std::cout << "   t: maximum number of threads to use\n";
    exit(0);
}

int main(int argc, char* argv[])
{
    if(argc < 2)
        displayHelp(argv[0]);

    bool use_singlethread = false;
    int verbosity_in, n_threads, c = 0;
    unsigned int num_threads = 1, verbosity = 0;
    while ((c = getopt(argc, argv, "v:t:s")) != -1) 
    {
        switch (c) 
        {
            // Set the verbosity	    
            case 'v':
                verbosity_in = strtol(optarg, NULL, 10);
                if ((verbosity_in < 0) || (verbosity_in > 3)) {
                    std::cout << "Invalid max range. Value must be between 0 and 3" << std::endl;
                    std::cout << "Format: " << argv[0] << " [<verbosity>] [<max_threads>]" << std::endl;
                    exit(0);
                }
                verbosity = (unsigned int) verbosity_in;
                break;

            // Set the max number of threads to use	 
            case 't':
                n_threads = strtol(optarg, NULL, 10);
                if ((n_threads < 1) || (n_threads > 10000)) {
                    std::cout << "Invalid max threads. Value must be between 1 and 10000" << std::endl;
                    std::cout << "Format: " << argv[0] << " [<verbosity>] [<max_threads>]" << std::endl;
                    exit(0);
                }
                num_threads = (unsigned int) n_threads;
                break;

            // Run the singlethreaded version of this code
            case 's':
                use_singlethread = true;
	            break;

            case '?':
            displayHelp(argv[0]);
            break;

            default:
            std::cout << "Unknown command line option '" << c << std::endl;
            displayHelp(argv[0]);
            break;
        }
    }

    if(use_singlethread)
    {
        DivFinderSP my_div_finder_sp((LARGEINT)argv[1]);
        my_div_finder_sp.setVerbose(atoi(argv[2]));

        auto start = std::chrono::system_clock::now();
        my_div_finder_sp.PolRho();
        std::chrono::duration<double, std::milli> elapsed = std::chrono::system_clock::now() - start;
        std::cout << "Time to complete: " << elapsed.count() << " ms" << std::endl;
    }
    else
    {
        /* code */
    }
    
    
    return 0;
}