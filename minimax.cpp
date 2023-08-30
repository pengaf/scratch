// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#include <array>
#include <memory>
#include <string>
#include <unordered_map>
#include <chrono>
#include <fstream>
#include <optional>
#include <math.h>
#include <assert.h>


class State
{
public:
	//int getState(int x, int y) const
	//{
	//	if (0 <= x && x <= 2 && 0 <= y && y <= 2)
	//	{
	//		return m_state[y][x];
	//	}
	//	return 0;
	//}
	//bool empty(int x, int y) const
	//{
	//	if (0 <= x && x <= 2 && 0 <= y && y <= 2)
	//	{
	//		return 0 == m_state[y][x];
	//	}
	//	return false;
	//}
	bool tryPlay(int xy, bool first)
	{
		return tryPlay(xy % 3, xy / 3, first);
	}
	bool tryPlay(int x, int y, bool first)
	{
		if (0 <= x && x <= 2 && 0 <= y && y <= 2)
		{
			if (0 == m_state[y][x])
			{
				m_state[y][x] = first ? 1 : -1;
				++m_numStones;
				return true;
			}
		}
		return false;
	}
	int numStones() const
	{
		return m_numStones;
	}
	bool full() const
	{
		return 9 == m_numStones;
	}
	int score() const
	{
		int sum;
		for (int i = 0; i < 3; ++i)
		{
			sum = m_state[i][0] + m_state[i][1] + m_state[i][2];
			if (3 == sum)
			{
				return 1;
			}
			else if (-3 == sum)
			{
				return -1;
			}
			sum = m_state[0][i] + m_state[1][i] + m_state[2][i];
			if (3 == sum)
			{
				return 1;
			}
			else if (-3 == sum)
			{
				return -1;
			}
		}
		sum = m_state[0][0] + m_state[1][1] + m_state[2][2];
		if (3 == sum)
		{
			return 1;
		}
		else if (-3 == sum)
		{
			return -1;
		}
		sum = m_state[0][2] + m_state[1][1] + m_state[2][0];
		if (3 == sum)
		{
			return 1;
		}
		else if (-3 == sum)
		{
			return -1;
		}
		return 0;
	}
	bool end(int& s) const
	{
		s = score();
		return (0 != s || full());
	}
private:
	int m_state[3][3]{};
	int m_numStones{};
};

const int infinite = 10;
int g_count = 0;

int minimax(const State& state, bool firstPlayer)
{
	++g_count;
	int score = state.score();
	if (0 != score)
	{
		return score;
	}
	if (state.full())
	{
		return 0;
	}
	if (firstPlayer)
	{
		int maxScore = -infinite;
		for (int i = 0; i < 9; ++i)
		{
			State nextState = state;
			if (nextState.tryPlay(i % 3, i / 3, firstPlayer))
			{
				int score = minimax(nextState, !firstPlayer);
				if (maxScore < score)
				{
					maxScore = score;
				}
			}
		}
		return maxScore;
	}
	else
	{
		int minScore = infinite;
		for (int i = 0; i < 9; ++i)
		{
			State nextState = state;
			if (nextState.tryPlay(i % 3, i / 3, firstPlayer))
			{
				int score = minimax(nextState, !firstPlayer);
				if (minScore > score)
				{
					minScore = score;
				}
			}
		}
		return minScore;
	}
}

int alphabeta(const State& state, bool firstPlayer, int alpha, int beta)
{
	++g_count;
	int score = state.score();
	if (0 != score)
	{
		return score;
	}
	if (state.full())
	{
		return 0;
	}
	if (firstPlayer)
	{
		int maxScore = -infinite;
		for (int i = 0; i < 9; ++i)
		{
			State nextState = state;
			if (nextState.tryPlay(i % 3, i / 3, firstPlayer))
			{
				int score = alphabeta(nextState, !firstPlayer, alpha, beta);
				if (maxScore < score)
				{
					maxScore = score;
				}
				if (alpha < score)
				{
					alpha = score;
				}
				if (beta <= alpha)
				{
					break;
				}
			}
		}
		return maxScore;
	}
	else
	{
		int minScore = infinite;
		for (int i = 0; i < 9; ++i)
		{
			State nextState = state;
			if (nextState.tryPlay(i % 3, i / 3, firstPlayer))
			{
				int score = alphabeta(nextState, !firstPlayer, alpha, beta);
				if (minScore > score)
				{
					minScore = score;
				}
				if (beta > score)
				{
					beta = score;
				}
				if (beta <= alpha)
				{
					break;
				}
			}
		}
		return minScore;
	}
}

class Agent
{
public:
	void play(State& state, bool firstPlayer)
	{
		assert(!state.full() && 0 == state.score());
		int numStones = state.numStones();
		doPlay(state, firstPlayer);
		assert(state.numStones() == numStones + 1);
	}
protected:
	virtual void doPlay(State& state, bool firstPlayer) = 0;
};

class RandomAgent : public Agent
{
protected:
	virtual void doPlay(State& state, bool firstPlayer)
	{
		int xy[] = { 0,1,2,3,4,5,6,7,8 };
		std::random_shuffle(&xy[0], &xy[9]);
		for (int i = 0; i < 9; ++i)
		{
			if (state.tryPlay(xy[i], firstPlayer))
			{
				break;
			}
		}
	}
};

class MinimaxAgent : public Agent
{
protected:
	virtual void doPlay(State& state, bool firstPlayer)
	{
		int xy;
		if (firstPlayer)
		{
			int maxScore = -infinite;
			for (int i = 0; i < 9; ++i)
			{
				State nextState = state;
				if (nextState.tryPlay(i, firstPlayer))
				{
					int score = minimax(nextState, !firstPlayer);
					if (maxScore < score)
					{
						maxScore = score;
						xy = i;
					}
				}
			}
		}
		else
		{
			int minScore = infinite;
			for (int i = 0; i < 9; ++i)
			{
				State nextState = state;
				if (nextState.tryPlay(i, firstPlayer))
				{
					int score = minimax(nextState, !firstPlayer);
					if (minScore > score)
					{
						minScore = score;
						xy = i;
					}
				}
			}
		}
		state.tryPlay(xy, firstPlayer);
	}
};


class AlphabetaAgent : public Agent
{
protected:
	virtual void doPlay(State& state, bool firstPlayer)
	{
		int xy;
		int alpha = -infinite;
		int beta = infinite;
		if (firstPlayer)
		{
			int maxScore = -infinite;
			for (int i = 0; i < 9; ++i)
			{
				State nextState = state;
				if (nextState.tryPlay(i % 3, i / 3, firstPlayer))
				{
					int score = alphabeta(nextState, !firstPlayer, alpha, beta);
					if (maxScore < score)
					{
						maxScore = score;
						xy = i;
					}
					if (alpha < score)
					{
						alpha = score;
					}
					if (beta <= alpha)
					{
						break;
					}
				}
			}
		}
		else
		{
			int minScore = infinite;
			for (int i = 0; i < 9; ++i)
			{
				State nextState = state;
				if (nextState.tryPlay(i % 3, i / 3, firstPlayer))
				{
					int score = alphabeta(nextState, !firstPlayer, alpha, beta);
					if (minScore > score)
					{
						minScore = score;
						xy = i;
					}
					if (beta > score)
					{
						beta = score;
					}
					if (beta <= alpha)
					{
						break;
					}
				}
			}
		}
		state.tryPlay(xy, firstPlayer);
	}
};


void test()
{
	MinimaxAgent angent0;
	//MinimaxAgent randomAgent;
	AlphabetaAgent angent1;
	//RandomAgent angent1;

	int winCount = 0;
	int loseCount = 0;

	Agent* agents[2];
	Agent* scores[2];

	int matches = 100;
	srand(time(NULL));
	for (int i = 0; i < matches; ++i)
	{
		int toss = rand() % 2;
		agents[toss] = &angent0;
		agents[1-toss] = &angent1;
		State state;
		int score;
		while (true)
		{
			agents[0]->play(state, true);
			if (state.end(score))
			{
				assert(score >= 0);
				if (0 != score)
				{
					if (0 == toss)
					{
						++winCount;
					}
					else
					{
						++loseCount;
					}
				}
				break;
			}
			agents[1]->play(state, false);
			if (state.end(score))
			{
				assert(score <= 0);
				if (0 != score)
				{
					if (0 == toss)
					{
						++loseCount;
					}
					else
					{
						++winCount;
					}
				}
				break;
			}
		}
	}
	std::cout << "win:" << winCount << "  lose:" << loseCount << "  draw:" << matches - winCount - loseCount << std::endl;
}

int main()
{
	test();

	//State state;
	//int alpha = -infinite;
	//int beta = infinite;


	//state.tryPlay(0, true);
	//state.tryPlay(1, true);
	////state.tryPlay(2, true);
	////int score = alphabeta(state, true, alpha, beta);
	////int score = minimax(state, true);
	//std::cout << g_count << "  " << score;
}

