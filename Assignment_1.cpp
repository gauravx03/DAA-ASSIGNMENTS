
// Program: Merge Sort for Customer Orders (Based on Timestamp)
// Author: Gaurav Biradar
// Roll No: 123B1F011
// Date: 08/07/2025


#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <chrono>   
#include <iomanip> 

using namespace std;
using namespace std::chrono;

// Structure for Customer Orders
struct Order {
    int order_id;
    long long timestamp;
    double value;
};

// Merge Function
void merge(vector<Order>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<Order> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i].timestamp <= R[j].timestamp)
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];
}

// Recursive Merge Sort
void mergeSort(vector<Order>& arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

// Generate Random Orders
vector<Order> generateOrders(int n) {
    vector<Order> orders(n);
    srand(time(0));
    for (int i = 0; i < n; i++) {
        orders[i].order_id = i + 1;
        orders[i].timestamp = rand() % 1000000000 + 1;
        orders[i].value = (double)(rand() % 100000) / 100.0;
    }
    return orders;
}

// Print Orders
void printOrders(const vector<Order>& orders, int limit = 10) {
    cout << "------------------------------------------------------------\n";
    cout << "Order ID\tTimestamp\tValue\n";
    cout << "------------------------------------------------------------\n";
    for (int i = 0; i < min(limit, (int)orders.size()); i++) {
        cout << orders[i].order_id << "\t\t" 
             << orders[i].timestamp << "\t" 
             << orders[i].value << "\n";
    }
    cout << "------------------------------------------------------------\n";
}

int main() {
    int n = 20;
    vector<Order> orders = generateOrders(n);

    cout << "\n================ BEFORE SORTING ================\n";
    printOrders(orders);

    auto start = high_resolution_clock::now();
    mergeSort(orders, 0, n - 1);
    auto end = high_resolution_clock::now();

    cout << "\n================ AFTER SORTING ================\n";
    printOrders(orders);

 
    auto duration = duration_cast<nanoseconds>(end - start);
    double ms = duration.count() / 1e6; 

    cout << fixed << setprecision(6);
    cout << "\nTime Taken: " << ms << " ms\n";

    return 0;
}
