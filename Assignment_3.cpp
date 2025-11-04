// Program: Emergency Relief Supply Distribution using Fractional Knapsack
// Author: Gaurav Biradar
// Roll No: 123B1F011
// Date: 05/08/2025


#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <chrono>
using namespace std;
using namespace std::chrono;

// ------------------- Structure Definition -------------------
struct Item {
    string name;
    double weight;
    double value;
    bool divisible; 
};

// Comparison based on value-to-weight ratio
bool cmp(const Item& a, const Item& b) {
    return (a.value / a.weight) > (b.value / b.weight);
}

// ------------------- Fractional Knapsack Algorithm -------------------
void fractionalKnapsack(vector<Item>& items, double capacity) {
    sort(items.begin(), items.end(), cmp);

    double totalValue = 0.0;
    double currentWeight = 0.0;

    cout << "\n================= Selected Items for Transport =================\n";
    cout << left << setw(15) << "Item"
         << setw(15) << "Weight(kg)"
         << setw(15) << "Value"
         << setw(15) << "Taken(kg)" << "\n";
    cout << "----------------------------------------------------------------\n";

    for (auto& item : items) {
        if (currentWeight >= capacity)
            break; 

        if (currentWeight + item.weight <= capacity) {
            currentWeight += item.weight;
            totalValue += item.value;
            cout << left << setw(15) << item.name
                 << setw(15) << item.weight
                 << setw(15) << item.value
                 << setw(15) << item.weight << "\n";
        } else if (item.divisible) {
            double remain = capacity - currentWeight;
            double fraction = remain / item.weight;
            totalValue += item.value * fraction;
            currentWeight += remain;
            cout << left << setw(15) << item.name
                 << setw(15) << item.weight
                 << setw(15) << item.value
                 << setw(15) << fixed << setprecision(2) << remain << "\n";
            break;
        }
    }

    cout << "----------------------------------------------------------------\n";
    cout << "Total Weight Loaded: " << fixed << setprecision(2) << currentWeight << " kg\n";
    cout << "Maximum Utility Value: " << fixed << setprecision(2) << totalValue << "\n";
    cout << "================================================================\n";
}

// ------------------- Main Function -------------------
int main() {
    cout << "=== Emergency Relief Supply Loading ===\n";

    int n;
    double W;
    cout << "Enter number of relief items: ";
    cin >> n;

    vector<Item> items(n);

    cout << "\nEnter details for each item:\n";
    for (int i = 0; i < n; i++) {
        cout << "\nItem #" << i + 1 << " name: ";
        cin >> items[i].name;
        cout << "Weight (kg): ";
        cin >> items[i].weight;
        cout << "Utility value: ";
        cin >> items[i].value;
        cout << "Divisible (1 for Yes, 0 for No): ";
        cin >> items[i].divisible;
    }

    cout << "\nEnter boat capacity (kg): ";
    cin >> W;

    auto start = high_resolution_clock::now();
    fractionalKnapsack(items, W);
    auto end = high_resolution_clock::now();

    double timeTaken = duration_cast<nanoseconds>(end - start).count() / 1e6;

    cout << "\nExecution Time: " << fixed << setprecision(3) << timeTaken << " ms\n";
    cout << "================================================================\n";

    return 0;
}
