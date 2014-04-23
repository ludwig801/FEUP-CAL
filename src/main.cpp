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

	g.getfloydWarshallPathAux(origem, destino, res);

	for(unsigned int i = 0; i < res.size(); i++) {
		path.push_back(res[i]);
	}

	path.push_back(destino);

	for(unsigned int i = 0; i < path.size() - 1; i++) {
		count += g.getWeightOfEgdeBetween(path[i],path[i+1]);
	}

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

void printStack(stack<Vertex<int> *> s) {
	cout << "-- stack --" << endl;
	while(!s.empty()) {
		cout << s.top()->getInfo() << endl;
		s.pop();
	}
	cout << "-----------" << endl;
}

void printQueue(queue<Vertex<int> *> q) {
	cout << "-- queue --" << endl;
	while(!q.empty()) {
		cout << q.front()->getInfo() << endl;
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

int rotten_potato(Graph<int> &g, Vertex<int>* v, int time, queue<Vertex<int> *> &q) {

	if(g.allAreVisited()) {
		//cout << "> all are visited <" << endl;
		return -1;
	}
	if(v->isVisited()) {
		//cout << "----------- is visited: " << v->getInfo() << endl;
		return 0;
	}

	if(v->hasPedido()) {
		if(time > v->getMinTime()) {
			cout << "----------- is late on: " << v->getInfo() << " by " << (time - v->getMinTime()) << " min" << endl;
			return 0;
		}
		if(time < (v->getMinTime() - OVERHEAD)) {
			cout << "----------- overhead on: " << v->getInfo() << " by " << (v->getMinTime() - OVERHEAD - time)  << " min" << endl;
			return 0;
		}
	}
	else {
		if(v->getInfo() != 0) {
			cout << "----------- does not have pedido: " << v->getInfo() << endl;
			return 0;
		}
	}

	cout << endl;
	cout << "  Vertex (" << v->getInfo() << ") is possible" << endl;

	v->setVisited(true);
	q.push(v);

	Vertex<int> *temp;

	int timeBetween;

	for(size_t i = 0; i < g.getVertexSet().size(); i++) {
		temp = g.getVertexSet()[i];

		if(temp->getInfo() != v->getInfo()) {
			timeBetween = getWeightBetweenVertex(g, v->getInfo(), temp->getInfo());

			if(timeBetween > 0) {
				if (rotten_potato(g, temp, time + timeBetween, q) < 0) {
					return -1;
				}
			}

		}
	}

	/*
	 * Only gets here if no solution worked with the above calls, so:
	 * Uncomment if you want the queue to be empty at "no solution" point,
	 *  otherwise leave comment and the resulting queue will give a possible solution,
	 *  leaving out the unreachable vertex.
	 */

	if(q.front()->getInfo() != 0) {
		q.front()->setVisited(false);
		q.pop();
	}

	return 0;
}

void printFinalTime(Graph<int> &g, queue<Vertex<int> *> q) {
	int finalTime = g.getTime();

	Vertex<int> * temp;

	while(!q.empty()) {
		temp = q.front();
		q.pop();
		finalTime += getWeightBetweenVertex(g, temp->getInfo(), q.front()->getInfo());
		temp = q.front();
		q.pop();
	}

	finalTime += getWeightBetweenVertex(g, temp->getInfo(), 0);

	cout << "final time: " << finalTime << endl;
}

void findSol1(Graph<int> &g) {

	// Initializes graph vertex visited to false
	g.setVisited(false);

	g.floydWarshallShortestPath();

	g.printGraph();

	getOptimalTime(g);

	g.setTime(420);

	cout << endl;

	queue<Vertex<int> *> q;

	rotten_potato(g, g.getVertex(0), g.getTime(), q);

	if(q.empty()) {
		cout << "No solution!" << endl;
	}
	else {
		printQueue(q);
		printFinalTime(g, q);
	}

}


