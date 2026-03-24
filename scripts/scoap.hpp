#ifndef SCOAP_H
#define SCOAP_H

#include <string>
#include <queue>
#define defINT_MAX 200200100

using namespace std;

FILE *out = fopen("scoap.txt","w");

class Node{
public: 
    int prev_gate;
    vector<int> nxt_gate;
    int node_name;
    int node_type;
    int CC0, CC1, SC0, SC1;
    int CO, SO;
    // 0 - PI
    // 1 - PO
    // 2 - Internal node
    Node(){
        node_type = 2;
        prev_gate = -1; // -1 for PI
        //nxt_gate = -1;  // -1 for PO
        node_name = -1;

        CC0 = defINT_MAX;
        CC1 = defINT_MAX;
        SC0 = defINT_MAX;
        SC1 = defINT_MAX;
        CO = defINT_MAX;
        SO = defINT_MAX;
    }

    // Insert or set member variables into a declared class variable "Node"  
    void insertNode(int nType, int prev, vector<int> nxt, int cc0, int cc1, int sc0, int sc1, int co, int so){
        node_type = nType;
        prev_gate = prev;
        nxt_gate = nxt;

        CC0 = cc0;
        CC1 = cc1;
        SC0 = sc0;
        SC1 = sc1;
        CO = co;
        SO = so;
    }

    // Print the existing node
    void printTableHeader() {
        fprintf(out,"+--------+------------+--------+-----------------+--------+--------+--------+--------+--------+--------+\n");
        fprintf(out,"|  Node  | Node_Type  |  Prev  |      Next       |  CC0   |  CC1   |  SC0   |  SC1   |   CO   |   SO   |\n");
        fprintf(out,"+--------+------------+--------+-----------------+--------+--------+--------+--------+--------+--------+\n");
    }

    void printTableFooter() {
        fprintf(out,"+--------+------------+--------+-----------------+--------+--------+--------+--------+--------+--------+\n");
    }

    void printNode(int id) {
        fprintf(out,"| %2d (%d) | %10d | %6d | ", id, node_name, node_type, prev_gate);
        
        if (nxt_gate.size() == 0) {
            fprintf(out,"-1");
        } else {
            for (int i = 0; i < nxt_gate.size(); i++) {
                fprintf(out,"%d ", nxt_gate[i]);
            }
        }
        
        fprintf(out,"%*s| %6s | %6s | %6s | %6s | %6s | %6s |\n",
            static_cast<int>(15 - nxt_gate.size() * 2), "",
            (CC0 == defINT_MAX ? "inf" : std::to_string(CC0).c_str()),
            (CC1 == defINT_MAX ? "inf" : std::to_string(CC1).c_str()),
            (SC0 == defINT_MAX ? "inf" : std::to_string(SC0).c_str()),
            (SC1 == defINT_MAX ? "inf" : std::to_string(SC1).c_str()),
            (CO == defINT_MAX ? "inf" : std::to_string(CO).c_str()),
            (SO == defINT_MAX ? "inf" : std::to_string(SO).c_str()));
    }

};

vector<Node> CktNodes;

class Gate{
public:
    int gateType;
    int gate_no;
    // 0 -> AND 
    // 1 -> OR
    // 2 -> NAND 
    // 3 -> NOR
    // 4 -> XOR
    // 5 -> XNOR
    // 6 -> NOT 
    // 7 -> BUFF
    // 8 -> DFF/FLIPFLOP

    int num_input, num_output;
    vector<int> input_nodes;
    vector<int> output_nodes;
    vector<int> input_gates;
    vector<int> output_gates;

    Gate() {
        gateType = -1;
        num_input = 0;
        num_output = 0;
    }

    // Insert or set member variables into a declared class variable "Gate"  
    void insertGate(int gtype, int g_num, int num_in, int num_out,vector<int> in_idx, vector<int> out_idx){
        gateType = gtype;
        gate_no = g_num;
        num_input = num_in;
        num_output = num_out;
        input_nodes = in_idx;
        output_nodes = out_idx;
    }

    void printTableHeader() {
        printf("+--------+------------+-------------------------------------+-----------+--------+-----------+\n");
        printf("|  Gate  | Gate_Type  | #inputs |    Input Nodes            |    Input Gates            | #outputs  |    Output Nodes    |   Output gates    |\n");
        printf("+--------+------------+--------+-----------------+--------+--------+--------+--------+--------+\n");
    }

    void printTableFooter() {
        printf("+--------+------------+--------+-----------------+--------+--------+--------+--------+--------+\n");
    }

    char* gateName(){
        char *name;
        if(gateType == 0){
            name = "AND";
        }
        else if(gateType == 1){
            name = "OR";
        }
        else if(gateType == 2){
            name = "NAND";
        }
        else if(gateType == 3){
            name = "NOR";
        }
        else if(gateType == 4){
            name = "XOR";
        }
        else if(gateType == 5){
            name = "XNOR";
        }
        else if(gateType == 6){
            name = "NOT";
        }
        else if(gateType == 7){
            name = "BUFF";
        }
        else if(gateType == 8){
            name = "DFF";
        }

        return name;
    }

    // Print the existing variables values of the gate
    void printGate(){

        char *gate_name;
        gate_name = gateName();

        printf("| %6d | %3d (%s) | %6d |", gate_no,  gateType, gate_name, num_input);
        printInputgateNodes();
        printf("| %6d |", num_output);
        printOutputgateNodes();
        return;
    }

    // print the input nodes of the current Gate
    void printInputgateNodes(){
        if(input_nodes.size() == 0){
            printf(" -1 ");
            return;
        }
        for(int i=0;i<input_nodes.size();i++){
            printf("%6d ",input_nodes[i]);
        }
        printf("|");
        for(int i=0;i<input_gates.size();i++){
            printf("%6d ",input_gates[i]);
        }
        return;
    }

    // print the output nodes of the current Gate
    void printOutputgateNodes(){ 
        // if(output_nodes.size() == 0){
        //     printf("-1 | -1 |\n");
        //     return;
        // }
        for(int i=0;i<output_nodes.size();i++){
            printf("%d ",output_nodes[i]);
        }
        printf("    |   ");
        if(output_gates.size() == 0){
            printf("-1 |\n");
            return;
        }
        for(int i=0;i<output_gates.size();i++){
            printf("%d ",output_gates[i]);
        }
        printf("|\n");
        return;
    }
    
};


class Track{
public:
    int Size;
    vector<bool> gate_id;
    

    Track(){
        Size = 0;
    }

    void InitTrackList(){
        gate_id.resize(Size,false); // Default : false = unvisited
    }

    int isEmpty(){
        if(Size == 0){
            return 1;
        }
        return 0;
    }

};

int findNode(int Nodename){
    int node_idx = -1;
    for(int i=0;i<CktNodes.size();i++){
        if(CktNodes[i].node_name == Nodename){
            node_idx = i;
            break;
        }
    }

    return node_idx;
}
#endif