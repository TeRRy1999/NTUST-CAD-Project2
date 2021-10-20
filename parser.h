#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_map>
#include "node.h"

using namespace std;

bool breakLineDetection(string &line)
{
    if(line[line.length() - 1] == '\\'){
        line = line.substr(0, line.length() - 1);
        return true;
    }
    else return false;
}

class Parse
{
private:
    /* data */
public:
    Parse(/* args */){}
    ~Parse(){}

    void parsing(ifstream &infile, unordered_map <string, Node> &graph);
};

void Parse::parsing(ifstream &infile, unordered_map <string, Node> &graph)
{
    string equation;
    string word;
    while(infile >> word) if(word == ".model"){ infile >> word; break;}
    
    // start to read the file
    vector <string> correspondingTerm;
    vector <string> correspondingTruth;
    while(infile >> word){
        
        if(word == ".inputs" || word == ".outputs" || word == ".names" || word == ".end"){
            string line, parameter;
            getline(infile, line); // parameter
            while (breakLineDetection(line)){
                string nextLine;
                getline(infile, nextLine);
                line += nextLine;
            }
            
            if(word == ".names" || word == ".end"){
                if(correspondingTerm.size() > 0){

                    unsigned int tagetTerm = correspondingTerm.size()-1;
                    if(correspondingTruth.size() != 1) graph[correspondingTerm[tagetTerm]].setType("Or");
                    else{
                        if(correspondingTruth[0].length() == 2 && correspondingTruth[0][0]^correspondingTruth[0][1] == 1) graph[correspondingTerm[tagetTerm]].setType("Not"); // NOT
                        else graph[correspondingTerm[tagetTerm]].setType("And");
                    }
                    
                    correspondingTerm.clear();
                    correspondingTruth.clear();
                }
            }

            stringstream ss(line);

            while(ss >> parameter){
                if(graph.count(parameter) == 0){
                    Node node = Node();
                    node.setType(word);
                    node.setName(parameter);
                    graph[parameter] = node;
                }
                if(word == ".names") correspondingTerm.push_back(parameter);
            }

            if(word == ".names") {
                // construct the graph
                Node* outNode = &graph[correspondingTerm[correspondingTerm.size()-1]];
                for(unsigned int i = 0; i < correspondingTerm.size() - 1; i++){
                    Node* inNode = &graph[correspondingTerm[i]];
                    outNode->finAdd(inNode);
                    inNode->foutAdd(outNode);
                }
            }
        }

        else{ // truth_val
            if(word[0] == '.'){ // special case for "aoi_alu4.blif"
                string trash;
                getline(infile, trash);
            }
            else{
                string truth_val = word;
                infile >> word;
                truth_val += word;
                if(truth_val[truth_val.length()-2] == '\\') truth_val.erase(truth_val.end()-2); // eliminate the '\\'(next line) in truth value 
                if(correspondingTerm.size() != truth_val.length()){
                    cout << "No match" << endl;
                    for(auto &ch:correspondingTerm) cout << ch << ' ';
                    cout << endl;
                    
                    cout <<truth_val<< endl;
                } // ligalization checker
                
                int last = truth_val.length() - 1;
                correspondingTruth.push_back(truth_val);
            }
        }
    }
}
