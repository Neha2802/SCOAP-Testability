#include <stdio.h>
#include <stdlib.h>
#include<limits.h>
#include <vector>
#include <string.h>
#include <queue>
#include <bits/stdc++.h>
#include "scoap.hpp"

int printQueue(queue<int> Q){
    if(Q.size() == 0){
        return -1;
    }

    int sz = Q.size();
    printf("Queue: ");
    for(int i=0;i<sz;i++){
        printf("%d ",Q.front());
        Q.pop();
    }
    printf("\n");

    return 1;
}

int checkOccurenceQueue(queue<int> Q, int val){
    if(Q.size() == 0){
        return -1;
    }

    int sz = Q.size();
    for(int i=0;i<sz;i++){
        if(val == Q.front()){
            return 1;
        }
        Q.pop();
    }
    printf("\n");

    return 0;
}

int printNodeList(){
    if(CktNodes.size() == 0){
        fprintf(out,"No nodes have been added to the circuit yet.\n");
        return -1;
    }

    fprintf(out,"printNodeList----> No.of nodes: %lu\n", CktNodes.size() - 1);
    CktNodes[0].printTableHeader();
    for(int i=0;i<CktNodes.size();i++){
        //printf("node: %d | ", i);
        CktNodes[i].printNode(i);
    }
    CktNodes[0].printTableFooter();
    return 1;
}

int ConstructGraph(vector<Gate> *G, int gate, int g_num, int ni, int no, vector<int> in_ID, vector<int> out_ID) {

    if(G->size() < 0){
        printf("No circuit detected\n");
        return -1;
    }
    printf("Size before push_back: %zu\n", G->size()-1);
    
    // Create a new Gate object and set its properties
    Gate newGate;
    newGate.insertGate(gate, g_num, ni, no, in_ID, out_ID);

    // newGate.printGate();
    
    // Add the new Gate object to the vector
    G->push_back(newGate);
       
    printf("    Size after push_back: %zu\n", G->size()-1);
    // (*G)[0].printGate();
    return 1;
}

int updateGraph(vector<Gate> *G){
    vector<Gate> gate;
    gate = (*G);
    if(gate.size() == 0){
        printf("No circuit detected\n");
        return -1;
    }

    for(int i=0;i<gate.size();i++){
        int in_num = gate[i].num_input, out_num = gate[i].num_output;

        for(int j=0;j<in_num;j++){
            int idx_in = findNode(gate[i].input_nodes[j]); // finds the array index at which a node is stored
            gate[i].input_gates.push_back(CktNodes[idx_in].prev_gate);
        }

        if(out_num > 1){
            printf("notice num output > 1\n");
        }
        for(int j=0;j<out_num;j++){
            int idx_out = findNode(gate[i].output_nodes[j]); // finds the array index at which a node is stored
            //printf("test0\n");
            
            gate[i].output_gates = CktNodes[idx_out].nxt_gate;
        }
    }
    (*G) = gate;
    return 1;
}

int printGraph(vector<Gate> *G){
    if(G->size() == 0){
        printf("No circuit has been constructed.\n");
        return -1;
    }

    (*G)[0].printTableHeader();

    for(int i=0;i<G->size();i++){
        (*G)[i].printGate();
    }

    (*G)[0].printTableFooter();
    return 1;
}



