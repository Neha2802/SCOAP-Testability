def parse_and_rename_netlist(input_filename, output_filename, mapping_filename):
    with open(input_filename, 'r') as file:
        input_netlist = file.read()

    inputs = []
    outputs = []
    nodes = []
    gates = []
    node_mapping = {}

    for line in input_netlist.strip().splitlines():
        if line.startswith("INPUT("):
            # Extract each input node
            node = line.split("INPUT(")[1].strip(")")
            inputs.append(node)
            if node not in node_mapping:
                node_mapping[node] = None
        elif line.startswith("OUTPUT("):
            # Extract each output node
            node = line.split("OUTPUT(")[1].strip(")")
            outputs.append(node)
            if node not in node_mapping:
                node_mapping[node] = None
        elif " = " in line:  # Parse gate definitions
            try:
                output, gate_info = line.split(" = ")
                gate_type, inputs_str = gate_info.split("(")
                input_nodes = inputs_str.strip(")").split(", ")

                # Add nodes to the node mapping
                if output not in node_mapping:
                    node_mapping[output] = None
                for input_node in input_nodes:
                    if input_node not in node_mapping:
                        node_mapping[input_node] = None

                gates.append((gate_type, input_nodes, output))
            except ValueError as e:
                print(f"Skipping line due to format error: {line}")

    # Assign serial numbers to nodes
    current_index = 1

    # Input nodes come first
    for node in inputs:
        node_mapping[node] = current_index
        current_index += 1

    # Internal nodes come next
    for node in node_mapping:
        if node not in inputs and node not in outputs:
            node_mapping[node] = current_index
            current_index += 1

    # Output nodes come last
    for node in outputs:
        node_mapping[node] = current_index
        current_index += 1

    # Generate the parsed netlist with renamed nodes
    num_inputs = len(inputs)
    num_outputs = len(outputs)
    num_nodes = len(node_mapping)
    num_gates = len(gates)

    parsed_netlist = [
        f"INPUTS {num_inputs}",
        " ".join(str(node_mapping[node]) for node in inputs),
        f"OUTPUTS {num_outputs}",
        " ".join(str(node_mapping[node]) for node in outputs),
        f"NODES {num_nodes}",
        " ".join(str(node_mapping[node]) for node in node_mapping),
        f"GATES {num_gates}",
    ]

    for gate_type, input_nodes, output_node in gates:
        serialized_inputs = [str(node_mapping[node]) for node in input_nodes]
        serialized_output = str(node_mapping[output_node])
        parsed_netlist.append(f"{gate_type} {len(input_nodes)} {' '.join(serialized_inputs)} {serialized_output}")

    # Write the renamed netlist to the output file
    with open(output_filename, 'w') as file:
        file.write("\n".join(parsed_netlist))

    # Write the mapping to the mapping file in tabular format
    with open(mapping_filename, 'w') as file:
        file.write("Original_Node\tSerialized_Node\n")
        for original, serialized in sorted(node_mapping.items(), key=lambda x: x[1]):
            file.write(f"{original}\t{serialized}\n")


# Usage:
input_filename = "raw_netlist.txt"
output_filename = "parsed_netlist.txt"
mapping_filename = "node_mapping.txt"

parse_and_rename_netlist(input_filename, output_filename, mapping_filename)
