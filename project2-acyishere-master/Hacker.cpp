using namespace std;
#include "Hacker.h"

Hacker::Hacker(int id,float arriveTime){
	this -> id = id;
	this -> totalCommits = 0;
	this -> validCommits = 0;
	this -> isValidforHoodie = false;
	this -> numOfGifts = 0;
	this -> arriveTime = arriveTime;
	this -> waitingInQueues = 0;
}
Hacker::~Hacker(){

}
