/*
 * main.cpp
 *
 *  Created on: 30 Sep 2021
 *      Author: mndx
 *
 *      Compare running time of Dijkstra's algorithm
 *      using a Fibonacci heap and a binary min heap.
 */

#include <iostream>
#include <time.h>
#include <vector>

#include "bin_heap.hpp"
#include "fib_heap.hpp"
#include "user_types.hpp"

int main(int argc, char* argv[]) {

    //Input parameters
    int s = 31; //Start vertex. The minimum index for vertices is 1
    int n = 2499; //Number of vertices
    int num_edges = 10*n; //Number of edges

    //Create edges
    srand((unsigned) time(NULL));
    std::vector< std::vector<int> > edges;
    for(int i = 0; i < num_edges; ++i) {
        int start_vert = rand() % n + 1;
        int end_vert = rand() % n + 1;
        int weight = rand() % n + 1;

        std::vector<int> edge_elem;
        edge_elem.push_back(start_vert);
        edge_elem.push_back(end_vert);
        edge_elem.push_back(weight);
        edges.push_back(edge_elem);
    }

    //Time results based on Fibonacci heap
    clock_t start_time, end_time;
    double time_fib;
    start_time = clock();

    //Compute distances to nodes from start vertex using a Fibonacci heap
    std::vector<int> results_fib = shortest_reach(n, edges, s);

    end_time = clock();
    time_fib = (double) (end_time - start_time) / CLOCKS_PER_SEC * 1000.0;

    //Time results based on binary heap
    double time_bin;
    start_time = clock();

    //Compute distances to nodes from start vertex using a binary heap
    std::vector<int> results_bin = shortest_reach2(n, edges, s);

    end_time = clock();
    time_bin = (double) (end_time - start_time) / CLOCKS_PER_SEC * 1000.0;

    //Check if results of both methods are the same
    bool results_match = true;
    int size_fib = (int) results_fib.size();
    int size_bin = (int) results_bin.size();
    if(size_fib != size_bin) { results_match = false; }
    for(int i = 0; i < size_fib; ++i) {
        if(results_fib[i] != results_bin[i]) { results_match = false; }
    }

    //Print results
    std::cout << "Execution results:" << std::endl;
    std::cout << "results obtained from both methods match: " << results_match << std::endl;
    std::cout << "timing execution fibonacci heap: " << time_fib << std::endl;
    std::cout << "timing execution binary heap: " << time_bin << std::endl;
    std::cout << std::endl;

    //Print results based on Fibonacci heap
    std::cout << "Shortest distances from start vertex:" << std::endl;
    for(int i = 0; i < size_fib; ++i) {
        std::cout << results_fib[i] << " ";
    }
    std::cout << std::endl;

    //Print results based on binary heap
    for(int i = 0; i < size_bin; ++i) {
        std::cout << results_bin[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "done" << std::endl;

    return 0;
}
