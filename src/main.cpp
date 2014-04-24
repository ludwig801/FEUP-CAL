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

#define	OVERHEAD	60

using namespace std;

int getTimeBetween(Graph<int> &g, const int v1, const int v2);
void setVertexLimitTime(Graph<int> &g);
void takeOutNonRequests(Graph<int> &g);

int anyWillBeLate(Graph<int> &g, stack<Vertex<int> *> s, Vertex<int>* actual, int time);

void reverseStack(stack<Vertex<int> *> &from, stack<Vertex<int> *> &to);
void eraseStack(stack<Vertex<int> *> &s);

string convertTime(int time);
string convertVertexInfo(int info);

void printStack(stack<Vertex<int> *> s);
int printDropOffTime(Graph<int> &g, stack<Vertex<int> *> s);

void findSol_1(Graph<int> &g);
void findSol_2(Graph<int> &g);

int findBestMinTime(Graph<int> &g, bool debug = false);

int main() {
	Graph<int> g;

	readFiles(g);

	// Find Solution for first scenario
	//findSol_1(g);

	// Find Solution for second scenario
	findSol_2(g);

	return 0;
}

int getTimeBetween(Graph<int> &g, const int v1, const int v2) {
	int count = 0;

	vector<int> res, path;

	path.push_back(v1);

	g.getfloydWarshallPathAux(v1, v2, res);

	for(unsigned int i = 0; i < res.size(); i++) {
		path.push_back(res[i]);
	}

	path.push_back(v2);

	for(unsigned int i = 0; i < path.size() - 1; i++) {
		count += g.getWeightOfEgdeBetween(path[i],path[i+1]);
	}

	return count;
}

void setVertexLimitTime(Graph<int> &g) {
	int source = 0;
	int count = 0;

	for(unsigned int i = 1; i < g.getVertexSet().size(); i++) {

		count = getTimeBetween(g,source,i);

		g.getVertex(i)->setMinTime(g.getVertex(i)->getPedido().getHora() - count);
	}
}

void takeOutNonRequests(Graph<int> &g) {
	Vertex<int> *temp;

	for(size_t i = 0; i < g.getVertexSet().size(); i++) {
		temp = g.getVertexSet()[i];

		if(!temp->hasPedido() /*&& temp->getInfo() != 0*/) {
			temp->setVisited(true);
		}
	}
}

int anyWillBeLate(Graph<int> &g, stack<Vertex<int> *> s, Vertex<int>* actual, int time) {
	int timeToAirport = 0;

	timeToAirport = getTimeBetween(g, 0, actual->getInfo());

	while(!s.empty() && (s.top()->getInfo() != 0)) {

		if(s.top()->hasPedido()) {

			if((time + timeToAirport) > s.top()->getPedido().getHora()) {
				return s.top()->getInfo();
			}

			s.pop();
		}
	}

	return 0;
}

void reverseStack(stack<Vertex<int> *> &from, stack<Vertex<int> *> &to) {
	while(!from.empty()) {
		to.push(from.top());
		from.pop();
	}
}

void eraseStack(stack<Vertex<int> *> &s) {
	while(!s.empty() && (s.top()->getInfo() != 0)) {
		if(s.top()->getInfo() != 0) {
			s.pop();
		}
	}
}

string convertTime(int time) {
	stringstream s;

	s << time/60 <<"h";

	time = time % 60;

	s << time << "min";

	return s.str();
}

string convertVertexInfo(int info) {
	stringstream s;
	s << (char)(info + 65);
	return s.str();
}

void printStack(stack<Vertex<int> *> s) {
	stack<Vertex<int> *> r;

	reverseStack(s,r);

	cout << "-- stack --" << endl;
	while(!r.empty()) {
		cout << convertVertexInfo(r.top()->getInfo());
		cout << " at: " << convertTime(r.top()->getPickupTime()) << endl;
		r.pop();
	}
	cout << "-----------" << endl;
}

int recursiveCall(Graph<int> &g, Vertex<int>* v, int time, stack<Vertex<int> *> &s) {
	//cout << "-- " << v->getInfo() << " ------" << endl;

	if(g.allAreVisited()) {
		cout << "----------- all are visited ----------- " << endl;
		return -1;
	}

	if(v->isVisited()) {
		//cout << "----------- is visited: " << v->getInfo() << endl;
		return 0;
	}

	if(v->getInfo() != 0) {

		int late = 0;

		if(time > v->getMinTime()) {
			cout << "----------- is late on: " << v->getInfo() << " by " << (time - v->getMinTime()) << " min" << endl;
			return 1;
		}
		if((late = anyWillBeLate(g, s, v, time)) > 0) {
			cout << "----------- would be late: " << late << " if: " << v->getInfo() << " was picked up"<< endl;
			return 2;
		}
		if(time < (v->getMinTime() - OVERHEAD)) {
			cout << "----------- overhead on: " << v->getInfo() << " by " << (v->getMinTime() - OVERHEAD - time)  << " min" << endl;
			return 3;
		}
	}

	//cout << "time on: " << v->getInfo() << " --- " << time << endl;

	v->setVisited(true);
	v->setPickupTime(time);

	s.push(v);
	//printStack(s);

	Vertex<int> *temp;

	int timeBetween;

	for(size_t i = 0; i < g.getVertexSet().size(); i++) {

		temp = g.getVertexSet()[i];

		if(temp->getInfo() != v->getInfo()) {
			timeBetween = getTimeBetween(g, v->getInfo(), temp->getInfo());

			if(timeBetween > 0) {
				//cout << endl;

				//cout << "  Vertex (" << v->getInfo() << ") at " << time << endl;
				//cout << "calling rotten_potato2() on: " << temp->getInfo() << endl;
				switch(recursiveCall(g, temp, time + timeBetween, s)) {
				case 0:
					/*
					 * NOT VIABLE
					 */
					break;
				case 1:
					/*
					 * LATE
					 */
					if(s.top()->getInfo() != 0) {
						s.top()->setVisited(false);
						s.pop();
						//printStack(s);
					}
					return 0;
				case -1:
					/*
					 * ALL VISITED
					 */
					return -1;
				default:
					break;
				}
			}
		}
	}

	return 0;
}

int scenario1_recursive(Graph<int> &g, Vertex<int>* v, int time, stack<Vertex<int> *> &s) {


	switch(recursiveCall(g, v, time, s)) {
	case -1:
		/*
		 * ALL VISITED
		 */
		return -1;
	default:
		break;
	}

	if(s.top()->getInfo() != 0) {
		s.top()->setVisited(false);
		s.pop();
		printStack(s);
	}

	return 0;

}

int scenario2_recursive(Graph<int> &g, Vertex<int>* v, int time, stack<Vertex<int> *> &s) {

	return recursiveCall(g, v, time, s);
}

int scenario1(Graph<int> &g, int time, stack<Vertex<int> *> &s) {

	Vertex<int> *temp = g.getVertex(0);

	int timeBetween;

	// Put the airport in the stack.
	temp->setVisited(true);
	temp->setPickupTime(time);
	s.push(temp);

	for(size_t i = 0; i < g.getVertexSet().size(); i++) {
		temp = g.getVertexSet()[i];

		if(temp->getInfo() != 0) {
			timeBetween = getTimeBetween(g, 0, temp->getInfo());

			if(timeBetween > 0) {
				if (scenario1_recursive(g, temp, time + timeBetween, s) < 0) {
					return -1;
				}
			}

		}
	}

	return 0;
}

int scenario2(Graph<int> &g, int time, stack<Vertex<int> *> &s) {

	// Actual vertex
	Vertex<int> *temp = g.getVertex(0);

	int timeBetween;

	// Put the airport in the stack.
	temp->setVisited(true);
	temp->setPickupTime(time);
	s.push(temp);

	while(true){
		for(size_t i = 0; i < g.getVertexSet().size(); i++) {
			temp = g.getVertexSet()[i];

			if(temp->getInfo() != 0) {
				timeBetween = getTimeBetween(g, 0, temp->getInfo());

				if(timeBetween > 0) {
					if (scenario2_recursive(g, temp, time + timeBetween, s) < 0) {
						return -1;
					}
				}

			}
		}

		if(g.allAreVisited()) {
			break;
		}
		else {


			if(s.top()->getInfo() == 0) {
				Vertex<int>* v = g.getVertex(findBestMinTime(g, true));
				if(time <= ( v->getMinTime() - getTimeBetween(g, 0, v->getInfo()))) {
					time = v->getMinTime() - getTimeBetween(g, 0, v->getInfo());
					g.setTime(time);
					g.getVertex(0)->setPickupTime(time);
				} else {
					//cout << "No solution found" << endl;
					return 0;
				}
			}
			else {


				// Check the results.
				printStack(s);
				time = printDropOffTime(g, s);

				//cout << "time: " << convertTime(time) << endl;

				eraseStack(s);
				g.getVertex(0)->setPickupTime(time);
//
//				g.printGraph();
			}



		}
	}

	return 0;
}

int printDropOffTime(Graph<int> &g, stack<Vertex<int> *> s) {
	int finalTime = g.getTime();

	Vertex<int> * temp;

	finalTime += getTimeBetween(g, 0, s.top()->getInfo());

	while(!s.empty()) {
		temp = s.top();
		s.pop();
		if(!s.empty()) {
			finalTime += getTimeBetween(g, temp->getInfo(), s.top()->getInfo());
		}
	}

	g.setTime(finalTime);

	cout << "drop-off: " << convertTime(finalTime) << endl;

	return finalTime;
}

void findSol_1(Graph<int> &g) {

	// Initializes graph vertex visited to false.
	g.setVisited(false);

	// Run FloydWarshall's algorithm to get the shortest paths from any vertex to another.
	g.floydWarshallShortestPath();

	// Set the limit time for each pickup.
	setVertexLimitTime(g);

	// Set the initial time.
	g.setTime(420);

	// Set any empty vertex (without pickup) to visited.
	takeOutNonRequests(g);

	stack<Vertex<int> *> s;

	// Begin the finding of a solution.
	scenario1(g, g.getTime(), s);

	// Check the results.
	if(s.top()->getInfo() == 0) {
		cout << "No solution found!" << endl;
	}
	else {
		printStack(s);
		printDropOffTime(g, s);
	}

}

void findSol_2(Graph<int> &g) {
	// Initializes graph vertex visited to false.
	g.setVisited(false);

	// Run FloydWarshall's algorithm to get the shortest paths from any vertex to another.
	g.floydWarshallShortestPath();

	// Set the limit time for each pickup.
	setVertexLimitTime(g);

	// Set the initial time.
	g.setTime(200);

	// Set any empty vertex (without pickup) to visited.
	takeOutNonRequests(g);

	stack<Vertex<int> *> s;

	// Begin the finding of a solution.
	scenario2(g, g.getTime(), s);

	// Check the results.
	if(s.top()->getInfo() == 0) {
		cout << "No solution found!" << endl;
	}
	else {
		printStack(s);
		printDropOffTime(g, s);
	}
}

int findBestMinTime(Graph<int> &g, bool debug) {

	int minTime = INT_MAX;
	int id = 0;
	vector<Vertex<int> *> v = g.getVertexSet();
	for(size_t i = 0; i < v.size(); i++) {
		if(!v[i]->isVisited()) {
			if(v[i]->getMinTime() <= minTime){
				minTime = v[i]->getMinTime();
				id = v[i]->getInfo();
			}
		}
	}

	if(debug) cout << "Best min time: " << minTime << endl;

	if(minTime != INT_MAX) {
		return id;
	} else {
		return -1;
	}
}
