// Program: Disaster Relief Resource Allocation using Knapsack Optimization
// Author: Gaurav biradar
// Roll No: 123B1F046
// Date: 23/09/2025


#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>
using namespace std;
using namespace std::chrono;

struct Item {
    string name;
    int weight;
    int utility;
};

// -------------------- Brute Force Approach --------------------
int bruteForceKnapsack(const vector<Item>& items, int W, int n) {
    if (n == 0 || W == 0) return 0;

    if (items[n - 1].weight > W)
        return bruteForceKnapsack(items, W, n - 1);

    int include = items[n - 1].utility + bruteForceKnapsack(items, W - items[n - 1].weight, n - 1);
    int exclude = bruteForceKnapsack(items, W, n - 1);
    return max(include, exclude);
}

// -------------------- Dynamic Programming Approach --------------------
int dpKnapsack(const vector<Item>& items, int W, vector<int>& selected) {
    int N = items.size();
    vector<vector<int>> dp(N + 1, vector<int>(W + 1, 0));

    for (int i = 1; i <= N; ++i) {
        for (int w = 0; w <= W; ++w) {
            if (items[i - 1].weight <= w)
                dp[i][w] = max(dp[i - 1][w], dp[i - 1][w - items[i - 1].weight] + items[i - 1].utility);
            else
                dp[i][w] = dp[i - 1][w];
        }
    }

    int w = W;
    for (int i = N; i > 0; --i) {
        if (dp[i][w] != dp[i - 1][w]) {
            selected.push_back(i - 1);
            w -= items[i - 1].weight;
        }
    }

    return dp[N][W];
}

// -------------------- Greedy Approximation Approach --------------------
bool cmpRatio(const Item& a, const Item& b) {
    double r1 = (double)a.utility / a.weight;
    double r2 = (double)b.utility / b.weight;
    return r1 > r2;
}

double greedyApproximation(vector<Item> items, int W, vector<int>& selected) {
    sort(items.begin(), items.end(), cmpRatio);

    double totalUtility = 0.0;
    double currentWeight = 0.0;

    for (int i = 0; i < items.size(); i++) {
        if (currentWeight + items[i].weight <= W) {
            currentWeight += items[i].weight;
            totalUtility += items[i].utility;
            selected.push_back(i);
        }
    }
    return totalUtility;
}

// -------------------- Utility Display Function --------------------
void displaySelectedItems(const vector<Item>& items, const vector<int>& selected) {
    for (int idx : selected) {
        cout << "- " << items[idx].name
             << " | Weight: " << items[idx].weight
             << " | Utility: " << items[idx].utility << "\n";
    }
}

// -------------------- Main --------------------
int main() {
    cout << "=== Disaster Relief Resource Allocation ===\n";

    int N, W;
    cout << "Enter number of relief items: ";
    cin >> N;
    cout << "Enter truck capacity (kg): ";
    cin >> W;

    vector<Item> items(N);
    cout << "\nEnter details for each item:\n";
    for (int i = 0; i < N; ++i) {
        cout << "Item " << i + 1 << " name: ";
        cin >> items[i].name;
        cout << "Weight (kg): ";
        cin >> items[i].weight;
        cout << "Utility value: ";
        cin >> items[i].utility;
    }

    cout << "\n--- Brute Force Approach ---\n";
    auto start = high_resolution_clock::now();
    int maxUtilityBF = bruteForceKnapsack(items, W, items.size());
    auto end = high_resolution_clock::now();
    double timeBF = duration_cast<microseconds>(end - start).count() / 1000.0;
    cout << "Max Utility: " << maxUtilityBF << "\nExecution Time: " << fixed << setprecision(3) << timeBF << " ms\n";

    cout << "\n--- Dynamic Programming Approach ---\n";
    vector<int> selectedDP;
    start = high_resolution_clock::now();
    int maxUtilityDP = dpKnapsack(items, W, selectedDP);
    end = high_resolution_clock::now();
    double timeDP = duration_cast<microseconds>(end - start).count() / 1000.0;
    cout << "Max Utility (DP): " << maxUtilityDP << "\nExecution Time: " << timeDP << " ms\n";
    cout << "Optimal Set of Items (DP):\n";
    displaySelectedItems(items, selectedDP);

    cout << "\n--- Greedy Approximation Approach ---\n";
    vector<int> selectedGreedy;
    start = high_resolution_clock::now();
    double greedyUtility = greedyApproximation(items, W, selectedGreedy);
    end = high_resolution_clock::now();
    double timeGreedy = duration_cast<microseconds>(end - start).count() / 1000.0;
    cout << "Approx Utility (Greedy): " << greedyUtility << "\nExecution Time: " << timeGreedy << " ms\n";
    cout << "Selected Items (Greedy):\n";
    displaySelectedItems(items, selectedGreedy);

    // Multi-Truck Allocation Simulation
    cout << "\n=== Multi-Truck Allocation Simulation ===\n";
    vector<int> capacities = {200, 220, 240};
    for (int t = 0; t < capacities.size(); ++t) {
        vector<int> selectedTruck;
        int truckUtility = dpKnapsack(items, capacities[t], selectedTruck);
        cout << "\nTruck " << t + 1 << " (Capacity: " << capacities[t] << " kg)\n";
        cout << "Optimal Set of Items (DP):\n";
        displaySelectedItems(items, selectedTruck);
        cout << "Total Utility for Truck " << t + 1 << ": " << truckUtility << "\n";
    }

    cout << "\n--- Performance Summary ---\n";
    cout << "Brute Force Time: " << timeBF << " ms\n";
    cout << "Dynamic Programming Time: " << timeDP << " ms\n";
    cout << "Greedy Approximation Time: " << timeGreedy << " ms\n";

    cout << "\n============================================================\n";
    cout << "Conclusion: Dynamic Programming gives optimal results efficiently,\n";
    cout << "while Greedy provides near-optimal solutions in less time.\n";
    cout << "Brute Force confirms correctness but is computationally expensive.\n";
    cout << "============================================================\n";

    return 0;
}
