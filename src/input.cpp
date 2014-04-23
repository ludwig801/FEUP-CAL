/*
 * input.cpp
 *
 *  Created on: 23 de Abr de 2014
 *      Author: Luís
 */

#include "input.h"

using namespace std;

void readVertex(Graph<int> &g, GraphViewer *gv) {
	ifstream inFile;

	//Ler o ficheiro nos.txt
	inFile.open("nos.txt");

	if (!inFile) {
		cerr << "Unable to open file datafile.txt";
		exit(1);   // call system to stop
	}

	std::string   line;

	int idNo=0;
	int X=0;
	int Y=0;

	while(std::getline(inFile, line))
	{
		std::stringstream linestream(line);
		std::string         data;

		linestream >> idNo;

		std::getline(linestream, data, ';');  // read up-to the first ; (discard ;).
		linestream >> X;
		std::getline(linestream, data, ';');  // read up-to the first ; (discard ;).
		linestream >> Y;
		gv->addNode(idNo,X,Y);

		g.addVertex(idNo);

	}

	inFile.close();
}

void readEdges(Graph<int> &g, GraphViewer *gv) {
	ifstream inFile;
	std::string line;

	//Ler o ficheiro arestas.txt
	inFile.open("arestas.txt");

	if (!inFile) {
		cerr << "Unable to open file datafile.txt";
		exit(1);   // call system to stop
	}

	int idAresta=0;
	int peso=0;
	int idNoOrigem=0;
	int idNoDestino=0;

	while(std::getline(inFile, line))
	{
		std::stringstream linestream(line);
		std::string data;


		linestream >> idAresta;

		std::getline(linestream, data, ';');  // read up-to the first ; (discard ;).
		linestream >> peso;
		std::getline(linestream, data, ';');  // read up-to the first ; (discard ;).
		linestream >> idNoOrigem;
		std::getline(linestream, data, ';');  // read up-to the first ; (discard ;).
		linestream >> idNoDestino;
		gv->addEdge(idAresta,idNoOrigem,idNoDestino, EdgeType::UNDIRECTED);

		g.addEdge(idNoOrigem,idNoDestino,peso);

		//cout << "Edge: (" << idNoOrigem << ", " << idNoDestino << ") - " << peso << endl;

	}

	inFile.close();
}

void readRequests(Graph<int> &g) {
	ifstream inFile;
	std::string line;

	//Ler o ficheiro arestas.txt
	inFile.open("pedidos.txt");

	if (!inFile) {
		cerr << "Unable to open file datafile.txt";
		exit(1);   // call system to stop
	}

	int idVertex=0;
	int hora=0;
	int numPessoas=0;

	while(std::getline(inFile, line))
	{
		std::stringstream linestream(line);
		std::string data;

		linestream >> idVertex;

		std::getline(linestream, data, ';');  // read up-to the first ; (discard ;).
		linestream >> hora;
		std::getline(linestream, data, ';');  // read up-to the first ; (discard ;).
		linestream >> numPessoas;

		g.addRequest(idVertex,Pedido(hora,numPessoas));

	}

	inFile.close();

}

void readFiles(Graph<int> &g) {

	GraphViewer *gv = new GraphViewer(600, 600, false);

	gv->createWindow(600, 600);

	gv->defineEdgeColor("blue");
	gv->defineVertexColor("yellow");

	readVertex(g,gv);

	readEdges(g,gv);

	readRequests(g);

	gv->rearrange();

	getchar();
}
