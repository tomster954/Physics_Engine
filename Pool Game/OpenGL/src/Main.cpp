#include "Application.h"


int main(void)
{
	Application *app = new Application(glm::vec4(0.25f, 0.25f, 0.25f, 1.0f));
	app->RunGame();
	delete app;

	return 0;
}
