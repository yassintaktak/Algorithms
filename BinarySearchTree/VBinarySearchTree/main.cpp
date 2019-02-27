#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include <math.h>
#include <string>
#include <stdio.h>
#include <vector>

using namespace std;

# define M_PI           3.14159265358979323846

struct Point {
	GLint x;
	GLint y;
};
struct Color {
	GLfloat r;
	GLfloat g;
	GLfloat b;
	GLfloat a;
};

template <typename T> struct Node {
	T value = -INT_MIN;
	int isRoot = 0;
	Node<T> *left;
	Node<T> *right;
	Node<T> *ownerNode;
};

vector<int> visitedNodes;

template <class T> class BinarySearchTree {
public:
	BinarySearchTree(T initial_value) {
		m_root.value = initial_value;
		m_root.left = new Node<T>;
		m_root.right = new Node<T>;
		m_root.isRoot = 1;
		m_root.ownerNode = m_root.right;
	}
	void insertValue(T value) {
		recInsertValue(value, m_root);
	}
	void recInsertValue(T& value, Node<T>& node) {
		Node<T> *right = node.right;
		Node<T> *left = node.left;

		if (value < node.value) {
			if (node.left->value != -INT_MIN) {
				recInsertValue(value, *node.left);
			}
			else {
				node.left->value = value;
				node.left->left = new Node<T>;
				node.left->right = new Node<T>;
				node.left->ownerNode = &node;
			}
		}
		else {
			if (node.right->value != -INT_MIN) {
				recInsertValue(value, *node.right);
			}
			else {
				node.right->value = value;
				node.right->left = new Node<T>;
				node.right->right = new Node<T>;
				node.left->ownerNode = &node;
			}
		}
	}
	Node<T> getRoot() {
		return m_root;
	}
	void traverseTree(vector<int>* m_v) {
		//cout << m_root.value << endl;
		m_v->push_back(m_root.value);
		if(m_root.left->value != -INT_MIN)
			visit(m_root.left, m_v);
		if(m_root.right->value != -INT_MIN)
			visit(m_root.right, m_v);
	}
	void visit(Node<T>* node, vector<int>* m_v) {
		//cout << node->value << endl;
		m_v->push_back(node->value);
		if (node->left->value != -INT_MIN) {
			visit(node->left, m_v);
		}
		if (node->right->value != -INT_MIN) {
			visit(node->right, m_v);
		}
	}
	Node<T> topLeftNode(Node<T> node) {
		Node<T>* current_node = &node;
		while (current_node->left->value != -INT_MIN) {
			current_node = current_node->left;
		}
		return *current_node;
	}
	void sortTree() {
		Node<T> currentNode = topLeftNode(m_root);
		cout << currentNode.value << endl;
		sortRecTree(currentNode, currentNode.ownerNode);
	}
	void sortRecTree(Node<T> node, Node<T>* ownerNode) {
		if (node.isRoot) {
			root_surfed += 1;
		}
		if (root_surfed == 1) {
			return;
		}
		if (node.value != ownerNode->left->value)
			cout << node.value << endl;
		cout << node.ownerNode->value << endl;
		if (node.ownerNode->right->value != -INT_MIN) {
			if (topLeftNode(*node.ownerNode->right).value != node.ownerNode->right->value) {
				sortRecTree(topLeftNode(*node.ownerNode->right), ownerNode);
			}
			else {
				cout << node.ownerNode->right->value << endl;
				sortRecTree(*ownerNode, ownerNode->ownerNode);
			}
		}
		else {
			sortRecTree(*ownerNode, ownerNode->ownerNode);
		}
	}
	bool searchTree(T value) {
		if (recSearchTree(value, m_root)) {
			return true;
		}
		else {
			return false;
		}
	}
	bool recSearchTree(T value, Node<T> node) {
		if (value == node.value) {
			visitedNodes.push_back(node.value);
			return true;
		}
		else if (value < node.value) {
			if (node.left->value != -INT_MIN) {
				visitedNodes.push_back(node.value);
				recSearchTree(value, *node.left);
			}
			else {
				visitedNodes.clear();
				return false;
			}
		}
		else if (value > node.value) {
			if (node.right->value != -INT_MIN) {
				visitedNodes.push_back(node.value);
				recSearchTree(value, *node.right);
			}
			else {
				visitedNodes.clear();
				return false;
			}
		}
	}
	Point getPosition(int value, Point relative_point) {
		if (value == m_root.value) {
			return relative_point;
		} else {
			int new_pos_x = getPositionRecX(m_root, value, relative_point, relative_point.x);
			int new_pos_y = getPositionRecY(m_root, value, relative_point, relative_point.y);
			return { new_pos_x, new_pos_y };
		}
	}
	int getPositionRecX(Node<T> Node, int value, Point relative_point, int x) {
		if (value == Node.value) {
			return x;
		} else if (value < Node.value) {
			if (Node.left->value != -INT_MIN) {
					getPositionRecX(*Node.left, value, relative_point, x-70);
			}	else {
				return x;
			}
			
		} else if(value > Node.value) {
			if (Node.right->value != -INT_MIN) {
				if(value > m_root.value)
					getPositionRecX(*Node.right, value, relative_point, x+100);
				else 
					getPositionRecX(*Node.right, value, relative_point, x + 70);
			} else {
				return x;
			}
		}
	}
	int getPositionRecY(Node<T> Node, int value, Point relative_point, int y) {
		if (value == Node.value) {
			return y;
		}
		else if (value < Node.value) {
			if (Node.left->value != -INT_MIN) {
				getPositionRecY(*Node.left, value, relative_point, y - 60);
			}
			else {
				return y;
			}

		}
		else if (value > Node.value) {
			if (Node.right->value != -INT_MIN) {
				getPositionRecY(*Node.right, value, relative_point, y - 60);
			}
			else {
				return y;
			}
		}
	}
	int getPrevValue(int value) {
		return getPrevValueRec(m_root, m_root, value);
	}
	int getPrevValueRec(Node<T> Node, Node<T> prev, int value) {
		if (Node.value == value) {
			return prev.value;
		}
		else if (value < Node.value) {
			if (Node.left->value != -INT_MIN) {
				getPrevValueRec(*Node.left, Node, value);
			}
			else {
				return prev.value;
			}
		}
		else if (value > Node.value) {
			if (Node.right->value != -INT_MIN) {
				getPrevValueRec(*Node.right, Node, value);
			}
			else {
				return prev.value;
			}
		}
	}
private:
	Node<T> m_root;
	int root_surfed = 0;
};

void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor3f(0.0, 0.0, 0.0);
	glPointSize(1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 640, 0, 480);
}


BinarySearchTree<int> *BST;

void initTree() {
	int n, k, j;
	cout << "Insert root Node : ";
	cin >> n;
	BST = new BinarySearchTree<int>(n);
	cout << "Insert number of nodes : ";
	cin >> k;
	for (int i = 0; i < k; i++) {
		cout << "Node[" << i << "] = ";
		cin >> j;
		BST->insertValue(j);
	}
}
bool inArray(int n, vector<int> v) {
	vector<int>::iterator it;
	for (it = v.begin(); it != v.end(); it++) {
		int item = *it;
		if (item == n) {
			return true;
		}
	}
	return false;
}
void drawCircle(GLfloat x, GLfloat y, GLfloat radius, Color color) {
	int i;
	int triangleAmount = 3000;
	GLfloat twicePi = 2.0f * M_PI;

	glEnable(GL_LINE_SMOOTH);
	glLineWidth(5.0);

	glBegin(GL_LINES);
	glColor4f(color.r, color.g, color.b, color.a);
	for (i = 0; i <= triangleAmount; i++)
	{
		glVertex2f(x, y);
		glVertex2f(x + (radius * cos(i * twicePi / triangleAmount)), y + (radius * sin(i * twicePi / triangleAmount)));
	}
	glEnd();
}
void Sprint(int x, int y, char *st)
{
	int l, i;
	glColor3f(1.0f, 1.0f, 1.0f);
	l = strlen(st); // see how many characters are in text string.
	glRasterPos2i(x, y); // location to start printing text
	for (i = 0; i < l; i++) // loop until i is greater then l
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, st[i]); // Print a character on the screen
	}

}

void changeViewPort(int w, int h)
{
	glViewport(0, 0, w, h);
}

void drawNode(Point pt, int text, Color color) {
	glBegin(GL_POINTS);
	drawCircle(pt.x, pt.y, 10, color);
	string temp = to_string(text);
	char tab2[1024];
	strncpy_s(tab2, temp.c_str(), sizeof(tab2));
	tab2[sizeof(tab2) - 1] = 0;
	Sprint(pt.x-(3+temp.size()), pt.y-3, tab2);
	glEnd();
}
void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	Point rootNode = { 315,  400 };
	vector<int> m_v;
	BST->traverseTree(&m_v);
	//cout << m_v.size() << endl;
	vector<int>::iterator i;
	Color redish = { 0.839, 0.188, 0.192, 1.0 };
	Color blueish = { 0.035 , 0.517 , 0.890, 1.0 };
	if (inArray(BST->getRoot().value, visitedNodes)) {
		drawNode(rootNode, BST->getRoot().value, blueish);
	}
	else {
		drawNode(rootNode, BST->getRoot().value, redish);
	}
	for (i = m_v.begin(); i != m_v.end(); i++) {
		//cout << *i << endl;
		Point position = BST->getPosition(*i, rootNode);
		//cout << "Val : " << *i << " X : " << position.x << " Y : " << position.y << endl;
		if (inArray(*i, visitedNodes)) {
			drawNode(position, *i, blueish);
		}
		else {
			drawNode(position, *i, redish);
		}
		
	}
	for (i = m_v.begin(); i != m_v.end(); i++) {
		Point position = BST->getPosition(*i, rootNode);
		Point prevPos = BST->getPosition(BST->getPrevValue(*i), rootNode);
		glBegin(GL_LINES);
		if (inArray(*i, visitedNodes)) {
			glColor4f(blueish.r, blueish.g, blueish.b, blueish.a);
		}
		else {
			glColor4f(redish.r, redish.g, redish.b, redish.a);
		}
		glVertex2i(prevPos.x, prevPos.y);
		glVertex2i(position.x, position.y);
		glEnd();
	}
	glFlush();
	glutSwapBuffers();
}
void pressKey(unsigned char key, int x, int y) {
	if (key == 's') {
		int toSearch;
		cout << "Search ? ";
		cin >> toSearch;
		visitedNodes.clear();
		if (BST->searchTree(toSearch)) {
			cout << toSearch << " is found!" << endl;
		}
		else {
			cout << toSearch << " not found." << endl;
		}
		render();
		cout << endl;
	}
}


int main(int argc, char* argv[]) {
	// Initialize GLUT
	glutInit(&argc, argv);
	// Set up some memory buffers for our display
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	// Set the window size
	glutInitWindowSize(800, 600);
	// Create the window
	glutCreateWindow("Visualize binary search tree");
	// Bind the two functions (above) to respond when necessary
	glutReshapeFunc(changeViewPort);
	glutKeyboardFunc(pressKey);
	init();
	initTree();
	glutDisplayFunc(render);
	// Very important!  This initializes the entry points in the OpenGL driver so we can 
	// call all the functions in the API.
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW error");
		return 1;
	}


	glutMainLoop();
	return 0;
}