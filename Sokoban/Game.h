#pragma once
#include <SFML/Graphics.hpp>
#include "GridNode.h"
#include <vector>
#include <chrono>
#include <thread>
using namespace sf;

void gameInit();

void refreshMenu();
void refreshKeyboardMenu(int&);

void refreshLevel();
void refreshKeyboardLevel();

void refreshPauseLevel();
void refreshKeyboardPauseLevel(int&);

void refreshLevelRecordMenu();
void refreshLevelRecordKeyboardMenu(int&);

void loopRefresh();