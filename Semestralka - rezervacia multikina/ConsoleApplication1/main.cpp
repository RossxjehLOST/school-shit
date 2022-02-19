#define  _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "func.h"

film_t *films;// s = new film_t[3];	//deklaracia filmu

int newTicket() {
    int selectedFilm,selectedTime;

    std::cout << "Vyber filmu\n";
    loadFilms("film_cas.txt", films);
    for (int i = 0; i < 3; i++) {
        std::cout << i + 1 << ' ' << films[i].name << '\n';
    }
    std::cout << "Vyberte film (1-3) a stlacte ENTER:\n";
    std::cin >> selectedFilm;

    if (selectedFilm < 1 || selectedFilm>3) {
        std::cout << "Bad input";
        return BAD_INPUT;
    }

    std::cout << "Casy vysielania:\n";
    for (int i = 0; i < 3; i++) {
        std::cout << i + 1 << ' ';
        std::cout << films[selectedFilm - 1].projTime[i].cas[0] << ':' << films[selectedFilm - 1].projTime[i].cas[1] << '\n';
    }
    std::cout << "Vyberte cas (1-3) a stlacte ENTER:\n";

    std::cin >> selectedTime;
    if (selectedTime < 1 || selectedTime>3) {
        std::cout << "Bad input";
        return BAD_INPUT;
    }
    //nacitaj stav saly
    loadSeats("seats.txt",films[selectedFilm-1].projTime[selectedTime-1].seats.uid, films[selectedFilm - 1].projTime[selectedTime - 1].seats.seat);
    
    std::cout << "Volne miesta su oznacene *, obsadene x:\n";
    //vypluj na stdout
    printSeats(films[selectedFilm - 1].projTime[selectedTime - 1].seats.seat);

    std::cout << "\nZadajte cislo radu:(1-6)\n";
    int row,seat;
    std::cin >> row;
    std::cout << "Zadajte cislo sedadla:(1-16)\n";
    std::cin >> seat;

    if (row < 1 || row>6 || seat < 1 || seat>16) {
        std::cout << "Bad input";
        return BAD_INPUT;
    }

    int ticketId = registerTicket(films[selectedFilm - 1].projTime[selectedTime - 1].seats.uid, seat, row);
    if (ticketId == SEAT_TAKEN) {
        std::cout << "Vybrane sedadlo je uz obsadene";
        return SEAT_TAKEN;
    }
    else {
        std::cout << "Cislo vaseho listku je: " << ticketId << " Dakujeme" << std::endl << std::endl;
    }
    return OK;
}

int changeTicket() {
    ticket_t ticket;
    loadFilms("film_cas.txt", films);
    std::cout << "Zadajte cislo vaseho listku:" << std::endl;
    int inTicketId = -1;
    std::cin >> inTicketId;
    if (getTicket(inTicketId, &ticket) == TICKET_NOT_FOUND) {
        std::cout << "Zadany listok neexistuje!";
        return TICKET_NOT_FOUND;
    }
    int ind = (ticket.uid - 1) / 3;
    std::cout << "Detaily listku c. " << ticket.ticketId << ":\n";
    std::cout << "Film: " << films[ind].name << std::endl;
    std::cout << "Cas: " << films[ind].projTime[(ticket.uid-ind*3)-1].cas[0];
    std::cout << ":" << films[ind].projTime[(ticket.uid - ind * 3) - 1].cas[1] << std::endl;
    std::cout << "Rad: " << ticket.Y << " Sedadlo: " << ticket.X << std::endl;

    std::cout << "Zvolte moznost:" << std::endl;
    std::cout << "1 Zmena filmu" << std::endl;
    std::cout << "2 Zmena miesta" << std::endl;
    std::cout << "3 Zrusit listok" << std::endl;
    std::cout << "4 Navrat na hlavne menu" << std::endl;
    
    int sel = 0;
    std::cin >> sel;
    switch (sel)
    {
    case 1:
        int selectedFilm, selectedTime;

        std::cout << "Vyber filmu\n";
        loadFilms("film_cas.txt", films);
        for (int i = 0; i < 3; i++) {
            std::cout << i + 1 << ' ' << films[i].name << '\n';
        }
        std::cout << "Vyberte film (1-3) a stlacte ENTER:\n";
        std::cin >> selectedFilm;

        if (selectedFilm < 1 || selectedFilm>3) {
            std::cout << "Bad input";
            return BAD_INPUT;
        }

        std::cout << "Casy vysielania:\n";
        for (int i = 0; i < 3; i++) {
            std::cout << i + 1 << ' ';
            std::cout << films[selectedFilm - 1].projTime[i].cas[0] << ':' << films[selectedFilm - 1].projTime[i].cas[1] << '\n';
        }
        std::cout << "Vyberte cas (1-3) a stlacte ENTER:\n";

        std::cin >> selectedTime;
        if (selectedTime < 1 || selectedTime>3) {
            std::cout << "Bad input";
            return BAD_INPUT;
        }
        ticket.uid = (((selectedFilm-1)*3) + selectedTime - 1)+1;
        if (modifyTicket(ticket) == SEAT_TAKEN) {
            std::cout << "Miesto vo na vybrany film a cas nie je dostupne. Zruste listok a zakupte si novy!\n";
        }
        else {
            std::cout << "Film bol zmeneny na: " << films[selectedFilm - 1].name << " o " << films[selectedFilm - 1].projTime[selectedTime-1].cas[0]
            << ":" << films[selectedFilm - 1].projTime[selectedTime - 1].cas[1] << std::endl;
        }
        break;
    case 2: {
        int X = 0, Y = 0;
        uint16_t seatSt[6];
        loadSeats("seats.txt", ticket.uid, seatSt); //nacitaj stav saly
        printSeats(seatSt); //vypluj to
        std::cout << "Zadajte novy rad:(1-6)" << std::endl;
        std::cin >> Y;
        std::cout << "Zadajte nove sedadlo:(1-6)" << std::endl;
        std::cin >> X;

        if (X == ticket.X && Y == ticket.Y)
            return OK;

        updateSeats("seats.txt", ticket.uid, ticket.X, ticket.Y, true); //vymaz stary zanam
        if (updateSeats("seats.txt", ticket.uid, X, Y, false) == SEAT_TAKEN) {  //zapis nove miesto
            std::cout << "Miesto je obsadene!" << std::endl;
            return SEAT_TAKEN;
        }
        std::cout << "Miesto bolo zmenene!" << std::endl;
        break;
    }
    case 3:
        updateSeats("seats.txt", ticket.uid, ticket.X, ticket.Y, true);
        removeTicket(ticket.ticketId);
        std::cout << "Listok bol zruseny" << std::endl;
        break;
    case 4:
    default:
        return OK;
        break;
    }
    return OK;
}

int main()
{
    //alokuj miesto na filmy
    films = new film_t[3];
    while (1) {
        std::cout << std::endl << "*********************" << std::endl;
        std::cout << "Rezervacny system multikina\n";
        std::cout << "1: Zakupit listok" << std::endl;
        std::cout << "2: Uprava listka" << std::endl;
        std::cout << "Zvolte funkciu (1-2):" << std::endl;
        int f = 0;
        std::cin >> f;
        //f -= '0';
        switch (f)
        {
        case 1:
            newTicket();
            break;
        case 2:
            changeTicket();
            break;
        default:
            std::cout << "Bad input" << std::endl;
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            break;
        }
    }
}