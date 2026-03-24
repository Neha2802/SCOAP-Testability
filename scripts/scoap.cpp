#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <vector>
#include <string.h>
#include <bits/stdc++.h>
#include "scoap.hpp"
struct ControlVals{
    int zero_con;
    int one_con;
};

struct Circuit{
    vector<int> in;
    vector<int> out;
    vector<int> internal;
    int dff;
};
int min(int len, vector<int> arr, int var){
    if(len <= 0){
        printf("Invalid value of 'len' in 'min' function\n");
        return -1;
    }

    int minimum = defINT_MAX;
    int idx = -1;
    //////
    // 0 -> CC0
    // 1 -> CC1
    // 2 -> SC0
    // 3 -> SC1
    if(var == 0){
        for(int i=0;i<len;i++){
            idx = findNode(arr[i]);
            if(CktNodes[idx].CC0 < minimum){ minimum = CktNodes[idx].CC0; }
        }
    }
    else if(var == 1){
        for(int i=0;i<len;i++){
            idx = findNode(arr[i]);
            if(CktNodes[idx].CC1 < minimum){ minimum = CktNodes[idx].CC1; }
        }
    }
    else if(var == 2){
        for(int i=0;i<len;i++){
            idx = findNode(arr[i]);
            if(CktNodes[idx].SC0 < minimum){ minimum = CktNodes[idx].SC0; }
        }
    }
    else if(var == 3){
        for(int i=0;i<len;i++){
            idx = findNode(arr[i]);
            if(CktNodes[idx].SC1 < minimum){ minimum = CktNodes[idx].SC1; }
        }
    }
    
    return minimum;
}


int sum(int len, vector<int> arr, int var){
    if(len <= 0){
        printf("Invalid value of 'len' in 'sum' function\n");
        return -1;
    }

    int summation = 0;
    int idx = -1;
    if(var == 0){
        for(int i=0;i<len;i++){
            idx = findNode(arr[i]);
            if(CktNodes[idx].CC0 == defINT_MAX ){
                summation = defINT_MAX;
                return summation;
            }

            summation += CktNodes[idx].CC0;
        }
    }
    else if(var == 1){
        for(int i=0;i<len;i++){
            idx = findNode(arr[i]);
            if(CktNodes[idx].CC1 == defINT_MAX){
                summation = defINT_MAX;
                return summation;
            }
            summation += CktNodes[idx].CC1;
        }
    }
    else if(var == 2){
        for(int i=0;i<len;i++){
            idx = findNode(arr[i]);
            if(CktNodes[idx].SC0 == defINT_MAX){
                summation = defINT_MAX;
                return summation;
            }
            summation += CktNodes[idx].SC0;
        }
    }
    else if(var == 3){
        for(int i=0;i<len;i++){
            idx = findNode(arr[i]);
            if(CktNodes[idx].SC1 == defINT_MAX){
                summation = defINT_MAX;
                return summation;
            }
            summation += CktNodes[idx].SC1;
        }
    }
    return summation;
}

struct ControlVals gate_CombControl(vector<Gate> G, queue<int> L){
    struct ControlVals cc = {0,0};   
    
    if(L.size() == 0){
        //printf("return\n");
        return cc;
    }
    int id = L.front();
    L.pop();

    int in_len;
    vector<int> in_nodes;
    vector<int> in_gates;
    int temp_gtype = G[id-1].gateType;
    int cc0,cc1;

    in_len = G[id-1].num_input;
    // printf("inlen = %d\n", in_len);
    in_nodes = G[id-1].input_nodes;
    in_gates = G[id-1].input_gates;

    vector<int> in_idx(in_len);

    for(int i=0;i<in_len;i++){
        in_idx[i] = findNode(in_nodes[i]);
    }

    if(temp_gtype == 0){ // AND gate
        //printf("test0\n");
        //printf("%d\n",in_idx[1]);

        cc0 = 1 + min(in_len, in_nodes, 0);
        cc1 = 1 + sum(in_len, in_nodes, 1);

        //printf("test0\n");
    }
    else if(temp_gtype == 1){ // OR gate
        cc0 = 1 + sum(in_len, in_nodes, 0);
        cc1 = 1 + min(in_len, in_nodes, 1);
        //printf("test1\n");
    }
    else if(temp_gtype == 2){ // NAND gate

        cc0 = 1 + sum(in_len, in_nodes, 1);
        cc1 = 1 + min(in_len, in_nodes, 0);
        //printf("test2\n");    
    }
    else if(temp_gtype == 3){ // NOR gate
        cc0 = 1 + min(in_len, in_nodes, 1);
        cc1 = 1 + sum(in_len, in_nodes, 0);
        
        //printf("test3\n");
    }
    else if(temp_gtype == 4){ // XOR gate
        if(in_len == 2){
            int var1, var2;
            var1 = (((CktNodes[in_idx[0]].CC0 + CktNodes[in_idx[1]].CC0) < (CktNodes[in_idx[0]].CC1 + CktNodes[in_idx[1]].CC1))?(CktNodes[in_idx[0]].CC0 + CktNodes[in_idx[1]].CC0):(CktNodes[in_idx[0]].CC1 + CktNodes[in_idx[1]].CC1));
            var2 = (((CktNodes[in_idx[0]].CC0 + CktNodes[in_idx[1]].CC1) < (CktNodes[in_idx[0]].CC1 + CktNodes[in_idx[1]].CC0))?(CktNodes[in_idx[0]].CC0 + CktNodes[in_idx[1]].CC1):(CktNodes[in_idx[0]].CC1 + CktNodes[in_idx[1]].CC0));
            
            cc0 = 1 + var1;
            cc1 = 1 + var2;
            
            // cc0 = 1 + (((CktNodes[in_idx[0]].CC0 + CktNodes[in_idx[1]].CC0) < (CktNodes[in_idx[0]].CC1 + CktNodes[in_idx[1]].CC1))?(CktNodes[in_idx[0]].CC0 + CktNodes[in_idx[1]].CC0):(CktNodes[in_idx[0]].CC1 + CktNodes[in_idx[1]].CC1));
            // cc1 = 1 + (((CktNodes[in_idx[0]].CC0 + CktNodes[in_idx[1]].CC1) < (CktNodes[in_idx[0]].CC1 + CktNodes[in_idx[1]].CC0))?(CktNodes[in_idx[0]].CC0 + CktNodes[in_idx[1]].CC1):(CktNodes[in_idx[0]].CC1 + CktNodes[in_idx[1]].CC0));
        }
        else{
            //printf("error in XOR gate \n");
            return cc;
        }
        //printf("test4\n");
    }
    else if(temp_gtype == 5){ // XNOR gate
        if(in_len == 2){
            int var1, var2;
            var1 = (((CktNodes[in_idx[0]].CC0 + CktNodes[in_idx[1]].CC0) < (CktNodes[in_idx[0]].CC1 + CktNodes[in_idx[1]].CC1))?(CktNodes[in_idx[0]].CC0 + CktNodes[in_idx[1]].CC0):(CktNodes[in_idx[0]].CC1 + CktNodes[in_idx[1]].CC1));
            var2 = (((CktNodes[in_idx[0]].CC0 + CktNodes[in_idx[1]].CC1) < (CktNodes[in_idx[0]].CC1 + CktNodes[in_idx[1]].CC0))?(CktNodes[in_idx[0]].CC0 + CktNodes[in_idx[1]].CC1):(CktNodes[in_idx[0]].CC1 + CktNodes[in_idx[1]].CC0));
            
            cc0 = 1 + var2;
            cc1 = 1 + var1;
            
            // cc0 = 1 + (((CktNodes[in_idx[0]].CC0 + CktNodes[in_idx[1]].CC0) < (CktNodes[in_idx[0]].CC1 + CktNodes[in_idx[1]].CC1))?(CktNodes[in_idx[0]].CC0 + CktNodes[in_idx[1]].CC0):(CktNodes[in_idx[0]].CC1 + CktNodes[in_idx[1]].CC1));
            // cc1 = 1 + (((CktNodes[in_idx[0]].CC0 + CktNodes[in_idx[1]].CC1) < (CktNodes[in_idx[0]].CC1 + CktNodes[in_idx[1]].CC0))?(CktNodes[in_idx[0]].CC0 + CktNodes[in_idx[1]].CC1):(CktNodes[in_idx[0]].CC1 + CktNodes[in_idx[1]].CC0));
        }
        else{
            //printf("error in XNOR gate \n");
            return cc;
        }
        //printf("test5\n");
    }
    else if(temp_gtype == 6){ // NOT gate

        cc0 = 1 + CktNodes[in_idx[0]].CC1;
        cc1 = 1 + CktNodes[in_idx[0]].CC0;
        
        //printf("test6\n");
    }
    else if(temp_gtype == 7){ // BUFF gate

        cc0 = 1 + CktNodes[in_idx[0]].CC0;
        cc1 = 1 + CktNodes[in_idx[0]].CC1;
        //printf("test7\n");
    }
    else if(temp_gtype == 8){ // FLIPFLOP/DFF gate

        cc0 = 2 + CktNodes[in_idx[0]].CC0;
        cc1 = 2 + CktNodes[in_idx[0]].CC1;
        //printf("test8\n");
    }

    int out_len = G[id-1].num_output;
    vector<int> out_nodes = G[id-1].output_nodes;
    vector<int> out_idx (out_len);

    for(int i=0;i<out_len;i++){
        out_idx[i] = findNode(out_nodes[i]);
    }
    //printf("-----------> cc0 = %d, cc1 = % d\n",cc0,cc1);
    cc.zero_con = cc0;
    cc.one_con = cc1;

    if( cc0 < defINT_MAX || cc1 < defINT_MAX){
        // assuming onlu one output for all gates
        int flag = 0;
        if(CktNodes[out_idx[0]].CC0 >  cc0){
            CktNodes[out_idx[0]].CC0 = cc0;
            // printf("new0\n");
        }
        else if(CktNodes[out_idx[0]].CC0 ==  cc0){
            flag++;
            // printf("new1\n");
        }

        if(CktNodes[out_idx[0]].CC1 >  cc1){
            CktNodes[out_idx[0]].CC1 = cc1;
            //printf("new2\n");
        }
        else if(CktNodes[out_idx[0]].CC1 ==  cc1){
            flag++;
            //printf("new3\n");
        }

        if(flag == 0 || flag == 1 ){
            for(int i=0; i< G[id-1].output_gates.size();i++){
                int chk = checkOccurenceQueue(L,G[id-1].output_gates[i]);
                //printf("chk = %d\n",chk);
                if(chk == 0 || chk == -1){
                    //printf("new4\n");
                    L.push(G[id-1].output_gates[i]);
                }
            }
        }
    }

    // printQueue((*L));
    // printf("size = %zu\n",L->size());

    gate_CombControl(G, L);
    return cc;
}


struct ControlVals gate_SeqControl(vector<Gate> G, queue<int> L, Track *M){

    struct ControlVals sc = {0,0};   
    printf("begin\n");
    //printQueue((*L));

    if(L.size() == 0){
        if(M->Size == 0){
            return sc;
        }
        for(int i=0;i<CktNodes.size();i++){
            if(CktNodes[i].SC0 >= defINT_MAX || CktNodes[i].SC1 >= defINT_MAX){
                if(CktNodes[i].prev_gate != -1){
                    int c = checkOccurenceQueue(L,CktNodes[i].prev_gate);
                    if(c == 0 || c == -1 ){
                        L.push(CktNodes[i].prev_gate);
                    }
                }
            }
        }
        printf("check \n");
        //return sc;
    }
    printQueue(L);
    int id = L.front();
    L.pop();
    // if(id > G.size()){
    //     id = L.front();
    //     L.pop();
    //     gate_SeqControl(G, L);
    //     //return sc;
    // }
    printf("id: %d\n",id);
    id = G[id-1].gate_no;
    int in_len = G[id-1].num_input;
    printf("inlen = %d\n", in_len);
    vector<int> in_nodes;
    vector<int> in_gates;
    in_nodes = G[id-1].input_nodes;
    in_gates = G[id-1].input_gates;
    printf("inlen = %d\n", in_len);
    int temp_gtype = G[id-1].gateType;
    int sc0,sc1;
    

    
    //printf("inlen = %d\n", in_len);
    in_nodes = G[id-1].input_nodes;
    in_gates = G[id-1].input_gates;

    vector<int> in_idx(in_len);

    for(int i=0;i<in_len;i++){
        in_idx[i] = findNode(in_nodes[i]);
    }

    if(temp_gtype == 0){ // AND gate
        sc0 = min(in_len, in_idx, 2);
        sc1 = sum(in_len, in_idx, 3);

        //printf("test0\n");
    }
    else if(temp_gtype == 1){ // OR gate
        sc0 = sum(in_len, in_idx, 2);
        sc1 = min(in_len, in_idx, 3);
        //printf("test1\n");
    }
    else if(temp_gtype == 2){ // NAND gate

        sc0 = sum(in_len, in_idx, 3);
        sc1 =  min(in_len, in_idx, 2);
        //printf("test2\n");
    
    }
    else if(temp_gtype == 3){ // NOR gate
        sc0 = min(in_len, in_idx, 3);
        sc1 = sum(in_len, in_idx, 2);
        
        //printf("test3\n");
    }
    else if(temp_gtype == 4){ // XOR gate
        
        if(in_len == 2){
            int var1, var2;
            var1 = (((CktNodes[in_idx[0]].SC0 + CktNodes[in_idx[1]].SC0) < (CktNodes[in_idx[0]].SC1 + CktNodes[in_idx[1]].SC1))?(CktNodes[in_idx[0]].SC0 + CktNodes[in_idx[1]].SC0):(CktNodes[in_idx[0]].SC1 + CktNodes[in_idx[1]].SC1));
            var2 = (((CktNodes[in_idx[0]].SC0 + CktNodes[in_idx[1]].SC1) < (CktNodes[in_idx[0]].SC1 + CktNodes[in_idx[1]].SC0))?(CktNodes[in_idx[0]].SC0 + CktNodes[in_idx[1]].SC1):(CktNodes[in_idx[0]].SC1 + CktNodes[in_idx[1]].SC0));
            
            sc0 = var1;
            sc1 = var2;
            
            // cc0 = 1 + (((CktNodes[in_idx[0]].CC0 + CktNodes[in_idx[1]].CC0) < (CktNodes[in_idx[0]].CC1 + CktNodes[in_idx[1]].CC1))?(CktNodes[in_idx[0]].CC0 + CktNodes[in_idx[1]].CC0):(CktNodes[in_idx[0]].CC1 + CktNodes[in_idx[1]].CC1));
            // cc1 = 1 + (((CktNodes[in_idx[0]].CC0 + CktNodes[in_idx[1]].CC1) < (CktNodes[in_idx[0]].CC1 + CktNodes[in_idx[1]].CC0))?(CktNodes[in_idx[0]].CC0 + CktNodes[in_idx[1]].CC1):(CktNodes[in_idx[0]].CC1 + CktNodes[in_idx[1]].CC0));
        }
        else{
            printf("error in XOR gate \n");
            return sc;
        }
        //printf("test4\n");
    }
    else if(temp_gtype == 5){ // XNOR gate
        if(in_len == 2){
            int var1, var2;
            var1 = (((CktNodes[in_idx[0]].SC0 + CktNodes[in_idx[1]].SC0) < (CktNodes[in_idx[0]].SC1 + CktNodes[in_idx[1]].SC1))?(CktNodes[in_idx[0]].SC0 + CktNodes[in_idx[1]].SC0):(CktNodes[in_idx[0]].SC1 + CktNodes[in_idx[1]].SC1));
            var2 = (((CktNodes[in_idx[0]].SC0 + CktNodes[in_idx[1]].SC1) < (CktNodes[in_idx[0]].SC1 + CktNodes[in_idx[1]].SC0))?(CktNodes[in_idx[0]].SC0 + CktNodes[in_idx[1]].SC1):(CktNodes[in_idx[0]].SC1 + CktNodes[in_idx[1]].SC0));
            
            sc0 = var2;
            sc1 = var1;
        }
        //printf("test5\n");
    }
    else if(temp_gtype == 6){ // NOT gate
        sc0 = CktNodes[in_idx[0]].SC1;
        sc1 = CktNodes[in_idx[0]].SC0;
        
        //printf("test6\n");
    }
    else if(temp_gtype == 7){ // BUFF gate

        sc0 = CktNodes[in_idx[0]].SC0;
        sc1 = CktNodes[in_idx[0]].SC1;

        // cc0 = 2 + CktNodes[in_idx[0]].CC1;
        // cc1 = 2 + CktNodes[in_idx[0]].CC0;
        //printf("test7\n");
    }
    else if(temp_gtype == 8){ // FLIPFLOP/DFF gate

        sc0 = 1 + CktNodes[in_idx[0]].SC0;
        sc1 = 1 + CktNodes[in_idx[0]].SC1;
        //printf("test8\n");
    }

    int out_len = G[id-1].num_output;
    vector<int> out_nodes = G[id-1].output_nodes;
    vector<int> out_idx (out_len);
    printf("out_len = %d\n",out_len);
    for(int i=0;i<out_len;i++){
        out_idx[i] = findNode(out_nodes[i]);
    }
    printf("-----------> sc0 = %d, sc1 = % d\n",sc0,sc1);
    sc.zero_con = sc0;
    sc.one_con = sc1;

    if( sc0 < defINT_MAX || sc1 < defINT_MAX){
        //printf("checking\n");
        // assuming onlu one output for all gates
        int flag = 0;
        if(CktNodes[out_idx[0]].SC0 >  sc0){
            CktNodes[out_idx[0]].SC0 = sc0;
            //printf("new0\n");
        }
        else if(CktNodes[out_idx[0]].SC0 == sc0){
            //CktNodes[out_idx[0]].SC0 = sc0;
            flag++;
            //printf("new1\n");
        }

        if(CktNodes[out_idx[0]].SC1 >  sc1){
            CktNodes[out_idx[0]].SC1 = sc1;
            //printf("new2\n");
        }
        else if(CktNodes[out_idx[0]].SC1 == sc1){
            //CktNodes[out_idx[0]].SC0 = sc0;
            flag++;
            //printf("new3\n");
        }

        printf("flag = %d\n",flag);

        if(flag == 0 || flag == 1 ){
            for(int i=0; i< CktNodes[out_idx[0]].nxt_gate.size();i++){
                printf("id --->%d\n",CktNodes[out_idx[0]].nxt_gate[i]);
                int chk = checkOccurenceQueue(L,CktNodes[out_idx[0]].nxt_gate[i]);
                printf("chk = %d\n",chk);
                if(chk == 0 || chk == -1){
                    //printf("new4\n");
                    printf("%d\n",CktNodes[out_idx[0]].nxt_gate[i]);
                    if(CktNodes[out_idx[0]].nxt_gate[i] != -1){
                        L.push(CktNodes[out_idx[0]].nxt_gate[i]);
                    }
                   //printf("now?\n");
                }
            }
            //L.push(id);
        }
        else{
            printf("both defined\n");
            printf("%d\n",id);
            if( M->gate_id[id-1] == false ){
                M->gate_id[id-1] = true;
                M->Size--;
            }
            printf("M size = %d\n",M->Size);
        }
    }
    printf("testing\n");
    //printNodeList();
    printQueue(L);
    //printf("M size = %d\n",M->Size);

    //printf("count = %d\n",count);
    printf("size = %zu\n",L.size());
    
    if(L.size() == 0){
        return sc;
    }
    gate_SeqControl(G, L, M);

    return sc;

}

vector<int> gate_CombObs(vector<Gate> G, int id, queue<int> L){

    //printf("id: %d\n",id);
    if(id == -1){
        id = L.front();
        L.pop();
    }

    if(L.size() == 0){
        return vector<int> (10,1);
    }

    int in_len,out_len;
    vector<int> in_nodes;
    vector<int> in_gates;
    int temp_gtype = G[id-1].gateType;
    vector<int> out_nodes;

    in_len = G[id-1].num_input;
    in_nodes = G[id-1].input_nodes;
    in_gates = G[id-1].input_gates;


    // printf("inlen : %d\n", in_len);
    //printf("%d\n",temp_gtype);
    vector<int> co (in_len);
    //printf("add0\n");

    //printf("inlen : %d\n", in_len);
    vector<int> in_idx(in_len);
    //printf("test_error\n");

    out_len = G[id-1].num_output;
    out_nodes = G[id-1].output_nodes;
    vector<int> out_idx(out_len);
    
    for(int i=0;i<in_len;i++){
        in_idx[i] = findNode(in_nodes[i]);
    }

    for(int i=0;i<out_len;i++){
        out_idx[i] = findNode(out_nodes[i]);
    }
        //BACKTRACE to calculate CO of all nodes
        /////////////
        // 0 -> AND 
        // 1 -> OR
        // 2 -> NAND 
        // 3 -> NOR
        // 4 -> XOR
        // 5 -> XNOR
        // 6 -> NOT 
        // 7 -> BUFF
        // 8 -> FLIPFLOP/DFF

        ////
        // 0 -> CC0
        // 1 -> CC1
        // 2 -> SC0
        // 3 -> SC1
    if(temp_gtype == 0){ // AND gate
        int tot = sum(in_len, in_nodes, 1);
        for(int i=0;i<in_len;i++){
            co[i] = 1 + CktNodes[out_idx[0]].CO + (tot - CktNodes[in_idx[i]].CC1); // CO(in_1) = 1 + CO(out) + sum(CC1 of all inputs except in_1)
        }
        //printf("test0\n");
    }
    else if(temp_gtype == 1){ // OR gate
        int tot = sum(in_len, in_nodes, 0);
        for(int i=0;i<in_len;i++){
            co[i] = 1 + CktNodes[out_idx[0]].CO + (tot - CktNodes[in_idx[i]].CC0); // CO(in_1) = 1 + CO(out) + sum(CC0 of all inputs except in_1)
        }
        //printf("test1\n");
    }
    else if(temp_gtype == 2){ // NAND gate

        co.resize(in_len);
        int tot = sum(in_len, in_nodes, 1);
        for(int i=0;i<in_len;i++){
            co[i] = 1 + CktNodes[out_idx[0]].CO + (tot - CktNodes[in_idx[i]].CC1); // CO(in_1) = 1 + CO(out) + sum(CC1 of all inputs except in_1)
        }
        //printf("test2\n");
    }
    else if(temp_gtype == 3){ // NOR gate
    
        int tot = sum(in_len, in_nodes, 0);
        for(int i=0;i<in_len;i++){
            co[i] = 1 + CktNodes[out_idx[0]].CO + (tot - CktNodes[in_idx[i]].CC0); // CO(in_1) = 1 + CO(out) + sum(CC0 of all inputs except in_1)
        }
        //printf("test3\n");
    }
    else if(temp_gtype == 4){ // XOR gate

        co[0] = 1 + CktNodes[out_idx[0]].CO; ((CktNodes[in_idx[1]].CC0 < CktNodes[in_idx[1]].CC1) ? CktNodes[in_idx[1]].CC0 : CktNodes[in_idx[1]].CC1);
        co[1] = 1 + CktNodes[out_idx[0]].CO; ((CktNodes[in_idx[0]].CC0 < CktNodes[in_idx[0]].CC1) ? CktNodes[in_idx[0]].CC0 : CktNodes[in_idx[0]].CC1);

        //printf("test4\n");
    }
    else if(temp_gtype == 5){ // XNOR gate

        co[0] = 1 + CktNodes[out_idx[0]].CO; ((CktNodes[in_idx[1]].CC0 < CktNodes[in_idx[1]].CC1) ? CktNodes[in_idx[1]].CC0 : CktNodes[in_idx[1]].CC1);
        co[1] = 1 + CktNodes[out_idx[0]].CO; ((CktNodes[in_idx[0]].CC0 < CktNodes[in_idx[0]].CC1) ? CktNodes[in_idx[0]].CC0 : CktNodes[in_idx[0]].CC1);
        //printf("test5\n");
    }
    else if(temp_gtype == 6){ // NOT gate
        
        for(int i=0;i<in_len;i++){
            co[i] = 1 + CktNodes[out_idx[0]].CO; // CO(in_1) = 1 + CO(out) + sum(CC0 of all inputs except in_1)
        }
        //printf("test6\n");
    }
    else if(temp_gtype == 7){ // BUFF gate

        for(int i=0;i<in_len;i++){
            co[i] = 2 + CktNodes[out_idx[0]].CO; // CO(in_1) = 1 + CO(out) + sum(CC0 of all inputs except in_1)
        }
        //printf("test7\n");
    }
    else if(temp_gtype == 8){ // FLIPFLOP/DFF gate
        co[0] = 2 + CktNodes[out_idx[0]].CO;
        //printf("test8\n");
    }

    // printf("----------\n");
    // for(int i=0;i<in_len;i++){
    //     printf("%d ",in_nodes[i]);
    // }
    // printf("\n");
    // for(int i=0;i<in_len;i++){
    //     printf("%d ",co[i]);
    // }
    // printf("\n---------\n");

    
    for(int i=0;i<in_len;i++){
        //printf("loop_old%d\n",i);
        if( co[i] <= CktNodes[in_idx[i]].CO ){
            CktNodes[in_idx[i]].CO = co[i];
            if(G[id-1].input_gates[i] > 0){
                L.push(G[id-1].input_gates[i]);
            }
        }
    }

    //printf("chk1\n");

    int g_id = L.front();
    L.pop();
    //printf("chk2\n");
    while(g_id == 17){
        //printf("chk\n");
        g_id = L.front();
        L.pop();
    }
    //printf("size = %zu\n",L->size());
    gate_CombObs(G,g_id,L);
    //printf("new1 %d\n",in_len);
    return co;
}

vector<int> gate_SeqObs(vector<Gate> G, queue<int> L){
    //printf("id: %d\n",id);

    printQueue(L);

    if(L.size() == 0){
        printf("yes\n");
        return vector<int> (10,1);
    }

    int id = L.front();
    L.pop();    

    int in_len,out_len;
    vector<int> in_nodes;
    vector<int> in_gates;
    int temp_gtype = G[id-1].gateType;
    vector<int> out_nodes;
    
    in_len = G[id-1].num_input;
    in_nodes = G[id-1].input_nodes;
        printf("new0\n");
    in_gates = G[id-1].input_gates;
    

    printf("inlen : %zu\n", in_nodes.size());
    printf("gtype = %d\n",temp_gtype);
    vector<int> so (in_len);
    //printf("add0\n");

    printf("inlen : %d\n", in_len);
    vector<int> in_idx(in_len);
    printf("test_error\n");

    out_len = G[id-1].num_output;
    out_nodes = G[id-1].output_nodes;
    vector<int> out_idx(out_len);
    
    for(int i=0;i<in_len;i++){
        in_idx[i] = findNode(in_nodes[i]);
    }

    for(int i=0;i<out_len;i++){
        out_idx[i] = findNode(out_nodes[i]);
    }
        //BACKTRACE to calculate CO of all nodes
        /////////////
        // 0 -> AND 
        // 1 -> OR
        // 2 -> NAND 
        // 3 -> NOR
        // 4 -> XOR
        // 5 -> XNOR
        // 6 -> NOT 
        // 7 -> BUFF
        // 8 -> FLIPFLOP/DFF

        ////
        // 0 -> CC0
        // 1 -> CC1
        // 2 -> SC0
        // 3 -> SC1
    if(temp_gtype == 0){ // AND gate
        int tot = sum(in_len, in_nodes, 3);
        for(int i=0;i<in_len;i++){
            so[i] =  CktNodes[out_idx[0]].SO + (tot - CktNodes[in_idx[i]].SC1); // CO(in_1) = 1 + CO(out) + sum(CC1 of all inputs except in_1)
        }
        //printf("test0\n");
    }
    else if(temp_gtype == 1){ // OR gate
        int tot = sum(in_len, in_nodes, 2);
        for(int i=0;i<in_len;i++){
            so[i] = CktNodes[out_idx[0]].SO + (tot - CktNodes[in_idx[i]].SC0); // CO(in_1) = 1 + CO(out) + sum(CC0 of all inputs except in_1)
        }
        //printf("test1\n");
    }
    else if(temp_gtype == 2){ // NAND gate

        so.resize(in_len);
        int tot = sum(in_len, in_nodes, 3);
        for(int i=0;i<in_len;i++){
            so[i] = CktNodes[out_idx[0]].SO + (tot - CktNodes[in_idx[i]].SC1); // CO(in_1) = 1 + CO(out) + sum(CC1 of all inputs except in_1)
        }
        //printf("test2\n");
    }
    else if(temp_gtype == 3){ // NOR gate
    
        int tot = sum(in_len, in_nodes, 2);
        for(int i=0;i<in_len;i++){
            so[i] =  CktNodes[out_idx[0]].SO + (tot - CktNodes[in_idx[i]].SC0); // CO(in_1) = 1 + CO(out) + sum(CC0 of all inputs except in_1)
        }
        //printf("test3\n");
    }
    else if(temp_gtype == 4){ // XOR gate

        so[0] =  CktNodes[out_idx[0]].SO + ((CktNodes[in_idx[1]].SC0 < CktNodes[in_idx[1]].SC1) ? CktNodes[in_idx[1]].SC0 : CktNodes[in_idx[1]].SC1);
        so[1] =  CktNodes[out_idx[0]].SO + ((CktNodes[in_idx[0]].SC0 < CktNodes[in_idx[0]].SC1) ? CktNodes[in_idx[0]].SC0 : CktNodes[in_idx[0]].SC1);

        //printf("test4\n");
    }
    else if(temp_gtype == 5){ // XNOR gate

        so[0] = CktNodes[out_idx[0]].SO + ((CktNodes[in_idx[1]].SC0 < CktNodes[in_idx[1]].SC1) ? CktNodes[in_idx[1]].SC0 : CktNodes[in_idx[1]].SC1);
        so[1] = CktNodes[out_idx[0]].SO + ((CktNodes[in_idx[0]].SC0 < CktNodes[in_idx[0]].SC1) ? CktNodes[in_idx[0]].SC0 : CktNodes[in_idx[0]].SC1);
        //printf("test5\n");
    }
    else if(temp_gtype == 6){ // NOT gate
        
        for(int i=0;i<in_len;i++){
            so[i] = CktNodes[out_idx[0]].SO; // CO(in_1) = 1 + CO(out) + sum(CC0 of all inputs except in_1)
        }
        //printf("test6\n");
    }
    else if(temp_gtype == 7){ // BUFF gate

        for(int i=0;i<in_len;i++){
            so[i] = CktNodes[out_idx[0]].SO; // CO(in_1) = 1 + CO(out) + sum(CC0 of all inputs except in_1)
        }
        //printf("test7\n");
    }
    else if(temp_gtype == 8){ // FLIPFLOP/DFF gate
        so[0] = 1 + CktNodes[out_idx[0]].SO;
        //printf("test8\n");
    }

    printf("----------\n");
    for(int i=0;i<in_len;i++){
        printf("%d ",in_nodes[i]);
    }
    printf("\n");
    for(int i=0;i<in_len;i++){
        printf("%d ",so[i]);
    }
    printf("\n---------\n");

    
    for(int i=0;i<in_len;i++){
        //printf("loop_old%d\n",i);
        if( so[i] <= CktNodes[in_idx[i]].SO ){
            CktNodes[in_idx[i]].SO = so[i];
            if(G[id-1].input_gates[i] > 0){
                L.push(G[id-1].input_gates[i]);
            }
        }
    }

    printf("chk1\n");

    // int g_id = L.front();
    // L.pop();
    printf("chk2\n");
    // while(g_id <= 0){
    //     //printf("chk\n");
    //     g_id = L.front();
    //     L.pop();
    // }
    //printf("size = %zu\n",L.size());
    gate_SeqObs(G,L);
    //printf("new1 %d\n",in_len);
    return so;
}

