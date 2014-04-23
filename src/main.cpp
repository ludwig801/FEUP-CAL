/*
 * main.cpp
 *
 *  Created on: 23 de Abr de 2014
 *      Author: Luís
 */
#include "input.h"
#include "Graph.h"

#include <iostream>

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

void findSol1(Graph<int> &g) {

	// Initializes graph vertex visited to false
	g.setVisited(false);

	g.printGraph();

//	bool Labirinth::findGoal2(int x, int y)
//	{
//		if (labirinth[y][x]==2)
//			return true;
//		if (labirinth[y][x]==0 || visited[y][x]) // assumes walls all around
//			return false;
//		visited[y][x] = true;
//		return findGoal2(x+1, y) || findGoal2(x-1, y) || findGoal2(x, y+1) || findGoal2(x,y-1);
//	}
//
//	bool Labirinth::findGoal(int x, int y)
//	{
//		initializeVisited(x, y);
//		return findGoal2(x, y);
//	}
}


