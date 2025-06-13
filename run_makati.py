# run_makati.py (remap + undirected version)
input_file = 'makati_edges.txt'
output_file = 'makati.gr'

edges = []
node_map = {}
next_id = 1

# Build node ID map and collect edges
with open(input_file, 'r') as f:
    for line in f:
        parts = line.strip().split()
        if len(parts) != 3:
            continue
        u_orig, v_orig, w = int(parts[0]), int(parts[1]), float(parts[2])

        # Map original node IDs to consecutive integers
        if u_orig not in node_map:
            node_map[u_orig] = next_id
            next_id += 1
        if v_orig not in node_map:
            node_map[v_orig] = next_id
            next_id += 1

        u = node_map[u_orig]
        v = node_map[v_orig]

        # Store both directions (undirected)
        edges.append((u, v, w))
        edges.append((v, u, w))

num_nodes = len(node_map)
num_edges = len(edges)

with open(output_file, 'w') as f:
    f.write(f"p sp {num_nodes} {num_edges}\n")
    for u, v, w in edges:
        f.write(f"a {u} {v} {w:.2f}\n")

print(f"✅ Wrote {num_edges} edges over {num_nodes} remapped nodes to {output_file}")