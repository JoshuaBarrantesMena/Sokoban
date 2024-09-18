#pragma once
#include <SFML/Graphics.hpp>
#include "GridNode.h"
#include <vector>
#include <chrono>
#include <thread>
using namespace sf;

void gameInit();
bool checkBoxPlaces();

void refreshMenu();
void refreshKeyboardMenu(int&);

void refreshLevel();
void refreshKeyboardLevel();

void refreshLevelRecordMenu();
void refreshLevelRecordKeyboardMenu(int&);

void loopRefresh();

