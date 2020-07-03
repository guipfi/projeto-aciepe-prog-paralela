#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <random>

using namespace std;

#define TAMANHO 10000

string random_string( size_t length )
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

vector<string> get_tokens(string source, int start_source, int end_source) {
    string token;
    vector<string> tokens;
    int size_token, begin=-1, end;
    string last_signal, first_signal;

    if(source[start_source]==' ') {
        first_signal="0";
    } else {
        first_signal="1";
    }

    if(source[end_source-1]==' ') {
        last_signal="0";
    } else {
        last_signal="1";
    }

    for(int i=start_source; i<end_source; i++) {
        // If there's a begin and find a space, then mark the end or if is the last position in the source and isn't a empty space then there's one more token
        if((begin!=-1 && source[i]==' ') || (i==end_source-1 && source[i]!=' ')) {
            if(i!=end_source-1 || source[i]==' ') {
                end=i;
            } else {
                end=i+1;
                if(begin==-1) {
                    begin=end-1;
                }
            }
            size_token=end-begin;
            token="";
            // Token extraction
            for(int j=0; j<size_token; j++) {
                token+=source[begin+j];
            }
            // Save token
            tokens.push_back(token);
            begin=-1;
        }
        // When finds the first space marks the token begin
        else if(begin==-1 && source[i]!=' ') {
            begin=i;
        }
    }
    tokens.push_back(first_signal);
    tokens.push_back(last_signal);

    return tokens;
}

int main() {

    string source= random_string(TAMANHO);
    int work_groups=2, source_start, source_end;
    vector<vector<string>> tokens(work_groups);
    cout<<source<<endl;;

    cout<<"INITIALIZING"<<endl<<endl;
    chrono::time_point<std::chrono::steady_clock> start_time = chrono::steady_clock::now();

    #pragma omp parallel sections private(source_start,source_end) 
    {
        #pragma omp section 
        {
            source_start=0;
            source_end=TAMANHO/work_groups;
            tokens[0]=get_tokens(source, source_start, source_end);
        }    
        #pragma omp section
        {
            source_start=TAMANHO/work_groups;
            source_end=TAMANHO;
            tokens[1]=get_tokens(source, source_start, source_end);
        }    
    }

    if(work_groups>1) {
        tokens[work_groups-1].pop_back();
        for(int w=work_groups; w>1; w--) {
            cout<<tokens[w-1].back()<<" "<<tokens[w-2].back()<<endl;
            if(tokens[w-1].back()=="1" && tokens[w-2].back()=="1") {
                tokens[w-1][0]=tokens[w-2][tokens[w-2].size()-3]+tokens[w-1][0]; 
                tokens[w-2].erase(tokens[w-2].end()-3);
            }
            tokens[w-1].pop_back();
            tokens[w-2].pop_back();   
        }   
        tokens[0].pop_back();
    } else {
        tokens[0].pop_back();
        tokens[0].pop_back();
    }
    
    // Printing tokens result
    for(int i=0; i<work_groups; i++) {
        for(vector<string>::iterator it=tokens[i].begin(); it!=tokens[i].end(); it++) {
            cout<<(*it)<<" ";
        }
    }

    chrono::time_point<std::chrono::steady_clock> end_time = chrono::steady_clock::now();
    chrono::duration<double> elapsed_seconds = end_time-start_time;
    cout << endl<< elapsed_seconds.count() << "s\n";

    return 0;
}