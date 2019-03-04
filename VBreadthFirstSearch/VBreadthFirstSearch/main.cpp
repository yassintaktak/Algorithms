#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <math.h>
#include <string>
#include <vector>
#include <unordered_map> 
#include <queue> 
#include <random>
#include <algorithm>

# define M_PI           3.14159265358979323846

using namespace std;


struct Color {
	float r;
	float g;
	float b;
	float a;
};

float width = 700;
float height = 600;
random_device rd;
mt19937 e2(rd());


template <typename T> struct Node {
	T value;
	vector<Node<T>*> edges;
	bool searched = false;
	Node<T>* parent;
	float x, y;
	float tx, ty;
};

template <typename T> class Graph {
public:
	Graph() {

	}
	void addNode(T value) {
		Node<T> newNode;
		newNode.value = value;
		m_nodes.push_back(newNode);
		m_graph[value] = newNode;
	}
	void setEdge(T value1, T value2) {
		m_graph[value1].edges.push_back(&m_graph[value2]);
		m_graph[value2].edges.push_back(&m_graph[value1]);
	}
	T getNodeValue(T value) {
		return m_graph[value].value;
	}
	void showConnection(T value) {
		for (int i = 0; i < m_graph[value].edges.size(); i++) {
			cout << m_graph[value].edges[i]->value << endl;
		}
	}
	void unSearch() {
		vector<Node<int>>::iterator it;
		for (it = m_nodes.begin(); it != m_nodes.end(); it++) {
			Node<T> temp_node = *it;
			m_graph[temp_node.value].searched = false;
		}
	}
	void getConnection(T value1, T value2, vector<T> &v) {
		unSearch();
		queue <T> nodes;
		nodes.push(value1);
		while (!nodes.empty()) {
			T current = nodes.front();
			nodes.pop();
			if (current == value2) {
				//cout << current << endl;
				v.push_back(current);
				Node<T> currentNode = m_graph[current];
				while (currentNode.parent->value != value1) {
					//cout << currentNode.parent->value << endl;
					v.push_back(currentNode.parent->value);
					currentNode = *currentNode.parent;
				}
				v.push_back(currentNode.parent->value);
				//cout << currentNode.parent->value << endl;
				break;
			}
			for (unsigned int i = 0; i < m_graph[current].edges.size(); i++) {
				Node<T>* c = m_graph[current].edges[i];
				if (!c->searched) {
					c->searched = true;
					c->parent = &m_graph[current];
					nodes.push(c->value);
				}
			}
		}
	}
	void getNodes(vector<Node<T>>& v) {
		vector<Node<int>>::iterator it;
		for (it = m_nodes.begin(); it != m_nodes.end(); it++) {
			Node<T> temp_node = *it;
			uniform_real_distribution<> distw(0, width - 50);
			temp_node.x = distw(e2);
			uniform_real_distribution<> disth(0, height - 50);
			temp_node.y = disth(e2);
			v.push_back(temp_node);
		}
	}
	vector<Node<T>*> getEdges(T value) {
		return m_graph[value].edges;
	}
	void shuffleNodes(vector<Node<T>>& v) {
		v.clear();
		vector<Node<int>>::iterator it;
		for (it = m_nodes.begin(); it != m_nodes.end(); it++) {
			Node<T> temp_node = *it;
			uniform_real_distribution<> distw(0, width - 50);
			temp_node.x = distw(e2);
			uniform_real_distribution<> disth(0, height - 50);
			temp_node.y = disth(e2);
			v.push_back(temp_node);
		}
	}
private:
	vector<Node<T>> m_nodes;
	unordered_map<T, Node<T>> m_graph;

};


Color red = { 255, 0, 0, 1 };
SDL_Color white = { 255, 255, 255 };
SDL_Color w = { 255, 255, 255 };
TTF_Font * font;
SDL_Surface * surface;
SDL_Texture * texture;
vector<Node<int>> public_nodes;
Graph<int> GR;
vector<int> path_vector;

void initBeadthFSearch() {
	GR.addNode(0);
	GR.addNode(2);
	GR.addNode(15);
	GR.addNode(10);
	GR.addNode(1);

	GR.setEdge(2, 0);
	GR.setEdge(2, 10);

	GR.setEdge(10, 15);
	GR.setEdge(10, 1);

	GR.setEdge(0, 1);
	GR.setEdge(0, 15);

	//GR.showConnection(20);
	//GR.getConnection(2, 1);
};

void drawCircleTriangle(SDL_Renderer* renderer, float x, float y, float radius, Color c) {
	int i;
	int triangleAmount = 500;
	float twicePi = 2.0f * M_PI;

	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, SDL_ALPHA_OPAQUE);
	
	for (i = 0; i <= triangleAmount; i++) {
		SDL_RenderDrawLine(renderer, x, y, x + (radius * cos(i * twicePi / triangleAmount)), y + (radius * sin(i * twicePi / triangleAmount)));
	}
	SDL_RenderPresent(renderer);

}

void drawCircleLine(SDL_Renderer *renderer, float x, float y, float radius, Color c) {
	float r2 = radius*radius;
	float ny, nx;
	//SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);

	for (int i = r2; i >= 0; i--) {
		ny = sqrt(r2 - i * i);
		SDL_RenderDrawLine(renderer, x - i, y-ny, x - i, y+ny);
		SDL_RenderDrawLine(renderer, x + i, y - ny, x + i, y + ny);
	}

	SDL_RenderPresent(renderer);
}


void drawNode(SDL_Renderer *renderer, float x, float y, int number) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	drawCircleLine(renderer, x, y, 20, red);

	/* handling text here */
	surface = TTF_RenderText_Solid(font, to_string(number).c_str(), w);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	int texW = 0;
	int texH = 0;
	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	SDL_Rect dstrect = { x-5, y-10, texW, texH };
	SDL_RenderCopy(renderer, texture, NULL, &dstrect);
	SDL_RenderPresent(renderer);
}

void update(SDL_Renderer *renderer) {
	vector<Node<int>>::iterator it;
	vector<Node<int>>::iterator tit;

	for (it = public_nodes.begin(); it != public_nodes.end(); it++) {
		Node<int> temp_node = *it;
		drawNode(renderer, temp_node.x, temp_node.y, temp_node.value);
		vector<Node<int>*> edges = GR.getEdges(temp_node.value);
		for (int i = 0; i < edges.size(); i++) {
			Node<int>* temp_edge = edges[i];
			for (tit = public_nodes.begin(); tit != public_nodes.end(); tit++) {
				if (tit->value == temp_edge->value) {
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
					SDL_RenderDrawLine(renderer, temp_node.x, temp_node.y, tit->x, tit->y);
					SDL_RenderPresent(renderer);
				}
			}
		}
	}
	int k = path_vector.size();
	if (k % 2 != 0) {
		k -= 1;
		Node<int> f_node;
		Node<int> s_node;
		if (path_vector.size() != 1) {
			for (it = public_nodes.begin(); it != public_nodes.end(); it++) {
				Node<int> temp_node = *it;
				if (temp_node.value == path_vector[path_vector.size() - 1]) {
					f_node = temp_node;
				}
			}
			for (it = public_nodes.begin(); it != public_nodes.end(); it++) {
				Node<int> temp_node = *it;
				if (temp_node.value == path_vector[path_vector.size() - 2]) {
					s_node = temp_node;
				}
			}
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			SDL_RenderDrawLine(renderer, f_node.x, f_node.y, s_node.x, s_node.y);
			SDL_RenderPresent(renderer);
		}
	}
	for (int i = 0; i < k / 2; i++) {
		Node<int> f_node;
		Node<int> s_node;
		for (it = public_nodes.begin(); it != public_nodes.end(); it++) {
			Node<int> temp_node = *it;
			if (temp_node.value == path_vector[i]) {
				f_node = temp_node;
			}
		}
		for (it = public_nodes.begin(); it != public_nodes.end(); it++) {
			Node<int> temp_node = *it;
			if (temp_node.value == path_vector[i + 1]) {
				s_node = temp_node;
			}
		}
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderDrawLine(renderer, f_node.x, f_node.y, s_node.x, s_node.y);
		SDL_RenderPresent(renderer);
	}
}


int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window *window = SDL_CreateWindow("Visual Breadth first search", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
	SDL_Event window_event;

	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
	Uint32 frameStart;
	int frameTime;

	TTF_Init();
	font = TTF_OpenFont("arial.ttf", 16);


	initBeadthFSearch();
	GR.getNodes(public_nodes);

	while (true) {
		if (SDL_PollEvent(&window_event)) {
			if (SDL_QUIT == window_event.type) {
				break;
			}
			if (window_event.type == SDL_KEYDOWN)
			{
				switch (window_event.key.keysym.sym) {
					case 115:
						cout << "Shuffling ..." << endl;
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
						SDL_RenderClear(renderer);

						GR.shuffleNodes(public_nodes);
						break;
					case 97:
						path_vector.clear();
						int source, dest;
						cout << "Source : ";
						cin >> source;
						cout << endl << "Destination :";
						cin >> dest;
						GR.getConnection(source, dest, path_vector);
						if (path_vector.size() != 0) {
							cout << path_vector.size() << endl;
						}
						break;
					case 112:
						int a;
						cout << "Pres 1 then enter to continue ...";
						cin >> a;
						break;
				}
			}
			
		}
		frameStart = SDL_GetTicks();
		update(renderer);
		
		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();

	return 0;
}