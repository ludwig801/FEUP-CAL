/*
 * input.cpp
 *
 *  Created on: 23 de Abr de 2014
 *      Author: Luís
 */

#include "input.h"

using namespace std;

void readDetails(Graph<int> &g, string s) {
	ifstream inFile;

	//Ler o ficheiro nos.txt
	inFile.open(s.c_str());

	if (!inFile) {
		cerr << "Unable to open file details.txt";
		exit(1);   // call system to stop
	}

	std::string   line;

	int overhead = 0;
	int seats = 0;

	while(std::getline(inFile, line))
	{
		std::stringstream linestream(line);
		std::string         data;

		linestream >> overhead;

		std::getline(linestream, data, ';');  // read up-to the first ; (discard ;).
		linestream >> seats;
	}

	g.seats = seats;
	g.overhead = overhead;

	inFile.close();

}

void readVertex(Graph<int> &g, GraphViewer *gv, string s) {
	ifstream inFile;

	//Ler o ficheiro nos.txt
	inFile.open(s.c_str());

	if (!inFile) {
		cerr << "Unable to open file " << s << endl;
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

void readEdges(Graph<int> &g, GraphViewer *gv, string s) {
	ifstream inFile;

	//Ler o ficheiro nos.txt
	inFile.open(s.c_str());

	if (!inFile) {
		cerr << "Unable to open file " << s << endl;
		exit(1);   // call system to stop
	}

	string line;

	int idAresta=0;
	int peso=0;
	int idNoOrigem=0;
	int idNoDestino=0;

	while(std::getline(inFile, line))
	{
		std::stringstream linestream(line);
		std::string data;

		std::stringstream s;

		linestream >> idAresta;

		std::getline(linestream, data, ';');  // read up-to the first ; (discard ;).
		linestream >> peso;
		std::getline(linestream, data, ';');  // read up-to the first ; (discard ;).
		linestream >> idNoOrigem;
		std::getline(linestream, data, ';');  // read up-to the first ; (discard ;).
		linestream >> idNoDestino;
		gv->addEdge(idAresta,idNoOrigem,idNoDestino, EdgeType::UNDIRECTED);

		s << peso;
		gv->setEdgeLabel(idAresta, s.str());

		s.str("");

		g.addEdge(idNoOrigem,idNoDestino,peso);

		//cout << "Edge: (" << idNoOrigem << ", " << idNoDestino << ") - " << peso << endl;

	}

	inFile.close();
}

void readRequests(Graph<int> &g, string s) {
	ifstream inFile;

	//Ler o ficheiro nos.txt
	inFile.open(s.c_str());

	if (!inFile) {
		cerr << "Unable to open file " << s << endl;
		exit(1);   // call system to stop
	}

	string line;

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

void readFiles(Graph<int> &g, int scenario, int index) {

	GraphViewer *gv = new GraphViewer(600, 600, false);

	gv->createWindow(600, 600);

	gv->defineEdgeColor("blue");
	gv->defineVertexColor("yellow");

	stringstream file;

	file << "tests/scenario" << scenario << "/nos" << index << ".txt";

	readVertex(g,gv,file.str());

	file.str("");

	file << "tests/scenario" << scenario << "/arestas" << index << ".txt";

	readEdges(g,gv,file.str());

	file.str("");
	file << "tests/scenario" << scenario << "/pedidos" << index << ".txt";

	readRequests(g,file.str());

	file.str("");
	file << "tests/scenario" << scenario << "/details" << index << ".txt";

	readDetails(g,file.str());

	gv->rearrange();
}
