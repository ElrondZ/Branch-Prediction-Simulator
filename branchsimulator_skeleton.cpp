#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <stdlib.h>
#include <cmath>
#include <bitset>
#include <stdio.h>
#include <map>

using namespace std;
#define SN 00 
#define WN 01 
#define WT 10 
#define ST 11 



string getBits(bitset<32> addr, unsigned long indexbits)
{
    string stringaddr = addr.to_string();
    string bits;
    bits = stringaddr.substr(32 - indexbits, indexbits);

    return bits;
}

vector<unsigned long> counter(unsigned long indexrow)
{
    vector<unsigned long> mycounter;

    for (unsigned long i = 0; i < indexrow; i++)
    {
        mycounter[i] = ST;
    }

    return mycounter;
}

unsigned long read()
{
    ifstream config;
    config.open("config.txt");

    unsigned long countindexbts;

    while (!config.eof())  // read config file
    {
        config >> countindexbts;

    }
    return countindexbts;
}

bitset<32>count(vector<unsigned long>& x, unsigned long& y)
{
    


    ifstream trace;
    ofstream tracesout;
    string outname;
    outname = string("traceout.txt");

    trace.open("trace.txt");
    tracesout.open(outname.c_str());

    string xaddr;       
    unsigned int addr;  
    string accesstp;  
    bitset<32> accessaddr;
    string line;

    int branchst = 0;
    unsigned long miss = 0;
     

    if (trace.is_open() && tracesout.is_open()) {
        while (getline(trace, line)) {

            istringstream iss(line);

            if (!(iss >> xaddr >> accesstp)) { break; }

            stringstream saddr(xaddr);
            saddr >> std::hex >> addr;
            accessaddr = bitset<32>(addr);


            char* ptr;
            string counterb = getBits(accessaddr, y);
            long bits = strtol(counterb.c_str(), &ptr, 2);


            unsigned long branchst = 0;

            if (accesstp.compare("1") == 0)
            {

                if (x[bits] == ST || x[bits] == WT) {

                    x[bits] = ST;
                    branchst = 1;
                }

                else if (x[bits] == WN) {

                    x[bits] = ST;
                    branchst = 0;
                }

                else if (x[bits] == SN) {

                    x[bits] = WN;
                    branchst = 0;
                }

            }

            else if (accesstp.compare("0") == 0) {

                if (x[bits] == WN || x[bits] == SN) {

                    x[bits] = SN;
                    branchst = 0;
                }

                else if (x[bits] == WT) {

                    x[bits] = SN;
                    branchst = 1;
                }

                else if (x[bits] == ST) {

                    x[bits] = WT;
                    branchst = 1;
                }

            }

            if (((accesstp.compare("1") == 0) && branchst == 0) || ((accesstp.compare("0") == 0) && branchst == 1))

            {
                miss = miss + 1;
            }

            tracesout << branchst
                << endl;

        }
        trace.close();
        tracesout.close();
    }
    else cout << "\n Unable to open trace or traceout file ";
    return 0;
}



int main(int argc, char* argv[]) {


   
    unsigned long indexbits;
    indexbits = read();
    unsigned long indexrow = (unsigned long)pow(2, indexbits);

    vector<unsigned long> counter(indexrow, ST);

    count(counter, indexbits);



    return 0;
}