
// Program: University Timetable Scheduling using Graph Coloring
// Author: Gaurav Biradar
// Roll No: 123B1F011
// Date: 30/09/2025


#include <iostream>
#include <vector>
#include <limits>
#include <iomanip>
#include <chrono>
using namespace std;
using namespace std::chrono;

// ============================================================================
// CLASS: ExamScheduler
// ============================================================================
class ExamScheduler {
private:
    int numCourses;
    vector<vector<int>> adj; 
    vector<int> color;      
    vector<int> courseSize;  

public:
    ExamScheduler(int n) : numCourses(n) {
        adj.resize(n);
        color.assign(n, -1);
        courseSize.assign(n, 0);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void setCourseSize(int course, int size) {
        courseSize[course] = size;
    }

    // ---------------- GREEDY COLORING ----------------
    int greedyColoring() {
        color.assign(numCourses, -1);
        color[0] = 0;
        vector<bool> available(numCourses, false);

        for (int u = 1; u < numCourses; u++) {
            fill(available.begin(), available.end(), false);
            for (int v : adj[u])
                if (color[v] != -1)
                    available[color[v]] = true;

            int cr;
            for (cr = 0; cr < numCourses; cr++)
                if (!available[cr])
                    break;
            color[u] = cr;
        }
        return *max_element(color.begin(), color.end()) + 1;
    }

    // ---------------- WELSH–POWELL COLORING ----------------
    int welshPowellColoring() {
        color.assign(numCourses, -1);
        vector<int> order(numCourses);
        iota(order.begin(), order.end(), 0);

        sort(order.begin(), order.end(), [&](int a, int b) {
            return adj[a].size() > adj[b].size();
        });

        int numColors = 0;
        for (int u : order) {
            vector<bool> used(numCourses, false);
            for (int v : adj[u])
                if (color[v] != -1)
                    used[color[v]] = true;

            int c;
            for (c = 0; c < numCourses; c++)
                if (!used[c])
                    break;
            color[u] = c;
            numColors = max(numColors, c + 1);
        }
        return numColors;
    }

    // ---------------- DSATUR COLORING ----------------
    int dsaturColoring() {
        color.assign(numCourses, -1);
        vector<int> degree(numCourses);
        vector<unordered_set<int>> neighborColors(numCourses);

        for (int i = 0; i < numCourses; i++)
            degree[i] = adj[i].size();

        int coloredCount = 0;
        int u = max_element(degree.begin(), degree.end()) - degree.begin();
        color[u] = 0;
        coloredCount++;

        while (coloredCount < numCourses) {
            for (int v : adj[u])
                if (color[v] != -1)
                    neighborColors[v].insert(color[u]);

            int next = -1, maxSat = -1, maxDeg = -1;
            for (int i = 0; i < numCourses; i++) {
                if (color[i] != -1) continue;
                int sat = neighborColors[i].size();
                if (sat > maxSat || (sat == maxSat && degree[i] > maxDeg)) {
                    maxSat = sat;
                    maxDeg = degree[i];
                    next = i;
                }
            }

            vector<bool> used(numCourses, false);
            for (int v : adj[next])
                if (color[v] != -1)
                    used[color[v]] = true;

            int c;
            for (c = 0; c < numCourses; c++)
                if (!used[c])
                    break;
            color[next] = c;
            u = next;
            coloredCount++;
        }
        return *max_element(color.begin(), color.end()) + 1;
    }

    // ---------------- ROOM ALLOCATION ----------------
    void allocateRooms(int roomCapacity) {
        int numColors = *max_element(color.begin(), color.end()) + 1;
        vector<vector<int>> slotCourses(numColors);

        for (int i = 0; i < numCourses; i++)
            slotCourses[color[i]].push_back(i);

        cout << "\n=== Room Allocation per Exam Slot ===\n";
        for (int s = 0; s < numColors; s++) {
            cout << "\nExam Slot " << s << ":\n";
            int room = 1, usedCapacity = 0;
            for (int course : slotCourses[s]) {
                if (usedCapacity + courseSize[course] > roomCapacity) {
                    room++;
                    usedCapacity = 0;
                }
                usedCapacity += courseSize[course];
                cout << "  Room " << room << " -> Course " << course
                     << " (" << courseSize[course] << " students)\n";
            }
        }
    }

    // ---------------- DISPLAY COLORING ----------------
    void displayColoringResult(const string& algoName, double timeMs) {
        cout << "\n--- " << algoName << " Results ---\n";
        cout << "Exam Slots (Colors Assigned): ";
        for (int i = 0; i < numCourses; i++)
            cout << color[i] << " ";
        cout << "\nTotal Exam Slots Required: "
             << *max_element(color.begin(), color.end()) + 1 << "\n";
        cout << "Execution Time: " << fixed << setprecision(3) << timeMs << " ms\n";
    }
};

// ============================================================================
// DRIVER FUNCTION
// ============================================================================
int main() {
    int n = 6; 
    ExamScheduler scheduler(n);

    scheduler.addEdge(0, 1);
    scheduler.addEdge(0, 2);
    scheduler.addEdge(1, 2);
    scheduler.addEdge(1, 3);
    scheduler.addEdge(3, 4);
    scheduler.addEdge(4, 5);

    scheduler.setCourseSize(0, 80);
    scheduler.setCourseSize(1, 50);
    scheduler.setCourseSize(2, 40);
    scheduler.setCourseSize(3, 60);
    scheduler.setCourseSize(4, 90);
    scheduler.setCourseSize(5, 30);

    // ---------------- GREEDY ----------------
    auto start = high_resolution_clock::now();
    int greedySlots = scheduler.greedyColoring();
    auto end = high_resolution_clock::now();
    double greedyTime = duration_cast<microseconds>(end - start).count() / 1000.0;
    scheduler.displayColoringResult("Greedy Coloring", greedyTime);

    // ---------------- WELSH–POWELL ----------------
    start = high_resolution_clock::now();
    int wpSlots = scheduler.welshPowellColoring();
    end = high_resolution_clock::now();
    double wpTime = duration_cast<microseconds>(end - start).count() / 1000.0;
    scheduler.displayColoringResult("Welsh–Powell Coloring", wpTime);

    // ---------------- DSATUR ----------------
    start = high_resolution_clock::now();
    int dsaturSlots = scheduler.dsaturColoring();
    end = high_resolution_clock::now();
    double dsaturTime = duration_cast<microseconds>(end - start).count() / 1000.0;
    scheduler.displayColoringResult("DSATUR Coloring", dsaturTime);

    // Room allocation
    scheduler.allocateRooms(100);

    cout << "\n============================================================\n";
    cout << "Performance Summary:\n";
    cout << "Greedy Time: " << greedyTime << " ms (" << greedySlots << " slots)\n";
    cout << "Welsh–Powell Time: " << wpTime << " ms (" << wpSlots << " slots)\n";
    cout << "DSATUR Time: " << dsaturTime << " ms (" << dsaturSlots << " slots)\n";
    cout << "============================================================\n";
    cout << "Code Execution Successful ✅\n";
    cout << "============================================================\n";
    return 0;
}
