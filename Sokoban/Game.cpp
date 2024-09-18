#include "Game.h"
using namespace std;

RenderWindow gameWindow(VideoMode(640, 640), "Sokoban");
Sprite Box, BoxLace, Brick, Floor, Player;
Event gameEvent;
RectangleShape fontImage, titleSQ, buttonSQ;
Font gameFont, buttonFont;
Text gameName, start, savedRepetitions;

GridNode gameGrid(10);
boxPlace* boxPlaces;

int selectedMenuOption;
int currentLevel;
bool levelLoaded;
bool isUp, isDown, isLeft, isRight;

void gameInit() {

	//gameWindow.setFramerateLimit(25);

	// load icon
	auto image = sf::Image{};
	image.loadFromFile("textures/player.png");
	gameWindow.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());
	//

	//load textures
	Texture boxFile; boxFile.loadFromFile("textures/box.png");
	Box.setTexture(boxFile, false);
	Box.setScale(2, 2);
	Texture boxLaceFile; boxLaceFile.loadFromFile("textures/box_lace.png");
	BoxLace.setTexture(boxLaceFile);
	BoxLace.setScale(2, 2);
	Texture brickFile; brickFile.loadFromFile("textures/brick.png");
	Brick.setTexture(brickFile);
	Brick.setScale(2, 2);
	Texture floorFile; floorFile.loadFromFile("textures/floor.png");
	Floor.setTexture(floorFile);
	Floor.setScale(2, 2);
	Texture playerFile; playerFile.loadFromFile("textures/player.png");
	Player.setTexture(playerFile);
	Player.setScale(2, 2);
	//

	// load menu
	fontImage.setSize(Vector2f(640, 640));
	fontImage.setFillColor(Color(222, 214, 174));

	buttonFont.loadFromFile("fonts/buttonFont.ttf");
	gameFont.loadFromFile("fonts/gameFont.otf");

	titleSQ.setSize(Vector2f(500, 110));
	titleSQ.setFillColor(Color(68, 64, 36, 255));
	titleSQ.setPosition(gameWindow.getSize().x / 2 - titleSQ.getSize().x / 2, 120);

	gameName.setFont(gameFont);
	gameName.setCharacterSize(65);
	gameName.setFillColor(Color(222, 214, 174));
	gameName.setString("SOKOBAN");
	gameName.setPosition(87, 128);

	buttonSQ.setSize(Vector2f(360, 60));
	buttonSQ.setFillColor(Color::White);
	buttonSQ.setPosition(gameWindow.getSize().x / 2 - buttonSQ.getSize().x / 2, 269);

	start.setFont(buttonFont);
	start.setCharacterSize(45);
	start.setFillColor(Color(222, 214, 174));
	start.setString("Empezar");
	start.setPosition(220, 270);

	savedRepetitions.setFont(buttonFont);
	savedRepetitions.setCharacterSize(45);
	savedRepetitions.setFillColor(Color::White);
	savedRepetitions.setString("Historial");
	savedRepetitions.setPosition(212, 350);

	//

	currentLevel = 1;
	levelLoaded = false;

	isUp = false, isDown = false, isLeft = false, isRight = false;

	int menu = 0;
	selectedMenuOption = 0;
	
	while (gameWindow.isOpen()) {

		//principal menu
		if (menu == 0) {

			refreshKeyboardMenu(menu);
			refreshMenu();
		}

		//startgame menu
		if (menu == 1) {
			if (!levelLoaded) {
				gameGrid.loadLevel(currentLevel, boxPlaces);
				levelLoaded = true;
			}
			if (checkBoxPlaces()) {
				cout << "Nivel #" << currentLevel << " ganado" << endl; //delete
				//cargar siguiente nivel
			}
			refreshKeyboardLevel();
			refreshLevel();
		}

		//saved records menu
 		if (menu == 2) {

			refreshLevelRecordKeyboardMenu(menu);
			refreshLevelRecordMenu();
		}

		gameWindow.display();
		loopRefresh();
		this_thread::sleep_for(chrono::milliseconds(25));
	}
}

bool checkBoxPlaces() {

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

void refreshKeyboardMenu(int& menu) {

	if ((Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W)) && !isUp) {
		selectedMenuOption--;
		if (selectedMenuOption < 0) {
			selectedMenuOption++;
		}
		isUp = true;
	}
	if (!(Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W))) {
		isUp = false;
	}

	if ((Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S)) && !isDown) {
		selectedMenuOption++;
		if (selectedMenuOption > 1) {
			selectedMenuOption--;
		}
		isDown = true;
	}
	if (!(Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S))) {
		isDown = false;
	}

	if (Keyboard::isKeyPressed(Keyboard::Enter) && selectedMenuOption == 0) {
		menu = 1;
	}
	if (Keyboard::isKeyPressed(Keyboard::Enter) && selectedMenuOption == 1) {
		menu = 2;
	}
}

void refreshLevel(){

	gameWindow.draw(fontImage);

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			switch (gameGrid.getStruct(j, i)) {
			case '_':
				Floor.setPosition(j * 64, i * 64);
				gameWindow.draw(Floor);
				break;
			case '@':
				Floor.setPosition(j * 64, i * 64);
				gameWindow.draw(Floor);
				Player.setPosition(j * 64, i * 64);
				gameWindow.draw(Player);
				break;
			case '#':
				Brick.setPosition(j * 64, i * 64);
				gameWindow.draw(Brick);
				break;
			case '.':
				Floor.setPosition(j * 64, i * 64);
				gameWindow.draw(Floor);
				BoxLace.setPosition(j * 64, i * 64);
				gameWindow.draw(BoxLace);
				break;
			case '$':
				Floor.setPosition(j * 64, i * 64);
				gameWindow.draw(Floor);
				Box.setPosition(j * 64, i * 64);
				gameWindow.draw(Box);
				break;
			case '!':
				Floor.setPosition(j * 64, i * 64);
				gameWindow.draw(Floor);
				Box.setPosition(j * 64, i * 64);
				Box.setColor(Color(Box.getColor().r - 100, Box.getColor().g - 100, Box.getColor().b - 100, 255));
				gameWindow.draw(Box);
				Box.setColor(Color(Box.getColor().r + 100, Box.getColor().g + 100, Box.getColor().b + 100, 255));
				break;
			}
		}
	}

}

void refreshKeyboardLevel(){

	//up
	if ((Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W)) && !isUp) {
		cout << "Up\n";
		gameGrid.movePlayer('w', boxPlaces);
		isUp = true;
		gameGrid.printList(boxPlaces); //delete
	}
	if (!(Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W))) {
		isUp = false;
	}

	//down
	if ((Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S)) && !isDown) {
		cout << "Down\n";
		gameGrid.movePlayer('s', boxPlaces);
		isDown = true;
		gameGrid.printList(boxPlaces); //delete
	}
	if (!(Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S))) {
		isDown = false;
	}

	//left
	if ((Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A)) && !isLeft) {
		cout << "Left\n";
		gameGrid.movePlayer('a', boxPlaces);
		isLeft = true;
		gameGrid.printList(boxPlaces); //delete
	}
	if (!(Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))) {
		isLeft = false;
	}

	//right
	if ((Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D)) && !isRight) {
		cout << "Right\n";
		gameGrid.movePlayer('d', boxPlaces);
		isRight = true;
		gameGrid.printList(boxPlaces); //delete
	}
	if (!(Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))) {
		isRight = false;
	}
	Event e;
}

void refreshMenu() {
	gameWindow.draw(fontImage);

	gameWindow.draw(titleSQ);
	gameWindow.draw(gameName);

	if (selectedMenuOption == 0) {
		start.setFillColor(Color(222, 214, 174));
		savedRepetitions.setFillColor(Color::White);
		buttonSQ.setPosition(gameWindow.getSize().x / 2 - buttonSQ.getSize().x / 2, 269);
	}
	else {
		start.setFillColor(Color::White);
		savedRepetitions.setFillColor(Color(222, 214, 174));
		buttonSQ.setPosition(gameWindow.getSize().x / 2 - buttonSQ.getSize().x / 2, 349);
	}
	gameWindow.draw(buttonSQ);
	gameWindow.draw(start);
	gameWindow.draw(savedRepetitions);
}

void refreshLevelRecordMenu() {
	gameWindow.draw(fontImage);
}

void refreshLevelRecordKeyboardMenu(int& menu) {
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		menu = 0;
	}
}

void loopRefresh() {
	while (gameWindow.pollEvent(gameEvent)) {
		if (gameEvent.type == Event::Closed) {
			gameWindow.close();
		}
	}
}