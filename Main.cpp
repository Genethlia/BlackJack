#include "game.h"
using namespace std;


int main() {

	InitWindow(screenWidth, screenHeight, "BlackJack");
	SetTargetFPS(60);

	Game game = Game();


	while (!WindowShouldClose()&&!ShouldWindowClose) {
		BeginDrawing();

		game.Update();

		ClearBackground(BLACK);
		game.Draw();

		EndDrawing();
	}

	CloseWindow();

	return 0;
}