#include "ScoreSystem.h"
#include <csignal>
#include "Logger.h"

void ScoreSystem::calculateLevel() {
	if (m_totalLines >= (m_level * 10) + 10) {
		m_level++;
	}
}

int ScoreSystem::addScore(int lines) {
	m_totalLines += lines;
	calculateLevel();
	const int newScore = calculateGivenScore(lines);
	m_score += newScore;
	return newScore;
}

int ScoreSystem::calculateGivenScore(int lines) const {
	switch (lines) {
		case 1: return 40 * (m_level+1);
		case 2: return 100 * (m_level+1);
		case 3: return 300 * (m_level+1);
		case 4: return 1200 * (m_level+1);
		default: FATAL("lines can't be more than 4");
	}
	return -1;
}
