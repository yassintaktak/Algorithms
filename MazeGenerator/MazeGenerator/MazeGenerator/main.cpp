#include <iostream>
#include <vector>
#include <SDL.h>
#include <cstdlib>
#include <ctime>
#include <stack> 

using namespace std;

float width = 400, height = 400;
float cw = 10, ch = 10;
float cols = width / cw, rows = height / ch;

struct Cell {
	int row = -1;
	int col = -1;

	bool walls[4] = { true, true, true, true };
	bool visited = false;
};

vector<Cell> cells;
Cell* current;
stack<Cell*> cellsStack;

int index(int i, int j) {
	if (i < 0 || j < 0 || i > cols - 1 || j > rows - 1) {
		return -1;
	}
	return i + j * cols;
}
int randNum(int min, int max) {
	return rand() % max + min;
}

void initMaze() {
	for (int i = 0; i < cols; i++) {
		for (int j = 0; j < rows; j++) {
			Cell temp = { i, j };
			cells.push_back(temp);
		}
	}
	current = &cells[0];
}

vector<Cell*> neighbours() {
	Cell *temp_cell = current;
	vector<Cell*> return_cells;
	int top_index = index(temp_cell->col, temp_cell->row - 1);
	int right_index = index(temp_cell->col + 1, temp_cell->row);
	int bottom_index = index(temp_cell->col, temp_cell->row + 1);
	int left_index = index(temp_cell->col - 1, temp_cell->row);
	Cell* top;
	Cell* right;
	Cell* left;
	Cell* bottom;

	if (top_index >= 0 && top_index < cells.size()) {
		top = &cells[top_index];
		if(!top->visited)
			return_cells.push_back(top);
	}

	if (right_index >= 0 && right_index < cells.size()) {
		right = &cells[right_index];
		if(!right->visited)
			return_cells.push_back(right);
	}

	if (bottom_index >= 0 && bottom_index < cells.size()) {
		bottom = &cells[bottom_index];
		if(!bottom->visited)
			return_cells.push_back(bottom);
	}

	if (left_index >= 0 && left_index < cells.size()) {
		left = &cells[left_index];
		if(!left->visited)
			return_cells.push_back(left);
	}

	return return_cells;
}

void removeWalls(Cell* current, Cell* random) {
	if (current->col - random->col < 0) {
		current->walls[3] = false;
		random->walls[1] = false;
	} else if (current->col - random->col > 0) {
		current->walls[1] = false;
		random->walls[3] = false;
	}

	if (current->row - random->row < 0) {
		current->walls[2] = false;
		random->walls[0] = false;
	} else {
		current->walls[0] = false;
		random->walls[2] = false;
	}
}

void recursiveNeighbours(SDL_Renderer *renderer) {
	if (current->col >= 0) {
		vector<Cell*> cell_neighbours = neighbours();

		if (cell_neighbours.size() > 0) {
			cellsStack.push(current);
			Cell* random = cell_neighbours[randNum(0, cell_neighbours.size())];
			removeWalls(current, random);
			current = random;
		} else {
			if (!cellsStack.empty()) {
				current = cellsStack.top();
				cellsStack.pop();
			} else {
				cout << "No more neighbours !!" << endl;
				Cell nocurrent = { -1, -1 };
				current = &nocurrent;
			}
		}
	}
}

void drawMaze(SDL_Renderer* renderer) {
	for (int i = 0; i < cells.size(); i++) {
		Cell temp_cell = cells[i];
		float x = temp_cell.col * cw ;
		float y = temp_cell.row * ch ;

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

		if (temp_cell.visited) {
			SDL_SetRenderDrawColor(renderer, 108, 92, 231, 255);
			SDL_Rect Rect = { x, y, cw, ch };
			SDL_RenderFillRect(renderer, &Rect);
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		if (temp_cell.walls[0])
			SDL_RenderDrawLine(renderer, x, y, x + cw, y); // top line
		if (temp_cell.walls[2])
			SDL_RenderDrawLine(renderer, x + cw, y + ch, x, y + ch); // bottom line
		if (temp_cell.walls[3])
			SDL_RenderDrawLine(renderer, x + cw, y, x + cw, y + ch); //right line
		if (temp_cell.walls[1])
			SDL_RenderDrawLine(renderer, x, y + ch, x, y); // left line	
	}
	
	SDL_SetRenderDrawColor(renderer, 214, 48, 49, 255);
	SDL_Rect Rect = { current->col * cw , current->row * ch , cw, ch };
	SDL_RenderFillRect(renderer, &Rect);
	SDL_RenderPresent(renderer);
	current->visited = true;
	recursiveNeighbours(renderer);
}

int main(int argc, char* argv[]) {
	srand(time(0));
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window *window = SDL_CreateWindow("Maze Generator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
	SDL_Event event;

	const int FPS = 10;
	const int frameDelay = 1000 / FPS;
	Uint32 frameStart;
	int frameTime;

	initMaze();

	

	while (true) {
		if (SDL_PollEvent(&event)) {
			if (SDL_QUIT == event.type) {
				break;
			}
		}
		frameStart = SDL_GetTicks();

		// Do all the work here!
		drawMaze(renderer);

		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	
	return 0;
}