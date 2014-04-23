/*
 * main.cpp
 *
 *  Created on: 23 de Abr de 2014
 *      Author: Luís
 */
#include "input.h"
#include "Graph.h"

#include <stack>
#include <iostream>

#define OVERHEAD	60

using namespace std;

void findSol1(Graph<int> &g);

int main() {
	Graph<int> g;

	readFiles(g);

	//cout << "readFiles()" << endl;
	findSol1(g);

	//cout << "findSol1()" << endl;
	return 0;
}

int getWeightBetweenVertex(Graph<int> &g, const int s, const int d) {
	int origem = s;
	int destino = d;

	int count = 0;

	vector<int> res, path;

	if(g.getVertex(destino)->getPedido().getHora() <= 0) {
		//cout << "nao e pedido" << endl;
		return 0;
	}

	path.push_back(origem);

	//cout << "antes" << endl;
	g.getfloydWarshallPathAux(origem, destino, res);
	//cout << "depois" << endl;

	for(unsigned int i = 0; i < res.size(); i++) {
		//cout << "index: " << i << " - vertex:" << res[i] << endl;
		path.push_back(res[i]);
	}

	path.push_back(destino);

	for(unsigned int i = 0; i < path.size() - 1; i++) {
		count += g.getWeightOfEgdeBetween(path[i],path[i+1]);
	}

//	cout << "Vertex: " << g.getVertex(destino)->getInfo()
//			<< " count: " << g.getVertex(destino)->getPedido().getHora() - count << endl;

	return count;
}

void getOptimalTime(Graph<int> &g) {
	int origem = 0;
	int count = 0;
	for(unsigned int i = 1; i < g.getVertexSet().size(); i++) {

		count = getWeightBetweenVertex(g,origem,i);

		g.getVertex(i)->setMinTime(g.getVertex(i)->getPedido().getHora() - count);
	}
}

void potato(Graph<int> &g) {

	int time = g.getTime();

	Vertex<int> * origem = g.getVertex(0);

	stack<Vertex<int> *> q;

	vector<Edge<int> >::const_iterator it = origem->getAdj().begin();

	while(true) {
		for(; it != origem->getAdj().end(); ++it) {

			if(g.allAreVisited()) {
				//cout << "true" << endl;
				return;
			}
			if((*it).getDest()->isVisited()) {
				//cout << "aqui parou: " << (*it).getDest()->getInfo() << " ta visitado!" << endl;
				continue;
			}

			int weight = getWeightBetweenVertex(g, origem->getInfo(),(*it).getDest()->getInfo());

			if(weight > 0) {
				weight += time;
				if(weight > (*it).getDest()->getMinTime()) {
					//cout << "aqui parou: " << (*it).getDest()->getInfo() << " ta atrasado!" << endl;
					continue;
				}
				if(weight < ((*it).getDest()->getMinTime() - OVERHEAD)) {
					//cout << "aqui parou: " << (*it).getDest()->getInfo() << " ta no overhead!" << endl;
					continue;
				}
			}

			origem->setVisited(true);
			q.push(origem);

			cout << "vertex: " << origem->getInfo() << endl;

			time += (*it).getWeight();

			origem = (*it).getDest();
			it = origem->getAdj().begin();
		}
	}
}

void findSol1(Graph<int> &g) {

	// Initializes graph vertex visited to false
	g.setVisited(false);

	g.floydWarshallShortestPath();

	//g.printGraph();

	getOptimalTime(g);

	g.setTime(420);

	potato(g);

}


