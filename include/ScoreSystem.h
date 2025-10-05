#pragma once

class ScoreSystem {
	public:
		ScoreSystem() : m_score(0), m_totalLines(0), m_level(0), m_startingLevel(0) {} 
		int getScore() { return m_score; }
		int getLevel() { return m_level; }
		int getTotalLines() { return m_totalLines; }
		int addScore(int lines);
	private:
		int m_score;
		int m_totalLines;
		int m_level;
		int m_startingLevel;

		int handleLevel0(int lines);
		int handleLevel1(int lines);
		int handleLevel2(int lines);
		int handleLevel9(int lines);
		int handleLevelN(int lines);

		void calculateLevel();

		int amountToAdd(int lines);
};
