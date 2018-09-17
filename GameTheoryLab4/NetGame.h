#pragma once
#include <vector>
#include <iostream>
using namespace std;
class NetGame
{
private:
	float accuracy; 
	int agentsCount; 
	float minOpinion; 
	float maxOpinion; 
	float firstBoughtOpinion; 
	float secondBoughtOpinion; 
	vector<float> x0;
	vector<vector<float>> A;
	vector<int> firstPlayerAgents;
	vector<int> secondPlayerAgents;

public:
	NetGame();
	NetGame(float accuracy, int agentsCount, float minOpinion, float maxOpinion, float firstBoughtOpinion = -1, float secondBoughtOpinion = 1);
	vector<vector<float>> generateInfluenceMatrix();
	vector<float> generateOpinionVector();
	bool checkGameSolved(vector<vector<float>> A, vector<float> x);
	void generateGame();
	void solveSimpleGame();
	void solveBoughtGame();
	~NetGame();
};

