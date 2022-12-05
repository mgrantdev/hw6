#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash
{
    HASH_INDEX_T rValues[5]{983132572, 1468777056, 552714139, 984953261, 261934300};
    MyStringHash(bool debug = true)
    {
        if (false == debug)
        {
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string &k) const
    {

        // @summary Convert char to int
        char c;
        unsigned long long wValues[5] = {0, 0, 0, 0, 0}; // use placeholder zeroies
        unsigned long long sum = 0;
        HASH_INDEX_T v, hash = 0;

        // short word case: getting w values for first and only group
        if (k.length() <= 6)
        {
            for (int n = 0; n < k.length(); n++)
            {
                c = k[n];
                v = letterDigitToNumber(c);
                // std::cout << "v value for " << c << ": " << v << std::endl;
                sum += pow(36, k.length() - 1 - n) * v;
            }
            wValues[4] = sum; // will be last group
        }
        else
        {
            // long word case: getting values are
            std::string tempStr = k;


            int group = 4;
            while (tempStr.length() != 0)
            {
                sum = 0; // reset sum for each group
                int offset;
                if (tempStr.length() <= 6)
                {
                    offset = 0;
                    for (int n = 0; n < tempStr.length(); n++)
                    {
                        c = tempStr[n];
                        v = letterDigitToNumber(c);
                        // std::cout << "v value for " << c << ": " << v << std::endl;
                        sum += pow(36, tempStr.length() - 1 - n) * v;
                    }
                }
                else
                {
                    offset = tempStr.length() - 6;
                    for (int n = 0; n < 6; n++)
                    {
                        c = tempStr[n + offset];
                        v = letterDigitToNumber(c);
                        // std::cout << "v value for " << c << ": " << v << std::endl;
                        sum += pow(36, 5 - n) * v;
                    }
                }
                wValues[group] = sum; // assign to proper group
                group--;
                tempStr.erase(offset);
            }

            /*for (int n = 0; n < tempStr.length(); n++)
            {
                c = tempStr[n];
                v = letterDigitToNumber(c);
                // std::cout << "v value for " << c << ": " << v << std::endl;
                sum += pow(36, tempStr.length() - 1 - n) * v;
            }
            wValues[group] = sum; // will be last group*/
        }

        // construct hash
        for (int n = 0; n < 5; n++)
        {
            // std::cout << "r value for w[" << n << "]=" << wValues[n] << ": " << rValues[n] << std::endl;
            hash += (wValues[n] * rValues[n]);
        }
        // std::cout << hash << std::endl;
        return hash;
    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T
    letterDigitToNumber(char letter) const
    {
        // Add code here or delete this helper function if you do not want it
        HASH_INDEX_T n;
        switch (std::tolower(letter))
        {
        case 'a':
            n = 0;
            break;
        case 'b':
            n = 1;
            break;
        case 'c':
            n = 2;
            break;
        case 'd':
            n = 3;
            break;
        case 'e':
            n = 4;
            break;
        case 'f':
            n = 5;
            break;
        case 'g':
            n = 6;
            break;
        case 'h':
            n = 7;
            break;
        case 'i':
            n = 8;
            break;
        case 'j':
            n = 9;
            break;
        case 'k':
            n = 10;
            break;
        case 'l':
            n = 11;
            break;
        case 'm':
            n = 12;
            break;
        case 'n':
            n = 13;
            break;
        case 'o':
            n = 14;
            break;
        case 'p':
            n = 15;
            break;
        case 'q':
            n = 16;
            break;
        case 'r':
            n = 17;
            break;
        case 's':
            n = 18;
            break;
        case 't':
            n = 19;
            break;
        case 'u':
            n = 20;
            break;
        case 'v':
            n = 21;
            break;
        case 'w':
            n = 22;
            break;
        case 'x':
            n = 23;
            break;
        case 'y':
            n = 24;
            break;
        case 'z':
            n = 25;
            break;
        case '0':
            n = 26;
            break;
        case '1':
            n = 27;
            break;
        case '2':
            n = 28;
            break;
        case '3':
            n = 29;
            break;
        case '4':
            n = 30;
            break;
        case '5':
            n = 31;
            break;
        case '6':
            n = 32;
            break;
        case '7':
            n = 33;
            break;
        case '8':
            n = 34;
            break;
        case '9':
            n = 35;
            break;
        }

        return n;
    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator(seed); // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for (int i{0}; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
