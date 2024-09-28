#include "PrimeChecker.h"

bool PrimeChecker::IsPrime(int n) 
{
    if (n <= 1) 
    {
        return false;
    }

    if (n <= 3) 
    {
        return true;
    }

    if (n % 2 == 0 || n % 3 == 0) 
    {
        return false;
    }

    // Check for divisors from 5
    // to the square root of n
    for (int i = 5; i * i <= n; i += 6) {

        // If n is divisible by i or (i + 2),
        // it is not prime
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
    }
    return true; 
}