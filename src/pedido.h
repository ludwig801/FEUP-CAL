/*
 * pedido.h
 *
 *  Created on: 23 de Abr de 2014
 *      Author: Luís
 */

#ifndef PEDIDO_H_
#define PEDIDO_H_

/**
 * Pedido/Cliente
 *
 * Guarda todas as informacoes acerca de um cliente. Como o numero de pessoas que pretende transportar e a hora
 * pretendida para chegada ao aeroporto.
 */
class Pedido {
	int hora;			/*< Hora pretendida de chegada ao aeroporto */
	int numPessoas;		/*< Numero de pessoas a transportar */

public:
	/**
	 * Construtor default.
	 */
	Pedido(): hora(0), numPessoas(0) {};

	/**
	 * Construtor completo.
	 */
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
