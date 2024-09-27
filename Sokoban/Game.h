#pragma once
#include <SFML/Graphics.hpp>
#include "GridNode.h"
#include <vector>
#include <chrono>
#include <thread>
#include <windows.h>
using namespace sf;

void gameInit();

void refreshMenu();
void refreshKeyboardMenu(int&);

void refreshLevel(int, int&);
void refreshKeyboardLevel();

void refreshPauseLevel();
void refreshKeyboardPauseLevel(int&, int&, int&);

void refreshWinLevel(int&);
void refreshKeyboardWinLevel(int&);

void refreshLevelRecordMenu();
void refreshLevelRecordKeyboardMenu(int&);

void refreshRecord(int&);
void refreshKeyboardRecord(int&);

int localReplay();
void makeReplay(int, int);
void deleteReplays(int, int, bool);
void chargeReplay(int);
void loadingRecordScreen();
void gameWinned();
void loopRefresh();