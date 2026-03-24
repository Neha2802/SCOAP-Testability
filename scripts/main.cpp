#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <vector>
#include <queue>
#include <string.h>
#include <bits/stdc++.h>
#include "scoap.hpp"
#include "circuit.cpp"
#include "scoap.cpp"


struct Circuit inputNetlist(FILE *fp, char *buff, vector<Gate> *G ){
    
    struct Circuit C;
    //vector<Gate> Graph
    int count = 0;

	if(!fp)
	{
		printf("Input file not found. Check name\n");
	}
    //printf("test1\n");
	int input_num, output_num, node_num, gate_num;

	fscanf(fp, "%s", buff);

    //////////////// reading #inputs /////////////
	if( strcmp(buff, "INPUTS")!=0 )
	{
		printf("Incorrect input format. Input header not found\n");
	}

	fscanf(fp, "%d", &input_num);

    printf("Number of inputs: %d\n",input_num);

    vector<int> inputs(input_num);

    for(int i=0;i<input_num;i++){
        fscanf(fp, "%d", &inputs[i]);
    }

    ////////////////// reading #outputs ///////////////
    fscanf(fp, "%s", buff);

    if( strcmp(buff, "OUTPUTS")!=0 )
	{
		printf("Incorrect output format. Output header not found\n");
	}

	fscanf(fp, "%d", &output_num);

    printf("Number of outputs: %d\n",output_num);

    vector<int> outputs(output_num);

    for(int i=0;i<output_num;i++){
        fscanf(fp, "%d", &outputs[i]);
    }

    ////////////// reading #nodes ///////////
    fscanf(fp, "%s", buff);
    printf("%s\n",buff);

    if( strcmp(buff, "NODES")!=0 )
	{
		printf("Incorrect input format. Node header not found\n");
	}

	fscanf(fp, "%d", &node_num);

    printf("Number of nodes: %d\n",node_num);
    CktNodes.resize(node_num);
    printf("Number of Cktnodes: %zu\n",CktNodes.size());
    vector<int> nodes(node_num);
    
    for(int i=0;i<node_num;i++){
        fscanf(fp, "%d", &nodes[i]);
        CktNodes[i].node_name = nodes[i];
    }

    for(int i=0;i<input_num;i++){
        int idx = findNode(inputs[i]);
        CktNodes[idx].node_type = 0;
        CktNodes[idx].CC0 = 1;
        CktNodes[idx].CC1 = 1;
        CktNodes[idx].SC0 = 0;
        CktNodes[idx].SC1 = 0;
    }

    for(int i=0;i<output_num;i++){
        int idx = findNode(outputs[i]);
        CktNodes[idx].node_type = 1;
        CktNodes[idx].CO = 0;
        CktNodes[idx].SO = 0;
    }

    // reading #gates
    fscanf(fp, "%s", buff);
    printf("%s\n",buff);

    if( strcmp(buff, "GATES")!=0 )
	{
		printf("Incorrect input format. Gate header not found\n");
	}

	fscanf(fp, "%d", &gate_num);

    printf("Number of gates: %d\n",gate_num);

    vector<Gate> Graph;
    
    for(int i=0;i<gate_num;i++){
        int g_type=-1, num_in, g_no = i+1;
        //printf("test %d\n", i);
        fscanf(fp, "%s", buff);
        //printf("%s ", buff);
        // 0 -> AND 
        // 1 -> OR
        // 2 -> NAND 
        // 3 -> NOR
        // 4 -> XOR
        // 5 -> XNOR
        // 6 -> NOT 
        // 7 -> BUFF
        // 8 ->FLIPFLOP/DFF
        if(strcmp(buff,"AND")==0){
            g_type = 0;
        }
        else if(strcmp(buff,"OR")==0){
            g_type = 1;
        }
        else if(strcmp(buff,"NAND")==0){
            g_type = 2;
        }
        else if(strcmp(buff,"NOR")==0){
            g_type = 3;
        }
        else if(strcmp(buff,"XOR")==0){
            g_type = 4;
        }
        else if(strcmp(buff,"XNOR")==0){
            g_type = 5;
        }
        else if(strcmp(buff,"NOT")==0){
            g_type = 6;
        }
        else if(strcmp(buff,"BUFF")==0){
            g_type = 7;
        }
        else if(strcmp(buff,"DFF")==0){
            g_type = 8;
            count++;
        }

        printf("gate_no:%d\n",g_type);

        fscanf(fp, "%d", &num_in);
        vector<int> ins(num_in);
        int out_temp;
        vector<int> outs;

        for(int j=0;j<num_in;j++){
            printf("inputs reading\n");
            fscanf(fp, "%d", &ins[j]);
            int idx_in = findNode(ins[j]); // finds the array index at which a node is stored
            CktNodes[idx_in].nxt_gate.push_back(i+1);
        }
        

        while(1){
            //printf("%d\n", fscanf(fp, "%d", &out_temp));          
            fscanf(fp, "%d", &out_temp);
            outs.push_back(out_temp);
            int idx_out = findNode(out_temp);
            CktNodes[idx_out].prev_gate = i+1;
            //printf("%d\n", fscanf(fp, "%d", &out_temp));
            if(fscanf(fp, "%d", &out_temp) <= 0){
            printf("no more outputs\n");
                break;
            } 
        }

        int num_out = outs.size();

        ConstructGraph(&Graph, g_type, g_no, num_in, num_out, ins, outs);

    }
    
    printf("LOOP EXIT 1.\n");

    (*G) = Graph;
    C.dff = count;
    C.in = inputs;
    C.out = outputs;
    C.internal = nodes;
    return {inputs, outputs, nodes, count};
}


int compareArr(vector<int> arr, int var){
    if(var == 1){
        for(int i=0;i<CktNodes.size();i++){
            if((arr[i] == CktNodes[i].CO) && (arr[i] < defINT_MAX)){
                continue;
            }
            else{
                return 0;
            }
    }
    }
    else if(var == 2){
        for(int i=0;i<CktNodes.size();i++){
            if((arr[i] == CktNodes[i].SO) && (arr[i] < defINT_MAX)){
                continue;
            }
            else{
                return 0;
            }
        }
    }
    return 1;
}

int chkInf(){
    for(int i=0;i<CktNodes.size();i++){
        if(CktNodes[i].SC0 < defINT_MAX && CktNodes[i].SC1 < defINT_MAX){
            continue;
        }
        else{
            return 0;
        }
    }
    return 1;
}

int main(){
    FILE *fp;
    FILE *out;
	char buff[255];
	fp = fopen("parsed_netlist.txt", "r");
    //fp = fopen("renamed_netlist.txt", "r");
    out = fopen("scoap.txt", "w");

    vector<Gate> Graph;
    struct Circuit C;

    C = inputNetlist(fp, buff, &Graph);

    // // printf("%d\n", Graph.size());

    updateGraph(&Graph);

    queue<int> List;
    queue<int> Q;
    queue<int> PO;

    for(int i=0;i<Graph.size();i++){
        for(int j=0;j<Graph[i].input_nodes.size();j++){
            int node_idx = findNode(Graph[i].input_nodes[j]);
            if(CktNodes[node_idx].node_type == 0){
                List.push(i+1);
            }
        }
    }

    

    printGraph(&Graph);

    for(int i=0;i<C.internal.size();i++){
        int idx = findNode(C.internal[i]);
        Q.push(CktNodes[idx].prev_gate);
    }

    for(int i=0;i<C.out.size();i++){
        int idx = findNode(C.out[i]);
        PO.push(CktNodes[idx].prev_gate);
    }

    vector<int> co_final (CktNodes.size());
    if(C.dff == 0){
        printf("Combinational Contollability\n");
        gate_CombControl(Graph,List);

        printf("Combinational Observability\n");
        int ret = 0;
        for(int i=0;i<C.out.size();i++){
            for(int i=0;i<CktNodes.size();i++){
                co_final[i] = CktNodes[i].CO;
            }
            int temp = PO.front();
            PO.pop();
            gate_CombObs(Graph,temp,PO);
            ret = compareArr(co_final,1);
            if(ret == 1){ break; }
        }
    
        printNodeList();
        return 0;
    }
    if(C.dff > 0){
        printf("Combinational Contollability\n");
        gate_CombControl(Graph,List);

        printf("Combinational Observability\n");
        int ret = 0;
        for(int i=0;i<C.out.size();i++){
            for(int i=0;i<CktNodes.size();i++){
                co_final[i] = CktNodes[i].CO;
            }
            int temp = PO.front();
            // PO.pop();
            gate_CombObs(Graph,temp,PO);
            ret = compareArr(co_final,2);
            if(ret == 1){ break; }
        }

        
        // for(int i=0;i<CktNodes.size();i++){
        //     fprintf(out,"%d\n",CktNodes[i].SO);
        // }

        Track M;
        M.Size = Graph.size();
        M.InitTrackList();

        printf("Sequential Controllability\n");
        gate_SeqControl(Graph,List, &M);

        for(int i=0;i<CktNodes.size();i++){
            gate_SeqControl(Graph,List, &M);
        }
        //printNodeList();

        printf("Sequential Observability\n");
        for(int i=0;i<C.out.size();i++){
            gate_SeqObs(Graph, PO);
        }
        gate_SeqObs(Graph, PO);
        printNodeList();
    }
    printQueue(PO);

    // printf("Sequential Controllability\n");
    
    // Track M;
    // M.Size = Graph.size();
    // M.InitTrackList();

    
    

    fclose(out);
    fclose(fp);


    return 0;
}