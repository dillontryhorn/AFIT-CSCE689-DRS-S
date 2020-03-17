#include "DivFinderT.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <map>
#include <atomic>
#include <thread>
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

void DivFinderT::isPrimeBF(LARGEINT n, atomic_ptr_t aBool) {
   if (verbose >= 3)
      std::cout << "Checking if prime: " << n << std::endl;

   LARGEINT divisor = 0;

   // Take care of simple cases
   if (n <= 3 && n > 1) {
      if (verbose >= 2)
         std::cout << "Prime found: " << n << std::endl;
      primes.push_back(n);
      *aBool = true; // tell the other treads to finish
      return;
   }

   //already taken care of before the start of factor?
   // else if ((n % 2) == 0) {
   //    if (verbose >= 2)
   //       std::cout << "Prime found: " << 2 << std::endl;
   //    primes.push_back(2);
   //    *aBool = true; // tell the other treads to finish
   //    return factor(n / 2);
   // } 
   // else if ((n & 3) == 0) {
   //    if (verbose >= 2)
   //       std::cout << "Prime found: " << 3 << std::endl;
   //    primes.push_back(3);
   //    *aBool = true; // tell the other treads to finish
   //    return factor(n / 3);
   // }

   //std::cout << "Brute force after intial checks\n";

   // Assumes all primes are to either side of 6k. Using 256 bit to avoid overflow
   // issues when calculating max range
   LARGEINT2X n_256t = n;
   #pragma omp parallel
   #pragma omp for
   for (LARGEINT2X k=5; k * k < n_256t; k = k+6) {
      //std::cout << "Brute force in loop\n";
      if ((n_256t % k == 0) || (n_256t % (k+2) == 0)) {
         *aBool = true; // tell the other treads to finish
         divisor = (LARGEINT) k;
         //if (verbose >= 2)
            std::cout << "Prime found: " << divisor << std::endl;
         primes.push_back(divisor);
         return factor(n / divisor);
      }
      if(*aBool)
         return;
   }
   *aBool = true; // tell the other treads to finish
   //if (verbose >= 2)
         std::cout << "Prime found: " << n << std::endl;
   primes.push_back(n);
   return;
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
      //if (verbose >= 2)
         std::cout << "Prime Found: 2\n";
      newval = newval / 2;
   } 

   // Now the 3s
   while (newval % 3 == 0) {
      primes.push_back(3);
      //if (verbose >= 2)
         std::cout << "Prime Found: 3\n";
      newval = newval / 3;
   }

   // Now use Pollards Rho to figure out the rest. As it's stochastic, we don't know
   // how long it will take to find an answer. Should return the final two primes
   // std::cout << "Trying to start\n";
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
   //std::cout << "Starting\n";


   // already prime
   if (n == 1) {
      return;
   }

   if (verbose >= 2)
      std::cout << "Factoring: " << n << std::endl;

   auto aBool = atomic_ptr_t(new std::atomic<bool>(false));

   
   std::thread bf(&DivFinderT::isPrimeBF, this, n, aBool); // launch a thread on this isPrime call

   std::vector<std::thread> thvec;	
   for(int i=0; i<(this->_num_threads - 1); i++)
   {
      thvec.push_back(std::thread(&DivFinderT::calcPollardsRho2, this, n, aBool)); // launch the rest of the threads on PolRho
   }
   bf.join();
   for(auto it = thvec.begin(); it != thvec.end(); it++)
      (*it).join();
   
   // std::cout << "Exiting\n";
   return;
}



/**********************************************************************************************
 * calcPollardsRho - Do the actual Pollards Rho calculations to attempt to find a divisor
 *
 *    Params:  n - the number to find a divisor within
 *
 *
 **********************************************************************************************/

void DivFinderT::calcPollardsRho2(LARGEINT n, atomic_ptr_t aBool) {
   if (n <= 3)
      return; //brute force will handle


   //std::cout << "Pol Rho after intial check\n";
   while(!aBool)
   {
      // Initialize our random number generator
      srand(time(NULL));

      // pick a random number from the range [2, N)
      LARGEINT2X x = (rand()%(n-2)) + 2;
      LARGEINT2X y = x;    // Per the algorithm

      // random number for c = [1, N)
      LARGEINT2X c = (rand()%(n-1)) + 1;

      LARGEINT2X d = 1;

      // Loop until either we find the gcd or gcd = 1
      while (d == 1) {
         //std::cout << "Pol Rho in loop\n";
         if(*aBool)
            break;
         // "Tortoise move" - Update x to f(x) (modulo n)
         // f(x) = x^2 + c f
         x = (modularPow(x, 2, n) + c + n) % n;

         // "Hare move" - Update y to f(f(y)) (modulo n)
         y = (modularPow(y, 2, n) + c + n) % n;
         y = (modularPow(y, 2, n) + c + n) % n;

         // Calculate GCD of |x-y| and tn
         LARGESIGNED2X z = (LARGESIGNED2X) x - (LARGESIGNED2X) y;
         if (z < 0)
            d = boost::math::gcd((LARGEINT2X) -z, (LARGEINT2X) n);
         else
            d = boost::math::gcd((LARGEINT2X) z, (LARGEINT2X) n);

         // If we found a divisor, factor primes out of each side of the divisor
         if ((d != 1) && (d != n)) {
            //if (verbose >= 1)
               //std::cout << "Divisor found: " << d << std::endl;
               // Factor the divisor
            *aBool = true;
            factor((LARGEINT) d);
            // factor the remaining number
            factor((LARGEINT) (n/d));
            return;
         }
      }
   }
   return;
}