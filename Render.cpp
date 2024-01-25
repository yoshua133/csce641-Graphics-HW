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
#include "Utilities.h"
#include "MeshOperations.h"
#include "DataStructure.h"
#include "Variables.h"

#include <cmath>
#include <algorithm>
#include <numeric>
#include <utility>
#include <iostream>
#include <sstream>
#include <string>
#include <time.h>
using namespace std;




void draw_string(int height, string str, int num){
    glWindowPos2i(10, height);
    string str_num = to_string(num);
    string result = str + str_num;
    for (int i = 0; i < result.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, result[i]);
    }   
}
void draw_string_on3d(double x,double y, double z, string str) {
    //glClear(GL_COLOR_BUFFER_BIT);
    glColor3f (1.0, 0.0, 0.0);
    glRasterPos3f(x,y,z);
    for (int i = 0; i < str.size(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
    }
}
void draw_string_on3d_s(double x,double y, double z, string str) { 
    glColor3f (1.0, 0.0, 0.0);
    glRasterPos3f(x,y,z);
    for (int i = 0; i < str.size(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, str[i]);
    }

}
//Initializes 3D rendering
void initRendering() {
	glClearColor(0, 0, 0, 1);
	keyControl = 0;
    GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 }; 
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 }; 
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 }; 
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient); 
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse); 
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular); 

    GLfloat light_position[] = { 1.0, 1.0, 1.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    // //glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    //Define Material Properties for the Objects in the Scene
    GLfloat mat_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_diffuse[] = { 0.0, 0.0, 1.0, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 10.0 };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glEnable(GL_LIGHTING);
	
	////Remember to enable the light you just defined 
	glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable( GL_COLOR_MATERIAL );
}


//Called when the window is resized
void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
}

float obj2_angle = 0.0f;

float obj1_angle_x = 0.0f;
float obj1_angle_y = 0.0f;

float obj1_trans[2] = { 0.0f, 0.0f };
float camera_zoom = 0.0f;
char render_mode = 'n';
bool dis_line = false;



void update(int value) {
	obj2_angle += 2.0f;
	if (obj2_angle > 360) {
		obj2_angle -= 360;
	}
	glutPostRedisplay();
	glutTimerFunc(150, update, 0);
}

void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
	case 'r': 
		keyControl = 'r';
        obj1_angle_x = 0.0f;
        obj1_angle_y = 0.0f;
        obj1_trans[0] = 0;
        obj1_trans[1] = 0;
        camera_zoom = 0.0f;

		break;
	case 'a': 
		keyControl = 'a';
		break;
	case 't':
		keyControl = 't';
		break;
	case 'z':
		keyControl = 'z';
		break;
    case 'f':
		render_mode = 'f';
		break;
    case 'v':
		render_mode = 'v';
		break;
    case 'l':
		dis_line= true;
		break;
    case 'n':
		dis_line= false;
		break;
	case 27: //Escape key
		exit(0);
	}
}
void mouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		mouseButton = GLUT_LEFT_BUTTON;
	else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
		mouseButton = GLUT_MIDDLE_BUTTON;
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		mouseButton = GLUT_RIGHT_BUTTON;
	mousePositionX0 = x;
	mousePositionY0 = y;
	return;
}

void mouseMove(int x, int y)
{
	float frictionFactor = 0.02f;  // just a scaling factor to make the mouse moving not too sensitive
	/* rotation*/
	if (mouseButton == GLUT_LEFT_BUTTON)
	{//Rotation
		if (keyControl == 'a'){
			int delta_x = x - mousePositionX0;
			int delta_y = y - mousePositionY0;
			obj1_angle_y += delta_x;
			obj1_angle_x += delta_y;
		}
		else if (keyControl == 't'){
			obj1_trans[0] += frictionFactor * (x - mousePositionX0);
			obj1_trans[1] += frictionFactor * (mousePositionY0 - y);
		}
		else if (keyControl == 'z'){
			camera_zoom += frictionFactor * (y - mousePositionY0);
		}
	}

	mousePositionX0 = x;
	mousePositionY0 = y;
	glutPostRedisplay();
}


//Draws the 3D scene
void drawScene() {
	
    // glEnable(GL_CULL_FACE); 

    glShadeModel(GL_SMOOTH);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 0, 0, 0, -1, 0, 1, 0);
    //gluLookAt(0, 0, 0, x_mean, y_mean, -z_mean-2, 0, 1, 0);
	glTranslatef(0.0f, 0.0f, -2 - 1.5*y_max);
    
    //xyz axis
    // glBegin(GL_LINES);
    //     //glTranslatef(-x_mean, -y_mean, -z_mean);
    //     glColor3f (1.0, 0.0, 0.0);
    //     glVertex3f(0.0, 0.0, 0.0);
    //     glVertex3f(20.0, 0.0, 0.0);

    //     glColor3f (0.0, 1.0, 0.0);
    //     glVertex3f(0.0, 0.0, 0.0);
    //     glVertex3f(0.0, 20.0, 0.0);

    //     glColor3f (0.0, 0.0, 1.0);
    //     glVertex3f(0.0, 0.0, 0.0);
    //     glVertex3f(0.0, 0.0, 20.0);
        
    //     glColor3f (1.0, 1.0, 0.0);
    //     glEnd();
    // draw_string_on3d(0,0,-3,"helllo world");
    // glRasterPos3f(0.1f, 0.0f, -5.0f);
    // glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'x');
    // glRasterPos3f(0.0f, 0.1f, -5.0f);
    // glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'y');
    // glRasterPos3f(0.0f, 0.0f, -4.9f);
    // glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'z');
		
	glTranslatef(0.0f, 0.0f, camera_zoom);
    int mesh_idx;    
    if (camera_zoom < -2){
        mesh_idx = k;
    }
    else if(camera_zoom >= 8){
        mesh_idx = 0;
    }
    else{
        mesh_idx = (-camera_zoom+2)/1;
    }
    mesh = * mesh_vec[mesh_idx];
    vertex_normal.clear();
    face_normal.clear();
    for (int i = 0; i < mesh.numFaces(); i++)
        {      
            Face *f = mesh.indFace(i);
            Point cur_normal = normal(f);
        };
	glPushMatrix();
		//glTranslatef(0.0f, -1.0f, 0.0f);
		glTranslatef(obj1_trans[0], obj1_trans[1], 0);
        

		glRotatef(obj1_angle_x, 1.0f, 0.0f, 0.0f);
		glRotatef(obj1_angle_y, 0.0f, 1.0f, 0.0f);
        glTranslatef(-x_mean, -y_mean, -z_mean);


        // Iterate through all the half-edges in the mesh 
        // find all boundaryLoops
        glColor3f(1.0f, 1.0f, 0.0f);
        //render all the triangles
        
        for (int i = 0; i < mesh.numFaces(); i++)
        {      
            Face *f = mesh.indFace(i);
            Point cur_normal = normal(f);
            if(deleted_faces.find(f) != deleted_faces.end()){
                continue;
            }
            // face_normal[i];//  normal(f);
            Halfedge *he = f->he();
            glBegin(GL_TRIANGLES);
            do
            {
                Vertex *v = he->source();
                int idx = v ->index();
                // if (he ->edge() ->boundary() ){
                //     boundaries.push_back(he);
                // }
                glColor3f(0.0, 0.0, 1.0);

                if(render_mode == 'f'){
                    //glNormal3d(face_normal[i][0], face_normal[i][1], face_normal[i][2]);
                    glNormal3d(cur_normal[0], cur_normal[1], cur_normal[2]);
                }
                else if (render_mode == 'v')
                {
                    glNormal3d(vertex_normal[idx][0]/vertex_normal[idx].norm(), vertex_normal[idx][1]/vertex_normal[idx].norm(), vertex_normal[idx][2]/vertex_normal[idx].norm());
                }

                //glVertex3f(norm(v->point()[0],'x') , norm(v->point()[1], 'y'), norm(v->point()[2], 'z'));
                glVertex3f(v->point()[0] , v->point()[1], v->point()[2]);
                
                he = he->next();
            } while (he != f->he());
            glEnd();



            // do{
            //     Vertex *v = he->source();
            //     int idx = v ->index();
            //     glColor3f(0.0, 0.0, 1.0);

            //     //draw_string_on3d(v->point()[0]+( (v->point()[0]>0)? 1 : -1) *0.2 , v->point()[1]+( (v->point()[1]>0)? 1 : -1) *0.2 , v->point()[2]+( (v->point()[2]>0)? 1 : -1) *0.2, to_string(v->index()));
        
            //     he = he->next();
            // } while (he != f->he());

            // glEnd();
        
            if (dis_line){
                glBegin(GL_LINES);
                for (int i = 0; i < 3; i++)
                {
                    
                    Vertex *s = he->source();
                    Vertex *t = he->target();
                    glColor3f (0.4, 0.0, 0.4);
                    glVertex3f(s->point()[0] , s->point()[1], s->point()[2]);
                    glVertex3f(t->point()[0] , t->point()[1], t->point()[2]);
                    
                    he = he->next();
                }
                glEnd();
            }
            
            // for (int i = 0; i < 3; i++)
            // {
            //     if(deleted_edges.find(he->edge())!= deleted_edges.end()){
            //         continue;
            //     }
            //     Vertex *s = he->source();
            //     Vertex *t = he->target();
            //     glColor3f (0.0, 1.0, 0.0);
            //     double x_mid = (s->point()[0] + t->point()[0])/2.0;
            //     double y_mid = (s->point()[1] + t->point()[1])/2.0;
            //     double z_mid = (s->point()[2] + t->point()[2])/2.0;
            //     //draw_string_on3d(x_mid+( (x_mid>0)? 1 : -1) *0.1 , y_mid+( (y_mid>0)? 1 : -1) *0.1, z_mid+( (z_mid>0)? 1 : -1) *0.1, to_string(he->edge()->index()));
            //     double dis = distance_error(he->edge());
            //     //draw_string_on3d_s(x_mid+( (x_mid>0)? 1 : -1) *0.05 , y_mid+( (y_mid>0)? 1 : -1) *0.05, z_mid+( (z_mid>0)? 1 : -1) *0.05, to_string(dis).substr(0,3));
            //     he = he->next();
            // }
            //draw_string_on3d(x_mid , y_mid, z_mid, "hello world hhhhhhhhhh");
    }
        

	glPopMatrix();

    glColor3f(1.0, 1.0, 0.0);
    //glRasterPos3f(0.0f, 0.0f, 0.0f);

    draw_string(10, "boundary_loop ", boundary_loop);
    draw_string(30, "connected components  ", com);

    double genus = (2 - mesh.numFaces() + mesh.numEdges() - mesh.numVertices()) /2;
    draw_string(50, "genus  ", genus);
    draw_string(70, "numVertices  ", mesh.numVertices());
    draw_string(90, "numFaces  ", mesh.numFaces());
    draw_string(110, "numEdges  ", mesh.numEdges());

	
	glutSwapBuffers();
}


int main(int argc, char** argv) {
//int main() {
	mesh.readOBJFile(object_path.c_str());//angel  Cylinder9

    //build the heap
    for (int i = 0; i < mesh.numEdges(); i++)
    {
        Edge * cur_edge = mesh.indEdge(i);
        double err = distance_error(cur_edge);
        edge2heap.insert(pair<int,int> (i, h.size()));
        h.insert(err, cur_edge);
        edge_v.insert(err, cur_edge);
    };
        
    edge_v.sort();
    //sort (edge_vector.begin(), edge_vector.end());
    cout<< "size "<< h.hp_vec() ->size()<<endl;
    cout<< "error vec size "<< edge_v.error_vec.size()<<endl;

    std::cout<< mesh.numVertices() <<' '<< mesh.numFaces()<< " "<< deleted_vertices.size();
    for (int i = 0; i < mesh.numVertices(); i++)
            {
                Vertex * v = mesh.indVertex(i) ;
                x_max = max(x_max, v->point()[0]);
                x_min = min(x_min, v->point()[0]);
                y_max = max(y_max, v->point()[1]);
                y_min = min(y_min, v->point()[1]);
                z_max = max(z_max, v->point()[2]);
                z_min = min(z_min, v->point()[2]);
            }
    // normalize all coordinates to [-1, 1]
    for (int i = 0; i < mesh.numVertices(); i++)
            {
                Vertex * v = mesh.indVertex(i) ;
                // v->point()[0] = norm(v->point()[0], 'x');
                // v->point()[1] = norm(v->point()[1], 'y');
                // v->point()[2] = norm(v->point()[2], 'z');
                x_mean += v->point()[0];
                y_mean += v->point()[1];
                z_mean += v->point()[2];
            }
    
    x_mean = x_mean / mesh.numVertices();
    y_mean = y_mean / mesh.numVertices();
    z_mean = z_mean / mesh.numVertices();
    Point center(x_mean, y_mean, z_mean);



    //bfs to find connected components
    std::set< Face *> visitedFaces;
    for (int i = 0; i < mesh.numFaces(); i++)
		{
			
            Face *f = mesh.indFace(i);
            if (visitedFaces.find(f) == visitedFaces.end()) {
                deque< Face *> face_queue;
                visitedFaces.insert(f);
                face_queue.push_back(f);
                while(!face_queue.empty()){
                    Face * cur_face = face_queue.front();
                    face_queue.pop_front();
                    Halfedge * cur_he = cur_face ->he();
                    do{
                        if ( (cur_he -> twin()) ){
                            Face * flipped = cur_he -> twin() ->face();
                            if (visitedFaces.find(flipped) == visitedFaces.end()){
                                face_queue.push_back(flipped);
                                visitedFaces.insert(flipped);
                                }
                            }
                        cur_he = cur_he -> next();
                    }
                    while(cur_he != cur_face ->he());
                }
                com += 1;
            }
        }
    cout << " com "<< com <<endl;

    //find boundary loop
    for (int i = 0; i < mesh.numEdges(); i++) {
        Halfedge* he = mesh.indEdge(i)->he(0);

        // Check if the twin half-edge is null
        if ((!he->twin()  ) && (visitedHEs.find(he) == visitedHEs.end())  ) {  // && (visitedHEs.find(he) == visitedHEs.end())
            // We've found a boundary half-edge
            std::vector< Point > boundaryLoop;
            Halfedge* currentHE = he;
            Halfedge* nextHE;
            do {
                // Add the vertex to the boundary loop
                boundaryLoop.push_back(currentHE->source() ->point());

                // Mark the half-edge as visited
                visitedHEs.insert(currentHE);

                // Move to the next half-edge on the boundary
                Vertex * cur_tar =  currentHE -> target();
                if (currentHE == cur_tar -> most_ccw_in_halfedge()){
                    nextHE = cur_tar -> most_clw_out_halfedge();
                }
                else{
                    nextHE = cur_tar -> most_ccw_out_halfedge();
                }
                currentHE = nextHE; //currentHE->next();

                // Check if we've returned to the starting half-edge
            } while (currentHE != he);
            boundaries_loop.push_back(boundaryLoop);
            // Draw the boundary loop
            for (int j = 0; j < boundaryLoop.size(); j++) {
                // cout << j<<endl;
                // cout <<"bound vertex "<< boundaryLoop[j][0] << " "<<boundaryLoop[j][1] << " " <<boundaryLoop[j][2] <<endl;
                }
            boundary_loop += 1;
            }
        }
    cout <<"boundary_loop  "<<boundary_loop<<endl;
    for (int i = 0; i < mesh.numFaces(); i++)
		{
            Face *f = mesh.indFace(i);
            face_normal[i] = normal(f);
        }


    // hw2
    int min_vec_num = 10;
    int step = (mesh.numVertices() - min_vec_num) / k;
    cout<<endl<<"step "<<step<<endl;

    Mesh * cur = new Mesh;
    mesh_vec.push_back(cur);
    writeOBJFile(&mesh, input_name + "_0.obj");
    cur ->readOBJFile( (input_name + "_0.obj").c_str());

    for (int i = 1; i <k+1 ; i++)
    {
        clock_t start = clock();
        for (int j = 0; j < step; j++)
        {
           collapse_once_edge();
        }
        clock_t finish = clock();
		cout << "collapse one iteration # time used: " << (double)(finish - start) / CLOCKS_PER_SEC  << " s"<< endl;
        Mesh * cur = new Mesh;
        mesh_vec.push_back(cur);
        writeOBJFile(&mesh, output_path+ input_name +"_"+to_string(i) + ".obj");
        cur->readOBJFile( (output_path+ input_name +"_"+to_string(i) + ".obj").c_str());

    }
    cout<< "size "<< mesh_vec.size() <<" "<<mesh_vec[0];
    for (int i = 0; i < mesh_vec.size(); i++)
    {
        cout<< i<< " "<<&mesh_vec[i];
    }

	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	
	//Create the window
	glutCreateWindow("Color");
	initRendering();
	 
	//Set handler functions
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove);
	
	glutTimerFunc(150, update, 0); //Add a timer
	
	glutMainLoop();
	return 0;
}

