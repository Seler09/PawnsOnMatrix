#include <iostream>
#include <queue>
#include <vector>
#include <ncurses.h>
#include <ctime>
#include <cstdlib>
#include <mutex>
#include <thread>

using namespace std;

std::mutex mPX;
std::mutex mPY;

std::queue<int>kolejka1;
std::queue<int>kolejka2;
std::mutex k1;
std::mutex k2;
std::mutex mx;


int sizeX=0;
int sizeY=0;

int pX;
int pY;

int pDodawania;
int pPrzesowania;



struct pionek
{
	int x;
	int y;
};

pionek tenPionek;

std::vector<pionek>wspPionka;


//--------------------FUNKCJE------------------------



void producent()
{
	do{
	int wspX = rand()% (sizeX) + 0;
	k1.lock();
	kolejka1.push(wspX);
	k1.unlock();

	int wspY = rand()% (sizeY) + 0;
	k2.lock();
	kolejka2.push(wspY);
	k2.unlock();
	}while(true);
}

void przetwarzacz()
{
	do{
		mx.lock();
			k1.lock();
				tenPionek.x = kolejka1.front();
				kolejka1.pop();
			k1.unlock();

			k2.lock();
				tenPionek.y = kolejka2.front();
				kolejka2.pop();
			k2.unlock();
		mx.unlock();
		std::this_thread::sleep_for (std::chrono::milliseconds(100));
	}while(true);
}

void rysowaniePionka()
{
	do{
		mx.lock();
			mvaddch(tenPionek.y,((tenPionek.x)*2)+1,'i');

			mPX.lock();
				wspPionka.push_back(tenPionek);
			mPX.unlock();
		mx.unlock();

		refresh();
		std::this_thread::sleep_for (std::chrono::seconds(pDodawania));
	}while(true);

}

void przesowaniePionka()
{
	do{

		mPX.lock();
			for(int i=0;i<wspPionka.size();i++)
			{
				int ruch = rand()% (4) + 0;

				if(ruch==0)//dól
				{
					mvaddch(wspPionka[i].y,((wspPionka[i].x)*2)+1,'_');

					if(wspPionka[i].y==sizeY-1)
					{
						wspPionka[i].y=wspPionka[i].y-1;
					}
					else
					{
						wspPionka[i].y=wspPionka[i].y+1;
					}
					mvaddch(wspPionka[i].y,((wspPionka[i].x)*2)+1,'i');

					refresh();

				}

				if(ruch==1)//góra
				{
					mvaddch(wspPionka[i].y,((wspPionka[i].x)*2)+1,'_');

					if(wspPionka[i].y==0)
					{
						wspPionka[i].y=wspPionka[i].y+1;
					}
					else
					{
						wspPionka[i].y=wspPionka[i].y-1;
					}
					mvaddch(wspPionka[i].y,((wspPionka[i].x)*2)+1,'i');

					refresh();

				}

				if(ruch==2)//lewo
				{
					mvaddch(wspPionka[i].y,((wspPionka[i].x)*2)+1,'_');

					if(wspPionka[i].x==0)
					{
						wspPionka[i].x=wspPionka[i].x+1;
					}
					else
					{
						wspPionka[i].x=wspPionka[i].x-1;
					}
					mvaddch(wspPionka[i].y,((wspPionka[i].x)*2)+1,'i');

					refresh();

				}

				if(ruch==3)//prawo
				{
					mvaddch(wspPionka[i].y,((wspPionka[i].x)*2)+1,'_');

					if(wspPionka[i].x==sizeX-1)
					{
						wspPionka[i].x=wspPionka[i].x-1;
					}
					else
					{
						wspPionka[i].x=wspPionka[i].x+1;
					}
					mvaddch(wspPionka[i].y,((wspPionka[i].x)*2)+1,'i');

					refresh();

				}

			}
		mPX.unlock();
		std::this_thread::sleep_for (std::chrono::milliseconds(pPrzesowania));
	}while(true);

}

void macierz()
{

	initscr();

	for(int rzad=0;rzad<sizeY;rzad++)
	{

		for(int kolumna=0;kolumna<sizeX+sizeX+1;kolumna++)
		{
			if(kolumna%2==0)
			{
				mvaddch(rzad,kolumna,'|');
			}
			else
			{
				mvaddch(rzad,kolumna,'_');
			}

		}
	}




}


void menu();


//--------------------MAIN------------------------------
int main() {

	srand(time(NULL));

	menu();

    macierz();

    auto tProducent = thread(producent);
    auto tPrzetwarzacz = thread(przetwarzacz);
    auto tRysowanie = thread(rysowaniePionka);
    auto tRelokacja = thread(przesowaniePionka);

    tProducent.join();
    tPrzetwarzacz.join();
    tRysowanie.join();
    tRelokacja.join();

    endwin();


	return 0;
}


void menu()
{
	int wybor;
		cout<<"MENU\n";
		cout<<"[1] Wpisz parametry.\n";
		cout<<"[2] Automatyczne parametry.\n";
		cin>>wybor;
		cout<<endl;

		switch(wybor){
		case 1:
			cout << "Podaj szerkosc macierzy: ";
			cin >> sizeX;
			cout << "Podaj wysokosc macierzy: ";
			cin >> sizeY;
			cout << "Podaj predkosc dodawania i przesowania pionkow [sek]: ";
			cin >> pDodawania;
			cin >> pPrzesowania;
			break;
		case 2:
			sizeX=10;
			sizeY=10;
			pDodawania=1;
			pPrzesowania=1000;
			break;
		default:
			cout<<"Error\n";
			break;
		}
}
