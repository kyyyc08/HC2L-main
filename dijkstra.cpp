#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <limits>
#include <sstream>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace std::chrono;

using NodeID = int;
using Distance = double;
const Distance INF = numeric_limits<Distance>::infinity();

struct Edge {
    NodeID to;
    Distance weight;
};

unordered_map<NodeID, vector<Edge>> graph;
int total_vertices = 0, total_edges = 0;

// Function to read .gr file and build graph
void read_gr_file(const string& filename) {
    ifstream file(filename);
    string line;
    unordered_map<NodeID, bool> seen;

    while (getline(file, line)) {
        if (line.empty()) continue;
        if (line[0] == 'a') {
            istringstream iss(line);
            char a;
            NodeID u, v;
            Distance w;
            iss >> a >> u >> v >> w;
            graph[u].push_back({v, w});
            graph[v].push_back({u, w});  // Assuming undirected

            seen[u] = true;
            seen[v] = true;
            total_edges++;
        }
    }

    total_vertices = seen.size();
}

// Dijkstra's algorithm with path support (but won't print path)
pair<Distance, vector<NodeID>> dijkstra(NodeID src, NodeID tgt) {
    unordered_map<NodeID, Distance> dist;
    unordered_map<NodeID, NodeID> prev;
    priority_queue<pair<Distance, NodeID>, vector<pair<Distance, NodeID>>, greater<>> pq;

    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (u == tgt) break;
        if (d > dist[u]) continue;

        for (const auto& edge : graph[u]) {
            Distance alt = d + edge.weight;
            if (!dist.count(edge.to) || alt < dist[edge.to]) {
                dist[edge.to] = alt;
                prev[edge.to] = u;
                pq.push({alt, edge.to});
            }
        }
    }

    if (!dist.count(tgt)) return {INF, {}};  // unreachable

    // Optional: still reconstruct the path, even if not printed
    vector<NodeID> path;
    for (NodeID at = tgt; at != src; at = prev[at]) {
        path.push_back(at);
    }
    path.push_back(src);
    reverse(path.begin(), path.end());

    return {dist[tgt], path};
}

// Main function with HC2L-style output (no path display)
int main(int argc, char* argv[]) {
    if (argc != 4) {
        cout << "Usage: dijkstra <graph_file.gr> <source_node> <target_node>" << endl;
        return 1;
    }

    string filename = argv[1];
    NodeID src = stoi(argv[2]);
    NodeID tgt = stoi(argv[3]);

    cout << "Dijkstra execution" << endl;
    cout << "multi-threading disabled\nshortcuts disabled\npath contraction disabled\npruning disabled\n\n";

    cout << "reading graph from " << filename << endl;
    auto start = high_resolution_clock::now();
    read_gr_file(filename);
    auto end = high_resolution_clock::now();
    duration<double> read_time = end - start;

    cout << "read " << total_vertices << " vertices and " << total_edges << " edges in " << read_time.count() << "s\n\n";

    cout << "running Dijkstra from " << src << " to " << tgt << "...\n";
    auto dijkstra_start = high_resolution_clock::now();
    auto [distance, path] = dijkstra(src, tgt);
    auto dijkstra_end = high_resolution_clock::now();
    duration<double> dijkstra_time = dijkstra_end - dijkstra_start;

    if (distance == INF) {
        cout << "No path found from " << src << " to " << tgt << endl;
    } else {
        cout << "Shortest distance: " << distance << endl;
    }

    cout << "Dijkstra runtime: " << dijkstra_time.count() << "s" << endl;

    return 0;
}
