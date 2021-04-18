#include "raylib.h"

enum Status {
	normal,
	clicked,
};

struct Grid {
	int num;
	enum Status status;
	struct Color color;
};

void click(struct Grid *grid, int x, int y); 

int main() {
	const int row = 9;
	const int col = 9;
	const int cellSize = 30;
	const int winWidth = row * cellSize + row;
	const int winHeight = col * cellSize + col;
	const int fontSize = 15;
	const int mineNum = 2;
	int count = 0;
	int gameover = 0;

	InitWindow(winWidth, winHeight, "mineSweeper");

	struct Grid grid[row][col];

	for (int y = 0; y < row; y++) {
		for (int x = 0; x < col; x++) {
			grid[y][x].num = 0;
			grid[y][x].status = normal;
			grid[y][x].color = GREEN;
		}
	}

	for (int i = 0; i < mineNum; i++) {
		int x = GetRandomValue(0, 8);
		int y = GetRandomValue(0, 8);

		while (grid[y][x].num == -1) {
			x = GetRandomValue(0, 8);
			y = GetRandomValue(0, 8);
		}

		grid[y][x].num = -1;
	}

	for (int y = 0; y < row; y++) {
		for (int x = 0; x < col; x++) {
			if (grid[y][x].num != -1) {
				for (int dy = -1; dy <= 1; dy++) {
					for (int dx = -1; dx <= 1; dx++) {
						if (y + dy >= 0 && y + dy < 9 && x + dx >= 0 && x + dx < 9 && grid[y + dy][x + dx].num == -1) {
							grid[y][x].num++;
						}
					}
				}
			}
		}
	}

	while (!WindowShouldClose()) {
		count = 0;

		int mx = GetMouseX();
		int my = GetMouseY();
		int selectedX = mx / cellSize;
		int selectedY = my / cellSize;

		for (int y = 0; y < row; y++) {
			for (int x = 0; x < col; x++) {
				if (selectedX == x && selectedY == y) {
					grid[y][x].color = BLUE;
					if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
						click((struct Grid *)grid, x, y);
						if (grid[y][x].num == -1) {
							gameover = 1;
						}
					}
				} else {
					grid[y][x].color = GREEN;
				}

				if (grid[y][x].status == clicked) {
					grid[y][x].color = WHITE;
					if (grid[y][x].num == -1) {
						grid[y][x].color = RED;
					}
					count++;
				}

				if (count == row * col - mineNum) {
					printf("yes\n");
				}

			}
		}

		BeginDrawing();
			ClearBackground(BLACK);
			for (int y = 0; y < row; y++) {
				for (int x = 0; x < col; x++) {
					int gridX = x * (cellSize + 1);
					int gridY = y * (cellSize + 1);
					DrawRectangle(gridX, gridY, cellSize, cellSize, grid[y][x].color);
					if (grid[y][x].status == clicked) {
						const char *text = TextFormat("%d", grid[y][x].num);
						int textWidth = MeasureText(text, fontSize);
						int textHeight = fontSize;
						if (grid[y][x].num != 0) {
							DrawText(TextFormat("%d", grid[y][x].num), gridX + cellSize / 2 - textWidth / 2, gridY + cellSize / 2 - textHeight / 2, fontSize, BLACK);
						}
					}
				}
			}
		EndDrawing();
	}

	CloseWindow();

	return 0;
}

void click(struct Grid *grid, int x, int y) {
	struct Grid *current = (grid + y * 9 + x);
	current->status = clicked;

	for (int dy = -1; dy <= 1; dy++) {
		for (int dx = -1; dx <= 1; dx++) {
			if (current->num == 0) {
				if (x + dx >= 0 && x + dx < 9 && y + dy >= 0 && y + dy < 9) {
					struct Grid *around = (grid + (y + dy) * 9 + x + dx);
					if (around->status == normal) {
						click(grid, x + dx, y + dy);		
					}
				}
			}
		}
	}
}
