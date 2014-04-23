/*
 * pedido.h
 *
 *  Created on: 23 de Abr de 2014
 *      Author: Luís
 */

#ifndef PEDIDO_H_
#define PEDIDO_H_


class Pedido {
	int hora;
	int numPessoas;

public:
	Pedido(): hora(0), numPessoas(0) {};

	Pedido(int hora, int num): hora(hora),numPessoas(num) {};

	int getHora() const {
		return hora;
	}

	void setHora(int hora) {
		this->hora = hora;
	}

	int getNumPessoas() const {
		return numPessoas;
	}

	void setNumPessoas(int numPessoas) {
		this->numPessoas = numPessoas;
	}
};


#endif /* PEDIDO_H_ */
