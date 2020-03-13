#include "DivFinderT.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <map>
#include <atomic>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/integer/common_factor.hpp>
#include <boost/lexical_cast.hpp>

DivFinderT::DivFinderT(LARGEINT number, unsigned int threads):DivFinder(number),_num_threads(threads) {
}

DivFinderT::~DivFinderT() {
}

/*******************************************************************************
 *
 * isPrimeBF - Uses a simple brute force primality test with 6k +/- 1 optimization
 *             
 *
 *    Params:  n - the number to test for prime
 *             divisor - return value of the discovered divisor if not prime
 *
 *    Returns: true if prime, false otherwise
 *
 *******************************************************************************/

bool DivFinderT::isPrimeBF(LARGEINT n, LARGEINT &divisor) {
   if (verbose >= 3)
      std::cout << "Checking if prime: " << n << std::endl;

   divisor = 0;

   // Take care of simple cases
   if (n <= 3) {
      return n > 1;
   }
   else if ((n % 2) == 0) {
      divisor = 2;
      return false;
   } else if ((n & 3) == 0) {
      divisor = 3;
      return false;
   }

   // Assumes all primes are to either side of 6k. Using 256 bit to avoid overflow
   // issues when calculating max range
   LARGEINT2X n_256t = n;
   #pragma omp parallel
   #pragma omp for
   for (LARGEINT2X k=5; k * k < n_256t; k = k+6) {
      if ((n_256t % k == 0) || (n_256t % (k+2) == 0)) {
         divisor = (LARGEINT) k;
         return false;
      }
   }
   return true;
}

/*******************************************************************************
 *
 * factor - Calculates a single prime of the given number and recursively calls
 *          itself to continue calculating primes of the remaining number. Variation
 *          on the algorithm by Yash Varyani on GeeksForGeeks. Uses a single
 *          process 
 *
 *
 ******************************************************************************/

void DivFinderT::PolRho() {

   // First, take care of the '2' factors
   LARGEINT newval = getOrigVal();
   while (newval % 2 == 0) {
      primes.push_back(2);
      if (verbose >= 2)
         std::cout << "Prime Found: 2\n";
      newval = newval / 2;
   } 

   // Now the 3s
   while (newval % 3 == 0) {
      primes.push_back(3);
      std::cout << "Prime Found: 3\n";
      newval = newval / 3;
   }

   // Now use Pollards Rho to figure out the rest. As it's stochastic, we don't know
   // how long it will take to find an answer. Should return the final two primes
   factor(newval);
   
}

/*******************************************************************************
 *
 * factor - same as above function, but can be iteratively called as numbers are
 *          discovered as the number n can be passed in
 *
 *
 ******************************************************************************/

void DivFinderT::factor(LARGEINT n) {

   std::string bignumstr;
   bignumstr  = boost::lexical_cast<std::string>(n);
   std::atomic_bool aBool = ATOMIC_VAR_INIT(false);
   this->atomicTable.insert(std::pair<std::string, std::atomic_bool>(bignumstr, aBool));

   // already prime
   if (n == 1) {
      return;
   }

   if (verbose >= 2)
      std::cout << "Factoring: " << n << std::endl;

   bool div_found = false;

   while (!div_found)
   {
      LARGEINT divisor; // try to get a divisor by brute6
      if (isPrimeBF(n, divisor)) // launch a thread on this isPrime call
      {
         if (verbose >= 2)
            std::cout << "Prime found: " << n << std::endl;
         primes.push_back(n);
         return;
      } // if returns true n is prime, else divisor is set
      else
      {   // We found a prime divisor, save it and keep finding primes
	      if (verbose >= 2)
	         std::cout << "Prime found: " << divisor << std::endl;
	      primes.push_back(divisor);
	      return factor(n / divisor); // obsiusly dont try to factor the prime
	   }				
      

      // We try to get a divisor using Pollards Rho, might need to stop and rerandomize
      LARGEINT d = calcPollardsRho(n); // launch of bunch of threads on this
      if (d != n)
      {
         if (verbose >= 1)
            std::cout << "Divisor found: " << d << std::endl;

         // Factor the divisor
         factor(d); // this is the recursive call, need to figure how to split
                     // the threads with factoring d and n/d which is the line under

         // Now the remaining number
         factor((LARGEINT) (n/d));
         return;
      }	
   }
   throw std::runtime_error("Reached end of function--this should not have happened.");
   return;
}

