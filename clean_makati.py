import json
import math
import networkx as nx

def load_osm_json(filename):
    with open(filename, 'r', encoding='utf-8') as f:
        data = json.load(f)
        # If it's a list, just return it
        if isinstance(data, list):
            return data
        # Else assume it has 'elements' key
        return data['elements']


def merge_elements(elements_list):
    nodes = {}
    ways = []
    for elements in elements_list:
        for el in elements:
            if el['type'] == 'node':
                nodes[el['id']] = el
            elif el['type'] == 'way':
                ways.append(el)
    return nodes, ways

def haversine_distance(lat1, lon1, lat2, lon2):
    # Earth radius in meters
    R = 6371000
    phi1, phi2 = math.radians(lat1), math.radians(lat2)
    dphi = math.radians(lat2 - lat1)
    dlambda = math.radians(lon2 - lon1)

    a = math.sin(dphi/2)**2 + math.cos(phi1)*math.cos(phi2)*math.sin(dlambda/2)**2
    c = 2 * math.atan2(math.sqrt(a), math.sqrt(1-a))

    return R * c  # meters

def build_graph(nodes, ways):
    G = nx.Graph()
    # Add nodes with lat/lon
    for nid, ndata in nodes.items():
        if 'lat' in ndata and 'lon' in ndata:
            G.add_node(nid, lat=ndata['lat'], lon=ndata['lon'])

    # Add edges based on ways
    for way in ways:
        node_list = way.get('nodes', [])
        for i in range(len(node_list) -1):
            src = node_list[i]
            tgt = node_list[i+1]
            if src in G.nodes and tgt in G.nodes:
                lat1, lon1 = G.nodes[src]['lat'], G.nodes[src]['lon']
                lat2, lon2 = G.nodes[tgt]['lat'], G.nodes[tgt]['lon']
                dist = haversine_distance(lat1, lon1, lat2, lon2)
                # Add edge with distance weight
                G.add_edge(src, tgt, weight=dist)
    return G

def export_hc2l_format(G, filename_nodes, filename_edges):
    # Save nodes as: node_id lat lon
    with open(filename_nodes, 'w') as f:
        for nid, data in G.nodes(data=True):
            f.write(f"{nid} {data['lat']} {data['lon']}\n")

    # Save edges as: node_id1 node_id2 distance_in_meters
    with open(filename_edges, 'w') as f:
        for u, v, data in G.edges(data=True):
            f.write(f"{u} {v} {data['weight']:.2f}\n")

if __name__ == "__main__":
    elements1 = load_osm_json('makati_part1.json')
    # If you have another part, include it here:
    # elements2 = load_osm_json('makati_part2.json')
    
    nodes, ways = merge_elements([elements1])  # or [elements1, elements2]

    print(f"Total nodes: {len(nodes)}, Total ways: {len(ways)}")

    G = build_graph(nodes, ways)

    print(f"Graph nodes: {G.number_of_nodes()}, edges: {G.number_of_edges()}")

    export_hc2l_format(G, 'makati_nodes.txt', 'makati_edges.txt')

    print("✅ Exported makati_nodes.txt and makati_edges.txt")

