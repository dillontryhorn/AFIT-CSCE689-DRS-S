#ifndef DIVFINDERT_H
#define DIVFINDERT_H

#include <string>
#include <map>
#include <atomic>
#include <memory>
#include "DivFinder.h"
#include <boost/multiprecision/cpp_int.hpp>

// If factor has been called on a number x times, check to see if the number is prime
//const unsigned int primecheck_depth = 10;

/******************************************************************************************
 * DivFinderSP - Used as a recursive calculator for prime numbers using Pollards Rho algorithm.
 *            A simple, recursive single process/thread version.
 *
 *  	   DivFinder(Const): 
 *  	   ~DivFinder(Dest):
 *
 *         PolRho - function that executes the stochastic Pollards' Rho algorithm for finding
 *                  divisors
 *         ifPrimeBF - uses brute force using 6k+-1 optimization to determine if a number is prime
 *
 *  	   Exceptions: sub-classes should throw a std::exception with the what string field
 *  	               populated for any issues.
 *
 *****************************************************************************************/
using atomic_ptr_t = std::shared_ptr<std::atomic<bool>>;

class DivFinderT : public DivFinder { 
   public:
      DivFinderT(LARGEINT input_value, unsigned int threads);
      virtual ~DivFinderT();

      virtual void PolRho() override;
      
      void calcPollardsRho2(LARGEINT n, atomic_ptr_t aBool);

      void isPrimeBF(LARGEINT n, atomic_ptr_t aBool);

   protected:

      void factor(LARGEINT n);


   private:
        unsigned int _num_threads = 1;
};

#endif
