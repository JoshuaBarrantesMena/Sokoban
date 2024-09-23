#include "Game.h"
using namespace std;

RenderWindow gameWindow(VideoMode(640, 640), "Sokoban");
Sprite Box, BoxLace, Brick, Floor, Player;
Event gameEvent;
RectangleShape fontImage, titleSQ, buttonSQ, pauseImage;
Font gameFont, buttonFont, pauseFont;
Text gameName, start, savedRepetitions, resume, reloadLevel, exitLevel, saveAndExit;

GridNode gameGrid(10);

int selectedMenuOption, selectedPauseOption;
int currentLevel;
bool levelLoaded, paused;
bool isUp, isDown, isLeft, isRight, isPausedKey, isEnter;

void gameInit() {

	gameWindow.setFramerateLimit(25);

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

	buttonSQ.setFillColor(Color::White);

	gameName.setFont(gameFont);
	gameName.setCharacterSize(65);
	gameName.setFillColor(Color(222, 214, 174));
	gameName.setString("SOKOBAN");
	gameName.setPosition(87, 128);

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

	// load pause
	pauseFont.loadFromFile("fonts/pauseFont.ttf");

	pauseImage.setSize(Vector2f(288, 640));
	pauseImage.setFillColor(Color(0, 0, 0, 128));
	pauseImage.setPosition(352, 0);

	resume.setFont(pauseFont);
	resume.setCharacterSize(30);
	resume.setFillColor(Color::White);
	resume.setString("reanudar");
	resume.setPosition(433, 80);

	reloadLevel.setFont(pauseFont);
	reloadLevel.setCharacterSize(30);
	reloadLevel.setFillColor(Color::White);
	reloadLevel.setString("Volver a empezar");
	reloadLevel.setPosition(377, 140);

	saveAndExit.setFont(pauseFont);
	saveAndExit.setCharacterSize(30);
	saveAndExit.setFillColor(Color::White);
	saveAndExit.setString("Guardar y salir");
	saveAndExit.setPosition(385, 200);

	exitLevel.setFont(pauseFont);
	exitLevel.setCharacterSize(30);
	exitLevel.setFillColor(Color::White);
	exitLevel.setString("salir");
	exitLevel.setPosition(459, 260);
	//

	currentLevel = 1;
	levelLoaded = false;
	paused = false;

	isUp = false, isDown = false, isLeft = false, isRight = false, isPausedKey = false, isEnter = false;

	int menu = 0;
	selectedMenuOption = 0, selectedPauseOption = 0;
	
	while (gameWindow.isOpen()) {

		//principal menu
		if (menu == 0) {

			refreshKeyboardMenu(menu);
			refreshMenu();
		}

		//startgame menu
		if (menu == 1) {
			if (!levelLoaded) {
				gameGrid.loadLevel(currentLevel);
				levelLoaded = true;
			}
			if (gameGrid.checkBoxPlaces()) {
				cout << "Nivel #" << currentLevel << " ganado" << endl; //delete
				//cargar siguiente nivel
				//limpiar matriz del nivel
				//limpiar lista de puntos de cajas
				//aumentar el nuvel actual en 1
			}
			refreshKeyboardLevel();
			refreshKeyboardPauseLevel(menu);
			if (menu == 1) {
				refreshLevel();
				refreshPauseLevel();
			}
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

	if (Keyboard::isKeyPressed(Keyboard::Enter) && !isEnter) { //hacer con switch
		
		switch (selectedMenuOption) {
		default:
			menu = 1;
			break;
		case 1:
			menu = 2;
			break;
		}
	}
	if (!(Keyboard::isKeyPressed(Keyboard::Enter))) {
		isEnter = false;
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

	if (!paused) {
		//up
		if ((Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W)) && !isUp) {
			cout << "Up\n";
			gameGrid.movePlayer('w');
			isUp = true;
			gameGrid.printList(); //delete
		}
		if (!(Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W))) {
			isUp = false;
		}

		//down
		if ((Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S)) && !isDown) {
			cout << "Down\n";
			gameGrid.movePlayer('s');
			isDown = true;
			gameGrid.printList(); //delete
		}
		if (!(Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S))) {
			isDown = false;
		}

		//left
		if ((Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A)) && !isLeft) {
			cout << "Left\n";
			gameGrid.movePlayer('a');
			isLeft = true;
			gameGrid.printList(); //delete
		}
		if (!(Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))) {
			isLeft = false;
		}

		//right
		if ((Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D)) && !isRight) {
			cout << "Right\n";
			gameGrid.movePlayer('d');
			isRight = true;
			gameGrid.printList(); //delete
		}
		if (!(Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))) {
			isRight = false;
		}
	}

	//escape
	if ((Keyboard::isKeyPressed(Keyboard::Escape) || Keyboard::isKeyPressed(Keyboard::Q)) && !isPausedKey) {
		cout << "Pause\n";
		isPausedKey = true;
		if (paused) {
			paused = false;
			selectedPauseOption = 0;
		}
		else {
			paused = true;
		}
	}
	if (!(Keyboard::isKeyPressed(Keyboard::Escape) || Keyboard::isKeyPressed(Keyboard::Q))) {
		isPausedKey = false;
	}
}

void refreshPauseLevel() {

	buttonSQ.setSize(Vector2f(264, 40));
	buttonSQ.setFillColor(Color::Transparent);
	buttonSQ.setOutlineThickness(5);
	buttonSQ.setOutlineColor(Color::White);


	if (paused) {
		gameWindow.draw(pauseImage);

		switch (selectedPauseOption) {
		default:
			buttonSQ.setPosition(364, 80);
			break;
		case 1:
			buttonSQ.setPosition(364, 140);
			break;
		case 2:
			buttonSQ.setPosition(364, 200);
			break;
		case 3:
			buttonSQ.setPosition(364, 260);
			break;
		}
		gameWindow.draw(buttonSQ);
		gameWindow.draw(resume);
		gameWindow.draw(reloadLevel);
		gameWindow.draw(saveAndExit);
		gameWindow.draw(exitLevel);
	}
}

void refreshKeyboardPauseLevel(int& menu) {

	if (paused) {
		//up
		if ((Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W)) && !isUp) {
			cout << "Up\n";
			if ((selectedPauseOption - 1) >= 0) {
				selectedPauseOption--;
			}
			isUp = true;
		}
		if (!(Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W))) {
			isUp = false;
		}

		//down
		if ((Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S)) && !isDown) {
			cout << "Down\n";
			if ((selectedPauseOption + 1) <= 3) {
				selectedPauseOption++;
			}
			isDown = true;
		}
		if (!(Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S))) {
			isDown = false;
		}

		//enter
		if (Keyboard::isKeyPressed(Keyboard::Enter) && !isEnter) {
			cout << "Enter\n";
			isEnter = true;
			switch (selectedPauseOption){
			default:
				paused = false;
				break;
			case 1:
				paused = false;
				levelLoaded = false;
				break;
			case 2:
				break;
			case 3:
				selectedPauseOption = 0;
				levelLoaded = false;
				menu = 0;
				break;
			}
		}
		if (!(Keyboard::isKeyPressed(Keyboard::Enter))) {
			isEnter = false;
		}
	}
}

void refreshMenu() {
	gameWindow.draw(fontImage);

	buttonSQ.setSize(Vector2f(360, 60));
	buttonSQ.setFillColor(Color::White);
	buttonSQ.setOutlineThickness(0);
	buttonSQ.setOutlineColor(Color::Transparent);

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