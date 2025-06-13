#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <limits>
#include <sstream>

using namespace std;

using NodeID = int;
using Distance = double;
const Distance INF = numeric_limits<Distance>::infinity();

struct Edge {
    NodeID to;
    Distance weight;
};

unordered_map<NodeID, vector<Edge>> graph;

void read_gr_file(const string& filename) {
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        if (line[0] == 'a') {
            istringstream iss(line);
            char a;
            NodeID u, v;
            Distance w;
            iss >> a >> u >> v >> w;
            graph[u].push_back({v, w});
            graph[v].push_back({u, w});  
        }
    }
}

Distance dijkstra(NodeID src, NodeID tgt) {
    unordered_map<NodeID, Distance> dist;
    priority_queue<pair<Distance, NodeID>, vector<pair<Distance, NodeID>>, greater<>> pq;

    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (u == tgt) return d;
        if (d > dist[u]) continue;

        for (const auto& edge : graph[u]) {
            Distance alt = d + edge.weight;
            if (!dist.count(edge.to) || alt < dist[edge.to]) {
                dist[edge.to] = alt;
                pq.push({alt, edge.to});
            }
        }
    }

    return INF; // unreachable
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cout << "Usage: dijkstra <graph_file.gr> <source_node> <target_node>" << endl;
        return 1;
    }

    string filename = argv[1];
    NodeID src = stoi(argv[2]);
    NodeID tgt = stoi(argv[3]);

    read_gr_file(filename);
    Distance result = dijkstra(src, tgt);

    if (result == INF)
        cout << "No path found from " << src << " to " << tgt << endl;
    else
        cout << "Dijkstra distance: " << result << endl;

    return 0;
}
