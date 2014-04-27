/*
 * Input.h
 *
 *  Created on: 23 de Abr de 2014
 *      Author: Luís
 */

#ifndef INPUT_H_
#define INPUT_H_

#include <cstdio>
#include "graphviewer.h"
#include <fstream>
#include <iostream>
#include <sstream>

#include "Graph.h"
#include "pedido.h"

/*
 * input.cpp
 *
 *  Created on: 23 de Abr de 2014
 *      Author: Luís
 */

#include "input.h"

using namespace std;

void readDetails(Graph<int> &g, string s);

void readVertex(Graph<int> &g, GraphViewer *gv, string s);

void readEdges(Graph<int> &g, GraphViewer *gv, string s);

void readRequests(Graph<int> &g, string s);

void readFiles(Graph<int> &g, int scenario, int index);

#endif /* INPUT_H_ */
