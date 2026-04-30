
#include <iostream>
#include <vector>
#include <string>
#include <climits>
using namespace std;

#define MAX 20

int graph[MAX][MAX];
string nodes[MAX];
int nodeCount = 0;

int getIndex(string name) {
    for (int i = 0; i < nodeCount; i++) {
        if (nodes[i] == name)
            return i;
    }
    nodes[nodeCount] = name;
    nodeCount++;
    return nodeCount - 1;
}

int minDistance(vector<int>& dist, vector<bool>& visited) {
    int minVal = INT_MAX, idx = -1;
    for (int i = 0; i < nodeCount; i++) {
        if (!visited[i] && dist[i] < minVal) {
            minVal = dist[i];
            idx = i;
        }
    }
    return idx;
}

int dijkstra(int start, int end, vector<int>& parent) {
    vector<int> dist(nodeCount, INT_MAX);
    vector<bool> visited(nodeCount, false);

    parent.assign(nodeCount, -1);
    dist[start] = 0;

    for (int i = 0; i < nodeCount - 1; i++) {
        int u = minDistance(dist, visited);
        if (u == -1) break;

        visited[u] = true;

        for (int v = 0; v < nodeCount; v++) {
            if (!visited[v] && graph[u][v] &&
                dist[u] != INT_MAX &&
                dist[u] + graph[u][v] < dist[v]) {

                dist[v] = dist[u] + graph[u][v];
                parent[v] = u;
            }
        }
    }

    return dist[end];
}

void printSegment(vector<int>& parent, int j) {
    if (parent[j] == -1) {
        cout << nodes[j];
        return;
    }
    printSegment(parent, parent[j]);
    cout << " -> " << nodes[j];
}

int main() {
    int choice;

    for (int i = 0; i < MAX; i++)
        for (int j = 0; j < MAX; j++)
            graph[i][j] = 0;

    while (true) {
        cout << "\n===== MENU =====\n";
        cout << "1. Add Road\n";
        cout << "2. Find Path (Forced via nodes)\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            string u, v;
            int w;
            cout << "Enter source destination weight: ";
            cin >> u >> v >> w;

            int ui = getIndex(u);
            int vi = getIndex(v);

            graph[ui][vi] = w;
            graph[vi][ui] = w;

            cout << "Road added successfully.\n";
        }
        else if (choice == 2) {
            int k;
            cout << "Enter number of intermediate nodes: ";
            cin >> k;

            vector<string> route(k + 2);
            cout << "Enter source, intermediates, destination:\n";
            for (int i = 0; i < k + 2; i++)
                cin >> route[i];

            int totalDist = 0;

            for (int i = 0; i < k + 1; i++) {
                int start = -1, end = -1;

                for (int j = 0; j < nodeCount; j++) {
                    if (nodes[j] == route[i]) start = j;
                    if (nodes[j] == route[i + 1]) end = j;
                }

                if (start == -1 || end == -1) {
                    cout << "Invalid node in path\n";
                    totalDist = -1;
                    break;
                }

                vector<int> parent;
                int dist = dijkstra(start, end, parent);

                if (dist == INT_MAX) {
                    cout << "No path found between "
                         << route[i] << " and " << route[i + 1] << "\n";
                    totalDist = -1;
                    break;
                }

                if (i == 0)
                    printSegment(parent, end);
                else {
                    vector<int> temp;
                    int cur = end;

                    while (cur != -1) {
                        temp.push_back(cur);
                        cur = parent[cur];
                    }

                    for (int x = temp.size() - 2; x >= 0; x--)
                        cout << " -> " << nodes[temp[x]];
                }

                totalDist += dist;
            }

            if (totalDist != -1) {
                cout << "\nTotal Distance: " << totalDist << "\n";
            }
        }
        else if (choice == 3) {
            cout << "Exiting program...\n";
            break;
        }
        else {
            cout << "Invalid choice\n";
        }
    }

    return 0;
}