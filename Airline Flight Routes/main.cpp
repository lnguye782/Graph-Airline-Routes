#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>

struct Route {
    std::string source;
    std::string destination;
};

class Graph {
public:
    void addEdge(const std::string& source, const std::string& destination) {
        adjList[source].insert(destination);
    }

    const std::unordered_map<std::string, std::unordered_set<std::string>>& getAdjList() const {
        return adjList;
    }

private:
    std::unordered_map<std::string, std::unordered_set<std::string>> adjList;
};

Graph readRoutes(const std::string& filename) {
    Graph graph;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string airline, airlineID, source, sourceID, destination, destinationID, codeshare, stops, equipment;

        std::getline(iss, airline, ',');
        std::getline(iss, airlineID, ',');
        std::getline(iss, source, ',');
        std::getline(iss, sourceID, ',');
        std::getline(iss, destination, ',');
        std::getline(iss, destinationID, ',');
        std::getline(iss, codeshare, ',');
        std::getline(iss, stops, ',');
        std::getline(iss, equipment, ',');

        if (!source.empty() && !destination.empty()) {
            graph.addEdge(source, destination);
        }
    }

    return graph;
}

bool isConnected(const Graph& graph) {
    const auto& adjList = graph.getAdjList();
    if (adjList.empty()) return true;

    std::unordered_set<std::string> visited;
    std::queue<std::string> q;
    auto start = adjList.begin()->first;
    q.push(start);
    visited.insert(start);

    while (!q.empty()) {
        auto node = q.front();
        q.pop();

        if (adjList.find(node) == adjList.end()) continue;

        for (const auto& neighbor : adjList.at(node)) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
    }

    return visited.size() == adjList.size();
}

std::vector<std::string> findShortestPath(const Graph& graph, const std::string& start, const std::string& end) {
    std::unordered_map<std::string, std::string> parent;
    std::queue<std::string> q;

    if (graph.getAdjList().find(start) == graph.getAdjList().end()) {
        std::cerr << "Start node " << start << " not found in graph." << std::endl;
        return {};
    }

    if (graph.getAdjList().find(end) == graph.getAdjList().end()) {
        std::cerr << "End node " << end << " not found in graph." << std::endl;
        return {};
    }

    q.push(start);
    parent[start] = "";

    while (!q.empty()) {
        auto node = q.front();
        q.pop();

        if (node == end) break;

        if (graph.getAdjList().find(node) == graph.getAdjList().end()) continue;

        for (const auto& neighbor : graph.getAdjList().at(node)) {
            if (parent.find(neighbor) == parent.end()) {
                parent[neighbor] = node;
                q.push(neighbor);
            }
        }
    }

    std::vector<std::string> path;
    for (auto at = end; at != ""; at = parent[at]) {
        path.push_back(at);
    }
    std::reverse(path.begin(), path.end());

    if (path.front() == start) return path;
    return {}; // Return empty if no path found
}

int main() {
    Graph graph = readRoutes("routes.dat");

    std::string start, end;
    std::cout << "Enter source airport code: ";
    std::cin >> start;
    std::cout << "Enter destination airport code: ";
    std::cin >> end;

    auto path = findShortestPath(graph, start, end);

    if (!path.empty()) {
        std::cout << "Shortest path from " << start << " to " << end << ":\n";
        for (const auto& airport : path) {
            std::cout << airport << " ";
        }
        std::cout << "\nNumber of stops: " << path.size() - 2 << "\n";
    } else {
        std::cout << "No path found from " << start << " to " << end << "\n";
    }

    return 0;
}
