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

/**
 * Le detalhes do ficheiro.
 *
 * @param g Grafo
 * @param s Nome do ficheiro
 */
void readDetails(Graph<int> &g, string s);

/**
 * Le vertices do ficheiro.
 *
 * @param g Grafo
 * @param gv GraphViewer
 * @param s Nome do ficheiro
 */
void readVertex(Graph<int> &g, GraphViewer *gv, string s);

/**
 * Le arestas do ficheiro.
 *
 * @param g Grafo
 * @param gv GraphViewer
 * @param s Nome do ficheiro
 */
void readEdges(Graph<int> &g, GraphViewer *gv, string s);

/**
 * Le pedidos do ficheiro.
 *
 * @param g Grafo
 * @param s Nome do ficheiro
 */
void readRequests(Graph<int> &g, string s);

/**
 * Efectua a chamada de leitura de todos os ficheiros necessarios.
 * Cria tambem o nome dos ficheiros.
 *
 * @param g Gravo
 * @param scenario Cenario a testar
 * @param index Caso a testar.
 */
void readFiles(Graph<int> &g, int scenario, int index);

#endif /* INPUT_H_ */
