// Program: SwiftShip – Optimal Delivery Route using Branch and Bound (LC Method)
// Author: Gaurav Biradar
// Roll No: 123B1F011
// Date: 14/10/2025


#include <iostream>
#include <vector>
#include <limits>
#include <iomanip>
#include <chrono>
using namespace std;
using namespace std::chrono;

const int INF = 9999999;

// Node structure for Branch and Bound
struct Node {
    vector<vector<int>> reducedMatrix;
    vector<int> path;
    int level, vertex;
    int cost, bound;
};

// ----------------------- Utility: Matrix Reduction -----------------------
int reduceMatrix(vector<vector<int>>& matrix, int n) {
    int reductionCost = 0;

    // Row Reduction
    for (int i = 0; i < n; i++) {
        int rowMin = INF;
        for (int j = 0; j < n; j++)
            if (matrix[i][j] < rowMin)
                rowMin = matrix[i][j];
        if (rowMin != INF && rowMin != 0) {
            reductionCost += rowMin;
            for (int j = 0; j < n; j++)
                if (matrix[i][j] != INF)
                    matrix[i][j] -= rowMin;
        }
    }

    // Column Reduction
    for (int j = 0; j < n; j++) {
        int colMin = INF;
        for (int i = 0; i < n; i++)
            if (matrix[i][j] < colMin)
                colMin = matrix[i][j];
        if (colMin != INF && colMin != 0) {
            reductionCost += colMin;
            for (int i = 0; i < n; i++)
                if (matrix[i][j] != INF)
                    matrix[i][j] -= colMin;
        }
    }

    return reductionCost;
}

// ----------------------- Create a Child Node -----------------------
Node createNode(const vector<vector<int>>& parentMatrix, vector<int> const& path,
                int level, int i, int j, int n) {
    Node child;
    child.reducedMatrix = parentMatrix;
    child.path = path;

    // Set outgoing and incoming edges as INF
    for (int k = 0; k < n; k++) {
        child.reducedMatrix[i][k] = INF;
        child.reducedMatrix[k][j] = INF;
    }
    // Prevent returning to the starting node prematurely
    child.reducedMatrix[j][0] = INF;

    child.path.push_back(j);
    child.level = level;
    child.vertex = j;
    return child;
}

// ----------------------- Solve TSP using Branch and Bound -----------------------
void solveTSP(const vector<vector<int>>& costMatrix, int n) {
    auto start = high_resolution_clock::now();

    // Initialize priority queue (min-heap)
    struct Compare {
        bool operator()(const Node& a, const Node& b) {
            return a.bound > b.bound;
        }
    };
    priority_queue<Node, vector<Node>, Compare> pq;

    // Root node initialization
    Node root;
    root.path = {0};
    root.reducedMatrix = costMatrix;
    root.level = 0;
    root.vertex = 0;
    root.cost = 0;
    root.bound = reduceMatrix(root.reducedMatrix, n);

    pq.push(root);

    int minCost = INF;
    vector<int> finalPath;

    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();

        int i = current.vertex;

        if (current.level == n - 1) {
            current.path.push_back(0); 
            int totalCost = current.cost + costMatrix[i][0];
            if (totalCost < minCost) {
                minCost = totalCost;
                finalPath = current.path;
            }
            continue;
        }

        for (int j = 0; j < n; j++) {
            if (current.reducedMatrix[i][j] != INF) {
                Node child = createNode(current.reducedMatrix, current.path, current.level + 1, i, j, n);
                child.cost = current.cost + costMatrix[i][j];
                child.bound = child.cost + reduceMatrix(child.reducedMatrix, n);

                if (child.bound < minCost)
                    pq.push(child);
            }
        }
    }

    auto end = high_resolution_clock::now();
    double execTime = duration_cast<microseconds>(end - start).count() / 1000.0;

    // Display Results
    cout << "\n================= SwiftShip Route Optimization =================\n";
    cout << "Optimal Delivery Route (City Indexes): ";
    for (int v : finalPath) cout << v << " ";
    cout << "\nMinimum Total Fuel Cost: " << minCost;
    cout << "\nEstimated Delivery Time (km proxy): " << (minCost * 2.75);
    cout << "\nExecution Time: " << fixed << setprecision(3) << execTime << " ms\n";
    cout << "===============================================================\n";
}

// ----------------------- MAIN FUNCTION -----------------------
int main() {
    cout << "=== SwiftShip – Optimal Delivery Route Finder ===\n";
    int n;
    cout << "Enter number of cities: ";
    cin >> n;

    vector<vector<int>> costMatrix(n, vector<int>(n));
    cout << "Enter cost matrix (use 9999999 for no direct route):\n";
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> costMatrix[i][j];

    solveTSP(costMatrix, n);
    return 0;
}
