/*
 * main.h
 *
 *  Created on: 28 de Abr de 2014
 *      Author: Luís
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
 * Verifica se o grafo inicial poderá levar a uma solução viável.
 *
 * No caso de um dos vértices ter um número de passageiros superior ao limite da carrinha
 * ou no caso de um dos vértices ter uma hora mínima superior ao tempo inicial, considera-se à
 * partida que não existirá uma solução possivel.
 *
 * @param g : Grafo
 * @param debug : Imprime mensagens se for true.
 *
 * @return True, se for possível chegar a uma solução.
 * @return False, se não for possível chegar a uma solução.
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
 * Converte o tempo passado para horas e minutos (legivel). O tempo passado é de minutos desde as 0h.
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
 * Imprime uma matriz de vértices.
 *
 * @param v : matriz a imprimir
 */
void printMultiVector(vector<vector<Vertex<int> *> > &v);

/**
 * Analisa um conjunto de soluções e imprime a melhor.
 *
 * @param solutions : vetor de soluções
 * @param pickups : vetor associado às soluções, com a hora em cada pickup
 * @param vans : vetor com o número de carrinhas usado em cada solução
 */
void printBestSolution(const vector<stack<Vertex<int> *> > &solutions, const vector<stack<int> > &pickups, const vector<int> &vans);

/**
 * Elimina todos os vértices inacessíveis a partir de outro qualquer,
 *   poupando assim cálculos desnecessários mais à frente no processo.
 *   Um vértice 'A' é inacessivel a partir de um vértice 'B' se:
 *     - apanhando o serviço de B no tempo limite de overhead a carrinha já chegue a A atrasada.
 *     - apanhando o serviço de B no tempo limite a carrinha chegue a A em overhead.
 *
 * @param g : grafo
 * @param v : matriz de vértices
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
 * Verifica qual o vértice mais próximo do vértice no índice 0.
 *   A procura é feita no outros vértices do vetor dado.
 *
 *   @param g : Grafo
 *   @param v : vetor de vértices possíveis.
 *
 *   @return o índice do vértice mais próximo, -1 em caso de erro.
 */
int getClosest(Graph<int> &g, const vector<Vertex<int> *> &v);

/**
 * Verifica qual o vértice mais próximo do aeroporto.
 *   A procura é feita na primeira coluna de cada linha da matriz v.
 *
 *   @param g : Grafo
 *   @param v : matriz de vértices possíveis.
 *   @param solutions : vetor de soluções já obtidas.
 *   @param first : verdade se for a primeira iteração.
 *
 *   @return o índice do vértice mais próximo, -1 em caso de erro.
 */
int getClosest(Graph<int> &g, const vector<vector<Vertex<int> *> > &v, const vector<stack<Vertex<int> *> > &solutions, bool first);

/**
 * Verifica a validade de um vértice para uma solução.
 *   Um vértice é inválido se:
 *     - o vértice já foi visitado;
 *     - a carrinha já está atrasada para o serviço desse vértice;
 *     - a carrinha está em overhead em relação ao serviço do vértice;
 *     - o número de pessoas do serviço ultrapassam a capacidade da carrinha.
 *
 * @param g : Grafo
 * @param from : vértice anterior
 * @param to : vértice a testar
 * @param time : tempo no vértice a testar
 *
 * @return true se o vértice é válido
 */
bool isValid(Graph<int> &g, Vertex<int> *from, Vertex<int> *to, int time);

/**
 * Verifica se um dado vértice já está presente na solução atual.
 *
 * @param v : vértice a testar
 * @param s : stack com a solução atual
 *
 * @return true se está na solução
 */
bool inSolution(Vertex<int> *v, stack<Vertex<int> *> s);

/**
 * Verifica se o vértice dado já foi utilizado com vértice de começo em alguma solução.
 *
 * @param v : vértice a testar
 * @param solutions : vetor com as soluções já obtidas
 *
 * @return true se está em alguma solução
 */
bool inAnySolution(Vertex<int> *v, vector<stack<Vertex<int> *> > solutions);

/**
 * Coloca todos os vértices que contém um serviço em cada linha da matriz v.
 *
 * @param g : grafo
 * @param v : matriz de vértices e ligações possíveis
 */
void putServicesInVector(Graph<int> &g, vector<vector<Vertex<int> *> > &v);

/**
 * Funcao recursiva para a resoluacao do cenario 3.
 *
 * @param g : Grafo
 * @param matrix : matriz de vértices e ligações possíveis
 * @param v : vetor de vértices possíveis a partir do atual
 * @param s : stack com a solução atual (vértices)
 * @param p : stack com a solução atual (tempos de pickup)
 * @param time : tempo no vértice atual
 */
void scenario3_recursive(Graph<int> &g, vector<vector<Vertex<int> *> > &matrix, vector<Vertex<int> *> v, stack<Vertex<int> *> &s, stack<int> &p, int time);

/**
 * Inicializacao para o cenario 3.
 *
 * @param g : Grafo
 * @param v : matriz de vértices e ligações possíveis
 */
void scenario3_firstcall(Graph<int> &g, vector<vector<Vertex<int> *> > &v);

/**
 * Chamada da resolucao do grafo para o cenario 3.
 *
 * @param g : Grafo a resolver.
 */
void scenario3(Graph<int> &g);


#endif /* MAIN_H_ */
