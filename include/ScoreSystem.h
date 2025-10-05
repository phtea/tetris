#pragma once

class ScoreSystem {
	public:
		ScoreSystem() : m_currentScore(0) {} 
		int getCurrentScore() { return m_currentScore; }
		int addScore(int level, int lines);
	private:
		int m_currentScore;

		int amountToAdd(int level, int lines);
};
