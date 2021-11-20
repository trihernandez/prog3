//My re-submission right before the deadline
//Deleted commented-out code I no longer needed and added a case such that
//  the edge index of one vertex is less than the other
//Disabled the print of the time the program ran
//I added this because I asked fixing the order of the indexed points on piazza,
//  but the answer on there was not very clear.

#include <utility>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <cfloat>
#include <algorithm>    // std::sort
//#include <time.h>       // to get the runtime for inputs

using namespace std;

#include <chrono>       // to get the runtime for inputs. Taken from https://www.geeksforgeeks.org/measure-execution-time-function-cpp/
using namespace std::chrono;



int main(int argc, char *argv[])
{    
    //obtain the coordinates of all points as a pair
    double score = 0;

    double m = std::stoi(argv[1]);
    double c = std::stoi(argv[2]);
    double d = std::stoi(argv[3]);
    std::string a = argv[4];
    std::string b = argv[5];

    std::cout << "m: " << a << "\n";
    std::cout << "c: " << b << "\n";
    std::cout << "d: " << b << "\n";
    std::cout << "a: " << a << "\n";
    std::cout << "b: " << b << "\n";
    
    return 0;
}
