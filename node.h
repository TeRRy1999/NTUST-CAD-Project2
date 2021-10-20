#include <iostream>
#include <vector>
using namespace std;

class Node
{
private:
    string name;
    string type;

    vector <Node*> fin;
    vector <Node*> fout;

    int slack;
    unsigned int asapV;
    unsigned int alapV;
    unsigned int level;

    bool trav;

public:
    Node(/* args */):asapV(0), alapV(0), level(0), slack(0), trav(false){};
    
    //setter
    void setType(string in_tyep) {type = in_tyep;}
    void setName(string in_name) {name = in_name;}

    void setSlack(int val)       {slack = val;}
    // void setLevel(int val)       {level = val;}
    void setasapV (int val)      {asapV = val;}
    void setalapV (int val)      {alapV = val;}

    void setTrav(bool in_trav)   {trav = in_trav;}

    //getter
    string getType() {return type;}
    string getName() {return name;}

    int getSlack () {return slack;}
    // unsigned int getLevel () {return level;}
    unsigned int getasapV () {return asapV;}
    unsigned int getalapV () {return alapV;}

    bool getTrav() {return trav;}

    // why not use the string to save, because string need to find from map with additional time comsuming
    void finAdd(Node* node){fin.push_back(node);}
    void foutAdd(Node* node){fout.push_back(node);}

    vector <Node*> &getfin() {return fin;}
    vector <Node*> &getfout() {return fout;}

    // list all node
    void finls(){for(auto &ele:fin) cout << ele->getName() << " "; cout << '\n';}
    void foutls(){for(auto &ele:fout) cout << ele->getName() << " "; cout << '\n';}
    
    ~Node(){};
};