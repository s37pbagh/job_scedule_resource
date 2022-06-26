//
//  main.cpp
//  nima-homa
//
//  Created by parham bagherzadeh on 4/5/1401 AP.
//

#include <iostream>
#include "input.h"
class Node {
public:
    int id;
    Node* successors[32];
    int resource[4];
    int duration;
    
};


int main(int argc, const char * argv[]) {
    // insert code here...
    Node mynodes [32];
    
    auto p = readFile("j301_1.sm");
    int maxDu;
    maxDu= p.horizon;
    for (int i=0 ; i<32;i++){
        mynodes[i].resource= p.jobs[4].modes[1].usedResources;
        mynodes[i].duration = p.jobs[i].modes[1].duration ;
        mynodes[i].id=i;
        for (int j = 0; j < p.jobs[i].amountSuccessors; ++j){
            mynodes[i].successors[j]=&mynodes[p.jobs[i].successors[j]];
            
        }
    }
    std::cout<<"parham"<<std::endl;
    return 0;
}
