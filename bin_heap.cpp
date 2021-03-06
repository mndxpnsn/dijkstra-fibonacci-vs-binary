/*
 * bin_heap.cpp
 *
 *  Created on: 7 Oct 2021
 *      Author: mndx
 */

#include <iostream>
#include <math.h>
#include <vector>

#include "memory.hpp"
#include "user_types.hpp"

Heap::Heap(int size) {
    heap_size = size;
    A = new node*[size+1];
    heap_ref = new node*[size+1];
    element_map = new int[size+1];
    size_array = size + 1;
    element_map[0] = 0;
    for(int i = 1; i <= heap_size; ++i) {
        element_map[i] = i;
        A[i] = new node;
        A[i]->pi = NULL;

    }
}

Heap::~Heap() {
    delete [] A;
    delete [] heap_ref;
    delete [] element_map;
}

int Heap::parent(int i) {
    return i/2;
}

int Heap::left(int i) {
    return 2*i;
}

int Heap::right(int i) {
    return 2*i + 1;
}

node* Heap::get_heap_element(int node_index) {
    return heap_ref[node_index];
}

int Heap::get_heap_index(int node_index) {
    int index_in_heap = element_map[node_index];
    return index_in_heap;
}

int Heap::get_root_index() {
    return A[1]->index;
}

void Heap::min_heapify(node* A[], int i) {
    int l, r, smallest;
    l = Heap::left(i);
    r = Heap::right(i);
    if(l < heap_size + 1 && A[l]->key < A[i]->key) {
        smallest = l;
    }
    else {
        smallest = i;
    }
    if(r < heap_size + 1 && A[r]->key < A[smallest]->key) {
        smallest = r;
    }
    if(smallest != i) {
        node* dummy;
        dummy = A[i];

        element_map[A[smallest]->index] = i;
        element_map[A[i]->index] = smallest;

        A[i] = A[smallest];
        A[smallest] = dummy;

        Heap::min_heapify(A, smallest);
    }
}

void Heap::build_min_heap() {
    for(int i = heap_size/2; i > 0; --i) {
        Heap::min_heapify(A, i);
    }
}

void Heap::set_heap(node* B[]) {
    for(int i = 1; i < heap_size + 1; ++i) {
        A[i] = B[i];
        heap_ref[i] = A[i];
    }
}

int Heap::get_heap_size() {
    return heap_size;
}

node* Heap::heap_extract_min() {

    if(heap_size < 1) {
        std::cout << "heap size is less than 1" << std::endl;
    }
    node* min = A[1];

    element_map[A[heap_size]->index] = 1;
    A[1] = A[heap_size];
    heap_size = heap_size - 1;

    Heap::min_heapify(A, 1);

    return min;
}

void Heap::heap_decrease_key(int index, double key) {
    if(key > A[index]->key) {
        printf("new key is larger than current key\n");
    }
    else {
        A[index]->key = key;
        while(index > 1 && A[parent(index)]->key > A[index]->key) {
            element_map[A[index]->index] = parent(index);
            element_map[A[parent(index)]->index] = index;

            node* dummy = A[index];
            A[index] = A[parent(index)];
            A[parent(index)] = dummy;

            index = parent(index);
        }
    }
}

void relax(node* u, node* v, int** w, Heap* heap) {

    int index_in_heap = heap->get_heap_index(v->index);
    if(v->key > u->key + w[u->index][v->index]) {
        int weight = u->key + w[u->index][v->index];
        heap->heap_decrease_key(index_in_heap, weight);
        v->pi = u;
    }
}

int map_index2(int n, int index, int s) {
    int r;

    if(index >= s) { r = index - s + 1; }
    else { r = n - s + index + 1; }

    return r;
}

int map_inverse2(int n, int index, int s) {
    int r;

    r = s + index - 1;
    if(r > n) {
        r = r - n;
    }

    return r;
}

void set_weight_and_heap_refs(int size_graph,
                              std::vector< std::vector<int> > &edges,
                              int s,
                              Heap& min_heap,
                              int** weight_mat,
                              node** node_refs) {

    //Initialize node references
    for(int i = 1; i < size_graph + 1; ++i) {
        node_refs[i] = new node;
        node_refs[i]->key = INF;
        node_refs[i]->pi = NULL;
        node_refs[i]->index = i;
        node_refs[i]->index_og = map_inverse2(size_graph, i, s);
    }

    //Initializing start vertex
    node_refs[1]->key = 0;

    //Set and build heap
    min_heap.set_heap(node_refs);
    min_heap.build_min_heap();

    //Set weight matrix
    int num_edges = (int) edges.size();
    for(int i = 0; i < num_edges; ++i) {
        int start_index = edges[i][0];
        int end_index = edges[i][1];
        int weight = edges[i][2];

        int start = map_index2(size_graph, start_index, s);
        int end = map_index2(size_graph, end_index, s);

        node_refs[start]->adj_nodes.push_back(end);
        node_refs[end]->adj_nodes.push_back(start);

        weight_mat[start][end] = weight;
        weight_mat[end][start] = weight;
    }

    //Traverse edges again to pick minimum weights
    for(int i = 0; i < num_edges; ++i) {
        int start_index = edges[i][0];
        int end_index = edges[i][1];
        int weight = edges[i][2];

        int start = map_index2(size_graph, start_index, s);
        int end = map_index2(size_graph, end_index, s);

        if(weight_mat[start][end] >= weight) {
            weight_mat[start][end] = weight;
            weight_mat[end][start] = weight;
        }
    }
}

void dijkstra(Heap* min_heap, int** weight_mat) {

    //Perform Dijkstra's algorithm
    int heap_size = min_heap->get_heap_size();
    while(heap_size > 0) {

        node* u = min_heap->heap_extract_min();
        heap_size = min_heap->get_heap_size();

        int num_adj_nodes = (int) u->adj_nodes.size();
        for(int i = 0; i < num_adj_nodes; ++i) {
            int it = u->adj_nodes[i];
            node* v = min_heap->get_heap_element(it);
            relax(u, v, weight_mat, min_heap);
        }
    }
}

void reorder_results_bin(int n, int s, node** node_refs, std::vector<int>& results) {

    for(int i = 1; i <= n; ++i) {
        int index = map_index2(n, i, s);
        if(node_refs[index]->index_og != s) {
            int key = node_refs[index]->key;
            if(key == INF) { key = -1; }
            results.push_back(key);
        }
    }
}

std::vector<int> shortest_reach2(int n, std::vector< std::vector<int> > &edges, int s) {

    std::vector<int> results;

    //Initialize weight and adjacency matrices and binary min heap
    Heap min_heap(n);
    node** node_refs = new node*[n + 1];
    int** weight_mat = int2D(n + 1);

    //Populate weight matrix and initialize heap and heap references
    set_weight_and_heap_refs(n, edges, s, min_heap, weight_mat, node_refs);

    //Perform Dijkstra's algorithm
    dijkstra(&min_heap, weight_mat);

    //Reorder results
    reorder_results_bin(n, s, node_refs, results);

    //Deallocate memory
    free_int2D(weight_mat, n + 1);
    free_node_ref_bin(node_refs, n + 1);

    return results;
}
