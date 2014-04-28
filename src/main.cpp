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
#include <algorithm>

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
template <class T>
void reverseStack(stack<T> &from, stack<T> &to);

/**
 * Limpa a stack.
 *
 * @param s Stack a limpar.
 */
template <class T>
void eraseStack(stack<T> &s);

/**
 * Converte o tempo passado para horas e minutos (legivel). O tempo passado é de minutos desde as 0h.
 *
 * @param time Tempo passado.
 */
string convertTime(int time);

/**
 * Converte a info de um vertice para um formato legivel.
 *
 * @param info Informacao do vertice.
 */
string convertVertexInfo(int info);

/**
 * Imprime uma stack.
 *
 * @param s Stack a imprimir.
 */
void printStack(stack<Vertex<int> *> s);

/**
 * Imprime e devolve o tempo em que o cliente foi deixado no aeroporto (ponto inicial).
 *
 * @param g Grafo.
 * @param s Stack de recolhas.
 */
int getDropOffTime(Graph<int> &g, stack<Vertex<int> *> s);

/**
 * Encontra o menor tempo minimo existente no grafo, nos clientes nao visitados.
 *
 * @param g Grafo.
 */
int findBestMinTime(Graph<int> &g, bool debug = false);
int findBestTime(Graph<int> &g, bool debug = false);

/**
 * Funcao recursiva para a resoluacao do cenario 1.
 *
 * @param g Grafo
 * @param v Vertice actual
 * @param time Tempo actual
 * @param s Stack de recolhas
 */
int scenario1_recursive(Graph<int> &g, Vertex<int>* v, int time, stack<Vertex<int> *> &s);
/**
 * Funcao recursiva para a resoluacao do cenario 2.
 *
 * @param g Grafo
 * @param v Vertice actual
 * @param time Tempo actual
 * @param s Stack de recolhas
 */
int scenario2_recursive(Graph<int> &g, Vertex<int>* v, int time, stack<Vertex<int> *> &s);

/**
 * Inicializacao para o cenario 1.
 *
 * @param g Grafo
 * @param time Tempo actual
 * @param s Stack de recolhas
 */
int scenario1_firstcall(Graph<int> &g, int time, stack<Vertex<int> *> &s);
/**
 * Inicializacao para o cenario 2.
 *
 * @param g Grafo
 * @param time Tempo actual
 * @param s Stack de recolhas
 */
int scenario2_firstcall(Graph<int> &g, int time, stack<Vertex<int> *> &s);

/**
 * Chamada da resolucao do grafo para o cenario 1.
 *
 * @param g Grafo a resolver.
 */
void scenario1(Graph<int> &g);
/**
 * Chamada da resolucao do grafo para o cenario 2.
 *
 * @param g Grafo a resolver.
 */
void scenario2(Graph<int> &g);
/**
 * Chamada da resolucao do grafo para o cenario 3.
 *
 * @param g Grafo a resolver.
 */
void scenario3(Graph<int> &g);

/**
 * Verifica se vertice esta em alguma das solucoes.
 *
 * @param v Vertice.
 * @param solutions Stack de solucoes.
 */
bool inAnySolution(Vertex<int> *v, vector<stack<Vertex<int> *> > solutions);


// ____________________________
//
//		END OF HEADERS SECTION
// ____________________________
//

int main() {
	Graph<int> g1, g2, g3;


//	// Read test files for scenario 1
//	readFiles(g1,1,1);
//
//	// Find Solution for first scenario
//	scenario1(g1);



//	// Read test files for scenario 2
//	readFiles(g2,2,1);
//
//	// Find Solution for second scenario
//	scenario2(g2);



	// Read test files for scenario 3
	readFiles(g3,3,1);

	// Find Solution for second scenario
	scenario3(g3);


	return 0;
}

// _________________________
//
//		AUXILIARY FUNCTIONS
// _________________________
//

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

int findBestMinTime(Graph<int> &g, bool debug) {

	int minTime = INT_MAX;
	int id = 0;

	vector<Vertex<int> *> v = g.getVertexSet();

	for(size_t i = 0; i < v.size(); i++) {

		if(!v[i]->isVisited()) {

			if(v[i]->getMinTime() < minTime){
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
		if((v[i]->getMinTime() < g.getTime()) && (v[i]->getMinTime() > 0)) {
			if(debug) {
				cout << convertVertexInfo(v[i]->getInfo())
								<< " can never be on time ("
								<< convertTime(v[i]->getMinTime())
								<< ")." << endl;
			}
			return false;
		}
	}

	// Se passar todas as condições, então é possível que seja viável.
	return true;
}

template <class T>
void reverseStack(stack<T> &from, stack<T> &to) {
	while(!from.empty()) {
		to.push(from.top());
		from.pop();
	}
}

template <class T>
void eraseStack(stack<T> &s) {
	while(!s.empty()) {
		s.pop();
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

void printStack(stack<int> s) {
	stack<int> r;

	reverseStack(s,r);

	cout << "-- stack --" << endl;
	while(!r.empty()) {
		cout << r.top();
		r.pop();
	}
	cout << "-----------" << endl;
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

void printStack(stack<Vertex<int> *> s, stack<int> p) {
	stack<Vertex<int> *> r;
	stack<int> pr;

	reverseStack(s,r);
	reverseStack(p,pr);

	bool start = true;

	int overheadLimit;
	int lateLimit;

	cout << "-- stack --" << endl;
	while(!r.empty()) {
		if(r.top()->getInfo() == AIRPORT) {
			if(start) {
				cout << "Start at: " << convertTime(pr.top()) << endl;
			}
			else {
				cout << "Dropoff at: " << convertTime(pr.top()) << endl;
			}
			start = !start;
		}
		else {
			lateLimit = r.top()->getMinTime();
			overheadLimit = lateLimit - OVERHEAD;

			cout << " --> on ";
			cout << convertVertexInfo(r.top()->getInfo());
			cout << " at: ";
			cout << convertTime(pr.top());
			cout << "  ["
					<< convertTime(overheadLimit)
					<< ", "
					<< convertTime(lateLimit)
					<< "]" << endl;
		}

		r.pop();
		pr.pop();
	}
	cout << "-----------" << endl << endl;
}

// ____________________________________
//
//		SCENARIO 1 : ONE VAN, ONE RIDE
// ____________________________________
//

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

int scenario1_firstcall(Graph<int> &g, int time, stack<Vertex<int> *> &s) {

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

void scenario1(Graph<int> &g) {

	cout << endl << "------------ BEGIN OF SCENARIO 1 -----------" << endl << endl;

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
	scenario1_firstcall(g, g.getTime(), s);

	// Check the results.
	if(s.top()->getInfo() == 0) {
		cout << "No solution found!" << endl;
	}
	else {
		printStack(s);
		getDropOffTime(g,s);
	}

	cout << endl << "------------- END OF SCENARIO 1 ------------" << endl << endl;

}

// __________________________________________
//
//		SCENARIO 2 : ONE VAN, INFINITE RIDES
// __________________________________________
//

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

int scenario2_firstcall(Graph<int> &g, int time, stack<Vertex<int> *> &s) {

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
			s.push(g.getVertex(AIRPORT));

			g.getVertex(AIRPORT)->setPickupTime(time);
			g.resetVanSeats();

			trips++;
			tripsWithSolution++;
		}

	}

	return 0;
}

void scenario2(Graph<int> &g) {

	cout << endl << "------------ BEGIN OF SCENARIO 2 -----------" << endl << endl;

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

	//g.sortVertex();

	if(!isViable(g, true)) {
		return;
	}

	// Begin the finding of a solution.
	scenario2_firstcall(g, g.getTime(), s);

	// Check the results.
	if(s.top()->getInfo() == 0) {
		cout << "No solution found!" << endl;
	}
	else {
		printStack(s);
		getDropOffTime(g, s);
	}

	cout << endl << "------------- END OF SCENARIO 2 ------------" << endl << endl;
}

// ________________________________________________
//
//		SCENARIO 3 : INFINITE VANS, INFINITE RIDES
// ________________________________________________
//

//int scenario3_firstcall(Graph<int> &g) {
//	vector<Vertex<int> *> sol;
//	vector<int> pickups;
//
//	Vertex<int>* v;
//	Vertex<int>* airport;
//
//	size_t count = 0;
//	int numVans;
//	int time = g.getTime();
//	int limitTime = INT_MAX;
//
//	bool valid;
//
//	airport = g.getVertex(AIRPORT);
//	sol.push_back(airport);
//	pickups.push_back(time);
//
//	numVans = 1;
//
//	while(!g.allAreVisited()) {
//
//		if(getBestChoice(g, sol, time, limitTime, pickups)) {
//			valid = true;
//			count++;
//			cout << "->" << count << " : " << convertVertexInfo(sol[sol.size() - 1]->getInfo()) << endl;
//		}
//		else {
//			if(valid) {
//				valid = false;
//				cout << "New van!" << endl;
//				numVans++;
//				g.resetVanSeats();
//
//				//cout << "Vans:" << numVans << endl;
//
//				limitTime = INT_MAX;
//
//				v = g.getVertex(findBestMinTime(g));
//
//				time = v->getMinTime() - getTimeBetween(g, AIRPORT, v->getInfo()) - g.overhead;
//				sol.push_back(airport);
//				pickups.push_back(time);
//			}
//			else {
//				cout << "Something wrong!" << endl;
//				break;
//			}
//		}
//	}
//
//	int van = 1;
//
//	for(size_t i = 0; i < sol.size(); i++) {
//		if(sol[i]->getInfo() == AIRPORT) {
//			cout << endl << "Van " << van << ":" << endl;
//			van++;
//		}
//		cout << convertVertexInfo(sol[i]->getInfo()) << " -> " << convertTime(pickups[i]) << endl;
//	}
//
//	return 0;
//}

int getClosest(Graph<int> &g, const vector<Vertex<int> *> &v) {

	int timeBetween;
	int min = INT_MAX;

	int closest = -1;

	for(size_t i = 1; i < v.size(); i++) {

		if(v[i]->isVisited()) {
			continue;
		}

		timeBetween = g.getTimeBetween(v[0]->getInfo(), v[i]->getInfo());

		if(timeBetween < min) {
			min = timeBetween;
			closest = i;
		}
	}

	return closest;
}

int getClosest(Graph<int> &g, const vector<vector<Vertex<int> *> > &v, const vector<stack<Vertex<int> *> > &solutions, bool first) {

	int timeBetween;
	int min = INT_MAX;

	int closest = -1;

	//cout  << endl << "solution: " << solutions.size() + 1 << endl;

	for(size_t i = 0; i < v.size(); i++) {

		if(v[i][0]->isVisited()) {
			continue;
		}

		if(first) {
			if(inAnySolution(v[i][0], solutions)) {
				//cout << convertVertexInfo(v[i][0]->getInfo()) << " is in a solution" << endl;
				continue;
			}

		}

		timeBetween = g.getTimeBetween(AIRPORT, v[i][0]->getInfo());

		if(timeBetween < min) {
			min = timeBetween;
			closest = i;
		}
	}

	return closest;
}

bool isValid(Graph<int> &g, Vertex<int> *from, Vertex<int> *to, int time) {

	if(to->isVisited()) {
		return false;
	}

	if(time > to->getMinTime()) {
		//cout << "----------- is late on: " << convertVertexInfo(to->getInfo()) << " by " << convertTime(time - to->getMinTime()) << " min" << endl;
		return false;
	}
	if(time < (to->getMinTime() - g.overhead)) {
		//cout << "----------- overhead on: " << convertVertexInfo(to->getInfo()) << " by " << convertTime(to->getMinTime() - g.overhead - time)  << " min" << endl;
		return false;
	}

	if(g.usedSeats + to->getPedido().getNumPessoas() > g.seats) {
		return false;
	}

	return true;
}

bool inSolution(Vertex<int> *v, stack<Vertex<int> *> s) {

	while(!s.empty()) {
		if(s.top() == v) {
			return true;
		}
		s.pop();
	}

	return false;
}

bool inAnySolution(Vertex<int> *v, vector<stack<Vertex<int> *> > solutions) {
	stack<Vertex<int> *> r;

	for(size_t i = 0; i < solutions.size(); i++) {
		reverseStack(solutions[i], r);
		r.pop();

		//cout << "r.top(): " << convertVertexInfo(r.top()->getInfo()) << endl;

		if(r.top() == v) {
			return true;
		}

		eraseStack(r);
	}

	return false;
}

void putPickupsInVector(Graph<int> &g, vector<vector<Vertex<int> *> > &v) {
	Vertex<int> *tmp;

	for(size_t i = 0; i < g.getVertexSet().size(); i++) {
		tmp = g.getVertexSet()[i];

		if(!tmp->isVisited()) {
			vector<Vertex<int> *> vec;
			vec.push_back(tmp);
			v.push_back(vec);
		}
	}
}

void printMultiVector(vector<vector<Vertex<int> *> > &v) {

	cout << "-- vector --" << endl;

	for(size_t i = 0; i < v.size(); i++) {
		cout << "[ ";

		for(size_t j = 0; j < v[i].size(); j++) {

			cout << convertVertexInfo(v[i][j]->getInfo()) << " ";
		}
		cout << "]" << endl;
	}
	cout << "------------" << endl;
	cout << endl;
}

void printBestSolution(const vector<stack<Vertex<int> *> > &solutions, const vector<stack<int> > &pickups, const vector<int> &vans) {
	int min = INT_MAX;
	int bestIndex = -1;

	for(size_t i = 0; i < vans.size(); i++) {

		cout << "Solution " << (i + 1) << " used: " << vans[i] << " vans." << endl;

		if(vans[i] < min) {
			min = vans[i];
			bestIndex = i;
		}
	}

	if(bestIndex == -1) {
		cerr << "ERROR! on printBestSolution(): bestIndex = -1" << endl;
		return;
	}

	cout << endl;
	cout << "-+-+-+-+-+-+-+-+-+-+-+-+-+-" << endl;
	cout << "------ BEST SOLUTION ------" << endl;
	cout << "Vans used: " << vans[bestIndex] << endl << endl;
	printStack(solutions[bestIndex], pickups[bestIndex]);
	cout << "---------------------------" << endl;
	cout << "-+-+-+-+-+-+-+-+-+-+-+-+-+-" << endl;
}

void calculatePossiblePaths(Graph<int> &g, vector<vector<Vertex<int> *> > &v) {
	Vertex<int> *tmp;
	Vertex<int> *cmp;

	int limitInterval;
	int timeBetween;

	for(size_t i = 0; i < v.size(); i++) {
		tmp = v[i][0];

		for(size_t j = 0; j < g.getVertexSet().size(); j++) {
			cmp = g.getVertexSet()[j];

			if(cmp->getInfo() == AIRPORT) {
				continue;
			}

			if(cmp->getInfo() == tmp->getInfo()) {
				continue;
			}

			// eliminate overheads
			timeBetween = g.getTimeBetween(tmp->getInfo(), cmp->getInfo());

			limitInterval = cmp->getMinTime() - g.overhead;

			if(tmp->getMinTime() + timeBetween < limitInterval ) {
//				cout << "overhead: ("
//						<< convertVertexInfo(tmp->getInfo())
//						<< ", "
//						<< convertVertexInfo(cmp->getInfo())
//						<< ")" << endl;
				continue;
			}

			limitInterval = tmp->getMinTime() - g.overhead;

			if(cmp->getMinTime() + timeBetween < limitInterval ) {
//				cout << "overhead: ("
//						<< convertVertexInfo(cmp->getInfo())
//						<< ", "
//						<< convertVertexInfo(tmp->getInfo())
//						<< ")" << endl;
				continue;
			}

			// eliminate delays
			if(cmp->getMinTime() > tmp->getMinTime()) {

				limitInterval = tmp->getMinTime() - g.overhead + timeBetween;

				if(limitInterval > cmp->getMinTime()) {
//					cout << "delay: ("
//							<< convertVertexInfo(tmp->getInfo())
//							<< ", "
//							<< convertVertexInfo(cmp->getInfo())
//							<< ")" << endl;
					continue;
				}

			}

			// If it hasn't break it
			v[i].push_back(cmp);
		}
	}
}

void calculateSolution(Graph<int> &g, vector<vector<Vertex<int> *> > &matrix, vector<Vertex<int> *> v, stack<Vertex<int> *> &s, stack<int> &p, int time) {

	int timeBetween;
	int best;
	size_t chosen = 1;

	while(chosen < v.size()) {
		while(true) {
			best = getClosest(g, v);

			if(best == -1) {
				return;
			}

			if(!inSolution(v[best], s)) {
				break;
			}
		}

		chosen++;

		timeBetween = g.getTimeBetween(v[0]->getInfo(), v[best]->getInfo());

		if(isValid(g, v[0], v[best], time + timeBetween)) {

			s.push(v[best]);
			v[best]->setVisited(true);
			v[best]->setPickupTime(time + timeBetween);
			p.push(time + timeBetween);

			g.usedSeats += v[best]->getPedido().getNumPessoas();

			calculateSolution(g, matrix, matrix[v[best]->getInfo() - 1], s, p, time + timeBetween);
			return;
		}
	}
}

void calculateSolutions(Graph<int> &g, vector<vector<Vertex<int> *> > &v) {

	stack<Vertex<int> *> sol;
	stack<int> p;

	vector<stack<Vertex<int> *> > solutions;
	vector<stack<int> > pickups;
	vector<int> vans;

	int time = g.getTime();
	int timeBetween = 0;

	int best = -1;
	int numVans = 0;

	while(solutions.size() < v.size()) {

		while(!g.allAreVisited()) {

			if(sol.size() > 0) {
				best = getClosest(g, v, solutions, false);
			}
			else {
				best = getClosest(g, v, solutions, true);
			}


			if(best == -1) {
				break;
			}

			//cout << "best: " << convertVertexInfo(v[best][0]->getInfo()) << endl;

			time = v[best][0]->getMinTime() - g.overhead - g.getTimeBetween(AIRPORT, v[best][0]->getInfo());

			sol.push(g.getVertex(AIRPORT));
			p.push(time);

			timeBetween = g.getTimeBetween(AIRPORT, v[best][0]->getInfo());

			sol.push(v[best][0]);
			v[best][0]->setVisited(true);
			v[best][0]->setPickupTime(time + timeBetween);
			p.push(time + timeBetween);

			calculateSolution(g, v, v[best], sol, p, time + timeBetween);

			time = p.top();

			timeBetween = g.getTimeBetween(AIRPORT, sol.top()->getInfo());

			time += timeBetween;

			sol.push(g.getVertex(AIRPORT));
			p.push(time);

			//printStack(sol, p);

			numVans++;
			g.resetVanSeats();
		}

		// SOLUTION ACHIEVED

		//printStack(sol, p);

		solutions.push_back(sol);
		pickups.push_back(p);
		vans.push_back(numVans);

		eraseStack(sol);
		eraseStack(p);
		numVans = 0;

		time = g.getTime();
		g.setVisited(false);
		g.resetVanSeats();
		takeOutNonRequests(g);

		//cout << "solutions: " << solutions.size() << " of " << v.size() << endl;

		if(numVans == 1) {
			// Best solution possible.
			break;
		}
	}

	printBestSolution(solutions, pickups, vans);
}

void scenario3(Graph<int> &g) {

	cout << endl << "------------ BEGIN OF SCENARIO 3 -----------" << endl << endl;

	g.resetVanSeats();

	// Initializes graph vertex visited to false.
	g.setVisited(false);

	// Run FloydWarshall's algorithm to get the shortest paths from any vertex to another.
	g.floydWarshallShortestPath();

	// Set the limit time for each pickup.
	setVertexLimitTime(g);

	// Set the initial time: 7h00
	g.setTime(0);

	// Set any empty vertex (without pickup) to visited.
	takeOutNonRequests(g);

	if(!isViable(g, true)) {
		return;
	}

	// --------------------------------------------

	vector<vector<Vertex<int> *> > v;

	g.printGraphMinTime();

	putPickupsInVector(g, v);

	//printMultiVector(v);

	calculatePossiblePaths(g, v);

	printMultiVector(v);

	calculateSolutions(g, v);

	cout << endl << "------------- END OF SCENARIO 3 ------------" << endl << endl;
}
