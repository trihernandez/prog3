#include <utility>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <cfloat>
#include <fstream>
#include <iterator>
#include <sstream>
#include <algorithm>
//#include <time.h>       // to get the runtime for inputs

using namespace std;

#include <chrono>       // to get the runtime for inputs. Taken from https://www.geeksforgeeks.org/measure-execution-time-function-cpp/
using namespace std::chrono;





int position_2d(int width, int i, int j)
{
    return (i*width)+j;
}





class Node
{
    public:

    //index along a
    int a_index;
    //index along b
    int b_index;
    //do the bases at a and b match?
    bool match;
    //Optimal score for this node
    double score;
    //optimal path to take to this node
    //  'a' = deleting b
    //  'b' = deleting a
    //  'c' = change
    //  'm' = match
    char path;
    
    Node()
    {
        a_index = -1;
        b_index = -1;
        match = false;
        score = 0.0;
        path = 'x';
    }

    Node(int a_ind, int b_ind, bool match_bool, double s, char p)
    {
        a_index = a_ind;
        b_index = b_ind;
        match = match_bool;
        score = s;
        path = p;
    }
};





std::string decode_path(std::string a, std::string b, int a_len, int b_len, Node * table)
{
    int a_index = 0;
    int b_index = 0;
    std::string a_output = "";
    std::string b_output = "";
    Node currentNode = *(table + position_2d(b_len, a_index, b_index));

    while((a_index < a_len) && (b_index < b_len))
    {
        currentNode = *(table + position_2d(b_len, a_index, b_index));
        if(currentNode.path == 'm')
        {
            a_output = a_output + a.at(a_index);
            b_output = b_output + b.at(b_index);
            a_index++;
            b_index++;
        }
        else if(currentNode.path == 'c')
        {
            a_output = a_output + a.at(a_index);
            b_output = b_output + "R";
            a_index++;
            b_index++;
        }
        else if(currentNode.path == 'a')
        {
            a_output = a_output + a.at(a_index);
            b_output = b_output + "_";
            a_index++;
        }
        else if(currentNode.path == 'b')
        {
            a_output = a_output + "_";
            b_output = b_output + b.at(b_index);
            b_index++;
        }
        else
        {
            a_index++;
            b_index++;
        }
    }
    
    std::string answer = a_output + "\n" + b_output;
    return answer;
}






void calculateNodeScore (double m, double c, double d, std::string a, std::string b,
                        int a_len, int b_len, int a_index, int b_index, Node * table)
{
    //if the node lies within the table, and we still have remaining bases in a and b
    if( (a_index < a_len-1) && (b_index < b_len-1) && (a_index >= 0) && (b_index >=0 ) )
    {
        Node * currentNode = (table + position_2d(b_len, a_index, b_index));

        double match_score = 0.0;
        double delete_a_score = 0.0;
        double delete_b_score = 0.0;

        char match_path = 'm';
            
        //match or mismatch
        //Node * matchNode = (table + position_2d(b_len, a_index+1, b_index+1));
        Node matchNode = *(table + position_2d(b_len, a_index+1, b_index+1));
        if( currentNode->match == true )
        {
            match_score = m + matchNode.score;

        }
        else
        {
            match_score = c + matchNode.score;
            match_path = 'c';
        }
            
        //delete a; increase b and keep a
        Node deleteANode = *(table + position_2d(b_len, a_index, b_index+1));
        delete_a_score = d + deleteANode.score;
            
        //delete b; increase a and keep b
        Node deleteBNode = *(table + position_2d(b_len, a_index+1, b_index));
        delete_b_score = d + deleteBNode.score;

            
        //calculate max scores
        double best_score = delete_b_score;
        char best_path = 'a';
        if(delete_a_score > best_score)
        {
            best_score = delete_a_score;
            best_path = 'b';
        }
        if(match_score > best_score)
        {
            best_score = match_score;
            best_path = match_path;
        }

        currentNode->score = best_score;
        currentNode->path = best_path;
    }
    //otherwise, if we have no bases left in either A or B
    //Adding these else statements because there was a bug in an earlier version code where
    //  these values were incorrectly calculated, throwing off the entire table
    //But we know at this part of the table, we can only perform deletions
    else if( (a_index == a_len-1) && (b_index == b_len-1) )
    {
        //we have no more moves left;
        //do not alter the table
        return;
    }
    else if(a_index == a_len-1)
    {
        //we have no bases left in b
        //we must delete a
        Node * currentNode = (table + position_2d(b_len, a_index, b_index));
        Node nextNode = *(table + position_2d(b_len, a_index, b_index+1));
        currentNode->score = d + nextNode.score;
        currentNode->path = 'b';
    }
    else if(b_index == b_len-1)
    {
        //we have no bases left in a
        //we must delete b
        Node * currentNode = (table + position_2d(b_len, a_index, b_index));
        Node nextNode = *(table + position_2d(b_len, a_index+1, b_index));
        currentNode->score = d + nextNode.score;
        currentNode->path = 'a';
    }
}









int main(int argc, char *argv[])
{    
    //obtain the coordinates of all points as a pair
    auto start = high_resolution_clock::now();

    double m = 2.0;
    double c = -0.5;
    double d = -1.0;
    std::string a = "";
    std::string b = "";

    bool m_read = false;
    bool c_read = false;
    bool d_read = false;
    bool a_read = false;
    bool b_read = false;

    if (argc > 11 || argc < 5)
    {
        std::cerr << "Usage: " << argv[0] << "-m m(o) -c c(o) -d d(o) -1 seq1 -2 seq2" << std::endl;
        return 1;
    }
    else if (argc%2 == 0)
    {
        std::cerr << "Usage: " << argv[0] << "-m m(o) -c c(o) -d d(o) -1 seq1 -2 seq2" << std::endl;
        return 1;
    }
    else {
        for( int i = 1; i<argc; i=i+2)
        {
            std::string argv_i0 = argv[i];
            std::string argv_i1 = argv[i+1];
            if((argv_i0 == "-m") && (m_read == false))
            {
                m = std::stod(argv_i1);
                m_read = true;
            }
            else if((argv_i0 == "-c") && (c_read == false))
            {
                c = std::stod(argv_i1);
                c_read = true;
            }
            else if((argv_i0 == "-d") && (d_read == false))
            {
                d = std::stod(argv_i1);
                d_read = true;
            }
            else if((argv_i0 == "-1") && (a_read == false))
            {
                std::string filename_a = argv_i1;
                filename_a = filename_a + ".txt";
                ifstream file_a(filename_a);
                a = { std::istreambuf_iterator<char>(file_a), std::istreambuf_iterator<char>{} };
                //taken from https://www.cplusplus.com/forum/beginner/151894/
                a_read = true;
            }
            else if((argv_i0 == "-2") && (b_read == false))
            {
                std::string filename_b = argv_i1;
                filename_b = filename_b + ".txt";
                ifstream file_b(filename_b);
                b = { std::istreambuf_iterator<char>(file_b), std::istreambuf_iterator<char>{} };
                b_read = true;
            }
            else
            {
                std::cerr << "Usage: " << argv[0] << "-m m(o) -c c(o) -d d(o) -1 seq1 -2 seq2" << std::endl;
                return 1;
            }
        }
    }
    if(( a_read == false ) || ( b_read == false ) )
    {
        std::cerr << "Usage: " << argv[0] << "-m m(o) -c c(o) -d d(o) -1 seq1 -2 seq2" << std::endl;
        return 1;
    }

    double score = 0;
    std::string a_compared = "";
    std::string b_compared = "";

    //lengths for a and b
    int a_len = (int)a.length()+1;
    int b_len = (int)b.length()+1;

    //keep track of the matching of letters
    //Node optimaility_table[a_len][b_len];
    Node * optimaility_table;
    optimaility_table = new Node[a_len * b_len];

    for(int i = 0; i< a_len; i++ )
    {
        for(int j = 0; j< b_len; j++ )
        {
            bool ij_match = false;
            //if( i<(a_len-1) || j<(b_len-1) )
                ij_match = a[i]==b[j];
            //optimaility_table[i][j] = n;
            Node n(i, j, ij_match, 0.0, 'x');
            optimaility_table[position_2d(b_len, i, j)] = n;
        }
    }
    
    //calculateNodeScore(m, c, d, a, b, 
    //              a_len, b_len, a_index, b_index,*opt_pointer)
    //calculate scores and paths of node on the tree
    //starting with lower left and approaching in a staircase manner;
    int algorithm_start = (a_len-1) + (b_len-1);
    for(int ij_sum = algorithm_start; ij_sum>=0; ij_sum--)
    {
        for(int i = 0; ((i<= ij_sum) && (i<a_len)); i++)
        {
            int j = ij_sum - i;
            if(i<a_len && j<b_len)
            {
                calculateNodeScore(m, c, d, a, b, a_len, b_len, i, j, optimaility_table);
            }
        }
    }

    cout << optimaility_table->score << "\n";
    cout << decode_path(a, b, a_len, b_len, optimaility_table) << "\n";

    auto end = high_resolution_clock::now();
    auto total_runtime = duration_cast<microseconds>(end - start);
    double runtime_in_seconds = (double)total_runtime.count() / 1000000.0;

    //cout << "Runtime: " << runtime_in_seconds << " s\n";
    
    return 0;
}
