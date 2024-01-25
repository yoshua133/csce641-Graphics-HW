// DataStructures.h
#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H
#include <iostream>
#include <stdlib.h>
#include <vector>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
// #include <OpenGL/gl.h>
// #include <OpenGL/glu.h>
// #include <GLUT/glut.h>

#else
#include <GL/glut.h>
#endif

#include <map>
#include <set>
#include <deque>
#include <tuple>
#include "../MeshLib_Core/Mesh.h"
#include "../MeshLib_Core/Iterators.h"
#include <cmath>
#include <algorithm>
#include <numeric>
#include <utility>
#include <iostream>
#include <sstream>
#include <string>
#include <time.h>
using namespace std;


// ... MinHeap and edge_vector class definitions ...

extern map<int, int> edge2heap;



class edge_vector{
    public:
        vector<pair<double, Edge *> > error_vec;
        vector<int> index;
        double max = 10000;
        void insert(double error, Edge* edge_ptr) {
            error_vec.push_back(make_pair(error, edge_ptr));
            index.push_back(index.size());
        }
        void sort(){
            std::sort(index.begin(), index.end(), [&](int n1, int n2){ return error_vec[n1].first  < error_vec[n2].first; });
            // { return std::tie(vComponents[n1].x, vComponents[n1].y) <
            //      std::tie(vComponents[n2].x, vComponents[n2].y); }); 
        }
        void updateKey(int i, double k){
            error_vec[i].first = k;
            sort();
        }
        void deleteKey(int i){
            error_vec[i].first = max;
            sort();
        }
        pair<double, Edge*> get(int i) {
            pair<double, Edge*> out = error_vec[index[i]];
            return out;
        }

};

class MinHeap {
private:
    vector< pair<double, Edge *> > heap;
    void heap_swap(int l, int r){
        int l_edge_idx = heap[l].second -> index();
        int r_edge_idx = heap[r].second ->index();
        edge2heap[l_edge_idx] =  r;
        edge2heap[r_edge_idx] =  l;
        swap(heap[l], heap[r]);
    }
    int parent(int i) { return (i - 1) / 2; }
    int left(int i) { return 2 * i + 1; }
    int right(int i) { return 2 * i + 2; }

    void heapifyUp(int i) {
        while (i != 0 && heap[i].first < heap[parent(i)].first) {
            //swap(heap[i], heap[parent(i)]);
            heap_swap(i,parent(i) );
            i = parent(i);
        }
    }

    void heapifyDown(int i) {
        int smallest = i;
        int l = left(i);
        int r = right(i);

        if (l < heap.size() && heap[l].first < heap[smallest].first)
            smallest = l;

        if (r < heap.size() && heap[r].first < heap[smallest].first)
            smallest = r;

        if (smallest != i) {
            //swap(heap[i], heap[smallest]);
            heap_swap(i,smallest );
            heapifyDown(smallest);
        }
    }

public:
    
    MinHeap() {}
    // void build_heap(Mesh * mesh){
    //     for (int i = 0; i < mesh -> numEdges(); i++)
    //     {
    //         Edge * cur_edge = mesh ->indEdge(i);
    //         double err = g_error(cur_edge);
    //         heap.push_back(err, cur_edge);
    //     }
    // }
    vector< pair<double, Edge *> > * hp_vec(){
        return &heap;
    }
    void insert(double error, Edge* edge_ptr) {
        heap.push_back(make_pair(error, edge_ptr));
        heapifyUp(heap.size() - 1);
    }

    void updateKey(int i, double k) {
        double old_key = heap[i].first;
        heap[i].first = k;

        if (k < old_key) {
            heapifyUp(i);
        } else {
            heapifyDown(i);
        }
    }

    pair<double, Edge*> get(int i) {
        pair<double, Edge*> out = heap[i];
        //heap[0] = heap.back();
        //heap.pop_back();
        //heapifyDown(0);
        return out;
    }

    int size() {
        return heap.size();
    }

    bool empty() {
        return heap.empty();
    }

    void print() {
        for (int i = 0; i < heap.size(); i++) {
            cout << "(" << heap[i].first << ", " << heap[i].second->index() << ") ";
        }
        cout << endl;
    }

    void deleteKey(int i) {
        // Find the element to be deleted in the heap.
        double key_to_delete = heap[i].first;

        // Swap the element to be deleted with the last element in the heap.
        int last_index = heap.size() - 1;
        heap_swap(i, last_index);
        pair<double, Edge*> to_delete = heap.back();
        int to_del_edge_idx = to_delete.second->index();
        edge2heap[to_del_edge_idx] = -1;
        // Remove the last element from the heap.
        heap.pop_back();


        // Call heapifyUp or heapifyDown as appropriate.
        if (heap[i].first < key_to_delete) {
            heapifyUp(i);
        } else {
            heapifyDown(i);
        }
    }


};



#endif // DATA_STRUCTURES_H