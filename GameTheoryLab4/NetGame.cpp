#include "NetGame.h"
#include <stdlib.h>
#include <iomanip>


NetGame::NetGame()
{
}

NetGame::NetGame(float accuracy, int agentsCount, float minOpinion, float maxOpinion, float firstBoughtOpinion, float secondBoughtOpinion)
	:accuracy(accuracy), agentsCount(agentsCount), minOpinion(minOpinion), firstBoughtOpinion(firstBoughtOpinion), secondBoughtOpinion(secondBoughtOpinion)
{
}

vector<vector<float>> NetGame::generateInfluenceMatrix()
{
	vector<vector<float>> A;
	float sum = 0;
	A.resize(agentsCount);
	this->A.resize(agentsCount);
	for (int i = 0; i < agentsCount; i++)
	{
		A[i].resize(agentsCount);
		for (int j = 0; j < agentsCount; j++)
		{
			A[i][j] = (rand() % 100) * 0.01;
		}
	}

	for (int i = 0; i < A.size(); i++)
	{
		for (int k = 0; k < A[i].size(); k++)
		{
			sum += A[i][k];
		}
		for (int j = 0; j < A[i].size(); j++)
			A[i][j] /= sum;
		sum = 0;
	}

	for (int i = 0; i < agentsCount; i++)
	{
		this->A[i].resize(agentsCount);
		for(int j = 0; j < A[i].size(); j++)
		{
			this->A[i][j] = A[i][j];
		}
	}
	return A;
}

vector<float> NetGame::generateOpinionVector()
{
	vector<float> x;
	x.resize(agentsCount);
	this->x0.resize(agentsCount);
	for (int i = 0; i < agentsCount; i++)
	{
		x[i] = (rand() % 100) * 0.01;
		x[i] = (maxOpinion - minOpinion) * x[i] + minOpinion;
		x0[i] = x[i];
	}
	return x;
}

bool NetGame::checkGameSolved(vector<vector<float>> A, vector<float> x)
{
	for (int j = 0; j < x.size(); j++)
	{
		for (int i = 0; i < x.size(); i++)
		{
			if (!(abs(A[i][j] - A[0][j]) < accuracy))
				return false;
		}
	}
	for (int i = 0; i < x.size(); i++)
		if (!(abs(x[i] - x[0]) < accuracy))
			return false;
	return true;
}

void NetGame::generateGame()
{
	this->A = generateInfluenceMatrix();
	this->x0 = generateOpinionVector();

	setlocale(LC_ALL, "Russian");

	vector<int> agentsPlayerVector;
	agentsPlayerVector.resize(x0.size());
	for (int i = 0; i < x0.size(); i++)
	{
		agentsPlayerVector[i] = rand() % 2 + 1;
		if (agentsPlayerVector[i] == 1)
			firstPlayerAgents.push_back(i);
		else if (agentsPlayerVector[i] == 2)
			secondPlayerAgents.push_back(i);
	}

	cout << "Сгенерированная игра:" << endl;
	cout << "Матрица влияния A :\n";
	for (int i = 0; i < A.size(); i++)
	{
		for (int j = 0; j < A[i].size(); j++)
		{
			cout << A[i][j] << ", ";
		}
		cout << endl;
	}
		
	cout << "Начальный вектор мнений: \n";
	for (int i = 0; i < x0.size(); i++)
		cout << x0[i] << ", ";
	cout << endl;
	cout << "Агенты первого игрока: \n";
	for (int i = 0; i < firstPlayerAgents.size(); i++)
		cout << firstPlayerAgents[i] << ", ";
	cout << endl; 
	cout << "Агенты второго игрока: \n";
	for (int i = 0; i < secondPlayerAgents.size(); i++)
		cout << secondPlayerAgents[i] << ", ";
	cout << endl;		
}

vector<vector<float>> matrixMult(vector<vector<float>> a, vector<vector<float>> b)
{
	vector<vector<float>> c;
	c.resize(a.size());

	for (int i = 0; i < a.size(); i++)
	{
		c[i].resize(b.size());
		for(int j = 0; j < b.size(); j++)
			for (int r = 0; r < b.size(); r++)
			{
				c[i][j] += a[i][r] * b[r][j];
			}
	}
	return c;
}

vector<float> maxtrixMultVector(vector<vector<float>> a, vector<float> x)
{

	vector<float> c;
	c.resize(a.size());

	for (int i = 0; i < a.size(); i++)
	{
		for (int r = 0; r < x.size(); r++)
		{
			c[i] += a[i][r] * x[r];
		}
	}
	return c;
}

void NetGame::solveSimpleGame()
{
	vector<vector<float>> A = generateInfluenceMatrix(), AInf;
	vector<float> x0 = generateOpinionVector(), x;
	setlocale(LC_ALL, "Russian");
	AInf.resize(A.size());
	x.resize(x0.size());
	for (int i = 0; i < A.size(); i++)
	{
		AInf[i].resize(A[i].size());
		for (int j = 0; j < A[i].size(); j++)
		{
			AInf[i][j] = A[i][j];
		}		
	}
	for (int i = 0; i < x.size(); i++)
		x[i] = x0[i];
	while (!checkGameSolved(AInf, x))
	{
		AInf = matrixMult(AInf, AInf);
		x = maxtrixMultVector(A, x);
	}
	cout << "-------------------------------------" << endl;
	cout << "Решение игры с независимыми агентами:" << endl;
	cout << "Итоговая матрица влияния: " << endl;
	for (int i = 0; i < AInf.size(); i++)
	{
		for (int j = 0; j < AInf[i].size(); j++)
		{
			cout << AInf[i][j] << ", ";
		}
			cout << endl;
	}
	cout << "Итоговое мнение : " << endl;
	for (int i = 0; i < x.size(); i++)
		cout << x[i] << ", ";
	cout << endl;
}

void NetGame::solveBoughtGame()
{
	vector<vector<float>> A = generateInfluenceMatrix(), AInf;
	vector<float> x0 = generateOpinionVector(), x;
	setlocale(LC_ALL, "Russian");

	for (int i : this->firstPlayerAgents)
		x0[i] = firstBoughtOpinion;
	for (int i : this->secondPlayerAgents)
		x0[i] = secondBoughtOpinion;
	AInf.resize(A.size());
	x.resize(x0.size());
	for (int i = 0; i < A.size(); i++)
	{
		AInf[i].resize(A[i].size());
		for (int j = 0; j < A[i].size(); j++)
		{
			AInf[i][j] = A[i][j];
		}
	}
	for (int i = 0; i < x.size(); i++)
		x[i] = x0[i];

	int n = 1;
	while (!checkGameSolved(AInf, x))
	{
		AInf = matrixMult(AInf, AInf);
		x = maxtrixMultVector(A, x);
		n++;
	}
	cout << "Количество итераций " << n << endl;
	cout << "---------------------------------" << endl;
	cout << "Решение игры с купленными агентами:" << endl;
	cout << "Начальный вектор мнений с купленными агентами:" << endl;
	for (int i = 0; i < this->x0.size(); i++)
		cout << this->x0[i] << ", ";
	cout << endl;
	cout << "Итоговая матрица влияния: " << endl;
	for (int i = 0; i < AInf.size(); i++)
	{
		for (int j = 0; j < AInf[i].size(); j++)
		{
			cout << AInf[i][j] << ", ";
		}
		cout << endl;
	}
	cout << "Итоговое мнение : " << endl;
	for (int i = 0; i < x.size(); i++)
		cout << x[i] << ", ";
	cout << endl;
}


NetGame::~NetGame()
{
}
