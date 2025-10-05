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
	int newScore = amountToAdd(lines);
	m_score += newScore;
	return newScore;
}

int ScoreSystem::amountToAdd(int lines) {
	switch (m_level) {
		case 0: return handleLevel0(lines);
		case 1: return handleLevel1(lines);
		case 2: return handleLevel2(lines);
		case 9: return handleLevel9(lines);
	}
	return handleLevelN(lines);
}

int ScoreSystem::handleLevel0(int lines) {
	switch (lines) {
		case 1: return 40;
		case 2: return 100;
		case 3: return 300;
		case 4: return 1200;
	}
	FATAL("lines can't be more than 4");
	return -1;
}

int ScoreSystem::handleLevel1(int lines) {
	switch (lines) {
		case 1: return 80;
		case 2: return 200;
		case 3: return 600;
		case 4: return 2400;
	}
	FATAL("lines can't be more than 4");
	return -1;
}

int ScoreSystem::handleLevel2(int lines) {
	switch (lines) {
		case 1: return 120;
		case 2: return 300;
		case 3: return 900;
		case 4: return 3600;
	}
	FATAL("lines can't be more than 4");
	return -1;
}

int ScoreSystem::handleLevel9(int lines) {
	switch (lines) {
		case 1: return 400;
		case 2: return 1000;
		case 3: return 3000;
		case 4: return 12000;
	}
	FATAL("lines can't be more than 4");
	return -1;
}

int ScoreSystem::handleLevelN(int lines) {
	switch (lines) {
		case 1: return 40 * (m_level+1);
		case 2: return 100 * (m_level+1);
		case 3: return 300 * (m_level+1);
		case 4: return 1200 * (m_level+1);
	}
	FATAL("lines can't be more than 4");
	return -1;
}
