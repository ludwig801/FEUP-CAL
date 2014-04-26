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

#define	AIRPORT	0

#define	VISITED			0
#define	LATE			1
#define WOULD_BE_LATE	2
#define IS_OVERHEAD		3
#define VAN_OVERLOAD	4

#define SUCCESS	-1

#define	DEBUG_MODE true

using namespace std;

/**
 * Verifica se o grafo inicial poderá levar a uma solução viável.
 *
 * No caso de um dos vértices ter um número de passageiros superior ao limite da carrinha
 * ou no caso de um dos vértices ter uma hora mínima superior ao tempo inicial, considera-se à
 * partida que não existirá uma solução possivel.
 *
 * @param g Grafo
 * @param debug Imprime mensagens se for true.
 *
 * @return True, se for possível chegar a uma solução.
 * @return False, se não for possível chegar a uma solução.
 *
 */
bool isViable(Graph<int> &g, bool debug = false);

/**
 * Retorna o tempo (peso) entre dois vertices.
 *
 * @param g Grafo
 * @param v1 Origem
 * @param v2 Destino
 *
 * @return Distancia entre os vertices.
 */
int getTimeBetween(Graph<int> &g, const int v1, const int v2, bool debug = false);

/**
 * Define para cada vertice o tempo minimo que o autocarro tem de chegar
 * para conseguir cumprir a hora pretendida.
 *
 * @param g Grafo
 */
void setVertexLimitTime(Graph<int> &g, bool debug = false);

/**
 * "Remove" todos os vertices que nao tem pedidos, por exemplo, os vertices de ligacao.
 * Esta remocao e feita mudando a flag 'visited' do vertice para true.
 *
 * @param g Grafo
 */
void takeOutNonRequests(Graph<int> &g, bool debug = false);

/**
 * Testa se algum dos clientes ira chegar atrasado ao aeroporto, considerando o estado actual da execucao.
 *
 * @param g Grafo
 * @param s Estado da stack de clientes atuais
 * @param actual Posicao atual da carrinha no grafo
 * @param time Tempo atual
 *
 * @return 0, se ninguem chegar atrasado, caso contrario retorna o indice do que chegar atrasado.
 */
int anyWillBeLate(Graph<int> &g, stack<Vertex<int> *> s, Vertex<int>* actual, int time, bool debug = false);

/**
 * Copia uma stack para a outra, de forma inversa.
 *
 * @param from Stack original
 * @param to Stack inversa
 */
void reverseStack(stack<Vertex<int> *> &from, stack<Vertex<int> *> &to);

/**
 * Limpa a stack.
 *
 * @param s Stack a limpar.
 */
void eraseStack(stack<Vertex<int> *> &s);

string convertTime(int time);
string convertVertexInfo(int info);

void printStack(stack<Vertex<int> *> s);

int getDropOffTime(Graph<int> &g, stack<Vertex<int> *> s);

void findSol_1(Graph<int> &g);
void findSol_2(Graph<int> &g);

int findBestMinTime(Graph<int> &g, bool debug = false);
int findBestTime(Graph<int> &g, bool debug = false);

int main() {
	Graph<int> g1, g2, g3;

	readFiles1(g1,1);

	// Find Solution for first scenario
	findSol_1(g1);

	readFiles2(g2,1);

	// Find Solution for second scenario
	findSol_2(g2);

	return 0;
}

int getTimeBetween(Graph<int> &g, const int v1, const int v2, bool debug) {
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

	if(debug) {
		cout << "> Time between " << v1 << " and " << v2 << ": " << count << endl;
	}

	return count;
}

void setVertexLimitTime(Graph<int> &g, bool debug) {
	int source = 0;
	int count = 0;

	for(unsigned int i = 1; i < g.getVertexSet().size(); i++) {

		count = getTimeBetween(g,source,i);

		g.getVertex(i)->setMinTime(g.getVertex(i)->getPedido().getHora() - count);

		if(debug) cout << "> Min time for Vertex "
				<< g.getVertex(i)->getInfo()
				<< ": " << g.getVertex(i)->getMinTime() << endl;
	}

}

void takeOutNonRequests(Graph<int> &g, bool debug) {
	Vertex<int> *temp;

	for(size_t i = 0; i < g.getVertexSet().size(); i++) {
		temp = g.getVertexSet()[i];

		if(!temp->hasPedido()) {
			temp->setVisited(true);

			if(debug) {
				cout << "> Removed vertex: " << temp->getInfo() << endl;
			}

		}


	}
}

int anyWillBeLate(Graph<int> &g, stack<Vertex<int> *> s, Vertex<int>* actual, int time, bool debug) {

	int timeToAirport = 0;

	timeToAirport = getTimeBetween(g, 0, actual->getInfo());

	while(!s.empty() && (s.top()->getInfo() != 0)) {

		if(s.top()->hasPedido()) {

			if((time + timeToAirport) > s.top()->getPedido().getHora()) {
				if(debug) {
					cout << "> The client " << s.top()->getInfo() << " will be late.";
				}
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

int scenario1_recursive(Graph<int> &g, Vertex<int>* v, int time, stack<Vertex<int> *> &s) {

	if(g.allAreVisited()) {
		//cout << "----------- all are visited ----------- " << endl;
		return SUCCESS;
	}

	if(v->isVisited()) {
		//cout << "----------- is visited: " << v->getInfo() << endl;
		return VISITED;
	}

	if(v->getInfo() != AIRPORT) {

		int late = 0;

		if(time > v->getMinTime()) {
			cout << "----------- is late on: " << convertVertexInfo(v->getInfo()) << " by " << convertTime(time - v->getMinTime()) << " min" << endl;
			return LATE;
		}
		if((late = anyWillBeLate(g, s, v, time)) > 0) {
			//cout << "----------- would be late: " << late << " if: " << v->getInfo() << " was picked up"<< endl;
			return WOULD_BE_LATE;
		}
		if(time < (v->getMinTime() - g.overhead)) {
			cout << "----------- overhead on: " << convertVertexInfo(v->getInfo()) << " by " << convertTime(v->getMinTime() - g.overhead - time)  << " min" << endl;
			return OVERHEAD;
		}
	}

	v->setVisited(true);
	v->setPickupTime(time);

	s.push(v);
	//printStack(s);

	Vertex<int> *temp;

	int timeBetween;

	for(size_t i = 0; i < g.getVertexSet().size(); i++) {

		temp = g.getVertexSet()[i];

		if((temp->getInfo() != v->getInfo()) && (temp->getInfo() != AIRPORT)) {
			timeBetween = getTimeBetween(g, v->getInfo(), temp->getInfo());

			if(timeBetween > 0) {

				switch(scenario1_recursive(g, temp, time + timeBetween, s)) {
				case VISITED:
					break;
				case LATE:
					if(s.top()->getInfo() != AIRPORT) {
						s.top()->setVisited(false);
						s.pop();
					}
					return 0;
				case SUCCESS:
					return SUCCESS;
				default:
					break;
				}
			}
		}
	}

	if(s.top()->getInfo() != AIRPORT) {
		s.top()->setVisited(false);
		s.pop();
	}

	return 0;
}

int scenario2_recursive(Graph<int> &g, Vertex<int>* v, int time, stack<Vertex<int> *> &s) {

	if(g.allAreVisited()) {
		//cout << "----------- all are visited ----------- " << endl;
		return SUCCESS;
	}

	if(v->isVisited()) {
		//cout << "----------- is visited: " << v->getInfo() << endl;
		return VISITED;
	}

	g.usedSeats += v->getPedido().getNumPessoas();
	if(g.isVanOverloaded()) {
		//cout << "------------ The van is full -----------" << endl;
		g.usedSeats -= v->getPedido().getNumPessoas();
		return VAN_OVERLOAD;
	}

	if(v->getInfo() != AIRPORT) {

		int late = 0;

		if(time > v->getMinTime()) {
			cout << "----------- is late on: " << convertVertexInfo(v->getInfo()) << " by " << convertTime(time - v->getMinTime()) << " min" << endl;
			return LATE;
		}
		if((late = anyWillBeLate(g, s, v, time)) > 0) {
			//cout << "----------- would be late: " << late << " if: " << v->getInfo() << " was picked up"<< endl;
			return WOULD_BE_LATE;
		}
		if(time < (v->getMinTime() - g.overhead)) {
			cout << "----------- overhead on: " << convertVertexInfo(v->getInfo()) << " by " << convertTime(v->getMinTime() - g.overhead - time)  << " min" << endl;
			return OVERHEAD;
		}
	}

	v->setVisited(true);
	v->setPickupTime(time);
	s.push(v);

	Vertex<int> *temp;
	int timeBetween;

	for(size_t i = 0; i < g.getVertexSet().size(); i++) {

		temp = g.getVertexSet()[i];

		if(temp->getInfo() != v->getInfo()) {
			timeBetween = getTimeBetween(g, v->getInfo(), temp->getInfo());

			if(timeBetween > 0) {
				switch(scenario2_recursive(g, temp, time + timeBetween, s)) {
				case VISITED:
					break;
				case LATE:
					break;
				case VAN_OVERLOAD:
					return VAN_OVERLOAD;
				case SUCCESS:
					return SUCCESS;
				default:
					break;
				}
			}
		}
	}

	return 0;
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

		if(temp->getInfo() != AIRPORT) {
			timeBetween = getTimeBetween(g, AIRPORT, temp->getInfo());

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
	int trips = 0;
	int tripsWithSolution = 0;

	bool full = false;

	// Put the airport in the stack.
	temp->setVisited(true);
	temp->setPickupTime(time);
	s.push(temp);

	while(true){
		full = false;

		for(size_t i = 0; i < g.getVertexSet().size(); i++ ) {
			temp = g.getVertexSet()[i];

			if(temp->getInfo() != AIRPORT) {
				timeBetween = getTimeBetween(g, AIRPORT, temp->getInfo());

				if(timeBetween > 0) {

					switch(scenario2_recursive(g, temp, time + timeBetween , s)) {
					case VISITED:
						break;
					case SUCCESS:
						return SUCCESS;
					case LATE:
						break;
					case VAN_OVERLOAD:
						full = true;
						break;
					default:
						break;
					}
				}

				if(full) break;

				if(s.top()->getInfo() != AIRPORT) break;

			}
		}

		if(s.top()->getInfo() == AIRPORT) {

			trips++;

			if(tripsWithSolution == (trips - 1)) {

				Vertex<int>* v = g.getVertex(findBestMinTime(g));

				if(time <= ( v->getMinTime() - getTimeBetween(g, 0, v->getInfo()))) {

					time = v->getMinTime() - getTimeBetween(g, 0, v->getInfo()) - g.overhead;

					g.setTime(time);
					g.getVertex(0)->setPickupTime(time);

				} else {
					return 0;
				}
			}
			else {
				return 0;
			}

		}
		else { // PARTIAL SOLUTION

			time = getDropOffTime(g, s);

			printStack(s);
			eraseStack(s);

			g.getVertex(AIRPORT)->setPickupTime(time);
			g.resetVanSeats();

			trips++;
			tripsWithSolution++;
		}

	}

	return 0;
}

int getDropOffTime(Graph<int> &g, stack<Vertex<int> *> s) {
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

	cout << endl << "---------- BEGIN OF SCENARIO 1 ---------" << endl << endl;

	g.resetVanSeats();

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

	if(!isViable(g, true)) {
		return;
	}

	// Begin the finding of a solution.
	scenario1(g, g.getTime(), s);

	// Check the results.
	if(s.top()->getInfo() == 0) {
		cout << "No solution found!" << endl;
	}
	else {
		printStack(s);
		getDropOffTime(g,s);
	}

	cout << endl << "----------- END OF SCENARIO 1 ----------" << endl << endl;

}

void findSol_2(Graph<int> &g) {

	g.resetVanSeats();

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

	g.sortVertex();

	if(!isViable(g, true)) {
		return;
	}

	// Begin the finding of a solution.
	scenario2(g, g.getTime(), s);

	// Check the results.
	if(s.top()->getInfo() == 0) {
		cout << "No solution found!" << endl;
	}
	else {
		printStack(s);
		getDropOffTime(g, s);
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

int findBestTime(Graph<int> &g, bool debug) {
	int best = INT_MAX;
	int id = 0;
	vector<Vertex<int> *> v = g.getVertexSet();
	for(size_t i = 0; i < v.size(); i++) {
		if(!v[i]->isVisited()) {
			if(v[i]->totalTime <= best){
				best = v[i]->totalTime;
				id = v[i]->getInfo();
			}
		}
	}

	if(debug) cout << "Best min time: " << best << endl;

	if(best != INT_MAX) {
		return id;
	} else {
		return -1;
	}
}

bool isViable(Graph<int> &g, bool debug) {

	vector<Vertex<int> *> v = g.getVertexSet();
	for(size_t i = 0; i < v.size(); i++) {

		// Verifica se algum dos vértices ultrapassa o limite da carrinha, em nr de pessoas.
		if(v[i]->getPedido().getNumPessoas() > g.seats) {
			if(debug) {
				cout << "Vertex " << v[i]->getInfo()
								<< " has too much passengers ("
								<< v[i]->getPedido().getNumPessoas()
								<< ")." << endl;
			}
			return false;
		}

		// Verifica se algum dos vértices tem um tempo mínimo inferior ao tempo atual.
		if(v[i]->getMinTime() < g.getTime() && v[i]->getMinTime() > 0) {
			if(debug) {
				cout << "Vertex " << v[i]->getInfo()
								<< " can never be on time ("
								<< v[i]->getMinTime()
								<< ")." << endl;
			}
			return false;
		}
	}

	// Se passar todas as condições, então é possível que seja viável.
	return true;
}
