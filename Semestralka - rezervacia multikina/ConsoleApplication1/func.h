#pragma once
#ifndef FUNC_H
#define FUNC_H

#define  _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stdint.h"
#include <fstream>
#include <iostream>

typedef struct {
	int uid;			//jedinecne id, na priradenie salu k filmu a casu
	uint16_t *seat;	//6 radov po 16 sedadiel, kazdy rad je bitfield (0 - volno, 1 - obsadene)
} seats_t;

typedef struct {
	int cas[2];	 //3 casy, index 0 - hodina, index 1 - minuta
	seats_t seats;	//obsadenost
} projTime_t;

typedef struct {
	char* name;
	projTime_t projTime[3];
} film_t;

typedef struct {
	int ticketId;
	int uid;
	int X;
	int Y;
} ticket_t;

enum returns {
	SEAT_TAKEN=-1,
	OK,
	ALLOC_ERR,
	BAD_INPUT,
	UID_NOT_FOUND,
	TICKET_NOT_FOUND,
};

int loadFilms(const char* filename, film_t* _films);

int getTicket(int ticketId, ticket_t* ticket);

int updateSeats(const char* filename, int uid, int X, int Y, bool free);

int loadSeats(const char* filename, int uid,uint16_t *seats);

void printSeats(uint16_t*);

int removeTicket(int uid);

int modifyTicket(ticket_t t);

int registerTicket(int uid, int X, int Y);

#endif // !FUNC_H
