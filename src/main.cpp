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

void printStack(stack<Vertex<int> *> q) {
	cout << "-- stack --" << endl;
	while(!q.empty()) {
		cout << q.top()->getInfo() << endl;
		q.pop();
	}
	cout << "-----------" << endl;
}

void potato(Graph<int> &g) {

	int time = g.getTime();

	Vertex<int> * vertex = g.getVertex(0);
	Vertex<int> * temp;

	stack<Vertex<int> *> q;

	vector<Edge<int> >::const_iterator it = vertex->getAdj().begin();

	int i = 0;

	while(true) {
		for(; it != vertex->getAdj().end(); ++it) {

			if(g.allAreVisited()) {
				return;
			}
			// DESTINY IS VISITED
			if((*it).getDest()->isVisited()) {
				continue;
			}

			cout << "weight between: (" << vertex->getInfo() << ", " << (*it).getDest()->getInfo() << ")" <<endl;
			int weight = getWeightBetweenVertex(g, vertex->getInfo(),(*it).getDest()->getInfo());
			weight += time;

			// PASSED THE MINIMUM HOUR
			if(weight > (*it).getDest()->getMinTime()) {
				continue;
			}
			// OVERHEAD
			if(weight < ((*it).getDest()->getMinTime() - OVERHEAD)) {
				continue;
			}

			/*
			 * NEXT VERTEX
			 */

			if(!q.empty()) {

				if((*it).getDest()->getInfo() == q.top()->getInfo()) {
					continue;
				}
			}

			i = 0;

			vertex->setVisited(true);
			q.push(vertex);

			time += (*it).getWeight();

			cout << "---- " << vertex->getInfo() << " ----" << endl;

			vertex = (*it).getDest();
			it = vertex->getAdj().begin();

			cout << "---- " << vertex->getInfo() << " ----" << endl;

			printStack(q);
			getchar();

			/*
			 * END OF NEXT VERTEX
			 */
		}

		if(!q.empty()) {

			temp = q.top();
			time -= getWeightBetweenVertex(g, vertex->getInfo(), temp->getInfo());

			q.pop();
			vertex = temp;
			vertex->setVisited(false);

			//cout << "time: " << time << endl;

			i++;
			it = vertex->getAdj().begin();

			for(int j = 0; j < i; j++) {
				++it;
			}
		}
		else {
			return;
		}

	}
}

void findSol1(Graph<int> &g) {

	// Initializes graph vertex visited to false
	g.setVisited(false);

	g.floydWarshallShortestPath();

	g.printGraph();

	getOptimalTime(g);

	g.setTime(385);

	potato(g);

}


