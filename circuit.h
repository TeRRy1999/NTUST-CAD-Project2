#include <iostream>
#include <unordered_map>

using namespace std;

class circuit
{
private:
    unordered_map <string, Node> graph;
    unordered_map <int, vector<Node*>> cycleNode;
    string fileName;
    unsigned int cycle;
    unsigned int _and;
    unsigned int _or;
    unsigned int _not;

public:
    circuit(string filename);
    ~circuit();
    
    void asap(); // implemented the ASAP.
    void alap(); // implemented the ALAP.
    void buildAsapCycle();
    void recursive(Node *node, int level, bool forMove);
    void moveOn(Node *node, int level);
    void resetTrav();
    void visulLevel();
    void sche(vector <Node*> &container, int level, bool &item);
    void removeNode(Node *node, int level);
    void mlrcs(unsigned int _and, unsigned int _or, unsigned int _not);
    void mrlcs(unsigned int latency);

    unordered_map <string, Node> &getgraph() {return graph;}
};

circuit::circuit(string _file)
:fileName(_file)
{
}

circuit::~circuit()
{
}

void circuit::resetTrav()
{
    for(auto &node:graph)
        if(node.second.getTrav()){
            node.second.setTrav(false);
        }
}

void circuit::recursive(Node *node, int level, bool forMove)
{
    if(!node->getTrav() || level > node->getasapV()) { 
        if(level != 0) node->setTrav(true); // input node always be false (which is allow to visit)
        if(level > cycle) cycle = level;
        if(node->getasapV() <= level) {
            if(forMove){ 
                removeNode(node, node->getasapV());
                cycleNode[level].push_back(node);
                node->setSlack(node->getSlack() - 1);
            }
            node->setasapV(level);
        }
        for(auto &next:node->getfout()) recursive(next, level+1, forMove);
    }
}

void circuit::removeNode(Node *node, int level)
{
    for(unsigned int i = 0; i < cycleNode[level].size(); i++)
        if(cycleNode[level][i]->getName() == node->getName()) cycleNode[level].erase(cycleNode[level].begin()+i);
}

void circuit::moveOn(Node *node, int level)
{
    recursive(node, level+1, true);
    resetTrav();
    
}

void circuit::sche(vector <Node*> &container, int level, bool &item)
{
    
    unsigned int index = 0;
    Node *maxNode = container[index];
   
    for(unsigned int i = 0; i < container.size(); i++){
        if(container[i]->getSlack() > maxNode->getSlack()){
            maxNode = container[i];
            index = i;
        }
    }

    if(maxNode->getSlack() <= 0 && item == true){
        item = false;
        container.erase(container.begin() + index);
        return;
    }

    moveOn(maxNode, level);
    
    container.erase(container.begin() + index);
}

void circuit::mrlcs(unsigned int latency)
{
    fileName += "r";
    cout << "Latency-constrained Scheduling\n";

    _and = 1; _or = 1; _not = 1;
    
    if(latency < cycle) {cout << "No feasible solution." << endl; return;}
    else{
        int bias = latency - cycle;
        for(auto &node:graph)
            if(node.second.getType() != ".inputs")  
                node.second.setSlack(node.second.getSlack() + bias);
        
        for(unsigned int i = 1; i <= cycle; i++){
            vector <Node*> andCont, orCont, notCont;
            for(auto &node:cycleNode[i]){
                if(node->getType() == "And") andCont.push_back(node);
                if(node->getType() == "Or") orCont.push_back(node);
                if(node->getType() == "Not") notCont.push_back(node);
            }

            int dif_and, dif_or, dif_not;
            dif_and = andCont.size() - _and;
            dif_or = orCont.size() - _or;
            dif_not = notCont.size() - _not;

            bool item = true;
            if(dif_and > 0){
                for(unsigned int k = 0; k < dif_and; k++) {
                    sche(andCont, i, item);
                    if(!item) {_and++; item = true;}
                }
            }

            if(dif_or > 0){
                for(unsigned int k = 0; k < dif_or; k++) {
                    sche(orCont, i, item);
                    if(!item) {_or++; item = true;}
                }
            }
            if(dif_not > 0){
                for(unsigned int k = 0; k < dif_not; k++){
                    sche(notCont, i, item);
                    if(!item) {_not++; item = true;}
                }
            }
        }
    }
    visulLevel();
}


void circuit::mlrcs(unsigned int _andv, unsigned int _orv, unsigned int _notv)
{
    fileName += "l";
    cout << "Resource-constrained Scheduling\n";
    _and = _andv; _or = _orv; _not = _notv;
    if(_and == 0 || _or == 0 || _not==0){cout << "Gate cannot be zero" << endl; return;}
    for(unsigned int i = 1; i <= cycle; i++){

        vector <Node*> andCont, orCont, notCont;
        for(auto &node:cycleNode[i]){
            if(node->getType() == "And") andCont.push_back(node);
            if(node->getType() == "Or") orCont.push_back(node);
            if(node->getType() == "Not") notCont.push_back(node);
        }
        
        int dif_and, dif_or, dif_not;
        dif_and = andCont.size() - _and;
        dif_or = orCont.size() - _or;
        dif_not = notCont.size() - _not;

        bool item = false;
        if(dif_and > 0){
            for(unsigned int k = 0; k < dif_and; k++) 
                sche(andCont, i, item);
        }

        if(dif_or > 0){
            for(unsigned int k = 0; k < dif_or; k++) 
                sche(orCont, i, item);
        }
        if(dif_not > 0){
            for(unsigned int k = 0; k < dif_not; k++) 
                sche(notCont, i, item);
        }
    }
    visulLevel();
}

void circuit::asap()
{
    for(auto &node:graph)
        if(node.second.getType() == ".inputs"){
            recursive(&node.second, 0, false);
            resetTrav(); // reset all the traversal node
        }
}

void circuit::alap() // calculate alap and slack base on the asap result
{
    for(unsigned int i = cycle; i > 0; i--){
        for(auto &node:cycleNode[i]){
            if(node->getfout().size() == 0) node->setalapV(cycle);
            else{
                unsigned int foutMinCycle = cycle;
                for(auto &out:node->getfout()){
                    if(foutMinCycle > out->getalapV()) foutMinCycle = out->getalapV();
                }
                node->setalapV(foutMinCycle - 1);
            }
            node->setSlack(node->getalapV() - node->getasapV());
        }
    }
}

void circuit::buildAsapCycle()
{
    for(auto &node:graph)
        cycleNode[node.second.getasapV()].push_back(&(node.second));
}

void circuit::visulLevel()
{
    ofstream out(fileName);
    for(unsigned int i = 1; i <= cycle; i++){
        out << "-----level[" << i << "]-----" << endl;
        for (auto &node:cycleNode[i]){
            out << node->getName() << ' '; 
        }
        out << '\n';
    }

    out << "#AND: " << _and << '\n';
    out << "#OR: " << _or << '\n';
    out << "#NOT: " << _not << '\n';
    out << "END" << endl;
}