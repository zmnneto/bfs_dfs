/*
 * City as Index
 * Road as Edge/Vertex
 */
#include <format>
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>

void static tab();

struct Cities_t {
    std::vector<std::string> citiesNames = {
        "Paulinia", "Campinas", "Sumare", "Valinhos", "Vinhedo",
        "Louveira", "Nova Odessa", "Americana", "Itatiba", "Jundiai"
    };
};

struct RouteData_t {
    std::string origin;
    std::string destin;
    float km;
    std::string road;
};

struct Route_t {
    // Colocamos tudo no vetor, exatamente como você sugeriu
    std::vector<RouteData_t> routes = {
        {"Paulinia", "Campinas", 18.5, "Prof. Zeferino Vaz"},
        {"Paulinia", "Sumare", 12.0, "Rodovia Municipal"},
        {"Campinas", "Valinhos", 10.5, "Francisco von Zuben"},
        {"Campinas", "Hortolandia", 22.0, "Bandeirantes"},
        {"Sumare", "Nova Odessa", 8.0, "Anhanguera"},
        {"Nova Odessa", "Americana", 11.2, "Anhanguera"},
        {"Valinhos", "Vinhedo", 6.5, "Rodovia Municipal"},
        {"Vinhedo", "Louveira", 9.0, "Romildo Prado"},
        {"Louveira", "Jundiai", 16.0, "Anhanguera"},
        {"Campinas", "Itatiba", 32.0, "Dom Pedro I"},
        {"Itatiba", "Morungaba", 18.0, "Eng. Constancio Cintra"},
        {"Jundiai", "Itatiba", 24.0, "Eng. Constancio Cintra"}

    };
};

struct Edge_t {
    int destINDEX;
    float km;
    std::string road;
};

struct Graph_t {
    std::unordered_map<std::string, int> graph;
    std::vector<std::string> citiesNames = {};

    //matri[][] adj && list[][]
    //std::vector<std::vector<std::string, int>> adjacencies = {};
    std::vector<std::vector<Edge_t> > adjList; //[][] Edge_t because we need the struct to calculate that thing.
    std::vector<std::vector<float> > adjMatrix; //[][] float because of km
};

void mkGraph(Graph_t &graph, Cities_t &cities) {
    //using unordered_map (hash map to search the city by the index number)
    int index = 0;
    for (auto &cityName: cities.citiesNames) {
        //"save" the name of cityName on the loop.
        graph.citiesNames.push_back(cityName);
        //save the index
        graph.graph[cityName] = index;
        index++;
    }
    //shit test:
    std::cout << "ID Campinas: " << graph.graph["Campinas"] << std::endl;
    std::cout << "ID graph.index[1]: " << graph.citiesNames[1] << std::endl;
    tab();
}

void GraphMem(struct Cities_t &cities, struct Graph_t &graph, struct Route_t &route, struct Edge_t &edge) {
    //older version buged at this point.
    //I was trying make: graph.adjList.resize(cities.citiesNames.size()); BUT citiesNames[] and adjList[][]
    //I was trying make: graph.adjMatriz.resize(cities.citiesNames.size());
    const int n = cities.citiesNames.size();
    //like the same KKKKKKKKKKKKKK calma filhote, calm down son
    graph.adjList.resize(n);
    //graph.adjMatrix.resize(n);//old version buged graph.adjMatrix.resize(n)
    graph.adjMatrix.assign(n, std::vector<float>(n, 0.0f)); //tudo [][]0.0f antes de input vlaue

    for (auto &r: route.routes) {
        const int u = graph.graph[r.origin]; // before graph.adjMatriz[i] = route.routes[i].road;
        //armazenando em u graph pointer graph on loop r are origin, dps push_back()?
        int v = graph.graph[r.destin];
        //the same to destin

        // te wrong idea: before graph.adjMatriz[i] = route.routes[i].road;
        // can be like:
        //graph.adjL não da pra ser list antes por causa da interceccao vector<vector
        graph.adjMatrix[u][v] = r.km;
        graph.adjMatrix[v][u] = r.km;

        graph.adjList[u].push_back({v, r.km, r.road});
        // bug version (v), but the system has (v, km, road.) make it better!
        graph.adjList[v].push_back({u, r.km, r.road}); // bug v.2, if [v], push[u] why? ORIGIN -> DEST DÃÃ!!!!!!

        //shit test: need \t?
        std::cout << "Origem: " << graph.graph[r.origin] <<
                " | Destino: " << graph.graph[r.destin] <<
                " | km: " << graph.adjMatrix[u][v] << std::endl;

        //adjList[u] seriam os nomes das cidades, corret? for para isso e print e sai.
    }
    tab();
}

//neighbor first
void bfs(const Graph_t &graph, const std::string &StartCity) {
    // -1 por seguranca
    int start = -1;
    for (size_t i = 0; i < graph.citiesNames.size(); i++) {
        //not buged but its fine to knowest errors
        if (StartCity == graph.citiesNames[i]) {
            start = static_cast<int>(i); //convertion of type
            break;
        }
    }
    if (start == -1) {
        std::cout << "No cities found" << std::endl;
        tab();
        return;
    }
    //visited? yes or no?
    std::vector<bool> visited(graph.citiesNames.size(), false);
    //FIFO
    std::vector<int> queue; //yes, #include<queue> fuck that bull shit! kkkkkkk
    //we have start, startcity, the bool, creation of queue, but, we need move the first pointer
    int head = 0; //?? like a snake game?

    //start[visited] =  true; //buged.
    visited[start] = true; //if (!start == -1)
    queue.push_back(start); //add start on queue


    std::cout << "Init BFS:" << "\nStart city: " << StartCity << std::endl;

    //fila tem 1 elemento (start) ->[] preciso passar ao proximo
    //cry buged: while (!queue.size() == route.routes.size()) {
    while (head < queue.size()) {
        //head a cabeça da cobra 0 queue já tem 1 elemento, então inicia menor. agr pos
        //the id of the now city
        int u = queue[head];
        //advance pointer
        head++;

        //buged: std::cout << "\nStart city: " << StartCity << std::endl;
        std::cout << graph.citiesNames[u] << "->";

        //buged: i < graph.citiesNames.size()
        for (size_t i = 0; i < graph.adjList[u].size(); i++) {
            // the vector created to EDGE!
            //neighbor adj.
            int neighbor = graph.adjList[u][i].destINDEX; // -> go to?
            //if (visited[neighbor]) continue; //buged how to know if it was visited?
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                queue.push_back(neighbor);
            }
        }
    }
    std::cout << "\nFinish BFS.\n" << std::endl;
    tab();
}


void recursive_dfs(const Graph_t &graph, int u, std::vector<bool> &visited) {
    visited[u] = true;
    std::cout << graph.citiesNames[u] << "->";

    for (size_t i = 0; i < graph.adjList[u].size(); i++) {
        int neighbor = graph.adjList[u][i].destINDEX;

        if (!visited[neighbor]) {
            recursive_dfs(graph, neighbor, visited);
        }
    }
}

//recursive, down to up
void dfs(const Graph_t &graph, const std::string &StartCity) {
    int start = -1;
    for (size_t i = 0; i < graph.citiesNames.size(); i++) {
        //buged if (StartCity == graph.citiesNames[i]) {
        if (graph.citiesNames[i] == StartCity) {
            start = static_cast<int>(i);
            break;
        }
    }
    if (start == -1) {
        std::cout << "No cities found" << std::endl;
        return;
    }
    std::cout << "Init DFS:" << "\nStart city: " << StartCity << std::endl;
    //buged: std::vector<bool> visited;
    std::vector<bool> visited(graph.citiesNames.size(), false);

    recursive_dfs(graph, start, visited);
}

int main(int argc, char *argv[]) {
    struct Graph_t graph;
    struct Cities_t cities;
    struct Route_t route;
    struct Edge_t edge;

    std::string StartCity = "Paulinia";

    mkGraph(graph, cities);
    GraphMem(cities, graph, route, edge);
    bfs(graph, StartCity);
    dfs(graph, StartCity);
}


void static tab() {
    for (int i = 0; i < 11; i++) {
        std::cout << "-";
    }
    std::cout << std::endl;
}
/*
 * City as Index
 * Road as Edge/Vertex
 */
#include <format>
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>

void static tab();

struct Cities_t {
    std::vector<std::string> citiesNames = {
        "Paulinia", "Campinas", "Sumare", "Valinhos", "Vinhedo",
        "Louveira", "Nova Odessa", "Americana", "Itatiba", "Jundiai"
    };
};

struct RouteData_t {
    std::string origin;
    std::string destin;
    float km;
    std::string road;
};

struct Route_t {
    // Colocamos tudo no vetor, exatamente como você sugeriu
    std::vector<RouteData_t> routes = {
        {"Paulinia", "Campinas", 18.5, "Prof. Zeferino Vaz"},
        {"Paulinia", "Sumare", 12.0, "Rodovia Municipal"},
        {"Campinas", "Valinhos", 10.5, "Francisco von Zuben"},
        {"Campinas", "Hortolandia", 22.0, "Bandeirantes"},
        {"Sumare", "Nova Odessa", 8.0, "Anhanguera"},
        {"Nova Odessa", "Americana", 11.2, "Anhanguera"},
        {"Valinhos", "Vinhedo", 6.5, "Rodovia Municipal"},
        {"Vinhedo", "Louveira", 9.0, "Romildo Prado"},
        {"Louveira", "Jundiai", 16.0, "Anhanguera"},
        {"Campinas", "Itatiba", 32.0, "Dom Pedro I"},
        {"Itatiba", "Morungaba", 18.0, "Eng. Constancio Cintra"},
        {"Jundiai", "Itatiba", 24.0, "Eng. Constancio Cintra"}

    };
};

struct Edge_t {
    int destINDEX;
    float km;
    std::string road;
};

struct Graph_t {
    std::unordered_map<std::string, int> graph;
    std::vector<std::string> citiesNames = {};

    //matri[][] adj && list[][]
    //std::vector<std::vector<std::string, int>> adjacencies = {};
    std::vector<std::vector<Edge_t> > adjList; //[][] Edge_t because we need the struct to calculate that thing.
    std::vector<std::vector<float> > adjMatrix; //[][] float because of km
};

void mkGraph(Graph_t &graph, Cities_t &cities) {
    //using unordered_map (hash map to search the city by the index number)
    int index = 0;
    for (auto &cityName: cities.citiesNames) {
        //"save" the name of cityName on the loop.
        graph.citiesNames.push_back(cityName);
        //save the index
        graph.graph[cityName] = index;
        index++;
    }
    //shit test:
    std::cout << "ID Campinas: " << graph.graph["Campinas"] << std::endl;
    std::cout << "ID graph.index[1]: " << graph.citiesNames[1] << std::endl;
    tab();
}

void GraphMem(struct Cities_t &cities, struct Graph_t &graph, struct Route_t &route, struct Edge_t &edge) {
    //older version buged at this point.
    //I was trying make: graph.adjList.resize(cities.citiesNames.size()); BUT citiesNames[] and adjList[][]
    //I was trying make: graph.adjMatriz.resize(cities.citiesNames.size());
    const int n = cities.citiesNames.size();
    //like the same KKKKKKKKKKKKKK calma filhote, calm down son
    graph.adjList.resize(n);
    //graph.adjMatrix.resize(n);//old version buged graph.adjMatrix.resize(n)
    graph.adjMatrix.assign(n, std::vector<float>(n, 0.0f)); //tudo [][]0.0f antes de input vlaue

    for (auto &r: route.routes) {
        const int u = graph.graph[r.origin]; // before graph.adjMatriz[i] = route.routes[i].road;
        //armazenando em u graph pointer graph on loop r are origin, dps push_back()?
        int v = graph.graph[r.destin];
        //the same to destin

        // te wrong idea: before graph.adjMatriz[i] = route.routes[i].road;
        // can be like:
        //graph.adjL não da pra ser list antes por causa da interceccao vector<vector
        graph.adjMatrix[u][v] = r.km;
        graph.adjMatrix[v][u] = r.km;

        graph.adjList[u].push_back({v, r.km, r.road});
        // bug version (v), but the system has (v, km, road.) make it better!
        graph.adjList[v].push_back({u, r.km, r.road}); // bug v.2, if [v], push[u] why? ORIGIN -> DEST DÃÃ!!!!!!

        //shit test: need \t?
        std::cout << "Origem: " << graph.graph[r.origin] <<
                " | Destino: " << graph.graph[r.destin] <<
                " | km: " << graph.adjMatrix[u][v] << std::endl;

        //adjList[u] seriam os nomes das cidades, corret? for para isso e print e sai.
    }
    tab();
}

//neighbor first
void bfs(const Graph_t& graph, const std::string &StartCity) {
    // -1 por seguranca
    int start = -1;
    for (size_t i = 0; i < graph.citiesNames.size(); i++) {
        //not buged but its fine to knowest errors
        if (StartCity == graph.citiesNames[i]) {
            start = static_cast<int>(i); //convertion of type
            break;
        }
    }
    if (start == -1) {
        std::cout << "No cities found" << std::endl;
        tab();
        return;
    }
    //visited? yes or no?
    std::vector<bool> visited(graph.citiesNames.size(), false);
    //FIFO
    std::vector<int> queue; //yes, #include<queue> fuck that bull shit! kkkkkkk
    //we have start, startcity, the bool, creation of queue, but, we need move the first pointer
    int head = 0; //?? like a snake game?

    //start[visited] =  true; //buged.
    visited[start] = true; //if (!start == -1)
    queue.push_back(start); //add start on queue


    std::cout << "Init BFS:" << "\nStart city: " << StartCity << std::endl;

    //fila tem 1 elemento (start) ->[] preciso passar ao proximo
    //cry buged: while (!queue.size() == route.routes.size()) {
    while (head < queue.size()) { //head a cabeça da cobra 0 queue já tem 1 elemento, então inicia menor. agr pos
        //the id of the now city
        int u = queue[head];
        //advance pointer
        head++;

       //buged: std::cout << "\nStart city: " << StartCity << std::endl;
        std::cout << graph.citiesNames[u] << "->";

        //buged: i < graph.citiesNames.size()
        for (size_t i = 0; i < graph.adjList[u].size(); i++) { // the vector created to EDGE!
            //neighbor adj.
            int neighbor = graph.adjList[u][i].destINDEX; // -> go to?
            //if (visited[neighbor]) continue; //buged how to know if it was visited?
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                queue.push_back(neighbor);
            }
        }
    }
    std::cout << "\nFinish BFS.\n"<< std::endl;
    tab();

}


void recursive_dfs(const Graph_t& graph, int u, std::vector<bool>& visited) {
    visited[u] = true;
    std::cout << graph.citiesNames[u] << "->";

    for (size_t i = 0; i < graph.adjList[u].size(); i++) {
        int neighbor = graph.adjList[u][i].destINDEX;

        if (!visited[neighbor]) {
            recursive_dfs(graph, neighbor, visited);
        }
    }
}

//recursive, down to up
void dfs(const Graph_t& graph, const std::string &StartCity) {
    int start = -1;
    for (size_t i = 0; i < graph.citiesNames.size(); i++) {
       //buged if (StartCity == graph.citiesNames[i]) {
        if (graph.citiesNames[i] == StartCity) {
            start = static_cast<int>(i);
            break;
        }
    }
    if (start == -1) {
        std::cout << "No cities found" << std::endl;
        return;
    }
    std::cout << "Init DFS:" << "\nStart city: " << StartCity << std::endl;
    //buged: std::vector<bool> visited;
    std::vector<bool> visited(graph.citiesNames.size(), false);

    recursive_dfs(graph, start, visited);

}

int main(int argc, char *argv[]) {
    struct Graph_t graph;
    struct Cities_t cities;
    struct Route_t route;
    struct Edge_t edge;

    std::string StartCity = "Paulinia";

    mkGraph(graph, cities);
    GraphMem(cities, graph, route, edge);
    bfs(graph, StartCity);
    dfs(graph, StartCity);
}


void static tab() {
    for (int i = 0; i < 11; i++) {
        std::cout << "-";
    }
    std::cout << std::endl;
}
