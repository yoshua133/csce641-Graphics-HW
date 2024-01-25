#include "DataStructure.h"
#include <iostream>
#include <stdlib.h>
#include <vector>

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

char keyControl;
int mouseButton;
Mesh mesh;
std::set< Face * > deleted_faces;
std::set< Edge * > deleted_edges;
std::set< Halfedge * > deleted_halfedges;
std::set< int  > deleted_halfedges_idx;
std::set< Vertex * > deleted_vertices;


map<Vertex *, int> ver2idx;

vector<Point > vertex_normal(1000000);
double x_max,y_max, z_max = -1000;
double x_min,y_min,z_min = 1000;
double x_mean,y_mean,z_mean = 0;


MinHeap h;
edge_vector edge_v;

//vector<Edge * > boundaries; 
vector<Halfedge * > boundaries; 

vector<Point > face_normal(1000000);
vector<Point > vertex_normal(1000000);
vector< vector< Point> > boundaries_loop(100);
std::set< Halfedge *> visitedHEs;
std::set< Halfedge *> visitedHEs2;


int boundary_loop = 0;
int com = 0;
int mousePositionX0 = 0, mousePositionY0 = 0;
string input_name = "buddha1500k"; //"igea_143k";//"MaxPlanck74k"; //"cube";
string object_path = "/Users/david/Library/CloudStorage/OneDrive-SharedLibraries-Onedrive/courses/csce641/HW2_TestMeshes/"+ input_name + ".obj";
//const char* object_path = "/Users/david/Library/CloudStorage/OneDrive-SharedLibraries-Onedrive/courses/csce641/HW2_TestMeshes/cube.obj";
//const char* object_path = "/Users/david/Library/CloudStorage/OneDrive-SharedLibraries-Onedrive/courses/csce641/HW1_TestMeshes/angel.obj";
//const char* object_path = "/Users/david/Library/CloudStorage/OneDrive-SharedLibraries-Onedrive/courses/csce641/HW1_TestMeshes/test.obj";
string output_path = "/Users/david/Library/CloudStorage/OneDrive-SharedLibraries-Onedrive/courses/csce641/HW2_TestMeshes/";

typedef tuple<double ,Edge*, int> tu;
//vector< pair<double, Edge *>  * > edge2error;
//vector< int > edge2heap;





int k = 10;
vector<Mesh * > mesh_vec;

