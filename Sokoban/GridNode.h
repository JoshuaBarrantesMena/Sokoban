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
	int size;

	void linkTopAndBot(node*&);
	void newNextNode(node*&);
	void newBottomNode(node*&);
	void setStruct(int, int, char);

public:

	GridNode(int);
	~GridNode();

	char getStruct(int, int);
	void loadLevel(int, boxPlace*&);
	void movePlayer(char, boxPlace*&);
	void printList(boxPlace*&);
};

void setBoxPlaceState(int, int, bool, boxPlace*&);