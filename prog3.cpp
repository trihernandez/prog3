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



std::tuple<double, std::string, std::string> matching_score(double m, double c, double d, std::string a, std::string b, 
                              double current_score, std::string a_matched, std::string b_matched, double worst_case)
{
    //the answer f
    std::tuple<double, std::string, std::string> answer(current_score, a_matched, b_matched);

    cout << std::get<1>(answer) << "\n";
    cout << std::get<2>(answer) << "\n";
    cout << "current score: " <<std::get<0>(answer) << "\n";
    cout << "worst-case:    " << worst_case << "\n";
    cout << "\n";


    //end of sequence
    if( a.length() == 0 && b.length() == 0 )
    {
        return answer;
    }
    //end of a; b is longer, and therefore rest of a was deleted
    else if( a.length() == 0 )
    {
        std::get<0>(answer) += d;
        std::get<1>(answer) += "_";
        std::get<2>(answer) += b[0];

        return matching_score(m,c,d,a,b.substr(1),
                                std::get<0>(answer),std::get<1>(answer),std::get<2>(answer), worst_case);
    }
    //end of b; a is longer, and therefore rest of b was deleted
    else if( b.length() == 0 )
    {
        std::get<0>(answer) += d;
        std::get<1>(answer) += a[0];
        std::get<2>(answer) += "_";

        return matching_score(m,c,d,a.substr(1),b,
                                std::get<0>(answer),std::get<1>(answer),std::get<2>(answer), worst_case);
    }
    //characters are matched, and this branch does not split
    else if( a[0] == b[0] )
    {
        std::get<0>(answer) += m;
        std::get<1>(answer) += a[0];
        std::get<2>(answer) += b[0];
        
        //re-calculate w using the remaning sub-strings
        double min_deletions = d * std::abs( (int)a.length()-1 - (int)b.length()-1 );
        double max_changes = c * std::min( (int)a.length()-1, (int)b.length()-1 );
        double w = max_changes + min_deletions;
        if(current_score > 0)
            w += current_score;

        return matching_score(m,c,d,a.substr(1),b.substr(1),
                                std::get<0>(answer),std::get<1>(answer),std::get<2>(answer), w);
    }
    //lower than the worst-case score, return here and reduce current_score by 1
    //to prevent this case from being chose at the end
    if( current_score < worst_case )
    {
        std::get<0>(answer) -= 1.0;
        return answer;
    }
    //characters are different, and this is the end of this sub-problem
    else if( (a[0] != b[0]) && (a.length() == 1) && (b.length() == 1) )
    {
        std::get<0>(answer) += c;
        std::get<1>(answer) += a[0];
        std::get<2>(answer) += "R";

        return answer;
    }
    //characters are not matched inside the problem, and we must choose between a deletion in a
    //a deletion in b, and a mismatch
    else
    {
        double mismatch_score = current_score + c;
        std::string a_mismatched = a_matched + a[0];
        std::string b_mismatched = b_matched + "R";
        double deletion_score = current_score + d;
        std::string a_deleted = a_matched + "_";
        std::string b_deleted = b_matched + "_";
        std::string a_not_deleted = a_matched + a[0];
        std::string b_not_deleted = b_matched + b[0];

        auto mismatch = matching_score(m,c,d,a.substr(1),b.substr(1),
                                mismatch_score,a_mismatched,b_mismatched, worst_case);
        auto delete_a = matching_score(m,c,d,a,b.substr(1),
                                deletion_score,a_deleted,b_not_deleted, worst_case);
        auto delete_b = matching_score(m,c,d,a.substr(1),b,
                                deletion_score,a_not_deleted,b_deleted, worst_case);

        double mismatch_branch_score = std::get<0>(mismatch);
        double delete_a_score = std::get<0>(delete_a);
        double delete_b_score = std::get<0>(delete_b);

            
        if( (mismatch_branch_score >= delete_a_score) && (mismatch_branch_score >= delete_b_score) )
        {
            return mismatch;
        }
        else
        if( (delete_a_score >= mismatch_branch_score) && (delete_a_score >= delete_b_score) )
        {
            return delete_a;
        }
        else
        {
            return delete_b;
        }
    }
}



int main(int argc, char *argv[])
{    
    //obtain the coordinates of all points as a pair
    auto start = high_resolution_clock::now();

    double score = 0;
    std::string a_compared = "";
    std::string b_compared = "";

    /*
    double m = std::stod(argv[1]);
    double c = std::stod(argv[2]);
    double d = std::stod(argv[3]);
    std::string a = argv[4];
    std::string b = argv[5];
    */

    //DEBUG VERSION OF DECLARATIONS FOR RUNNING OFF CSIL
    double m = 2;
    double c = -0.5;
    double d = -1;
    std::string a = "ACACACTAC";
    std::string b = "AGCACACAG";
    /*
    A=
        8.5
        A_CACACTA
        AGCACAC_A
    */
    //to prevent very long branches
    //it is best to assume a change across the two shared slots
    //and then perform deletions with the bases left over
    //then to perform too many deletions
    //our worst-case score w is equal to the maximum number of changes,
    //and the minimum number of deletions
    double min_deletions = d * std::abs( (int)a.length() - (int)b.length() );
    double max_changes = c * std::min( (int)a.length(), (int)b.length() );
    double w = max_changes + min_deletions;
    
    auto answer = matching_score(m, c, d, a, b, 0, "", "", w);

    cout << std::get<0>(answer) << "\n";
    cout << std::get<1>(answer) << "\n";
    cout << std::get<2>(answer) << "\n";

    auto end = high_resolution_clock::now();
    auto total_runtime = duration_cast<microseconds>(end - start);
    double runtime_in_seconds = (double)total_runtime.count() / 1000000.0;

    cout << "Runtime: " << runtime_in_seconds << " s\n";
    
    return 0;
}
