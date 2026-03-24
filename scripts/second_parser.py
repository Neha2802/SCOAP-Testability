def parse_netlist(input_netlist):
    inputs = []
    outputs = []
    nodes = set()
    gates = []

    for line in input_netlist.strip().splitlines():
        if line.startswith("INPUT("):
            # Extract each input node
            node = line.split("INPUT(")[1].strip(")")
            inputs.append(node)
            nodes.add(node)
        elif line.startswith("OUTPUT("):
            # Extract each output node
            node = line.split("OUTPUT(")[1].strip(")")
            outputs.append(node)
            nodes.add(node)
        elif " = " in line:  # Check if the line contains '=' before splitting
            # Parse gate list
            try:
                output, gate_info = line.split(" = ")
                gate_type, inputs_str = gate_info.split("(")
                input_nodes = inputs_str.strip(")").split(", ")

                # Store the output and input nodes
                nodes.add(output)
                nodes.update(input_nodes)

                # Format the gate line
                gate_line = f"{gate_type} {len(input_nodes)} {' '.join(input_nodes)} {output}"
                gates.append(gate_line)
            except ValueError as e:
                # Log or handle unexpected format in gate line
                print(f"Skipping line due to format error: {line}")
        else:
            print(f"Skipping line without '=': {line}")
        # else:
        #     # Parse gate list
        #     output, gate_info = line.split(" = ")
        #     gate_type, inputs_str = gate_info.split("(")
        #     input_nodes = inputs_str.strip(")").split(", ")

        #     # Store the output and input nodes
        #     nodes.add(output)
        #     nodes.update(input_nodes)

        #     # Format the gate line
        #     gate_line = f"{gate_type} {len(input_nodes)} {' '.join(input_nodes)} {output}"
        #     gates.append(gate_line)

    # Sort nodes and calculate totals
    nodes = sorted(nodes, key=int)
    num_inputs = len(inputs)
    num_outputs = len(outputs)
    num_nodes = len(nodes)
    num_gates = len(gates)

    # Format each section
    output_lines = [
        f"INPUTS {num_inputs}",
        " ".join(inputs),
        f"OUTPUTS {num_outputs}",
        " ".join(outputs),
        f"NODES {num_nodes}",
        " ".join(nodes),
        f"GATES {num_gates}"
    ] + gates

    return "\n".join(output_lines)

def linearize_nodenum(input_filename, output_filename):
    with open(input_filename, 'r') as file:
        netlist_lines = file.readlines()

    nodes_section = False
    gates_section = False
    node_set = set()
    updated_netlist = []

    # First pass: Collect all nodes and identify sections
    for line in netlist_lines:
        if line.startswith("NODES"):
            nodes_section = True
            node_count = int(line.split()[1])
            updated_netlist.append(line)  # Keep the NODES line in memory
            continue
        elif line.startswith("GATES"):
            nodes_section = False
            gates_section = True
        elif gates_section and not line.strip():
            gates_section = False

        if nodes_section:
            # Add node numbers to the set
            node_numbers = line.split()
            node_set.update(node_numbers)
            updated_netlist.append(line)
        else:
            updated_netlist.append(line)

    # Create a sorted list of nodes and map them to a serial order
    sorted_nodes = sorted(node_set, key=int)
    node_mapping = {node: str(i + 1) for i, node in enumerate(sorted_nodes)}

    # Second pass: Replace nodes in the relevant sections
    final_netlist = []
    for line in updated_netlist:
        if line.startswith("NODES"):
            # Rewrite NODES line with the updated count and sorted nodes
            final_netlist.append(f"NODES {len(node_set)}\n")
        elif line.startswith("GATES"):
            final_netlist.append(line)
            gates_section = True
        elif gates_section:
            # Split the line into parts: gate type, number of inputs, input nodes, and output node
            parts = line.split()
            if len(parts) > 2:
                gate_type = parts[0]
                num_inputs = parts[1]
                input_nodes = parts[2:-1]
                output_node = parts[-1]

                # Remap nodes
                remapped_inputs = [node_mapping.get(node, node) for node in input_nodes]
                remapped_output = node_mapping.get(output_node, output_node)

                # Reconstruct the gate definition line
                updated_line = f"{gate_type} {num_inputs} {' '.join(remapped_inputs)} {remapped_output}"
                final_netlist.append(updated_line + "\n")
            else:
                final_netlist.append(line)
        elif line.startswith("INPUTS") or line.startswith("OUTPUTS"):
            # Keep the INPUTS and OUTPUTS lines unchanged
            final_netlist.append(line)
        elif nodes_section:
            # Rewrite the nodes section with updated mappings
            updated_line = ' '.join(node_mapping[node] for node in line.split())
            final_netlist.append(updated_line + "\n")
        else:
            final_netlist.append(line)

    # Write the updated netlist to the output file
    with open(output_filename, 'w') as file:
        file.writelines(final_netlist)


# Usage:
input_filename = "raw_netlist.txt"
output_filename = "output_netlist.txt"

# Read the netlist as a single string
with open(input_filename, 'r') as file:
    netlist = file.read()

# Parse and save the netlist in the desired format
parsed_netlist = parse_netlist(netlist)

with open("parsed_netlist.txt", 'w') as file:
    file.write(parsed_netlist)

# Linearize node numbering in the parsed netlist
linearize_nodenum("parsed_netlist.txt", output_filename)