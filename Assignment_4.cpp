// Program: Smart Traffic Management for Emergency Vehicles
// Author: Gaurav Biradar
// Roll No: 123B1F011
// Date: 19/08/2025


#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
#include <chrono>
#include <algorithm>
using namespace std;
using namespace std::chrono;

// -------------------- Structures --------------------
struct Edge {
    int to;
    double weight; 
};

struct Node {
    int id;
    double dist;
    bool operator>(const Node& other) const { return dist > other.dist; }
};

// -------------------- Dijkstra's Algorithm --------------------
vector<double> dijkstra(int N, int source, vector<vector<Edge>>& adj, vector<int>& parent) {
    vector<double> dist(N, 1e9);
    parent.assign(N, -1);
    dist[source] = 0.0;

    priority_queue<Node, vector<Node>, greater<Node>> pq;
    pq.push({source, 0.0});

    while (!pq.empty()) {
        Node current = pq.top(); pq.pop();
        int u = current.id;
        double d = current.dist;

        if (d > dist[u]) continue; 

        for (auto& edge : adj[u]) {
            int v = edge.to;
            double w = edge.weight;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                parent[v] = u;
                pq.push({v, dist[v]});
            }
        }
    }
    return dist;
}

// -------------------- Path Reconstruction --------------------
vector<int> reconstructPath(int dest, const vector<int>& parent) {
    vector<int> path;
    for (int v = dest; v != -1; v = parent[v]) path.push_back(v);
    reverse(path.begin(), path.end());
    return path;
}

// -------------------- Display Function --------------------
void displayResults(int source, const vector<int>& hospitals, int nearestHospital,
                    double minDist, const vector<int>& path, double execTime) {
    cout << "\n================= Emergency Route =================\n";
    cout << "Ambulance Start: " << source << "\n";
    cout << "Hospitals: [ ";
    for (auto h : hospitals) cout << h << " ";
    cout << "]\n";
    cout << "Nearest Hospital: " << nearestHospital << " (Time: " 
         << fixed << setprecision(2) << minDist << " mins)\n";

    cout << "Shortest Path: ";
    for (size_t i = 0; i < path.size(); ++i) {
        cout << path[i];
        if (i != path.size() - 1) cout << " -> ";
    }
    cout << "\n";

    cout << "Execution Time: " << fixed << setprecision(3) << execTime << " ms\n";
    cout << "===================================================\n";
}

// -------------------- Main Function --------------------
int main() {
    cout << "=== Smart Traffic Management for Emergency Vehicles ===\n";

    int N, M;
    cout << "Enter number of intersections (nodes) and roads (edges): ";
    cin >> N >> M;

    vector<vector<Edge>> adj(N);

    cout << "\nEnter road details (start end travelTime in minutes):\n";
    for (int i = 0; i < M; i++) {
        int u, v;
        double w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w}); 
    }

    int source;
    cout << "\nEnter ambulance starting location (source node): ";
    cin >> source;

    int H;
    cout << "Enter number of hospitals: ";
    cin >> H;

    vector<int> hospitals(H);
    cout << "Enter hospital node indices:\n";
    for (int i = 0; i < H; i++) cin >> hospitals[i];

    cout << "\nCalculating shortest paths...\n";

    auto start = high_resolution_clock::now();
    vector<int> parent;
    vector<double> dist = dijkstra(N, source, adj, parent);
    auto end = high_resolution_clock::now();
    double execTime = duration_cast<nanoseconds>(end - start).count() / 1e6;

    // Find nearest hospital
    double minDist = 1e9;
    int nearestHospital = -1;
    for (int h : hospitals) {
        if (dist[h] < minDist) {
            minDist = dist[h];
            nearestHospital = h;
        }
    }

    vector<int> path = reconstructPath(nearestHospital, parent);

    displayResults(source, hospitals, nearestHospital, minDist, path, execTime);

    return 0;
}
