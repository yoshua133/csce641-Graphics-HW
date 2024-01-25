// MeshOperations.h
#ifndef MESH_OPERATIONS_H
#define MESH_OPERATIONS_H

#include "../MeshLib_Core/Mesh.h"
#include "../MeshLib_Core/Iterators.h"

#include "Variables.h"
double distance_error(Edge * e);
void collapse_edge(Edge* e);
int check_valid(Edge* to_delete_edge);
void collapse_once_edge();
void collapse_once_edge_sort();

#endif // MESH_OPERATIONS_H
