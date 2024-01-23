#include "MainApplication.h"

int main(){

	MainApplication app;
	MainApplicationCreate(&app);

	MainApplicationRun(&app);

	return EXIT_SUCCESS;
}