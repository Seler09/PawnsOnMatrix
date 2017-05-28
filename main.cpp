#include <iostream>
#include <queue>
#include <vector>
#include <ncurses.h>
#include <ctime>
#include <cstdlib>
#include <mutex>
#include <thread>

using namespace std;

std::mutex mx;

std::queue<int>kolejka1;
std::queue<int>kolejka2;

std::vector<int>x;
std::vector<int>y;

int sizeX=0;
int sizeY=0;

int pX;
int pY;

int kolumna=0;
int ala=0;

void producent()
{

	int wspX = rand()% (sizeX-1) + 0;
	mx.lock();
	kolejka1.push(wspX);
	mx.unlock();

	int wspY = rand()% (sizeY-1) + 0;
	mx.lock();
	kolejka2.push(wspY);
	mx.unlock();

}

void przetwarzacz()
{
	mx.lock();
	pX = kolejka1.front();
	kolejka1.pop();
	mx.unlock();

	mx.lock();
	pY = kolejka2.front();
	kolejka2.pop();
	mx.unlock();
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
				if(rzad==pY&&kolumna==((pX*2)+1))
				{
					mvaddch(rzad,kolumna,'o');
					cout<<kolumna;
					cout<<rzad;
					cout<<pX;
					cout<<"\n\n";
				}
				else
					mvaddch(rzad,kolumna,'_');
			}

		}
	}
	getch();
	endwin();
}



int main() {

	srand(time(NULL));

	sizeX = 10;
	sizeY = 10;
	//cin>>pX;
	//cin>>pY;
	for(int lel=0;lel<100;lel++)
	{
	producent();
	przetwarzacz();
	macierz();
	}
	cout<<endl<<"pX po operacji: "<<pX<<endl;

	cout<<endl;




	return 0;
}
