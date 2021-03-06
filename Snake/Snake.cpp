//Vježbovni tutorial koji pokriva temeljna početna znanja c++, rađen prema materijalima s neta ali izmjenjen i prilagođen
// Ideja je svladati logiku a kasnije graditi grafičko sučelje prvo 2D nakon toga 3D u Unreal

#pragma comment(lib, "th32.lib")
#define _WIN32_WINNT 0x0501 // WinXP only. Za one koji obozavaju Legacy
#define VC_EXTRALEAN
#include "stdafx.h"
#include <shlwapi.h>
#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>

using namespace std;

// Struktura
typedef struct tailpos
{
	int x;   // x koordinata zmije
	int y;  // y koordinata zmije
	struct tailpos *next;   // pointer prema slijedećem segmentu repa
	struct tailpos *prev;   // pointer prema prethodnom segmentu repa
} tail;

// smijer kretanja
int d = 4; // up = 1 , down = 2 , left =3 , right = 4;
int score = 0;


		   /*
		   SNAKE CLASS
		   sadrzi varijeble tailpos strukture
		   odradjuje kretanje zmije i crtanje


		   */
class snake
{
public:
	int wallsX, wallsY;          // pocetna pozicija zida
	int walleX, walleY;          // krajnja pozicija zida
	int wall[10][5];

	int foodx, foody;

	HANDLE console_handle; // handler konzolni       
	COORD cur_cord;        // COORD struktura za pohranu koordinata
						   
	tail *start, *current, *newtail;
							/*
						   tail struct ==>
						   start = pointer prema prvom dijelu 
						   current = pointer prema zadnjem dijelu
						   newtail = temp node za novi segment

						   */
	snake();  // inicijalizacija varijable, vrijednost null
	void insert(int x, int y); // umetanje tijela zmije prethodnom
	void draw();    // crta zmiju
	void drawWall(); // crta zid
	void move();    // kretanje
	bool collision();  //provjera da li je zmija dotaknula zid ili sebe
	void drawfood(int x = 0);


};

// implementacija snake klase
snake::snake()
{
	start = NULL;
	current = NULL;
	newtail = NULL;
	console_handle = GetStdHandle(STD_OUTPUT_HANDLE);  

	foodx = 12;
	foody = 14;
}

void snake::insert(int x, int y)
{
	// provjera starta
	if (start == NULL)
	{
		newtail = new tail;
		newtail->x = x;
		newtail->y = y;
		newtail->next = NULL;
		newtail->prev = NULL;
		start = newtail;
		current = newtail;
	}
	else            // umetni nivi član u rep
	{
		newtail = new tail;
		newtail->x = x;
		newtail->y = y;
		newtail->next = NULL;
		newtail->prev = current;
		current->next = newtail;
		current = newtail;
	}


}

/*
move() :
sadrži logiku za micanje zmije inkrementom ili dekrementom X i Y koordinata.
Pozicija prethodnog dijela pomaknuta je prema slijedećoj a kopiranje je u obrnutom redu.
Pozicija zadnjeg segmenta 

*/

void snake::move()
{
	tail *tmp, *cur;  

	tmp = current; // point tmp prema zadnjem segmentu
	//cur =start->next;

	while (tmp->prev != NULL)
	{
		tmp->x = tmp->prev->x;        // copy val od prethodne zadnjoj
		tmp->y = tmp->prev->y;
		tmp = tmp->prev;              // set tmp prethoddnom segmentu

	}
	/*
	provjera d kako bi promjenio smjer

	*/
	if (d == 1)
		start->y--;

	if (d == 2)
		start->y++;

	if (d == 3)
		start->x--;

	if (d == 4)
		start->x++;

}

/*
draw :
crta zmiju u odnosu na poziciju
*/
void snake::draw()
{
	/*
	cur_cord.X=10;
	cur_cord.Y=5;

	SetConsoleCursorPosition(console_handle,cur_cord);
	cout << "Aljo kako si? Idemo se igrati!!";
	*/

	drawWall(); // draw wall

	tail *tmp;
	tmp = start;

	while (tmp != NULL)
	{
		cur_cord.X = tmp->x;
		cur_cord.Y = tmp->y;
		SetConsoleCursorPosition(console_handle, cur_cord);
		//cout << "*" << cur_cord.X <<"-" <<  cur_cord.Y  ; 
		cout << "O"; // izgled tijela
		tmp = tmp->next;
	}
	//Voće
	cur_cord.X = foodx;
	cur_cord.Y = foody;
	SetConsoleCursorPosition(console_handle, cur_cord);
	cout << "F"; // izgled voća

}

void snake::drawWall()
{
	int size = sizeof(wall) / sizeof(wall[0][0]);
	// draw lijevi zid
	cur_cord.X = 0;
	for (int y = 0; y <= 30; y++)
	{
		cur_cord.Y = y;
		SetConsoleCursorPosition(console_handle, cur_cord);
		cout << '#';
	}
	// draw gornji zid   
	cur_cord.Y = 0;
	for (int x = 0; x <= 30; x++)
	{
		cur_cord.X = x;
		SetConsoleCursorPosition(console_handle, cur_cord);
		cout << '#';
	}
	// draw desni zid 
	cur_cord.X = 30;
	for (int y = 0; y <= 30; y++)
	{
		cur_cord.Y = y;
		SetConsoleCursorPosition(console_handle, cur_cord);
		cout << '#';
	}

	// draw donji zid 
	cur_cord.Y = 30;
	for (int x = 0; x <= 30; x++)
	{
		cur_cord.X = x;
		SetConsoleCursorPosition(console_handle, cur_cord);
		cout << '#';
	}


}

void snake::drawfood(int x)
{
	tail *tmp;
	tmp = start->next;
	if (x == 1) // nova voćka na rand poziciji
	{
		foodx = rand() % 20 + 7;  
		foody = rand() % 20 + 7;
		// zaštita od stvareanja voćke na poziciji zmije
		while (tmp->next != NULL)
		{
			if (foodx == tmp->x && foody == tmp->y)
			{
				drawfood(1);
			}


			tmp = tmp->next;
		}



	}

}


/*
provjera udara u zid ili tijelo
*/
bool snake::collision()
{
	tail *tmp;
	tmp = start->next;
	//provjera s tijelom
	while (tmp->next != NULL)
	{
		if (start->x == tmp->x && start->y == tmp->y)
			return true;


		tmp = tmp->next;
	}
	//provjera sa voćem
	if (start->x == foodx && start->y == foody)
	{
		insert(foodx, foody);
		drawfood(1);
		score += 10;


	}
	//provjera sa zidom
	for (int x = 0; x <= 30; x++)
	{
		if (start->x == x && start->y == 0)
		{
			return true;
		}

	}
	for (int y = 0; y <= 30; y++)
	{
		if (start->x == 0 && start->y == y)
		{
			return true;
		}

	}
	for (int y = 0; y <= 30; y++)
	{
		if (start->x == 30 && start->y == y)
		{
			return true;
		}

	}
	for (int x = 0; x <= 30; x++)
	{
		if (start->x == x && start->y == 30)
		{
			return true;
		}

	}
	return false;

}

//===== main function
int main()
{
	snake ob;         // snake objekt
	ob.insert(10, 5); // snake body
	ob.insert(10, 4); // snake body
	ob.insert(10, 3);
	ob.insert(10, 2);

	//ob.draw();
	int dir = 1; // početni smjer
	while (1) 
	{
		ob.draw(); // nacrtaj sve
		Sleep(100); 
		system("CLS"); 
		cout << "Score: " << score << endl;

		if (ob.collision())
		{
			cout << "Dragi nema dalje:/" << endl;;
			break;
		}

		if (_kbhit())// provjera pritisnute tipke
		{
			switch (_getch())
			{
			case 'w':d = 1;
				break;
			case 's':d = 2;
				break;
			case 'a':d = 3;
				break;
			case 'd':d = 4;
				break;
			case 'm':ob.insert(10, 7);
				break;

			}
		}
		ob.move();  
	}
	_getch();
	return 0;
}

