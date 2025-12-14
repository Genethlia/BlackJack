#include "game.h"
using namespace std;


int main() {

	InitWindow(1000, 900, "BlackJack");
	SetTargetFPS(60);

	Game game = Game();


	while (!WindowShouldClose()) {
		BeginDrawing();

		if (!game.Drown) {
			game.DrawStartingCards(0.2);
		}

		ClearBackground(tableGreen);
		game.Draw();

		EndDrawing();
	}

	CloseWindow();

	return 0;
}