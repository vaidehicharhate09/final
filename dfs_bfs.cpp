#include <iostream>
#include <atomic>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;
class Graph {
    int V;  
    vector<vector<int>> adj; 

public:
    Graph(int V) : V(V), adj(V) {}
        
    void addEdge(int v, int w) {
        adj[v].push_back(w);
    }

    
    
    void parallelDFSUtil(int v, vector<atomic<bool>>& visited) {
        visited[v] = true;
        cout << v << " ";

        #pragma omp parallel for
        for (int i = 0; i < adj[v].size(); ++i) {
            int n = adj[v][i];
            if (!visited[n])
                parallelDFSUtil(n, visited);
        }
    }

    
    void parallelDFS(int startVertex) {
        vector<atomic<bool>> visited(V);
        for(int i=0; i<V; ++i)
            visited[i] = false;
        parallelDFSUtil(startVertex, visited);
    }

    
    void parallelBFS(int startVertex) {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[startVertex] = true;
        q.push(startVertex);

        while (!q.empty()) {
            int v = q.front();
            q.pop();
            cout << v << " ";

            #pragma omp parallel for
            for (int i = 0; i < adj[v].size(); ++i) {
                int n = adj[v][i];
                if (!visited[n]) {
                    visited[n] = true;
                    q.push(n);
                }
            }
        }
    }
};
