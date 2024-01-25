// Utilities.h
#ifndef UTILITIES_H
#define UTILITIES_H

#include "../MeshLib_Core/Mesh.h"
#include "../MeshLib_Core/Iterators.h"

bool writeOBJFile(Mesh* m, const std::string& outputFile);
Point normal(Face* f);
#endif // UTILITIES_H
