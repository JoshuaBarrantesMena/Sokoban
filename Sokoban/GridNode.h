#pragma once
#include <iostream>
#include <fstream>
#include <string>

struct node {
	char character;
	int x;
	int y;
	node* next;
	node* previous;
	node* top;
	node* bottom;
};

struct boxPlace {
	int x;
	int y;
	boxPlace* next;
	bool isUsed = false;
	boxPlace(int X, int Y, bool Used) : x(X), y(Y), isUsed(Used), next(nullptr) {}
};

class GridNode {
private:
	
	node* head;
	boxPlace* boxPlaces;
	int size;

	void linkTopAndBot(node*&);
	void newNextNode(node*&);
	void newBottomNode(node*&);
	void addBoxPlace(int, int, bool);
	void clear();

public:

	GridNode(int);
	~GridNode();

	char getStruct(int, int);
	bool checkBoxPlaces();
	void loadLevel(int);
	void movePlayer(char);
	void printList();
	void setBoxPlaceState(int, int, bool);
};