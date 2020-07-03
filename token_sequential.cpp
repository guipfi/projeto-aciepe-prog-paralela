#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <string.h>
#include <chrono>
#include <random>

using namespace std;

std::string random_string( size_t length )
{
    auto randchar = []() -> char
    {
        const char charset[] =
        "         "
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}

int main() {
    // Put here the size of the string
    string source= random_string(100000000);
    cout<<source<<endl;;

    chrono::time_point<std::chrono::steady_clock> start_time = chrono::steady_clock::now();

    string token="";
    vector<string> tokens;
    int size, size_token, begin=-1, end, n_tokens=0;

    size=source.size();
    
    for(int i=0; i<size; i++) {
        // If there's a begin and find a space, then mark the end or if is the last position in the source and isn't a empty space then there's one more token
        if((begin!=-1 && source[i]==' ') || (i==size-1 && source[i]!=' ')) {
            if(i==size-1) {
                end=i+1;
                if(begin==-1) {
                    begin=end-1;
                }
            } else {
                end=i;
            }
            size_token=end-begin;
            // Token extraction
            for(int j=0; j<size_token; j++) {
                token+=source[begin];
                begin++;
            }
            // Save token
            tokens.push_back(token);
            token="";
            begin=-1;
        }
        // When finds the first space marks the token begin
        else if(begin==-1 && source[i]!=' ') {
            begin=i;
        }
    }

    // Printing tokens result
    for(vector<string>::iterator it=tokens.begin(); it!=tokens.end(); it++) {
        cout<<(*it)<<" ";
    }

    chrono::time_point<std::chrono::steady_clock> end_time = chrono::steady_clock::now();
    chrono::duration<double> elapsed_seconds = end_time-start_time;
    cout << endl<< elapsed_seconds.count() << "s\n";

    return 0;
}