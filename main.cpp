#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "parser.h"
#include "circuit.h"

using namespace std;

int main(int argc, char* argv[])
{
    clock_t tStart = clock();
    ifstream infile(argv[2]);
    

    if(!infile) {cerr << "The file is not exist! " << endl; exit(1);}
    circuit cir(string(argv[2])+".log");

    Parse parser;
    parser.parsing(infile, cir.getgraph());

    cir.asap();
    cir.buildAsapCycle(); // build the cycle data structure
    cir.alap();

    if (strcmp(argv[1],"-l") == 0 && argc == 6) cir.mlrcs(stoi(argv[3]), stoi(argv[4]), stoi(argv[5]));
    else if(strcmp(argv[1],"-r") == 0 && argc == 4) cir.mrlcs(stoi(argv[3]));
    else{ cout << "command error" << '\n'; exit(1);}
    printf("[Time taken: %.5fs]\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
}