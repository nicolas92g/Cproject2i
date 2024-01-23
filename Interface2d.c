#include "Interface2d.h"


int newSave(int score) {
	FILE* file;
	char buffer[100];

	sprintf_s(buffer, 100, "SaveScore=%d", score);

	file = fopen( "save/save.txt", "w+");
	if (file == NULL) {
		printf("error1");
		return 0;
	}
	fprintf_s(file, "%s", buffer);

	fclose(file);
	printf("***update***");

}

int recoverSave() {
	FILE* file;
	int score = 0;

	file = fopen( "save/save.txt", "r");
	if (file == NULL) {
		printf("error2");
		return 0;
	}
	
	fscanf_s(file, "SaveScore=%d", &score);

	fclose(file);
	return score;
}





//Interface 2D

void interface2dCreate(Interface2d* self, Window* window, Camera* camera, Game* game)
{
	self->camera = camera;
	self->window = window;
	self->game = game;

	self->currentMenu = MAIN_MENU;
	
	Renderer2dCreate(&self->r2d, self->window);


	DatabaseReaderCreate(&self->dbReader);

	self->bestScore = DatabaseReaderGetBestScore(&self->dbReader);
	self->avrScore = DatabaseReaderGetMeanScore(&self->dbReader);
}



void interface2dObjectAdd(Interface2d* self)
{
	self->windowHeigt = WindowGetHeight(self->window);
	self->windowWidth = WindowGetWidth(self->window);

	Object2dDataCreate(&self->mainMenu, make_vec2((self->windowWidth)/2, (self->windowHeigt)/2), make_vec2(self->windowWidth, self->windowHeigt));
	
}



void interface2dFrame(Interface2d* self)
{


	switch (self->currentMenu)
	{
		case MAIN_MENU:
			interface2dMainMenu(self);
			break;

		case HISTORIC_MENU:
			interface2dHistoricMenu(self);
			break;

		case GAME_MENU:
			interface2dGameMenu(self);
			break;

		case PAUSE_MENU:
			interface2dPauseMenu(self);
			break;

		case END_MENU:
			interface2dEndMenu(self);
			break;

		case CONFIG_MENU:
			interface2dConfigMenu(self);
			break;

		default:
			break;
	}
}




void interface2dMainMenu(Interface2d* self)
{

	Renderer2dUpdate(&self->r2d);

	Renderer2dColor(&self->r2d, make_vec4(.5f, .5f, .5f, .8f), &self->mainMenu);

	
	sprintf_s(self->buffer, 200, "Best Score : %u", self->bestScore);
	Renderer2dText(& self->r2d, self->buffer, self->windowWidth /6,self->windowHeigt / 4, self->windowHeigt * 0.05);

	sprintf_s(self->buffer, 200, "Average Score : %.2lf", self->avrScore);
	Renderer2dText(&self->r2d, self->buffer, self->windowWidth / 1.75, self->windowHeigt / 4, self->windowHeigt * 0.05);

	
	Object2dDataCreate(&self->button, make_vec2((self->windowWidth) / 2, (self->windowHeigt) /2), make_vec2((self->windowWidth) * 0.45, (self->windowHeigt) * 0.15));

	char word[10];
	if (GameIsSavedGameExists(self->game)) {
		strcpy_s(word, 10, "Restart");
	}
	else {
		strcpy_s(word, 10, "Start");
	}

	if (Renderer2dColorButton(&self->r2d, word, ButtonColor(make_vec4f(.9)), &self->button))
	{
		GameStart(self->game);

		printf("***Game Start***\n");
		self->currentMenu = GAME_MENU;
	}
		
	
	Object2dDataCreate(& self->button, make_vec2(((self->windowWidth) / 2), ((self->windowHeigt) / 10)), make_vec2((self->windowWidth) * 0.3, (self->windowHeigt) * 0.1));
	if (Renderer2dColorButton(&self->r2d, "Historical", ButtonColor(make_vec4f(.9)), &self->button))
	{
		printf("***Hystorical***\n");
		self->currentMenu = HISTORIC_MENU;
	}

	Object2dDataCreate(&self->button, make_vec2((self->windowWidth) * 0.90, ((self->windowHeigt) / 10)), make_vec2((self->windowWidth) * 0.15, (self->windowHeigt) * 0.1));
	if (Renderer2dColorButton(&self->r2d, "Leave", ButtonColor(make_vec4f(.9)), &self->button))
	{
		printf("***Game Close***\n");
		glfwSetWindowShouldClose(self->window->ptr, GLFW_TRUE);
	}
}




void interface2dHistoricMenu(Interface2d* self)
{

	if (WindowGetKey(self->window, GLFW_KEY_ESCAPE))
	{
		self->currentMenu = MAIN_MENU;
	}

	Renderer2dUpdate(&self->r2d);

	Renderer2dColor(&self->r2d, make_vec4(.5f, .5f, .5f, .8f), &self->mainMenu);

	//LIRE LES PARTIES
	unsigned gamesCount;
	const GameModel* games = DatabaseReaderGetGames(&self->dbReader, &gamesCount);
	if (games) {

		Renderer2dSetCenterPoint(&self->r2d, TOP_LEFT);
		for (size_t i = 0; i < min(gamesCount, 5); i++)
		{
			size_t j = gamesCount - i - 1;
			double fontSize = self->windowHeigt * 0.05;
			sprintf_s(self->buffer, 200, "GAME %u | SCORE %u | DATE %s", games[j].id, games[j].score, games[j].datetime);
			Renderer2dText(&self->r2d, self->buffer, 25, fontSize * 2 * (i + 1), fontSize);

		}
		Renderer2dSetCenterPoint(&self->r2d, BOTTOM_LEFT);
		free(games);
	}


	Object2dDataCreate(&self->button, make_vec2((self->windowWidth) * 0.10, ((self->windowHeigt) / 10)), make_vec2((self->windowWidth) * 0.15, (self->windowHeigt) * 0.1));
	if (Renderer2dColorButton(&self->r2d, "Retour", ButtonColor(make_vec4f(.9)), &self->button))
	{
		printf("***Retour***\n");
		self->currentMenu = MAIN_MENU;
	}
}




void interface2dGameMenu(Interface2d* self)
{
	if (WindowGetKey(self->window, GLFW_KEY_ESCAPE))
	{
		self->currentMenu = PAUSE_MENU;
	}

	if (!self->game->isStarted)
	{
		self->currentMenu = END_MENU;
	}
	

	//ATH
	sprintf_s(self->buffer, 200, "Score : %d", self->game->score);
	Object2dDataCreate(&self->button, make_vec2((self->windowWidth) * 0.15, ((self->windowHeigt) * 0.05)), make_vec2((self->windowWidth) * 0.35, (self->windowHeigt) * 0.1));
	Renderer2dColorButton(&self->r2d, self->buffer, ButtonColor(make_vec4(1, 1, 1, .3)), &self->button);

}

void interface2dPauseMenu(Interface2d* self)
{	
	Renderer2dUpdate(&self->r2d);
	Renderer2dColor(&self->r2d, make_vec4(.5f, .5f, .5f, .8f), &self->mainMenu);

	sprintf_s(self->buffer, 200, "Score : %d", self->game->score);
	Object2dDataCreate(&self->button, make_vec2((self->windowWidth) / 2, ((self->windowHeigt) * 0.70)), make_vec2((self->windowWidth) * 0.35, (self->windowHeigt) * 0.1));
	Renderer2dColorButton(&self->r2d, self->buffer, ButtonColor(make_vec4f(1)), &self->button);

	Object2dDataCreate(&self->button, make_vec2((self->windowWidth) / 2, (self->windowHeigt) / 2), make_vec2((self->windowWidth) * 0.35, (self->windowHeigt) * 0.15));
	if (Renderer2dColorButton(&self->r2d, "Continue", ButtonColor(make_vec4f(.9)), &self->button))
	{
		printf("***Game Start***\n");
		self->currentMenu = GAME_MENU;
	}

	Object2dDataCreate(&self->button, make_vec2(((self->windowWidth) / 2), ((self->windowHeigt) / 5)), make_vec2((self->windowWidth) * 0.3, (self->windowHeigt) * 0.1));
	if (Renderer2dColorButton(&self->r2d, "Main Menu", ButtonColor(make_vec4f(.9)), &self->button))
	{
		GameSave(self->game);
		printf("***Save***\n");

		GameStop(self->game);

		printf("***Main Menu***\n");
		self->currentMenu = MAIN_MENU;
	}
}

void interface2dEndMenu(Interface2d* self)
{
	Renderer2dUpdate(&self->r2d);
	Renderer2dColor(&self->r2d, make_vec4(.5f, .5f, .5f, .8f), &self->mainMenu);

	if (self->game->score > self->bestScore) {
		
		Object2dDataCreate(&self->button, make_vec2((self->windowWidth) / 2, ((self->windowHeigt) * 0.70)), make_vec2((self->windowWidth) * 0.35, (self->windowHeigt) * 0.1));
		Renderer2dColorButton(&self->r2d, "NEW BEST SCORE !", ButtonColor(make_vec4f(1)), &self->button);
	}

	sprintf_s(self->buffer, 200, "Final score : %d", self->game->score);
	Object2dDataCreate(&self->button, make_vec2((self->windowWidth) / 2, ((self->windowHeigt) / 2)), make_vec2((self->windowWidth) * 0.35, (self->windowHeigt) * 0.1));
	Renderer2dColorButton(&self->r2d, self->buffer, ButtonColor(make_vec4f(1)), &self->button);

	Object2dDataCreate(&self->button, make_vec2(((self->windowWidth) / 2), ((self->windowHeigt) / 5)), make_vec2((self->windowWidth) * 0.3, (self->windowHeigt) * 0.1));
	if (Renderer2dColorButton(&self->r2d, "Main Menu", ButtonColor(make_vec4f(.9)), &self->button))
	{

		DatabaseReaderAddGame(&self->dbReader, self->game->score);
		self->bestScore = DatabaseReaderGetBestScore(&self->dbReader);
		self->avrScore = DatabaseReaderGetMeanScore(&self->dbReader);

		GameStop(self->game);
		
		printf("***Main Menu***\n");
		self->currentMenu = MAIN_MENU;
	}

}



void interface2dConfigMenu(Interface2d* self)
{
	
}





