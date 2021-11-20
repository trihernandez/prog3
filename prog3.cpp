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
    std::string a_compared = "";
    std::string b_compared = "";

    double m = std::stod(argv[1]);
    double c = std::stod(argv[2]);
    double d = std::stod(argv[3]);
    std::string a = argv[4];
    std::string b = argv[5];

    std::cout << "m: " << m << "\n";
    std::cout << "c: " << c << "\n";
    std::cout << "d: " << d << "\n";
    std::cout << "a: " << a << "\n";
    std::cout << "b: " << b << "\n";

    int l_a = a.length();
    int l_b = b.length();
    int l_diff = l_a - l_b;

    int i1_a = 0;
    int i1_b = 0;
    int i2_a = 0;
    int i2_b = 0;

    while((i1_a < l_a) &&(i1_b < l_b))
    {
        //characters are matched
        if( a[i1_a] == b[i1_a] )
        {
            score += m;
        }
        i1_a++;
        i2_a++;
    }
    std::cout << m << "\n";

    return 0;
}
