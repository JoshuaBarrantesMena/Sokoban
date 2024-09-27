#include "Game.h"
using namespace std;

RenderWindow gameWindow(VideoMode(640, 640), "Sokoban");
Sprite Box, BoxLace, Brick, Floor, Player;
Event gameEvent;
RectangleShape fontImage, titleSQ, buttonSQ, pauseImage;
Font gameFont, buttonFont, pauseFont, winFont;
Text gameName, start, chargeGame, record, resume, reloadLevel, exitLevel, saveAndExit, win1, win2;
Text savedRecords, recName, recLevel;

GridNode gameGrid(10);
vector<Texture> Replay;

int selectedMenuOption, selectedPauseOption, selectedReplay;
int currentLevel;
bool levelLoaded, paused, levelReset, playerMoved, ingame, recordLoaded;
bool isUp, isDown, isLeft, isRight, isPausedKey, isEnter, isDeleteKey;

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
	gameName.setPosition(86, 128);

	start.setFont(buttonFont);
	start.setCharacterSize(45);
	start.setFillColor(Color(222, 214, 174));
	start.setString("Empezar");
	start.setPosition(229, 270);

	chargeGame.setFont(buttonFont);
	chargeGame.setCharacterSize(45);
	chargeGame.setFillColor(Color::White);
	chargeGame.setString("Cargar Partida");
	chargeGame.setPosition(157, 350);

	record.setFont(buttonFont);
	record.setCharacterSize(45);
	record.setFillColor(Color::White);
	record.setString("Historial");
	record.setPosition(221, 420);
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

	// load win
	winFont.loadFromFile("fonts/winFont.ttf");

	win1.setFont(winFont);
	win2.setFont(winFont);
	win1.setCharacterSize(50);
	win2.setCharacterSize(50);
	win1.setFillColor(Color::White);
	win1.setOutlineThickness(3);
	win1.setOutlineColor(Color::Black);
	win2.setFillColor(Color::White);
	win2.setOutlineThickness(3);
	win2.setOutlineColor(Color::Black);
	win1.setString("Nivel 1");
	win2.setString("Completado");
	win1.setPosition(237, 220);
	win2.setPosition(163, 275);
	//

	//records
	savedRecords.setFont(pauseFont);
	savedRecords.setCharacterSize(45);
	savedRecords.setOutlineThickness(2);
	savedRecords.setFillColor(Color::White);
	savedRecords.setString("Repeticiones guardadas");
	savedRecords.setPosition(65, 40);

	recName.setFont(pauseFont);
	recName.setCharacterSize(30);
	recName.setFillColor(Color::White);

	recLevel.setFont(pauseFont);
	recLevel.setCharacterSize(20);
	recLevel.setFillColor(Color(222, 214, 174));
	//

	currentLevel = 1;
	levelLoaded = false;
	paused = false;
	levelReset = false;
	playerMoved = true;

	isUp = false, isDown = false, isLeft = false, isRight = false, isPausedKey = false, isEnter = false, isDeleteKey = false;

	int menu = 0, time = 0, currentReplay = 0, currentFrame = 0, currentVecFrame = 1;
	selectedMenuOption = 0, selectedPauseOption = 0, selectedReplay = 0;
	
	while (gameWindow.isOpen()) {
		ingame = false;
		//principal menu
		if (menu == 0) {

			refreshKeyboardMenu(menu);
			refreshMenu();
		}

		//win screen
		if (currentLevel >= 6) {
			gameGrid.loadLevel(currentLevel);
			gameWinned();
			menu = 0;
			currentLevel = 1;
			paused = false;
			selectedMenuOption = 0;
		}
		//startgame menu
		if (menu == 1) {
			ingame = true;
			if (!levelLoaded) { //load level
				gameGrid.loadLevel(currentLevel);
				levelLoaded = true;
				if (!levelReset) {
					currentReplay = localReplay();
					levelReset = true;
				}
			}

			refreshLevel(currentReplay, currentFrame); //refresh window;
			refreshPauseLevel();
			this_thread::sleep_for(chrono::milliseconds(25));

			if (!gameGrid.checkBoxPlaces()) { //if game is no winned
				refreshKeyboardPauseLevel(menu, currentReplay, currentFrame);
				refreshKeyboardLevel();
			}
			else { // if game is winned
				refreshWinLevel(time);
				refreshKeyboardWinLevel(currentFrame);
			}
		}

		if (menu == 2) {
			gameGrid.loadSavedLevel(currentLevel, currentReplay, currentFrame);
			levelLoaded = true;
			menu = 1;
		}

		//saved records menu
 		if (menu == 3) {

			refreshLevelRecordMenu();
			refreshLevelRecordKeyboardMenu(menu);

			if (recordLoaded) {
				refreshKeyboardRecord(currentVecFrame);
				refreshRecord(currentVecFrame);
			}
		}

		gameWindow.display();
		loopRefresh();
	}
	if (ingame) { //if window closed while is playing a level
		deleteReplays(1, currentReplay, true);
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
		if (selectedMenuOption > 2) {
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
		case 2:
			menu = 3;
		}
		isEnter = true;
	}
	if (!(Keyboard::isKeyPressed(Keyboard::Enter))) {
		isEnter = false;
	}
}

void refreshLevel(int currentReplay, int& currentFrame){

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
				Box.setColor(Color(255, 255, 255, 255));
				Box.setPosition(j * 64, i * 64);
				gameWindow.draw(Box);
				break;
			case '!':
				Floor.setPosition(j * 64, i * 64);
				gameWindow.draw(Floor);
				Box.setPosition(j * 64, i * 64);
				Box.setColor(Color(155, 155, 155, 255));
				gameWindow.draw(Box);
				Box.setColor(Color(255, 255, 255, 255));
				break;
			}
		}
	}
	if (playerMoved) {
		makeReplay(currentReplay, currentFrame);
		currentFrame++;
		playerMoved = false;
	}
}

void refreshKeyboardLevel(){

	if (!paused) {
		//up
		if ((Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W)) && !isUp) {
			gameGrid.movePlayer('w');
			isUp = true;
			playerMoved = true;
		}
		if (!(Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W))) {
			isUp = false;
		}

		//down
		if ((Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S)) && !isDown) {
			gameGrid.movePlayer('s');
			isDown = true;
			playerMoved = true;
		}
		if (!(Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S))) {
			isDown = false;
		}

		//left
		if ((Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A)) && !isLeft) {
			gameGrid.movePlayer('a');
			isLeft = true;
			playerMoved = true;
		}
		if (!(Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))) {
			isLeft = false;
		}

		//right
		if ((Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D)) && !isRight) {
			gameGrid.movePlayer('d');
			isRight = true;
			playerMoved = true;
		}
		if (!(Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))) {
			isRight = false;
		}
	}

	//escape
	if ((Keyboard::isKeyPressed(Keyboard::Escape) || Keyboard::isKeyPressed(Keyboard::Q)) && !isPausedKey) {
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

void refreshKeyboardPauseLevel(int& menu, int& currentReplay, int& currentFrame) {

	if (paused) {
		//up
		if ((Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W)) && !isUp) {
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
			isEnter = true;
			switch (selectedPauseOption){
			default:
				break;
			case 1:
				deleteReplays(currentLevel, currentReplay, false);
				currentFrame = 1;
				levelReset = true;
				levelLoaded = false;
				break;
			case 2:
				menu = 0;
				gameGrid.saveLevel(currentLevel, currentReplay, currentFrame);
				levelLoaded = false;
				break;
			case 3:
				deleteReplays(1, currentReplay, true);
				currentFrame = 0;
				playerMoved = true;
				levelLoaded = false;
				menu = 0;
				break;
			}
			paused = false;
			selectedPauseOption = 0;
			selectedMenuOption = 0;
		}
		if (!(Keyboard::isKeyPressed(Keyboard::Enter))) {
			isEnter = false;
		}
	}
}

void refreshWinLevel(int& time){

	boxPlace* box;
	int pos = 0;

	if (time < 12) {
		Box.setColor(Color(255, 255, 55, 255));

		win1.setOutlineColor(Color::Black);
		win1.setFillColor(Color::White);
		win2.setOutlineColor(Color::Black);
		win2.setFillColor(Color::White);
	}
	else{
		Box.setColor(Color(155, 155, 155, 255));

		win1.setOutlineColor(Color::White);
		win1.setFillColor(Color::Black);
		win2.setOutlineColor(Color::White);
		win2.setFillColor(Color::Black);
	}
	if (time == 25) {
		time = 0;
	}

	while ((box = gameGrid.getBoxPlace(pos)) != nullptr) {
		Box.setPosition(box->x * 64, box->y * 64);
		gameWindow.draw(Box);
		pos++;
	}
	time++;
	String level = "Nivel ";
	level += to_string(currentLevel);
	win1.setString(level);
	gameWindow.draw(win1);
	gameWindow.draw(win2);
}

void refreshKeyboardWinLevel(int& currentFrame){

	if (Keyboard::isKeyPressed(Keyboard::Enter) && !isEnter) {
		currentLevel++;
		currentFrame = 0;
		playerMoved = true;
		levelLoaded = false;
		isEnter = true;
	}
	if (!(Keyboard::isKeyPressed(Keyboard::Enter))) {
		isEnter = false;
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
	switch (selectedMenuOption) {
	default:
		start.setFillColor(Color(222, 214, 174));
		chargeGame.setFillColor(Color::White);
		record.setFillColor(Color::White);
		buttonSQ.setPosition(gameWindow.getSize().x / 2 - buttonSQ.getSize().x / 2, 269);
		break;
	case 1:
		start.setFillColor(Color::White);
		chargeGame.setFillColor(Color(222, 214, 174));
		record.setFillColor(Color::White);
		buttonSQ.setPosition(gameWindow.getSize().x / 2 - buttonSQ.getSize().x / 2, 349);
		break;
	case 2:
		start.setFillColor(Color::White);
		chargeGame.setFillColor(Color::White);
		record.setFillColor(Color(222, 214, 174));
		buttonSQ.setPosition(gameWindow.getSize().x / 2 - buttonSQ.getSize().x / 2, 419);
		break;
	}

	gameWindow.draw(buttonSQ);
	gameWindow.draw(start);
	gameWindow.draw(chargeGame);
	gameWindow.draw(record);
}

void refreshLevelRecordMenu() {
	gameWindow.draw(fontImage);
	gameWindow.draw(savedRecords);

	buttonSQ.setOutlineThickness(0);
	buttonSQ.setFillColor(Color::White);
	buttonSQ.setSize(Vector2f(360, 60));

	ifstream file;
	int y = 150;

	for (int i = 1; i <= 5; i++) {
		recLevel.setString("");
		file.open("replays/replay" + to_string(i) + "/l1f0.jpg");
		if (file.is_open()) {
			recName.setString("repeticion guardada " + to_string(i));
			for (int j = 1; j <= 5; j++) {
				file.close();
				file.open("replays/replay" + to_string(i) + "/l" + to_string(j) + "f0.jpg");
				if (file.is_open()) {
					recLevel.setString("nivel: " + to_string(j));
				}
			}
		}
		else {
			recName.setString("vacio");
		}
		recName.setPosition(40, y);
		recLevel.setPosition(50, y + 30);
		y += 90;

		if (selectedReplay == i - 1) {
			buttonSQ.setPosition(30, 150 + (i - 1)*90);
			recName.setFillColor(Color(222, 214, 174));
			gameWindow.draw(buttonSQ);
		}
		else {
			recName.setFillColor(Color::White);
		}

		gameWindow.draw(recName);
		gameWindow.draw(recLevel);
		file.close();
	}
}

void refreshLevelRecordKeyboardMenu(int& menu) {
	
	if ((Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W)) && !isUp) {
		selectedReplay--;
		if (selectedReplay < 0) {
			selectedReplay++;
		}
		isUp = true;
	}
	if (!(Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W))) {
		isUp = false;
	}

	if ((Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S)) && !isDown) {
		selectedReplay++;
		if (selectedReplay > 4) {
			selectedReplay--;
		}
		isDown = true;
	}
	if (!(Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S))) {
		isDown = false;
	}
	
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		menu = 0;
		selectedReplay = 0;
	}

	if (Keyboard::isKeyPressed(Keyboard::Enter) && !isEnter) {
		Replay.clear();
		loadingRecordScreen();
		chargeReplay(selectedReplay + 1);
		recordLoaded = true;
		isEnter = true;
	}
	if (!Keyboard::isKeyPressed(Keyboard::Enter)) {
		isEnter = false;
	}

	if (Keyboard::isKeyPressed(Keyboard::E) && !isDeleteKey) {
		deleteReplays(1, selectedReplay + 1, true);
		isDeleteKey = true;
	}
	if (!(Keyboard::isKeyPressed(Keyboard::E))) {
		isDeleteKey = false;
	}
}

void refreshRecord(int& currentFrame){

	if (Replay.size() <= currentFrame) {
		recordLoaded = false;
		currentFrame = 1;
		return;
	}

	Sprite currentSprite;
	currentSprite.setTexture(Replay.at(currentFrame));
	gameWindow.draw(currentSprite);
}

void refreshKeyboardRecord(int& currentFrame){
	//left
	if ((Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A)) && !isLeft) {
		currentFrame--;
		if (currentFrame < 0) {
			currentFrame++;
		}
		isLeft = true;
	}
	if (!(Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))) {
		isLeft = false;
	}

	//right
	if ((Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Space)) && !isRight) {
		currentFrame++;
		isRight = true;
	}
	if (!(Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Space))) {
		isRight = false;
	}
}

int localReplay(){

	int current = 1;
	string directory = "replays/replay" + to_string(current) + "/l1f0.jpg";

	ifstream replay;
	replay.open(directory);
	
	while (replay.is_open()) {
		replay.close();
		current++;
		directory = "replays/replay" + to_string(current) + "/l1f0.jpg";
		replay.open(directory);
	}
	replay.close();
	return current;
}

void makeReplay(int currentReplay, int currentFrame){

	Texture texture;
	texture.create(gameWindow.getSize().x, gameWindow.getSize().y);
	texture.update(gameWindow);

	Image screenshot = texture.copyToImage();

	string fileName = "replays/replay" + to_string(currentReplay) + "/l" + to_string(currentLevel) + "f" + to_string(currentFrame) + ".jpg";
	screenshot.saveToFile(fileName);
}

void deleteReplays(int level, int currentReplay, bool deleteAll){

	int frame = 0;
	string directory = "replays/replay" + to_string(currentReplay) + "/l" + to_string(level) + "f" + to_string(frame) + ".jpg";
	ifstream file;
	file.open(directory);
	if (file.is_open() && !deleteAll) {
		file.close();
		frame++;
		directory = "replays/replay" + to_string(currentReplay) + "/l" + to_string(level) + "f" + to_string(frame) + ".jpg";
		file.open(directory);
	}

	while (file.is_open()) {
		file.close();
		remove(directory.c_str());
		frame++;
		directory = "replays/replay" + to_string(currentReplay) + "/l" + to_string(level) + "f" + to_string(frame) + ".jpg";
		file.open(directory);
		if (!file.is_open()) {
			level++;
			frame = 0;
			directory = "replays/replay" + to_string(currentReplay) + "/l" + to_string(level) + "f" + to_string(frame) + ".jpg";
			file.open(directory);
		}
	}
	file.close();
}

void chargeReplay(int replay){

	int currentFrame = 0;

	ifstream file;

	for (int i = 1; i <= 5; i++) {
		string directory = "replays/replay" + to_string(replay) + "/l" + to_string(i) + "f" + to_string(currentFrame) + ".jpg";
		file.open(directory);
		while (file.is_open()) {
			file.close();
			Texture frame;
			frame.loadFromFile(directory);
			Replay.push_back(frame);
			directory = "replays/replay" + to_string(replay) + "/l" + to_string(i) + "f" + to_string(currentFrame) + ".jpg";
			file.open(directory);
			currentFrame++;
		}
		currentFrame = 0;
	}
}

void loadingRecordScreen(){

	gameWindow.draw(fontImage);
	Text loading;
	loading.setFont(buttonFont);
	loading.setFillColor(Color::White);
	loading.setCharacterSize(60);
	loading.setString("Cargando...");
	loading.setPosition(178, 250);
	gameWindow.draw(loading);
	gameWindow.display();
}

void gameWinned(){

	bool finish = false;
	playerMoved = false;
	int n = 0;

	Text winGame1;
	winGame1.setFont(gameFont);
	winGame1.setCharacterSize(55);
	winGame1.setFillColor(Color::White);
	winGame1.setOutlineThickness(5);
	winGame1.setOutlineColor(Color(68, 64, 36));
	winGame1.setString("Juego");
	winGame1.setPosition(175, 200);
	Text winGame2;
	winGame2.setFont(gameFont);
	winGame2.setCharacterSize(55);
	winGame2.setFillColor(Color::White);
	winGame2.setOutlineThickness(5);
	winGame2.setOutlineColor(Color(68, 64, 36));
	winGame2.setString("Terminado");
	winGame2.setPosition(66, 300);

	while (!finish && gameWindow.isOpen()) {
		refreshLevel(0, n);

		if (Keyboard::isKeyPressed(Keyboard::Enter)) {
			finish = true;
			isEnter = true;
		}
		gameWindow.draw(winGame1);
		gameWindow.draw(winGame2);
		gameWindow.display();
		loopRefresh();
	}
}

void loopRefresh() {
	while (gameWindow.pollEvent(gameEvent)) {
		if (gameEvent.type == Event::Closed) {
			gameWindow.close();
		}
	}
}