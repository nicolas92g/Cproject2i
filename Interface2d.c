#include "Interface2d.h"


int addBestScore(int nScore) {
	FILE* file;
	char buffer[100];

	sprintf_s(buffer, 100, "Best Score : %d", nScore);

	file = fopen( "save/bestScore.txt", "w+");
	if (file == NULL) {
		printf("error1");
		return 0;
	}
	fprintf_s(file, "%s", buffer);

	fclose(file);
	printf("***update***");

}

int recoverBestScore() {
	FILE* file;
	int score = 0;

	file = fopen( "save/bestScore.txt", "r");
	if (file == NULL) {
		printf("error2");
		return 0;
	}
	
	fscanf_s(file, "Best Score : %d", &score);

	fclose(file);
	return score;
}





//Interface 2D

void interface2dCreate(Interface2d* self, Window* window, Camera* camera, Game* game)
{
	self->camera = camera;
	self->window = window;
	self->game = game;

	self->currentMenu = 0;
	self->bestScore = recoverBestScore();
	
	Renderer2dCreate(&self->r2d, self->window);
	
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

	
	sprintf_s(self->buffer, 200, "Best Score : %d", self->bestScore);
	Renderer2dText(& self->r2d, self->buffer, self->windowWidth /6,self->windowHeigt / 4, self->windowHeigt * 0.05);

	Renderer2dText(&self->r2d, "Average Score :", self->windowWidth / 1.75, self->windowHeigt / 4, self->windowHeigt * 0.05);

	
	Object2dDataCreate(&self->button, make_vec2((self->windowWidth) / 2, (self->windowHeigt) / 2), make_vec2((self->windowWidth) * 0.35, (self->windowHeigt) * 0.15));
	if (Renderer2dColorButton(&self->r2d, "Start Game", ButtonColor(make_vec4f(1)), &self->button))
	{
		printf("***Game Start***");
		self->currentMenu = 2;
	}

	Object2dDataCreate(& self->button, make_vec2(((self->windowWidth) / 2), ((self->windowHeigt) / 10)), make_vec2((self->windowWidth) * 0.3, (self->windowHeigt) * 0.1));
	if (Renderer2dColorButton(&self->r2d, "Historical", ButtonColor(make_vec4f(1)), &self->button))
	{
		printf("***Hystorical***");
		self->currentMenu = 1;
	}

	Object2dDataCreate(&self->button, make_vec2((self->windowWidth) * 0.90, ((self->windowHeigt) / 10)), make_vec2((self->windowWidth) * 0.15, (self->windowHeigt) * 0.1));
	if (Renderer2dColorButton(&self->r2d, "Leave", ButtonColor(make_vec4f(1)), &self->button))
	{
		printf("***Game Close***");
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

	Object2dDataCreate(&self->button, make_vec2((self->windowWidth) * 0.10, ((self->windowHeigt) / 10)), make_vec2((self->windowWidth) * 0.15, (self->windowHeigt) * 0.1));
	if (Renderer2dColorButton(&self->r2d, "Retour", ButtonColor(make_vec4f(1)), &self->button))
	{
		printf("***Retour***");
		self->currentMenu = 0;
	}

	Object2dDataCreate(&self->button, make_vec2((self->windowWidth) / 2, ((self->windowHeigt) / 10)), make_vec2((self->windowWidth) * 0.15, (self->windowHeigt) * 0.1));
	if (Renderer2dColorButton(&self->r2d, "Clear", ButtonColor(make_vec4f(1)), &self->button))
	{
		printf("***Clear***");
		Rendere2dClearHistory(self);
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
	Renderer2dColorButton(&self->r2d, self->buffer, ButtonColor(make_vec4f(1)), &self->button);

}

void interface2dPauseMenu(Interface2d* self)
{	
	Renderer2dUpdate(&self->r2d);
	Renderer2dColor(&self->r2d, make_vec4(.5f, .5f, .5f, .8f), &self->mainMenu);

	sprintf_s(self->buffer, 200, "Score : %d", self->game->score);
	Object2dDataCreate(&self->button, make_vec2((self->windowWidth) / 2, ((self->windowHeigt) * 0.70)), make_vec2((self->windowWidth) * 0.35, (self->windowHeigt) * 0.1));
	Renderer2dColorButton(&self->r2d, self->buffer, ButtonColor(make_vec4f(1)), &self->button);

	Object2dDataCreate(&self->button, make_vec2((self->windowWidth) / 2, (self->windowHeigt) / 2), make_vec2((self->windowWidth) * 0.35, (self->windowHeigt) * 0.15));
	if (Renderer2dColorButton(&self->r2d, "Continue", ButtonColor(make_vec4f(1)), &self->button))
	{
		printf("***Game Start***");
		self->currentMenu = 2;
	}

	Object2dDataCreate(&self->button, make_vec2(((self->windowWidth) / 2), ((self->windowHeigt) / 5)), make_vec2((self->windowWidth) * 0.3, (self->windowHeigt) * 0.1));
	if (Renderer2dColorButton(&self->r2d, "Main Menu", ButtonColor(make_vec4f(1)), &self->button))
	{
		if (self->game->score > self->bestScore) {
			addBestScore(self->game->score);
			self->bestScore = self->game->score;
		}
		self->game->score = 0;

		printf("***Main Menu***");
		self->currentMenu = 0;
	}
}

void interface2dEndMenu(Interface2d* self)
{


}



void interface2dConfigMenu(Interface2d* self)
{

}


void Rendere2dClearHistory(Interface2d* self)
{

}




