#include <iostream>
#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "circuit.h"

using namespace std;

int main(int argc, char* argv[])
{
    ifstream infile(argv[2]);
    

    if(!infile) {cerr << "The file is not exist! " << endl; exit(1);}
    circuit cir;

    Parse parser;
    parser.parsing(infile, cir.getgraph());

    cir.asap();
    cir.buildAsapCycle(); // build the cycle data structure
    cir.alap();

    if (strcmp(argv[1],"-l") == 0 && argc == 6) cir.mlrcs(stoi(argv[3]), stoi(argv[4]), stoi(argv[5]));
    else if(strcmp(argv[1],"-r") == 0 && argc == 4) cir.mrlcs(stoi(argv[3]));
    else{ cout << "command error" << '\n'; exit(1);}

}