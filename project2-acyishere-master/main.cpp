#include "Hacker.h"
#include <bits/stdc++.h>
#include <cmath>
using namespace std;
#define epsilon 0.00001


int clofsq = 0;
int mlofsq = 0;
int clofhq = 0;
int mlofhq = 0;
float timeInStickerQueue = 0;
float timeInHoodieQueue = 0;
int invalidQA;
int invalidGA;

	class stickerDesks { 

		public:
		float Tprocess;
		bool isAvailable = true;
		int deskID;
		stickerDesks(int deskID,float Tprocess){
			this -> deskID = deskID;
			this -> Tprocess = Tprocess;
		}
	};
	class hoodieDesks { 

		public:
		float Tprocess;
		bool isAvailable = true;
		int deskID;
		hoodieDesks(int deskID,float Tprocess){
			this -> deskID = deskID;
			this -> Tprocess = Tprocess;
		}
	};

	class Events {

	public:

		float time;
		string type;
		int hackerID,lineChanged,deskID;
		Events(float time,string type,int hackerID,int lineChanged,int deskID){ 
			this -> time = time;
			this -> type = type;
			this -> hackerID = hackerID;
			this -> lineChanged = lineChanged;
			this -> deskID = deskID;
		}
	};

		void getHoodie(int hackerID,float entranceTime,int hddID,vector<Hacker> &hackers,vector<hoodieDesks> &hdDesks,float currentTime) { 

			hackers[hackerID-1].numOfGifts++;
			hdDesks[hddID-1].isAvailable = false; 
			timeInHoodieQueue += currentTime - entranceTime;
			hackers[hackerID-1].waitingInQueues += currentTime - entranceTime;

		}
		void commitCode(int changedLine,int hackerID,float time,vector<Hacker> &hackers) {

			hackers[hackerID-1].totalCommits++;
			hackers[hackerID-1].commitLines.push_back(changedLine);

			if(changedLine >= 20)
				hackers[hackerID-1].validCommits++;
			
			if(hackers[hackerID-1].validCommits >= 3)
				hackers[hackerID-1].isValidforHoodie = true;

		}

		void getSticker(int hackerID,float entranceTime,int stdID,vector<Hacker> &hackers,vector<stickerDesks> &stDesks,float currentTime) { 

			stDesks[stdID-1].isAvailable = false; 
			timeInStickerQueue += currentTime - entranceTime;
			hackers[hackerID-1].waitingInQueues += currentTime - entranceTime;

		} 

	class CompareEvents {

		public:

		bool operator() (Events &x, Events &y) {
    		
    		if(abs(x.time - y.time) <= epsilon) { 

    			if(x.hackerID < y.hackerID){
    				return false; 
    			}
    			else {
    				return true;
    			}
    		}

    		else if(x.time < y.time){
    			return false;
    		}

    		return true;
  	  }	
	};

	class CompareHoodieQueue { 
					
		public:

    	bool operator() (Hacker &x, Hacker &y) {

    		if(x.validCommits < y.validCommits){
    			return true;
    		}
    		else if(x.validCommits == y.validCommits){
    			if(abs(x.queueEntranceTime - y.queueEntranceTime) <= epsilon) { 
    				if(x.id < y.id){
    					return false;
    				}
    				return true;
    			}
    			else if(x.queueEntranceTime < y.queueEntranceTime){ 
    				return false;
    			}
    			return true;
    		}
    		return false;
    	}
	};

	class CompareForWaitMost {

		public:

		bool operator() (Hacker &x, Hacker &y) { //önce waitingqueue sonra id düşük olana

			if(y.waitingInQueues - x.waitingInQueues > epsilon) {
				return true;
			}
			else if (abs(x.waitingInQueues - y.waitingInQueues) <= epsilon){
				if(x.id < y.id){
					return false;
				}
				return true;
			}
			return false;
		}
	};

	class CompareForWaitLeast {

		public:

		bool operator() (Hacker &x, Hacker &y) { //önce waitingqueue sonra id düşük olana, 3 hediyeli olmayanı queue sonuna at

			if(y.waitingInQueues - x.waitingInQueues > epsilon) {
				return false;
			}
			else if (abs(x.waitingInQueues - y.waitingInQueues) <= epsilon){
				if(x.id < y.id){
					return false;
				}
				return true;
			}
			return true;
		}
	};

int main(int argc, char* argv[]) {

	priority_queue<Events,vector<Events>, CompareEvents> allEvents; 
	priority_queue<Hacker,vector<Hacker>, CompareHoodieQueue> hoodieQueue;
	queue<Hacker> stickerQueue;
	
	ifstream readFrom(argv[1]);
	ofstream writeTo(argv[2]);

	int N; 

	readFrom >> N;

	float Tarrival; 

	priority_queue <float,vector<float>,greater<float>> times ; 

	for(int i = 0;i < N;i++){
		readFrom >> Tarrival;
		times.push(Tarrival);
	}

	vector<Hacker> hackers;

	for(int i = 1;i<N+1;i++){
		Hacker x = Hacker(i,times.top()); 
		times.pop();
		hackers.push_back(x);
	}

	int tnoc; 

	readFrom >> tnoc;

	int nolc,idofh;

	float Tcommit; 

	for(int i = 0; i < tnoc;i++){ 

		readFrom >> idofh;
		readFrom >> nolc;
		readFrom >> Tcommit;

		Events x = Events(Tcommit,"commit",idofh,nolc,0);
		allEvents.push(x); 
	}

	int Q; 
	readFrom >> Q;
	int secid; 
	float toqa; 

	for(int i = 0;i < Q;i++){

		readFrom >> secid;
		readFrom >> toqa;

		Events x = Events(toqa,"queueE",secid,0,0);
		allEvents.push(x);

	}

	int nosd,nohd;

	float sdp,hdp; 

	readFrom >> nosd;

	vector<stickerDesks> stDesks;

	for(int i = 1;i<nosd+1;i++){
		readFrom >> sdp;
		stickerDesks x = stickerDesks(i,sdp);
		stDesks.push_back(x);
	}

	readFrom >> nohd;

	vector<hoodieDesks> hdDesks;

	for(int i = 1;i<nohd+1;i++){
		readFrom >> hdp;
		hoodieDesks x = hoodieDesks(i,hdp);
		hdDesks.push_back(x);
	}

	float maxTime = 0;
	float nowTime = 0;

	while(!allEvents.empty()) {

		Events x = allEvents.top();
		allEvents.pop();
		nowTime = x.time;

		if(nowTime > maxTime) {
			maxTime = nowTime;
		}


		if(x.type == "commit"){

			commitCode(x.lineChanged, x.hackerID, x.time, hackers);

		}

		else if(x.type == "queueE"){

			if(hackers[x.hackerID-1].validCommits >= 3 and hackers[x.hackerID-1].numOfGifts < 3) { 

				clofsq++; 

				hackers[x.hackerID-1].queueEntranceTime = x.time;
				hackers[x.hackerID-1].stqueueEntranceTimes.push_back(x.time);
				stickerQueue.push(hackers[x.hackerID-1]); 

				for(int i = 0;i<nosd;i++){

					if(clofsq == 0)
						break;

					if(stDesks[i].isAvailable == true) {

						Hacker a = stickerQueue.front();
						stickerQueue.pop();
						clofsq--;
						getSticker(a.id , a.queueEntranceTime, i+1 , hackers, stDesks, x.time);
						Events m = Events(x.time+stDesks[i].Tprocess,"leaveStDesk",a.id,0,i+1); 
						allEvents.push(m);
					}
				}

				if(clofsq > mlofsq)
					mlofsq = clofsq;

			}

			else {

				if(hackers[x.hackerID-1].validCommits < 3) { 
					invalidQA++;
				}
				else{
					invalidGA++;
				}

			}

		}

		else if(x.type == "leaveStDesk") {

			stDesks[x.deskID-1].isAvailable = true;

			for(int i = 0;i<nosd;i++){

				if(clofsq == 0)
					break;

				if(stDesks[i].isAvailable == true) {

					Hacker a = stickerQueue.front();
					stickerQueue.pop();
					clofsq--;
					getSticker(a.id , a.queueEntranceTime, i+1 , hackers, stDesks, x.time);
					Events m = Events(x.time+stDesks[i].Tprocess,"leaveStDesk",a.id,0,i+1); 
					allEvents.push(m);
				}
			}

			hackers[x.hackerID-1].queueEntranceTime = x.time;
			hoodieQueue.push(hackers[x.hackerID-1]);
			clofhq++;

			for(int i = 0;i<nohd;i++){

				if(clofhq == 0)
					break;

				if(hdDesks[i].isAvailable == true) {

					Hacker a = hoodieQueue.top();
					hoodieQueue.pop();
					clofhq--;
					getHoodie(a.id , a.queueEntranceTime, i+1 , hackers, hdDesks, x.time);
					Events m = Events(x.time+hdDesks[i].Tprocess,"leaveHdDesk",a.id,0,i+1); 
					allEvents.push(m);
				}
			}

				if(clofhq > mlofhq)
					mlofhq = clofhq;

		}

		else if(x.type == "leaveHdDesk"){

			hdDesks[x.deskID-1].isAvailable = true;
			hackers[x.hackerID-1].hdqueueLeaveTimes.push_back(x.time);

			for(int i = 0;i<nohd;i++){

				if(clofhq == 0)
					break;

				if(hdDesks[i].isAvailable == true) {

					Hacker a = hoodieQueue.top();
					hoodieQueue.pop();
					clofhq--;
					getHoodie(a.id , a.queueEntranceTime, i+1 , hackers, hdDesks, x.time);
					Events m = Events(x.time+hdDesks[i].Tprocess,"leaveHdDesk",a.id,0,i+1); 
					allEvents.push(m);
				}
			}
		}
	}

		writeTo << fixed << setprecision(3);

		writeTo << mlofsq << endl;

		writeTo << mlofhq << endl;

		int totalGiftNum = 0;

		for(int i = 0;i<N;i++){
			totalGiftNum += hackers[i].numOfGifts;
		}

		writeTo << totalGiftNum/(float)N << endl;

		writeTo << timeInStickerQueue/totalGiftNum << endl;

		writeTo << timeInHoodieQueue/totalGiftNum << endl;

		writeTo << tnoc/(float)N << endl;

		int totalLine = 0;
		for(int i = 0;i<N;i++){
			for(int j = 0;j<hackers[i].commitLines.size();j++){
				totalLine += hackers[i].commitLines[j];
			}
		}

		writeTo << totalLine/(float)tnoc << endl;

		float totalWait = 0;
		for(int i = 0;i<N;i++){
			for(int j = 0;j<hackers[i].stqueueEntranceTimes.size();j++){
				totalWait +=  hackers[i].hdqueueLeaveTimes[j] - hackers[i].stqueueEntranceTimes[j];
			}
		}

		writeTo << totalWait / totalGiftNum << endl;

		writeTo << invalidQA << endl;

		writeTo << invalidGA << endl;

		priority_queue<Hacker,vector<Hacker>, CompareForWaitMost> waitOrderMost;

		for(int i = 0;i < N;i++) {
			waitOrderMost.push(hackers[i]);
		}

		Hacker waiterMost = waitOrderMost.top(); 

		writeTo << waiterMost.id << " " << waiterMost.waitingInQueues << endl;

		priority_queue<Hacker,vector<Hacker>, CompareForWaitLeast> waitOrderLeast; 

		for(int i = 0;i < N;i++) {
			if(hackers[i].numOfGifts == 3) {
				waitOrderLeast.push(hackers[i]);
			}
		}

		if(!waitOrderLeast.empty()) {
			Hacker waiterLeast = waitOrderLeast.top();
			writeTo << waiterLeast.id << " " << waiterLeast.waitingInQueues << endl;
		}
		else{
			writeTo << -1 << " " << -1.000 << endl;
		}

		writeTo << maxTime;

    return 0;
};