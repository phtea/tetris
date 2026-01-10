#pragma once

class ScoreSystem {
	public:
		[[nodiscard]] int getScore() const { return m_score; }
		[[nodiscard]] int getLevel() const { return m_level; }
		[[nodiscard]] int getTotalLines() const { return m_totalLines; }
		int addScore(int lines);
	private:
		int m_score{0};
		int m_totalLines{0};
		int m_level{0};
		int m_startingLevel{0};

		int calculateGivenScore(int lines);
		void calculateLevel();
};
