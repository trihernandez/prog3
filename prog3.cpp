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


double optimal_score(double m, double c, double d, std::string a, std::string b)
{
    //case #1: performing the max number of matches
    //and the minimum number of deletions
    double max_matches = m * std::min( (int)a.length(), (int)b.length() );
    double min_deletions = d * std::abs( (int)a.length() - (int)b.length() );
    double b1 = max_matches + min_deletions;
    
    //case #2: performing the max number of changes
    //and the minimum number of deletions
    double max_changes = c * std::min( (int)a.length(), (int)b.length() );
    double b2 = max_changes + min_deletions;
    
    //case #3: deleting every base of a from b and vice-versa
    double b3 = d * ( (int)a.length() + (int)b.length() );
    
    //return the maximum of the cases
    if(( b1 > b2 ) && ( b1 > b3 ))
        return b1;
    if(( b2 > b1 ) && ( b2 > b3 ))
        return b2;
    return b3;
}

double worst_case_score(double m, double c, double d, std::string a, std::string b)
{
    //case #1: performing the max number of matches or changes
    //(which one is the lowest)
    //and the minimum number of deletions
    double lowest_of_m_c = 0.0;
    
    if(m < c)
        lowest_of_m_c = m;
    else
        lowest_of_m_c = c;
    double max_matches = lowest_of_m_c * std::min( (int)a.length(), (int)b.length() );
    double min_deletions = d * std::abs( (int)a.length() - (int)b.length() );
    double w1 = max_matches + min_deletions;
    
    //case #2: deleting every base of a from b and vice-versa
    double w2 = d * ( (int)a.length() + (int)b.length() );
    
    //offset the worst case by 0.1 * minimum of m,c,d
    double offset = std::fmin( std::abs(m), std::abs(c));
    offset = std::fmin(offset, std::abs(d));
    offset = 0.1 * offset;

    //return the maximum of the cases
    return std::fmax(w1, w2) - offset;
}

std::tuple<double, std::string, std::string> matching_score(double m, double c, double d, std::string a, std::string b, 
                              double current_score, std::string a_matched, std::string b_matched, 
                              double highest_possible_score, double *highest_score)
{
    //the answer for the problem
    std::tuple<double, std::string, std::string> answer(current_score, a_matched, b_matched);

    /*
    cout << std::get<1>(answer) << "\n";
    cout << std::get<2>(answer) << "\n";
    cout << " current score:         " <<std::get<0>(answer) << "\n";
    cout << " highest possible score:" << highest_possible_score << "\n";
    cout << "*highest_score:         " << *highest_score << "\n";
    cout << " highest_score:         " << highest_score << "\n";
    cout << "\n";
    */

    //sub-optimal branch; will only be below worst-case score.  Stop here.
    if( highest_possible_score <= *highest_score )
    {
        return answer;
    }
    //end of sequence
    if( a.length() == 0 && b.length() == 0 )
    {
        cout << std::get<1>(answer) << "\n";
        cout << std::get<2>(answer) << "\n";

        if(current_score > *highest_score )
        {
            *highest_score = current_score;
        }
        cout << " current_score:         " << current_score << "\n";
        cout << " highest possible score:" << highest_possible_score << "\n";
        cout << "*highest_score:         " << *highest_score << "\n";
        cout << "\n";
        return answer;
    }
    //end of a; b is longer, and therefore rest of a was deleted
    else if( a.length() == 0 )
    {
        std::get<0>(answer) += d;
        std::get<1>(answer) += "_";
        std::get<2>(answer) += b[0];
        double optimal = std::get<0>(answer) + optimal_score(m, c, d, a, b.substr(1));

        return matching_score(m,c,d,a,b.substr(1),
                                std::get<0>(answer),std::get<1>(answer),std::get<2>(answer),
                                optimal,highest_score);
    }
    //end of b; a is longer, and therefore rest of b was deleted
    else if( b.length() == 0 )
    {
        std::get<0>(answer) += d;
        std::get<1>(answer) += a[0];
        std::get<2>(answer) += "_";
        double optimal = std::get<0>(answer) + optimal_score(m, c, d, a.substr(1), b);

        return matching_score(m,c,d,a.substr(1),b,
                                std::get<0>(answer),std::get<1>(answer),std::get<2>(answer),
                                optimal,highest_score);
    }
    //else, iterate between our possible cases
    else
    {
        double match_score = std::get<0>(answer) + m;
        double delete_score = std::get<0>(answer) + d;
        double change_score = std::get<0>(answer) + c;
        
        std::string a_kept = std::get<1>(answer) + a[0];
        std::string b_kept = std::get<2>(answer) + b[0];
        std::string a_deleted = std::get<1>(answer) + "_";
        std::string b_deleted = std::get<2>(answer) + "_";
        std::string b_changed = std::get<2>(answer) + "R";

        double match_optimal = match_score + optimal_score(m, c, d, a.substr(1), b.substr(1));
        double delete_a_optimal = delete_score + optimal_score(m, c, d, a, b.substr(1));
        double delete_b_optimal = delete_score + optimal_score(m, c, d, a.substr(1), b);
        double change_optimal = change_score + optimal_score(m, c, d, a.substr(1), b.substr(1));
        
        //match if a[0] == b[0]; change if a[0] != b[0]
        //either case will be stored in match
        std::tuple<double, std::string, std::string> match;
        if( a[0] == b[0] )
        {
            match = matching_score(m,c,d,a.substr(1),b.substr(1),
                                match_score,a_kept,b_kept,
                                match_optimal,highest_score);
        }
        else
        {
            match = matching_score(m,c,d,a.substr(1),b.substr(1),
                                change_score,a_kept,b_changed,
                                change_optimal,highest_score);
        }
        
        //deletion in a
        auto delete_a = matching_score(m,c,d,a,b.substr(1),
                                delete_score,a_deleted,b_kept,
                                delete_a_optimal,highest_score);

        //deletion in b
        auto delete_b = matching_score(m,c,d,a.substr(1),b,
                                delete_score,a_kept,b_deleted,
                                delete_b_optimal,highest_score);
        
        //choose the branch with largest score
        double match_final_score = std::get<0>(match);
        double delete_a_final_score = std::get<0>(delete_a);
        double delete_b_final_score = std::get<0>(delete_b);

            
        if( (match_final_score >= delete_a_final_score) && (match_final_score >= delete_b_final_score) )
        {
            return match;
        }
        else
        if( (delete_a_final_score >= match_final_score) && (delete_a_final_score >= delete_b_final_score) )
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
    double c = -0.2;
    double d = -1.5;
    //std::string a = "ACACACTA";
    //std::string b = "AGCACACA";
    //std::string a = "ACACACTAC";
    //std::string b = "AGCACACAG";
    //std::string a = "ACACACTEEEEEEEACACTA";
    //std::string b = "AGCACACAAGCACACA";
    std::string a = "ACATGAGACAGACAGACCCCCAGAGACAGACCCCTAGACACAGAGAGAGTATGCAGGACAGGGTTTTTGCCCAGGGTGGCAGTATG";
    std::string b = "AGGATTGAGGTATGGGTATGTTCCCGATTGAGTAGCCAGTATGAGCCAGAGTTTTTTACAAGTATTTTTCCCAGTAGCCAGAGAGAGAGTCACCCAGTACAGAGAGC";
    /*
    A=
        8.5
        A_CACACTA
        AGCACAC_A
    */
    
    //the worst case will be inputted as our high score
    double worst = worst_case_score(m, c, d, a, b);

    //the best case will be compared to our high score
    double best = optimal_score(m, c, d, a, b);
    
    auto answer = matching_score(m, c, d, a, b, 0, "", "", best, &worst);

    cout << std::get<0>(answer) << "\n";
    cout << std::get<1>(answer) << "\n";
    cout << std::get<2>(answer) << "\n";

    auto end = high_resolution_clock::now();
    auto total_runtime = duration_cast<microseconds>(end - start);
    double runtime_in_seconds = (double)total_runtime.count() / 1000000.0;

    cout << "Runtime: " << runtime_in_seconds << " s\n";
    
    return 0;
}
