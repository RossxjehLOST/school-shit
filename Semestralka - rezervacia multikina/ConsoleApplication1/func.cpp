#include "func.h"


static char* loadString(FILE* f) {
	char c;
	int index = 0;
	char* str = new char[30];
	c = fgetc(f);
	while (c != ' ' && c != '\n' && c != EOF && index < 30) {
		str[index++] = c;
		c = fgetc(f);
	}
	str[index] = '\0';
	return str;
}

int loadFilms(const char* filename,film_t *_films) {
	FILE* file;
	char* tmp = new char[30];	//30 by malo stacit, ku koncu sa uvolni
	film_t tmpF[3];

	file = fopen(filename, "r");	//otvor subor
	if (file == NULL)
		return ALLOC_ERR;	//nepodarilo sa otvorit subor

	int i = 0;		//ktory film nacitavame

	while (1) {	//precitaj cely subor az do konca
		tmp = loadString(file);				//nacitaj prve slovo/ nazov filmu
		tmpF[i].name = new char[30];		//+1 kvoli \0 znaku
		strcpy(tmpF[i].name, tmp);		//nakopiruj do trvalej premennej
		//teraz nacitaj cas
		for (int j = 0; j < 3; j++) {
			tmp = loadString(file);
			//prevod na integer a ulozenie
			int minOfDay = atoi(tmp);
			tmpF[i].projTime[j].cas[0] = minOfDay / 60;
			tmpF[i].projTime[j].cas[1] = minOfDay % 60;
			tmpF[i].projTime[j].seats.uid = (i *3)+ (j + 1);	//priradenie uid saly
			tmpF[i].projTime[j].seats.seat = new uint16_t[6];
		}

		if (feof(file))
			break;	//sme na konci suboru
		i++;
	}

	memcpy(_films, tmpF, sizeof(film_t) * 3);
	delete tmp;
	_fcloseall();
	return OK;
}

int loadSeats(const char* filename, int uid,uint16_t* seats) {
	uint16_t shSeats[6];
	FILE* file;

	file = fopen(filename, "r");
	if (file == NULL)
		return ALLOC_ERR;	//nepodarilo sa otvorit subor

	while ((fgetc(file)-'0') != uid) {
		while (fgetc(file) != '\n') {
			//ignorujeme vsetko az do noveho riadku
		}
	}
	if (feof(file)) {
		printf("UID not present in file");
		return UID_NOT_FOUND;
	}
	fgetc(file);
	//nasli sme UID, nacitaj obsadenost
	for (int i = 0; i < 6; i++) {
		char *tmp=new char[10];
		tmp=loadString(file);
		shSeats[i] = atoi(tmp);
	}
	memcpy(seats, shSeats, sizeof(uint16_t) * 6);
	_fcloseall();
	return OK;
	
}

int updateSeats(const char* filename, int uid, int X, int Y, bool clearing) {
	uint16_t* origSeats = new uint16_t[6];
	loadSeats("seats.txt", uid, origSeats);
	
	std::ifstream file;
	std::ofstream write;

	X -= 1;
	Y -= 1;

	write.open("tmp.txt", std::ofstream::trunc);
	if (!write.is_open())
		return ALLOC_ERR;	//nepodarilo sa otvorit subor
	file.open(filename);
	if (!file.is_open())
		return ALLOC_ERR;	//nepodarilo sa otvorit subor

	char* line = new char[50];
	file.getline(line, 50);
	while ((line[0] - '0') != uid) {
		write << line << std::endl;
		file.getline(line, 50);
	}
	
	uint16_t mask = 1 << X;
	if (!(origSeats[Y] & mask) || clearing) {
		if (!clearing)
			origSeats[Y] |= mask;
		else
			origSeats[Y] &= ~mask;

		write << uid << " " << origSeats[0] << " " << origSeats[1] << " " << origSeats[2] << " " << origSeats[3] << " " << origSeats[4] << " " << origSeats[5] << std::endl;

		while (!file.eof()) {	//nakopiruje zbytok
			file.getline(line, 50);
			write << line << std::endl;
		}
		write.close();
		file.close();
		remove("seats.txt");
		rename("tmp.txt", "seats.txt");
		return OK;
	}
	else {
		//pokusili sme sa kupit listok na obsadene miesto, ak nejde o vymaz nerob nic a vymaz tmp
		write.close();
		file.close();
		remove("tmp.txt");
		return SEAT_TAKEN;
	}
}

void printSeats(uint16_t* s) {
	printf("  ");
	for (int i = 0; i < 16; i++) {
		printf("%.2d ", i + 1);
	}
	printf("\n");
	for (int i = 0; i < 6; i++) {
		printf("%d ", i+1);
		for (int j = 0; j < 16;j++) {
			uint16_t x = 1 << j;
			if (s[i] & x)
				printf("x  ");
			else
				printf("*  ");
		}
		printf("\n");
	}
}

int removeTicket(int uid) {
	std::ifstream rd;
	std::ofstream wr;
	rd.open("tickets.txt");
	if (!rd.is_open())
		return ALLOC_ERR;
	wr.open("tmp.txt", std::ofstream::trunc);
	if (!wr.is_open())
		return ALLOC_ERR;
	int id = 0;
	char* line = new char[20];
	rd.getline(line, 20);
	sscanf(line, "%d", &id);
	while (id != uid) {
		wr << line << std::endl;
		rd.getline(line, 20);
		sscanf(line, "%d", &id);
	}
	while (1) {
		rd.getline(line, 20);
		wr << line << std::endl;
		if (rd.eof())
			break;
	}
	wr.close();
	rd.close();
	remove("tickets.txt");
	rename("tmp.txt", "tickets.txt");
}

int modifyTicket(ticket_t t) {
	std::ifstream rd;
	std::ofstream wr;
	uint16_t seatSt[6];
	loadSeats("seats.txt", t.uid, seatSt);
	uint16_t mask = 1 << t.X;
	if (seatSt[t.Y - 1] & mask) {
		return SEAT_TAKEN;	//miesto v inom stave je uz obsadene
	}

	rd.open("tickets.txt");
	if (!rd.is_open())
		return ALLOC_ERR;
	wr.open("tmp.txt", std::ofstream::trunc);
	if (!wr.is_open())
		return ALLOC_ERR;
	int id = 0;
	char* line = new char[20];
	rd.getline(line, 20);
	sscanf(line, "%d", &id);
	while (id != t.ticketId) {
		wr << line << std::endl;
		rd.getline(line, 20);
		sscanf(line, "%d", &id);
	}
	wr << t.ticketId << " " << t.uid << " " << t.X << " " << t.Y;
	while (1) {
		rd.getline(line, 20);
		wr << line << std::endl;
		if (rd.eof())
			break;
	}
	wr.close();
	rd.close();
	remove("tickets.txt");
	rename("tmp.txt", "tickets.txt");
}

int registerTicket(int uid,int X, int Y) {
	//get last used uid from file
	std::ifstream tid_r;
	std::ofstream tid_w;
	tid_r.open("ticketId.txt");
	if (!tid_r.is_open())
		return 0;

	int ticketId;
	char* b = new char[34];
	tid_r.getline(b, 34);
	ticketId = atoi(b);
	tid_r.close();

	//teraz mozeme otvorit samotne listky
	ticketId++;

	tid_w.open("ticketId.txt",std::fstream::trunc);
	if (!tid_w.is_open())
		return 0;

	tid_w << ticketId << std::endl;
	tid_w.close();

	std::ofstream tickets;
	tickets.open("tickets.txt", std::ofstream::app);
	if (!tickets.is_open())
		return 0;

	tickets << ticketId << " " << uid << " " << X << " " << Y << std::endl;

	tickets.close();

	int ret = updateSeats("seats.txt", uid, X, Y,false);
	if (ret == SEAT_TAKEN)
		return ret;

	return ticketId;
}

int getTicket(int ticketId, ticket_t* ticket) {
	std::ifstream tickets;
	tickets.open("tickets.txt");
	if (!tickets.is_open())
		return ALLOC_ERR;
	char* line = new char[20];
	tickets.getline(line, 20);
	int id = 0;
	sscanf(line, "%d", &id);
	while (id != ticketId) {	//prebehni cely subor a hladaj tickeid
		tickets.getline(line, 20);
		sscanf(line, "%d", &id);
		if (tickets.eof())
			return TICKET_NOT_FOUND;	//nenasli sme take cislo
	}

	sscanf(line, "%d %d %d %d",&ticket->ticketId,&ticket->uid,&ticket->X,&ticket->Y);

	tickets.close();
	return OK;
}