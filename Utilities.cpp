// Utilities.cpp
#include "Utilities.h"
#include "MeshOperations.h"
#include "DataStructure.h"
#include "Variables.h"
#include "../MeshLib_Core/Mesh.h"
#include "../MeshLib_Core/Iterators.h"






bool writeOBJFile(Mesh * m, string outputFile) {
    //vertex indexing
    int offset = 0;
    for(int vit = 0; vit< mesh.numVertices(); vit++){
		Vertex * ver = mesh.indVertex(vit); 
        if (deleted_vertices.find(ver) != deleted_vertices.end()){
            offset += 1;
        }
        else{
            ver2idx[ver] = ver->index() - offset;
        }
    }

	FILE * fp = fopen(outputFile.c_str(), "w");
	if (!fp) {
		std::cerr << "Cannot open file " << outputFile << "to write!" << std::endl;
		return false;
	}
	std::cout << "Writing mesh " << outputFile << " ...";
	std::vector<Vertex *>::iterator vit;
    for(int vit = 0; vit< m->numVertices(); vit++){
	//for (vit = m_verts.begin(); vit != m_verts.end(); ++vit) {
		Vertex * ver = m->indVertex(vit); //*vit;
        if (deleted_vertices.find(ver) != deleted_vertices.end()){
            continue;
        }
		std::ostringstream oss;
		oss.precision(6); oss.setf(std::ios::fixed, std::ios::floatfield);  //setting the output precision: now 6
		oss << "v " << ver->point()[0] << " " << ver->point()[1] << " " << ver->point()[2];
		fprintf(fp, "%s ", oss.str().c_str());
		fprintf(fp, "\n");
	}

	//std::vector<Face *>::iterator fit;
	for (int fit = 0; fit != m->numFaces(); ++fit)
	{
		Face * face = m-> indFace(fit);     //*fit;
        if (deleted_faces.find(face) != deleted_faces.end()){
            continue;
        }
		Halfedge * the0 = face->he();
		Halfedge * the1 = the0->next();
		int v0 = ver2idx[the0->source()] + 1;
		int v1 = ver2idx[the0->target()] + 1;
		int v2 = ver2idx[the1->target()] + 1;
		fprintf(fp, "f %d %d %d", v0, v1, v2);
		fprintf(fp, "\n");
	}

	fclose(fp);
	std::cout << "Done!" << std::endl;
	return true;
}


// below are the functions from hw1 rendering tool


double norm(double input, char pos){
    switch (pos)
    {
    case 'x':
        input = (input - x_min)/(x_max- x_min) *2 -1;
        break;
    case 'y':
        input = (input - y_min)/(y_max- y_min)*2 -1;
        break;
    case 'z':
        input = (input - z_min)/(z_max- z_min)*2 -1;
        break;
    };
    return input;
    
}

Point normal(Face *f){
		Halfedge * he1 = f ->he(); // one halfedge
		Halfedge * he2 = he1->next(); // its next halfedge inside this face
		// Point & pt1 = he1->source()->point();
		// Point & pt2 = he1->target()->point();
		// Point & pt3 = he2->target()->point();
        Point & pt1 = he1->target()->point();
		Point & pt2 = he2->target()->point();
		Point & pt3 = he2->next() ->target()->point();
		Point p12 = pt2 - pt1;
		Point p13 = pt3 - pt1;
        Point p23 = pt3 - pt2;
		Point cprod = p12 ^ p13;
        Point face_n = cprod / cprod.norm();// cprod / cprod.norm();//Point(1,1,1);//

        double  alpha1 = acos( p12*p13 / (p12.norm() * p13.norm()) );
        Point  a1 = face_n * alpha1;
        vertex_normal[he1->source()->index()] += a1;

        double  alpha2 = acos( -p12*p23 / (p12.norm() * p23.norm()) );
        Point  a2 = face_n * alpha2;
        vertex_normal[he1->target()->index()] += a2;

        double  alpha3 = acos( p23*p13 / (p13.norm() * p23.norm()) );
        Point  a3 = face_n * alpha3;
        vertex_normal[he2->target()->index()] += a3;

		return face_n;
	}

