#include "GridNode.h"
using namespace std;

GridNode::GridNode(int pSize){

	head = nullptr;
	this->size = pSize;
	for (int i = 0; i < size; i++){
		newNextNode(head);
	}
	for (int i = 0; i < size; i++){
		newBottomNode(head);
	}
}

GridNode::~GridNode(){

	node* currentNode = head;
	node* bottomNode = head->bottom;
	node* auxNode = head;

	while (currentNode != nullptr) {
		auxNode = currentNode;
		while (auxNode != nullptr) {
			node* deleteNode = auxNode;
			auxNode = auxNode->next;
			delete deleteNode;
		}
		currentNode = bottomNode;
		if (bottomNode != nullptr) {
			bottomNode = bottomNode->bottom;
		}
	}
	boxPlace* currentBox = boxPlaces;

	while (currentBox != nullptr) {

		boxPlace* deleteBox = currentBox;
		currentBox = currentBox->next;
		delete deleteBox;
	}
}

void GridNode::clear(){

	node* currentNode = head;
	node* auxNode = head;

	while (currentNode != nullptr) {
		auxNode = currentNode;
		while (auxNode != nullptr) {
			auxNode->character = '_';
			auxNode = auxNode->next;
		}
		currentNode = currentNode->bottom;
	}

	boxPlace* currentBox = boxPlaces;
	boxPlace* deleteBox;

	while (currentBox != nullptr) {

		deleteBox = currentBox;
		currentBox = currentBox->next;
		delete deleteBox;
	}
	boxPlaces = nullptr;
}

char GridNode::getStruct(int x, int y){

	node* current = head;
	while (current != nullptr && current->x != x) {
		current = current->next;
	}
	while (current != nullptr && current->y != y) {
		current = current->bottom;
	}
	if (current == nullptr) {
		return '#';
	}
	return current->character;
}

bool GridNode::checkBoxPlaces() {

	boxPlace* current = boxPlaces;

	if (current == nullptr) {
		return true;
	}

	int completes = 0, size = 0;

	while (current != nullptr) {
		if (current->isUsed) {
			completes++;
		}
		current = current->next;
		size++;
	}
	return completes == size;
}

void GridNode::linkTopAndBot(node*& top) {

	node* topNode = top;
	node* botNode = topNode->bottom;
	while (topNode != nullptr && botNode != nullptr) {

		topNode->bottom = botNode;
		botNode->top = topNode;
		topNode = topNode->next;
		botNode = botNode->next;
	}
}

void GridNode::newNextNode(node*& current) {

	node* newNode = new node;
	newNode->character = '_';
	newNode->next = nullptr;
	newNode->previous = nullptr;
	newNode->top = nullptr;
	newNode->bottom = nullptr;

	if (current == nullptr){
		current = newNode;
		newNode->x = 0;
		newNode->y = 0;
	}
	else
	{
		node* currentNode = current;
		currentNode->x = current->x;
		currentNode->y = current->y;

		while (currentNode->next != nullptr){
			currentNode = currentNode->next;
		}

		currentNode->next = newNode;
		newNode->previous = currentNode;
		newNode->x = currentNode->x;
		newNode->y = currentNode->y;

		newNode->x++;
	}
}

void GridNode::newBottomNode(node*& current) {

	node* newNode = new node;
	newNode->character = '_';
	newNode->next = nullptr;
	newNode->previous = nullptr;
	newNode->top = nullptr;
	newNode->bottom = nullptr;

	if (current == nullptr){
		head = newNode;
		newNode->x = 0;
		newNode->y = 0;
	}
	else{

		node* currentNode = current;
		currentNode->y = current->y;
		currentNode->x = current->x;

		while (currentNode->bottom != nullptr){
			currentNode = currentNode->bottom;
		}

		currentNode->bottom = newNode;
		newNode->top = currentNode;
		newNode->y = currentNode->y;
		newNode->x = currentNode->x;

		currentNode = newNode->top;
		newNode->y++;
		for (int i = 0; i < size; i++){
			newNextNode(newNode);
		}
		linkTopAndBot(currentNode);
	}
}

void GridNode::addBoxPlace(int x, int y, bool isUsed){

	boxPlace* newPlace = new boxPlace(x, y, isUsed);

	if (!boxPlaces) {
		boxPlaces = newPlace;
		return;
	}
	boxPlace* current = boxPlaces;

	while (current->next != nullptr) {
		current = current->next;
	}
	current->next = newPlace;
}

void GridNode::loadLevel(int levelIndex) {

	clear();

	boxPlace* currentPlace = boxPlaces;
	ifstream level;
	string text;
	level.open("levels/l" + to_string(levelIndex) + ".txt", ios::in);

	if (level.fail()){
		exit(1);
	}
	node* auxNode1 = head;
	node* auxNode2 = head;
	int y = 0;

	while (!level.eof()){

		getline(level, text);
		for (int x = 0; x < text.size(); x++){

			auxNode1->character = text[x];
			auxNode1 = auxNode1->next;
			if (text[x] == '.') {
				addBoxPlace(x, y, false);
			}
			if (text[x] == '!') {
				addBoxPlace(x, y, true);
			}
		}
		auxNode2 = auxNode2->bottom;
		auxNode1 = auxNode2;
		y++;
	}
	level.close();
}

void GridNode::movePlayer(char move) {

	node* currentNode = head;
	node* auxNode = head;
	bool flag = false;
	while (currentNode != nullptr && !flag) {
		auxNode = currentNode;
		while (auxNode != nullptr) {
			if (auxNode->character == '@') {
				flag = true;
				break;
			}
			auxNode = auxNode->next;
		}
		currentNode = currentNode->bottom;
	}
	currentNode = auxNode;
	if (currentNode == nullptr) {
		return;
	}
	cout << currentNode->x << ", " << currentNode->y << "\n";

	switch (move) {
	case 'w': // Movimiento hacia arriba
		if (currentNode->top != nullptr && currentNode->top->character != '#') {
			if (currentNode->top->character == '$' || currentNode->top->character == '!'){

				if (currentNode->top->top != nullptr && currentNode->top->top->character != '#') {
					setBoxPlaceState(currentNode->top->x, currentNode->top->y, false);
				}
				if (currentNode->top->top != nullptr && currentNode->top->top->character == '.') {
					setBoxPlaceState(currentNode->top->top->x, currentNode->top->top->y, true);
					currentNode->top->top->character = '!';
					currentNode->top->character = '@';
					currentNode->character = '_';
				}
				if (currentNode->top->top != nullptr && currentNode->top->top->character == '_') {
					currentNode->top->top->character = '$';
					currentNode->top->character = '@';
					currentNode->character = '_';
				}
			}
			else if (currentNode->top->character == '_' || currentNode->top->character == '.') {
				currentNode->top->character = '@';
				currentNode->character = '_';
			}
		}
		break;
	case 's': // Movimiento hacia abajo
		if (currentNode->bottom != nullptr && currentNode->bottom->character != '#') {
			if (currentNode->bottom->character == '$' || currentNode->bottom->character == '!'){

				if (currentNode->bottom->bottom != nullptr && currentNode->bottom->bottom->character != '#') {
					setBoxPlaceState(currentNode->bottom->x, currentNode->bottom->y, false);
				}
				if (currentNode->bottom->bottom != nullptr && currentNode->bottom->bottom->character == '.') {
					setBoxPlaceState(currentNode->bottom->bottom->x, currentNode->bottom->bottom->y, true);
					currentNode->bottom->bottom->character = '!';
					currentNode->bottom->character = '@';
					currentNode->character = '_';
				}
				if (currentNode->bottom->bottom != nullptr && currentNode->bottom->bottom->character == '_') {
					currentNode->bottom->bottom->character = '$';
					currentNode->bottom->character = '@';
					currentNode->character = '_';
				}
			}
			else if (currentNode->bottom->character == '_' || currentNode->bottom->character == '.') {
				currentNode->bottom->character = '@';
				currentNode->character = '_';
			}
		}
		break;
	case 'a': // Movimiento hacia la izquierda
		if (currentNode->previous != nullptr && currentNode->previous->character != '#') {
			if (currentNode->previous->character == '$' || currentNode->previous->character == '!'){

				if (currentNode->previous->previous != nullptr && currentNode->previous->previous->character != '#') {
					setBoxPlaceState(currentNode->previous->x, currentNode->previous->y, false);
				}
				if (currentNode->previous->previous != nullptr && currentNode->previous->previous->character == '.') {
					setBoxPlaceState(currentNode->previous->previous->x, currentNode->previous->previous->y, true);
					currentNode->previous->previous->character = '!';
					currentNode->previous->character = '@';
					currentNode->character = '_';
				}
				if (currentNode->previous->previous != nullptr && currentNode->previous->previous->character == '_') {
					currentNode->previous->previous->character = '$';
					currentNode->previous->character = '@';
					currentNode->character = '_';
				}
			}
			else if (currentNode->previous->character == '_' || currentNode->previous->character == '.') {
				currentNode->previous->character = '@';
				currentNode->character = '_';
			}
		}
		break;
	case 'd': // Movimiento hacia la derecha
		if (currentNode->next != nullptr && currentNode->next->character != '#') {
			if (currentNode->next->character == '$' || currentNode->next->character == '!') {

				if (currentNode->next->next != nullptr && currentNode->next->next->character != '#') {
					setBoxPlaceState(currentNode->next->x, currentNode->next->y, false);
				}
				if (currentNode->next->next != nullptr && currentNode->next->next->character == '.') {
					setBoxPlaceState(currentNode->next->next->x, currentNode->next->next->y, true);
					currentNode->next->next->character = '!';
					currentNode->next->character = '@';
					currentNode->character = '_';
				}
				if (currentNode->next->next != nullptr && currentNode->next->next->character == '_') {
					currentNode->next->next->character = '$';
					currentNode->next->character = '@';
					currentNode->character = '_';
				}
			}
			else if (currentNode->next->character == '_' || currentNode->next->character == '.') {
				currentNode->next->character = '@';
				currentNode->character = '_';
			}
		}
		break;
	}
	boxPlace* current = boxPlaces;
	currentNode = head;
	auxNode = head;

	while (currentNode != nullptr) {
		auxNode = currentNode;
		while (auxNode != nullptr) {
			if (current != nullptr && auxNode->x == current->x && auxNode->y == current->y) {
				if (auxNode->character == '_' && !current->isUsed) {
					auxNode->character = '.';
				}
				current = current->next;
			}
			auxNode = auxNode->next;
		}
		currentNode = currentNode->bottom;
	}
}

void GridNode:: printList() { //delete
	
	system("cls");

	node* currentNode = head;
	node* auxNode = head;
	while (currentNode != nullptr) {
		auxNode = currentNode;
		while (auxNode != nullptr) {
			cout << auxNode->character;
			auxNode = auxNode->next;
		}
		cout << endl;
		currentNode = currentNode->bottom;
	}

	boxPlace* actual = boxPlaces;
	while (actual != nullptr) {
		cout << actual->x << ", " << actual->y << ", " << actual->isUsed << endl;
		actual = actual->next;
	}
}

void GridNode::setBoxPlaceState(int x, int y, bool isUsed){

	boxPlace* current = boxPlaces;
	while (current != nullptr) {
		if (current->x == x && current->y == y) {
			current->isUsed = isUsed;
			break;
		}
		current = current->next;
	}
}