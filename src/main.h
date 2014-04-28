/*
 * main.h
 *
 *  Created on: 28 de Abr de 2014
 *      Author: Lu�s
 */

#ifndef MAIN_H_
#define MAIN_H_

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
 * Verifica se o grafo inicial poder� levar a uma solu��o vi�vel.
 *
 * No caso de um dos v�rtices ter um n�mero de passageiros superior ao limite da carrinha
 * ou no caso de um dos v�rtices ter uma hora m�nima superior ao tempo inicial, considera-se �
 * partida que n�o existir� uma solu��o possivel.
 *
 * @param g : Grafo
 * @param debug : Imprime mensagens se for true.
 *
 * @return True, se for poss�vel chegar a uma solu��o.
 * @return False, se n�o for poss�vel chegar a uma solu��o.
 *
 */
bool isViable(Graph<int> &g, bool debug = false);

/**
 * Retorna o tempo (peso) entre dois vertices.
 *
 * @param g : Grafo
 * @param v1 : Origem
 * @param v2 : Destino
 *
 * @return Distancia entre os vertices.
 */
int getTimeBetween(Graph<int> &g, const int v1, const int v2, bool debug = false);

/**
 * Imprime e devolve o tempo em que o cliente foi deixado no aeroporto (ponto inicial).
 *
 * @param g : Grafo.
 * @param s : Stack de recolhas.
 */
int getDropOffTime(Graph<int> &g, stack<Vertex<int> *> s);

/**
 * Define para cada vertice o tempo minimo que o autocarro tem de chegar
 * para conseguir cumprir a hora pretendida.
 *
 * @param g : Grafo
 */
void setVertexLimitTime(Graph<int> &g, bool debug = false);

/**
 * "Remove" todos os vertices que nao tem pedidos, por exemplo, os vertices de ligacao.
 * Esta remocao e feita mudando a flag 'visited' do vertice para true.
 *
 * @param g : Grafo
 */
void takeOutNonRequests(Graph<int> &g, bool debug = false);

/**
 * Testa se algum dos clientes ira chegar atrasado ao aeroporto, considerando o estado actual da execucao.
 *
 * @param g : Grafo
 * @param s : Estado da stack de clientes atuais
 * @param actual : Posicao atual da carrinha no grafo
 * @param time : Tempo atual
 *
 * @return 0, se ninguem chegar atrasado, caso contrario retorna o indice do que chegar atrasado.
 */
int anyWillBeLate(Graph<int> &g, stack<Vertex<int> *> s, Vertex<int>* actual, int time, bool debug = false);

/**
 * Encontra o menor tempo minimo existente no grafo, nos clientes nao visitados.
 *
 * @param g : Grafo.
 */
int findBestMinTime(Graph<int> &g, bool debug = false);

/**
 * Copia uma stack para a outra, de forma inversa.
 *
 * @param from : Stack original
 * @param to : Stack inversa
 */
template <class T>
void reverseStack(stack<T> &from, stack<T> &to);

/**
 * Limpa a stack.
 *
 * @param s : Stack a limpar.
 */
template <class T>
void eraseStack(stack<T> &s);

/**
 * Converte o tempo passado para horas e minutos (legivel). O tempo passado � de minutos desde as 0h.
 *
 * @param time : Tempo passado.
 */
string convertTime(int time);

/**
 * Converte a info de um vertice para um formato legivel.
 *
 * @param info : Informacao do vertice.
 */
string convertVertexInfo(int info);

/**
 * Imprime uma stack de inteiros.
 *
 * @param s : Stack a imprimir.
 */
void printStack(stack<int> s);

/**
 * Imprime uma stack de Vertex<int> *.
 *
 * @param s : Stack a imprimir.
 */
void printStack(stack<Vertex<int> *> s);

/**
 * Imprime uma stack de Vertex<int> * e os tempos de pickup respectivos (na stack<int>).
 *
 * @param s : Stack de Vertex<int> * a imprimir.
 * @param p : Stack de int a imprimir.
 */
void printStack(stack<Vertex<int> *> s, stack<int> p);

/**
 * Imprime uma matriz de v�rtices.
 *
 * @param v : matriz a imprimir
 */
void printMultiVector(vector<vector<Vertex<int> *> > &v);

/**
 * Analisa um conjunto de solu��es e imprime a melhor.
 *
 * @param solutions : vetor de solu��es
 * @param pickups : vetor associado �s solu��es, com a hora em cada pickup
 * @param vans : vetor com o n�mero de carrinhas usado em cada solu��o
 */
void printBestSolution(const vector<stack<Vertex<int> *> > &solutions, const vector<stack<int> > &pickups, const vector<int> &vans);

/**
 * Elimina todos os v�rtices inacess�veis a partir de outro qualquer,
 *   poupando assim c�lculos desnecess�rios mais � frente no processo.
 *   Um v�rtice 'A' � inacessivel a partir de um v�rtice 'B' se:
 *     - apanhando o servi�o de B no tempo limite de overhead a carrinha j� chegue a A atrasada.
 *     - apanhando o servi�o de B no tempo limite a carrinha chegue a A em overhead.
 *
 * @param g : grafo
 * @param v : matriz de v�rtices
 */
void calculatePossiblePaths(Graph<int> &g, vector<vector<Vertex<int> *> > &v);

/**
 * Funcao recursiva para a resoluacao do cenario 1.
 *
 * @param g : Grafo
 * @param v : Vertice actual
 * @param time : Tempo actual
 * @param s : Stack de recolhas
 */
int scenario1_recursive(Graph<int> &g, Vertex<int>* v, int time, stack<Vertex<int> *> &s);

/**
 * Inicializacao para o cenario 1.
 *
 * @param g : Grafo
 * @param time : Tempo actual
 * @param s : Stack de recolhas
 */
int scenario1_firstcall(Graph<int> &g, int time, stack<Vertex<int> *> &s);

/**
 * Chamada da resolucao do grafo para o cenario 1.
 *
 * @param g : Grafo a resolver.
 */
void scenario1(Graph<int> &g);

/**
 * Funcao recursiva para a resoluacao do cenario 2.
 *
 * @param g : Grafo
 * @param v : Vertice actual
 * @param time : Tempo actual
 * @param s : Stack de recolhas
 */
int scenario2_recursive(Graph<int> &g, Vertex<int>* v, int time, stack<Vertex<int> *> &s);

/**
 * Inicializacao para o cenario 2.
 *
 * @param g : Grafo
 * @param time : Tempo actual
 * @param s : Stack de recolhas
 */
int scenario2_firstcall(Graph<int> &g, int time, stack<Vertex<int> *> &s);

/**
 * Chamada da resolucao do grafo para o cenario 2.
 *
 * @param g : Grafo a resolver.
 */
void scenario2(Graph<int> &g);

/**
 * Verifica se vertice esta em alguma das solucoes.
 *
 * @param v : Vertice.
 * @param solutions : Stack de solucoes.
 */
bool inAnySolution(Vertex<int> *v, vector<stack<Vertex<int> *> > solutions);

/**
 * Verifica qual o v�rtice mais pr�ximo do v�rtice no �ndice 0.
 *   A procura � feita no outros v�rtices do vetor dado.
 *
 *   @param g : Grafo
 *   @param v : vetor de v�rtices poss�veis.
 *
 *   @return o �ndice do v�rtice mais pr�ximo, -1 em caso de erro.
 */
int getClosest(Graph<int> &g, const vector<Vertex<int> *> &v);

/**
 * Verifica qual o v�rtice mais pr�ximo do aeroporto.
 *   A procura � feita na primeira coluna de cada linha da matriz v.
 *
 *   @param g : Grafo
 *   @param v : matriz de v�rtices poss�veis.
 *   @param solutions : vetor de solu��es j� obtidas.
 *   @param first : verdade se for a primeira itera��o.
 *
 *   @return o �ndice do v�rtice mais pr�ximo, -1 em caso de erro.
 */
int getClosest(Graph<int> &g, const vector<vector<Vertex<int> *> > &v, const vector<stack<Vertex<int> *> > &solutions, bool first);

/**
 * Verifica a validade de um v�rtice para uma solu��o.
 *   Um v�rtice � inv�lido se:
 *     - o v�rtice j� foi visitado;
 *     - a carrinha j� est� atrasada para o servi�o desse v�rtice;
 *     - a carrinha est� em overhead em rela��o ao servi�o do v�rtice;
 *     - o n�mero de pessoas do servi�o ultrapassam a capacidade da carrinha.
 *
 * @param g : Grafo
 * @param from : v�rtice anterior
 * @param to : v�rtice a testar
 * @param time : tempo no v�rtice a testar
 *
 * @return true se o v�rtice � v�lido
 */
bool isValid(Graph<int> &g, Vertex<int> *from, Vertex<int> *to, int time);

/**
 * Verifica se um dado v�rtice j� est� presente na solu��o atual.
 *
 * @param v : v�rtice a testar
 * @param s : stack com a solu��o atual
 *
 * @return true se est� na solu��o
 */
bool inSolution(Vertex<int> *v, stack<Vertex<int> *> s);

/**
 * Verifica se o v�rtice dado j� foi utilizado com v�rtice de come�o em alguma solu��o.
 *
 * @param v : v�rtice a testar
 * @param solutions : vetor com as solu��es j� obtidas
 *
 * @return true se est� em alguma solu��o
 */
bool inAnySolution(Vertex<int> *v, vector<stack<Vertex<int> *> > solutions);

/**
 * Coloca todos os v�rtices que cont�m um servi�o em cada linha da matriz v.
 *
 * @param g : grafo
 * @param v : matriz de v�rtices e liga��es poss�veis
 */
void putServicesInVector(Graph<int> &g, vector<vector<Vertex<int> *> > &v);

/**
 * Funcao recursiva para a resoluacao do cenario 3.
 *
 * @param g : Grafo
 * @param matrix : matriz de v�rtices e liga��es poss�veis
 * @param v : vetor de v�rtices poss�veis a partir do atual
 * @param s : stack com a solu��o atual (v�rtices)
 * @param p : stack com a solu��o atual (tempos de pickup)
 * @param time : tempo no v�rtice atual
 */
void scenario3_recursive(Graph<int> &g, vector<vector<Vertex<int> *> > &matrix, vector<Vertex<int> *> v, stack<Vertex<int> *> &s, stack<int> &p, int time);

/**
 * Inicializacao para o cenario 3.
 *
 * @param g : Grafo
 * @param v : matriz de v�rtices e liga��es poss�veis
 */
void scenario3_firstcall(Graph<int> &g, vector<vector<Vertex<int> *> > &v);

/**
 * Chamada da resolucao do grafo para o cenario 3.
 *
 * @param g : Grafo a resolver.
 */
void scenario3(Graph<int> &g);


#endif /* MAIN_H_ */
