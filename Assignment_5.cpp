// Program: SwiftCargo - Optimal Delivery Route using Dynamic Programming
// Author: Gaurav Biradar
// Roll No: 123B1F011
// Date: 02/09/2025


#include <iostream>
#include <vector>
#include <limits>
#include <iomanip>
#include <chrono>
using namespace std;
using namespace std::chrono;

const double INF = 1e9;

struct Edge {
    int to;
    double cost;
};

// Function to find optimal path in a multistage directed acyclic graph (DAG)
pair<double, vector<int>> multistageOptimalPath(int N, int source, int dest, const vector<vector<Edge>>& adj) {
    vector<double> cost(N, INF);
    vector<int> parent(N, -1);

    cost[dest] = 0; 

    // Process nodes in reverse order 
    for (int i = N - 1; i >= 0; i--) {
        for (auto& e : adj[i]) {
            if (cost[i] > e.cost + cost[e.to]) {
                cost[i] = e.cost + cost[e.to];
                parent[i] = e.to;
            }
        }
    }

    // Reconstruct path
    vector<int> path;
    for (int v = source; v != -1; v = parent[v])
        path.push_back(v);

    return {cost[source], path};
}

// ------------------ Main Function ------------------
int main() {
    cout << "=== SwiftCargo Multistage Optimal Delivery System ===\n";

    int N, M;
    cout << "Enter number of nodes in multistage graph: ";
    cin >> N;
    cout << "Enter number of edges: ";
    cin >> M;

    vector<vector<Edge>> adj(N);

    cout << "Enter edges as: from to cost\n";
    for (int i = 0; i < M; i++) {
        int u, v;
        double w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
    }

    int source, dest;
    cout << "Enter source node: ";
    cin >> source;
    cout << "Enter destination node: ";
    cin >> dest;

    auto start = high_resolution_clock::now();
    auto result = multistageOptimalPath(N, source, dest, adj);
    auto end = high_resolution_clock::now();
    double execTime = duration_cast<nanoseconds>(end - start).count() / 1e6;

    double minCost = result.first;
    vector<int> path = result.second;

    cout << "\n--- Optimal Delivery Route ---\n";
    cout << "Path: ";
    for (int i = 0; i < path.size(); i++) {
        cout << path[i];
        if (i < path.size() - 1) cout << " -> ";
    }
    cout << "\nTotal Delivery Cost: " << fixed << setprecision(3) << minCost << endl;
    cout << "Execution Time: " << fixed << setprecision(6) << execTime << " ms\n";
    cout << "=====================================================\n";

    return 0;
}
