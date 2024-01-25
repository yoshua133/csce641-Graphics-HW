// MeshOperations.cpp
#include "MeshOperations.h"
#include "DataStructure.h"
#include "Variables.h"
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



// Implementations of mesh operation functions like collapse_edge

double distance_error(Edge * e){
    Point source = e -> he(0) ->source()->point();
    Point target = e -> he(0) ->target()->point();
    double length = (source-target).norm();
    return length;
}

void collapse_edge(Edge * e) {

    Vertex * v1 = e ->he(0) ->source();
    Vertex * v2 = e ->he(0) ->target();
    Halfedge* he1 = e -> he(0);
    Halfedge* he2 = e -> he(1);

    // Find the faces that are adjacent to the edge
    Face* f1 = he1->face();
    Face* f2 = he2->face();

    
    Vertex * vs, * vt;
    if (v1 -> index() < v2 -> index()){
        vs = v1;
        vt = v2;
    }
    else{
        vs = v2;
        vt = v1;
    }
    Halfedge * phe1, *phe0;// halfedge between vs and vt
    if (he1 ->target() == vt)
    {
        phe1= he1;
        phe0 = he2;
    }
    else{
        phe1= he2;
        phe0 = he1;
    }
    
    vs -> point() = (vs->point() + vt->point())/2.0;
    // Find the halfedges that point to vs and vt from their adjacent faces
    Halfedge* phe5 = phe1->next();
    Halfedge* phe3 = phe1->prev();
    Halfedge* phe2 = phe0->next();
    Halfedge* phe4 = phe0->prev();

    Halfedge* dhe2 = phe2 -> twin();
    Halfedge* dhe4 = phe4 -> twin();
    Halfedge* dhe3 = phe3 -> twin();
    Halfedge* dhe5 = phe5 -> twin();

    //change all half edge relationship around remove vertex
    phe1 ->target() = vs;
    Halfedge * cur = phe4;
    while(cur != phe1){
        cur -> target() = vs;
        cur->twin() -> source() = vs;
        cur = cur ->ccw_rotate_about_target();

    }

    //set twin relation
    dhe2 -> twin() = dhe4;
    dhe4 -> twin() = dhe2;
    dhe3 -> twin() = dhe5;
    dhe5 -> twin() = dhe3;

    //update left and right vertex's halfedges
    Vertex * vl = phe2-> target();
    Vertex * vr = dhe3-> target();
    if (vl ->he()  == phe2){
        vl ->he() = dhe4;             //cur1 ->ccw_rotate_about_target(); bug is here
    }
    if (vr ->he()  == phe5){
        vr ->he() = dhe3;             //cur2 ->ccw_rotate_about_target();
    }
    //update vs's halfedge
    if (  (vs->he() ==phe0) || (vs->he() ==phe3) ){
        vs ->he() = dhe2; 
    }

    //update remaining hfedges' edge
    dhe5 -> edge() = dhe3 -> edge();
    dhe4 -> edge() = dhe2 -> edge();
    
    // delete faces
    deleted_vertices.insert(vt);
    deleted_faces.insert(f1);
    deleted_faces.insert(f2);
    
    deleted_edges.insert(e);
    deleted_edges.insert(phe4 ->edge());
    deleted_edges.insert(phe5 ->edge()); 
    //deleted_edges.insert(prev2 ->edge());

    deleted_halfedges.insert(phe0);
    deleted_halfedges.insert(phe1);
    deleted_halfedges.insert(phe2);
    deleted_halfedges.insert(phe3);
    deleted_halfedges.insert(phe4);
    deleted_halfedges.insert(phe5);
    deleted_halfedges_idx.insert(phe0->index());
    deleted_halfedges_idx.insert(phe1->index());
    deleted_halfedges_idx.insert(phe2->index());
    deleted_halfedges_idx.insert(phe3->index());
    deleted_halfedges_idx.insert(phe4->index());
    deleted_halfedges_idx.insert(phe5->index());

}



int check_valid(Edge * to_delete_edge){
    Vertex * vs = to_delete_edge ->he(0)->source();
    Vertex * vt = to_delete_edge ->he(0)->target();
    
    std::set< Vertex * > vs_neighbors, vt_neighbors;

    Halfedge * cur = to_delete_edge ->he(0);
    vs_neighbors.insert(cur -> target());
    cur = cur->ccw_rotate_about_source();
    while(cur != to_delete_edge ->he(0)){
        vs_neighbors.insert(cur -> target());
        cur = cur->ccw_rotate_about_source();
    }

    cur = to_delete_edge ->he(1);
    vt_neighbors.insert(cur -> target());
    cur = cur->ccw_rotate_about_source();
    while(cur != to_delete_edge ->he(1)){
        vt_neighbors.insert(cur -> target());
        cur = cur->ccw_rotate_about_source();
    }

    std::set<Vertex *> intersection;
    std::set_intersection(vs_neighbors.begin(), vs_neighbors.end(),
                        vt_neighbors.begin(), vt_neighbors.end(),
                        std::inserter(intersection, intersection.begin()));
    return intersection.size();
}

std::set< Edge * > affected_edges;
void collapse_once_edge(){
    int i = 0;
    pair<double, Edge*> res;
    Edge * to_delete_edge;
    while(true){
        res = h.get(i);
        to_delete_edge = res.second;
        int inters_num = check_valid(to_delete_edge);
        if(inters_num==2){
            break;
        }
        else if(inters_num<2){
            i += 1;
            cout<<i<<" less than 2 intersection"<<endl;
        }
        else{
            i += 1;
        }
    }
    
    affected_edges.clear();
    
    Vertex * v1 = to_delete_edge ->he(0)->source();
    Vertex * v2 = to_delete_edge ->he(0)->target();

    Halfedge * cur = to_delete_edge -> he(0);
    affected_edges.insert(cur -> edge());
    cur = cur ->clw_rotate_about_target();
    while ((cur != to_delete_edge -> he(0) )&& (cur != to_delete_edge -> he(1) ))
    {
        affected_edges.insert(cur -> edge());
        cur = cur ->clw_rotate_about_target();
    }
    
    cur = to_delete_edge -> he(1);
    //affected_edges.insert(cur -> edge());
    cur = cur ->clw_rotate_about_target();
    
    while (   (cur != to_delete_edge -> he(0) )&& (cur != to_delete_edge -> he(1) )    )
    {
        affected_edges.insert(cur -> edge());
        cur = cur ->clw_rotate_about_target();
    }
    //display_affected();
    collapse_edge(to_delete_edge);
    set<Edge * >::iterator itr;
    for (itr = affected_edges.begin(); itr != affected_edges.end(); itr++)
    {
        Edge * to_check = *itr;
        int correspond_heap_idx = edge2heap[to_check->index()];
        if (deleted_edges.find(to_check) != deleted_edges.end()){
            h.deleteKey(correspond_heap_idx);
            continue;
        }
        //if not deleted, then update the error
        double new_err = distance_error(to_check);
        h.updateKey(correspond_heap_idx, new_err);
    }

}

void collapse_once_edge_sort(){
    int i = 0;
    pair<double, Edge*> res;
    Edge * to_delete_edge;
    while(true){
        res = edge_v.get(i);
        to_delete_edge = res.second;
        int inters_num = check_valid(to_delete_edge);
        if(inters_num==2){
            break;
        }
        else if(inters_num<2){
            i += 1;
            cout<<i<<" less than 2 intersection"<<endl;
        }
        else{
            i += 1;
        }
    }
    
    affected_edges.clear();
    
    Vertex * v1 = to_delete_edge ->he(0)->source();
    Vertex * v2 = to_delete_edge ->he(0)->target();

    Halfedge * cur = to_delete_edge -> he(0);
    affected_edges.insert(cur -> edge());
    cur = cur ->clw_rotate_about_target();
    while ((cur != to_delete_edge -> he(0) )&& (cur != to_delete_edge -> he(1) ))
    {
        affected_edges.insert(cur -> edge());
        cur = cur ->clw_rotate_about_target();
    }
    
    cur = to_delete_edge -> he(1);
    //affected_edges.insert(cur -> edge());
    cur = cur ->clw_rotate_about_target();
    
    while (   (cur != to_delete_edge -> he(0) )&& (cur != to_delete_edge -> he(1) )    )
    {
        affected_edges.insert(cur -> edge());
        cur = cur ->clw_rotate_about_target();
    }
    //display_affected();
    collapse_edge(to_delete_edge);
    set<Edge * >::iterator itr;
    for (itr = affected_edges.begin(); itr != affected_edges.end(); itr++)
    {
        Edge * to_check = *itr;
        int correspond_idx = to_check->index();
        if (deleted_edges.find(to_check) != deleted_edges.end()){
            edge_v.deleteKey(correspond_idx);
            continue;
        }
        //if not deleted, then update the error
        double new_err = distance_error(to_check);
        edge_v.updateKey(correspond_idx, new_err);
    }
}
