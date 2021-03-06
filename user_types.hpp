/*
 * user_types.hpp
 *
 *  Created on: 7 Oct 2021
 *      Author: mndx
 */

#ifndef USER_TYPES_HPP_
#define USER_TYPES_HPP_

#include <vector>

const int INF = 3e+8;

typedef struct FibHeapProperties {
    bool deg_is_num_child;
    int num_nodes;
} fib_props;

typedef struct Node {
    Node* left;
    Node* right;
    Node* p;
    Node* child;

    std::vector<int> adj_nodes;

    int key;
    int degree;
    int index;
    bool mark;

    int index_og;
    Node* pi;
} node;

class FibHeap {
public:
    int n;
    node* min;
    FibHeap() { min = NULL; n = 0; }
};

class Heap {
private:
    int heap_size;
    int size_array;
    node** A;
    node** heap_ref;
    int* element_map;

    int parent(int i);
    int left(int i);
    int right(int i);
    void min_heapify(node* A[], int i);

public:
    Heap(int size);
    ~Heap();

    void set_heap(node* B[]);
    int get_heap_size();
    node* heap_extract_min();
    void heap_decrease_key(int index, double key);
    void build_min_heap();

    node* get_heap_element(int index);
    int get_root_index();
    int get_heap_index(int index);
};

#endif /* USER_TYPES_HPP_ */
