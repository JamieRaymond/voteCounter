//Student Name: Jamie Raymond - C00208873
#include "stdafx.h"
#include <fstream>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include <time.h>

using namespace std;
ifstream file("candidates.txt");

//BallotPaper Class***************************************************
class BallotPaper {
public:
	BallotPaper(int candCount, string position);

	void setPreference(int allVotes, int cand);
	int getPreference(int allVotes);
	vector<int> getBallot(); //gets total number of 1's

private:
	vector<int> allVotes; //vector of all votes
	int size; //size of allVotes
};
BallotPaper::BallotPaper(int canCount, string position)
{
	int votePref = canCount;
	int splitter = canCount;
	int size = canCount;

	allVotes.resize(size);
	for (int i = 0; i<size; i++)
	{
		stringstream(position) >> votePref;
		splitter = position.find(",");
		allVotes[votePref - 1] = i;
		position = position.substr(splitter + 1); //Reading in the file
	}
}
void BallotPaper::setPreference(int votePref, int cand)
{
	allVotes[votePref - 1] = cand;
}
int BallotPaper::getPreference(int votePref)
{
	return allVotes[votePref];
}
vector<int> BallotPaper::getBallot()
{
	return allVotes;
};

//Candidate Class************************************************************

class Candidate {
	public:
		void setName(string name);
		string getName(void);

		void setParty(string party);
		string getParty(void);

		Candidate(string name, string party); //Calls constructor

	private:
		string canName;
		string canParty;
};
Candidate::Candidate(string name, string party)
{
	canName = name;
	canParty = party;
	cout << "Candidate " << name << " is running in the election for " << party << "." << endl;
}
void Candidate::setName(string name)
{
	cout << canName;
	canName = name;
	cout << " updated to " << canName;
}
string Candidate::getName()
{
	return canName;
}
void Candidate::setParty(string party)
{
	cout << canParty;
	canParty = party;
	cout << " updated to " << canParty;
}
string Candidate::getParty()
{
	return canParty;
};

//VoteCounter********************************************************************************************

int voteCounter(vector<BallotPaper> votes, vector<Candidate> canVec ,int size)
{
	int currentWinner, eliminated;
	int round = 1;
	int x, y;
	bool check;
	int* count = new int[size];
	int* validation = new int[size];
	
	vector<int> draws;

	srand(time(NULL));

	for (x = 0; x < size; x++)
	{
		validation[x] = true; //making sure something in the vector
	}
	while (!false)
	{
		for (x = 0; x < size; x++)
		{
			count[x] = 0; //Counting number of elements
		}
		for (x = 0; x < (int)votes.size(); x++)
		{
			for (y = 0; y < size; y++)
			{
				if (validation[votes[x].getPreference(y)])
				{
					count[votes[x].getPreference(y)]++;
					break; // gets the number of 1's
				}
			}
		}

		cout << endl;

		for (x = 0; x < size; x++)
		{
			cout << "|" << count[x]; //dividing line
		}

		cout << "|" << endl;

		check = true;
		for (x = 0; x < size; x++)
		{
			if (check && validation[x]) //makes sure they're both true
			{
				check = false;
				draws.push_back(x);//draws is 1
				currentWinner = x; 
			}
			else if (validation[x])
			{
				if (count[draws[0]] > count[x])
				{
					draws.clear();
					draws.push_back(x); //checks first index of low, if greater than count clear low and pushback x
				}
				else if (count[draws[0]] == count[x])
				{
					draws.push_back(x);//if first index of low is equal to count, pushback x
				}
				if (count[currentWinner] < count[x])
				{
					currentWinner = x;
				}
			}
		}
		if (count[currentWinner] * 2 > (int)votes.size())
		{
			break; // checks number of votes of the highest candidates is more than half of the total votes
		}

		eliminated = draws[rand() % draws.size()]; //Randomizes between draws vector to see who get eliminated
		draws.clear(); //gets cleared
		validation[eliminated] = false;

		cout << "ROUND " << round++ << endl;
		cout << "Candidate " << canVec[eliminated].getName() << " has been knocked out of the running." << endl;
		cout << "*---------------------------------------*" <<endl;
	}
	return currentWinner; 
}

// MAIN******************************************************************************

int main()
{
	string position;
	string name;
	string party;

	vector<Candidate> canVec;
	vector<BallotPaper> voteVec;
	int splitter;

	//Reads in the candidate's Names and Party and stores them into canVec
	while (getline(file, position))
	{
		if (position == "")
		{
			break;
		}

		splitter = position.find("|");
		name = position.substr(0, splitter);
		splitter++;
		party = position.substr(splitter);

		//Declares candidate
		Candidate candidate(name, party);
		canVec.push_back(candidate);
	}
	//Reads in the votes for each BallotPaper stores them into canVec vector
	while (getline(file, position))
	{
		BallotPaper ballot(canVec.size(), position);
		voteVec.push_back(ballot);
	}

	//Finds the winner using the vote counter method
	Candidate winner = canVec[voteCounter(voteVec, canVec ,(int)canVec.size())];
	cout << "WINNER: " << winner.getName() << " of the " << winner.getParty() << " Party." << endl;
	system("pause");
}
