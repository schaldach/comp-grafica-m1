#include <iostream>
#include <GL/freeglut.h>
#include <vector>
#include <tuple>
#include <math.h>
using vertice = std::tuple<double, double, double>;
using lista_vertices = std::vector<vertice>;
using aresta = std::pair<int, int>;
using lista_arestas = std::vector<aresta>;

struct Cubo {
	double tamanhoLado;
	int numLados;
	vertice posicao;
	double escala;
	double rotacao;
	lista_vertices vertices;
	lista_arestas arestas;
};

Cubo criar_cubo(double posicao_x, double posicao_y, double posicao_z, double tamanho_raio);
void desenhar(Cubo poligono);
void movimentar(Cubo& poligono, double distancia_x, double distancia_y, double distancia_z);
void escalar(Cubo& poligono, double escala);
void rotacionar(Cubo& poligono, double angulo, int eixo);

void display();
void redraw(int value);
void keyboard(unsigned char key, int x, int y);

Cubo cubo;
float velocidadeRotacao = 0;
int delay = 10;

void initGL() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //Coloca a cor de background para preto e opaco
	glClearDepth(1.0f);                   // Define o buffer de profundidade para o mais distante possível
	glEnable(GL_DEPTH_TEST);   // Habilita o culling de profundidade
	glDepthFunc(GL_LEQUAL);    // Define o tipo de teste de profundidade
}

void reshape(GLsizei width, GLsizei height) {
	if (height == 0) height = 1;
	GLfloat aspect = (GLfloat)width / (GLfloat)height;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

int main(int argc, char** argv) {

	cubo = criar_cubo(0, 0, -5, 1);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);

	glutInitWindowSize(640, 480);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("3D");

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	//glutSpecialFunc(keyboard_special);
	glutTimerFunc(10, redraw, 0);
	glutReshapeFunc(reshape);
	initGL();

	glutMainLoop();

	return 0;
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	desenhar(cubo);

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
	std::cout << key;
	switch (key) {
	case 27:
		exit(0);
		break;
	case 'o':
		escalar(cubo, 0.9);
		break;
	case 'p':
		escalar(cubo, 1.1);
		break;

	case 'w':
		rotacionar(cubo, -0.1, 0);
		break;
	case 's':
		rotacionar(cubo, 0.1, 0);
		break;
	case 'd':
		rotacionar(cubo, -0.1, 1);
		break;
	case 'a':
		rotacionar(cubo, 0.1, 1);
		break;
	case 'e':
		rotacionar(cubo, -0.1, 2);
		break;
	case 'q':
		rotacionar(cubo, 0.1, 2);
		break;
	case 'g':
		movimentar(cubo, -0.1, 0, 0);
		break;
	case 'j':
		movimentar(cubo, 0.1, 0, 0);
		break;
	case 'n':
		movimentar(cubo, 0, -0.1, 0);
		break;
	case 'h':
		movimentar(cubo, 0, 0.1, 0);
		break;
	case 'b':
		movimentar(cubo, 0, 0, -0.1);
		break;
	case 'm':
		movimentar(cubo, 0, 0, +0.1);
		break;
	}
}

void redraw(int value) {
	glutPostRedisplay();
	glutTimerFunc(delay, redraw, 0);
}

Cubo criar_cubo(double posicao_x, double posicao_y, double posicao_z, double tamanho_raio) {
	Cubo novo_cubo;
	std::get<0>(novo_cubo.posicao) = posicao_x;
	std::get<1>(novo_cubo.posicao) = posicao_y;
	std::get<2>(novo_cubo.posicao) = posicao_z;

	novo_cubo.escala = 1;

	novo_cubo.rotacao = 0;

	double posicao_x_v, posicao_y_v, posicao_z_v;

	posicao_x_v = posicao_x + tamanho_raio;
	posicao_y_v = posicao_y + tamanho_raio;
	posicao_z_v = posicao_z + tamanho_raio;
	novo_cubo.vertices.push_back(vertice(posicao_x_v, posicao_y_v, posicao_z_v));

	posicao_x_v = posicao_x - tamanho_raio;
	posicao_y_v = posicao_y + tamanho_raio;
	posicao_z_v = posicao_z + tamanho_raio;
	novo_cubo.vertices.push_back(vertice(posicao_x_v, posicao_y_v, posicao_z_v));

	posicao_x_v = posicao_x - tamanho_raio;
	posicao_y_v = posicao_y + tamanho_raio;
	posicao_z_v = posicao_z - tamanho_raio;
	novo_cubo.vertices.push_back(vertice(posicao_x_v, posicao_y_v, posicao_z_v));

	posicao_x_v = posicao_x + tamanho_raio;
	posicao_y_v = posicao_y + tamanho_raio;
	posicao_z_v = posicao_z - tamanho_raio;
	novo_cubo.vertices.push_back(vertice(posicao_x_v, posicao_y_v, posicao_z_v));

	posicao_x_v = posicao_x + tamanho_raio;
	posicao_y_v = posicao_y - tamanho_raio;
	posicao_z_v = posicao_z + tamanho_raio;
	novo_cubo.vertices.push_back(vertice(posicao_x_v, posicao_y_v, posicao_z_v));

	posicao_x_v = posicao_x - tamanho_raio;
	posicao_y_v = posicao_y - tamanho_raio;
	posicao_z_v = posicao_z + tamanho_raio;
	novo_cubo.vertices.push_back(vertice(posicao_x_v, posicao_y_v, posicao_z_v));

	posicao_x_v = posicao_x - tamanho_raio;
	posicao_y_v = posicao_y - tamanho_raio;
	posicao_z_v = posicao_z - tamanho_raio;
	novo_cubo.vertices.push_back(vertice(posicao_x_v, posicao_y_v, posicao_z_v));

	posicao_x_v = posicao_x + tamanho_raio;
	posicao_y_v = posicao_y - tamanho_raio;
	posicao_z_v = posicao_z - tamanho_raio;
	novo_cubo.vertices.push_back(vertice(posicao_x_v, posicao_y_v, posicao_z_v));

	for (int i = 0; i < 4; i++) {
		novo_cubo.arestas.push_back(aresta(i, (i + 1) % 4));
	}
	for (int i = 0; i < 4; i++) {
		novo_cubo.arestas.push_back(aresta(i + 4, ((i + 1) % 4) + 4));
	}
	for (int i = 0; i < 4; i++) {
		novo_cubo.arestas.push_back(aresta(i, i + 4));
	}

	return novo_cubo;
}

void movimentar(Cubo& poligono, double distancia_x, double distancia_y, double distancia_z) {
	std::cout << std::get<0>(poligono.posicao) << "\n";

	std::get<0>(poligono.posicao) = std::get<0>(poligono.posicao) + distancia_x;
	std::get<1>(poligono.posicao) = std::get<1>(poligono.posicao) + distancia_y;
	std::get<2>(poligono.posicao) = std::get<2>(poligono.posicao) + distancia_z;

	for (int i = 0; i < 8; i++) {
		std::get<0>(poligono.vertices[i]) += distancia_x;
		std::get<1>(poligono.vertices[i]) += distancia_y;
		std::get<2>(poligono.vertices[i]) += distancia_z;
	}

	std::cout << std::get<0>(poligono.posicao) << "\n";
}

void escalar(Cubo& poligono, double escala) {
	poligono.escala *= escala;

	std::get<0>(poligono.posicao) = std::get<0>(poligono.posicao);
	std::get<1>(poligono.posicao) = std::get<1>(poligono.posicao);
	std::get<2>(poligono.posicao) = std::get<2>(poligono.posicao);

	for (int i = 0; i < 8; i++) {
		std::get<0>(poligono.vertices[i]) -= std::get<0>(poligono.posicao);
		std::get<1>(poligono.vertices[i]) -= std::get<1>(poligono.posicao);
		std::get<2>(poligono.vertices[i]) -= std::get<2>(poligono.posicao);

		std::get<0>(poligono.vertices[i]) = std::get<0>(poligono.vertices[i]) * escala;
		std::get<1>(poligono.vertices[i]) = std::get<1>(poligono.vertices[i]) * escala;
		std::get<2>(poligono.vertices[i]) = std::get<2>(poligono.vertices[i]) * escala;

		std::get<0>(poligono.vertices[i]) += std::get<0>(poligono.posicao);
		std::get<1>(poligono.vertices[i]) += std::get<1>(poligono.posicao);
		std::get<2>(poligono.vertices[i]) += std::get<2>(poligono.posicao);
	}
}

void rotacionar(Cubo& poligono, double angulo, int eixo) {
	double novoX;
	double novoY;
	double novoZ;

	for (int i = 0; i < 8; i++) {
		std::get<0>(poligono.vertices[i]) -= std::get<0>(poligono.posicao);
		std::get<1>(poligono.vertices[i]) -= std::get<1>(poligono.posicao);
		std::get<2>(poligono.vertices[i]) -= std::get<2>(poligono.posicao);

		switch (eixo) {
		case 0: // x
			novoY = std::get<1>(poligono.vertices[i]) * cos(angulo) - std::get<2>(poligono.vertices[i]) * sin(angulo);
			novoZ = std::get<1>(poligono.vertices[i]) * sin(angulo) + std::get<2>(poligono.vertices[i]) * cos(angulo);

			// std::get<0>(poligono.vertices[i]) = std::get<0>(poligono.vertices[i]);
			std::get<1>(poligono.vertices[i]) = novoY;
			std::get<2>(poligono.vertices[i]) = novoZ;

			break;
		case 1: // y
			novoX = std::get<0>(poligono.vertices[i]) * cos(angulo) - std::get<2>(poligono.vertices[i]) * sin(angulo);
			novoZ = std::get<0>(poligono.vertices[i]) * sin(angulo) + std::get<2>(poligono.vertices[i]) * cos(angulo);

			std::get<0>(poligono.vertices[i]) = novoX;
			// std::get<1>(poligono.vertices[i]) = std::get<1>(poligono.vertices[i]);
			std::get<2>(poligono.vertices[i]) = novoZ;

			break;
		case 2: // z
			novoX = std::get<0>(poligono.vertices[i]) * cos(angulo) - std::get<1>(poligono.vertices[i]) * sin(angulo);
			novoY = std::get<0>(poligono.vertices[i]) * sin(angulo) + std::get<1>(poligono.vertices[i]) * cos(angulo);

			std::get<0>(poligono.vertices[i]) = novoX;
			std::get<1>(poligono.vertices[i]) = novoY;
			// std::get<2>(poligono.vertices[i]) = std::get<2>(poligono.vertices[i]);

			break;
		}

		std::get<0>(poligono.vertices[i]) += std::get<0>(poligono.posicao);
		std::get<1>(poligono.vertices[i]) += std::get<1>(poligono.posicao);
		std::get<2>(poligono.vertices[i]) += std::get<2>(poligono.posicao);
	}

	//for (int i = 0; i < poligono.arestas.size(); i++) {
	//	poligono.vertices[i].first -= poligono.posicao.first;
	//	poligono.vertices[i].second -= poligono.posicao.second;

	//	float novoX = poligono.vertices[i].first * cos(angulo) - poligono.vertices[i].second * sin(angulo);
	//	float novoY = poligono.vertices[i].second = poligono.vertices[i].first * sin(angulo) + poligono.vertices[i].second * cos(angulo);

	//	poligono.vertices[i].first = novoX;
	//	poligono.vertices[i].second = novoY;

	//	poligono.vertices[i].first += poligono.posicao.first;
	//	poligono.vertices[i].second += poligono.posicao.second;
	//}

}

void desenhar(Cubo poligono) {
	glColor3f(1, 1, 1);
	for (int i = 0; i < poligono.arestas.size(); i++) {
		glBegin(GL_LINES);
		float v_o = poligono.arestas[i].first;
		float v_d = poligono.arestas[i].second;

		glVertex3f(std::get<0>(poligono.vertices[v_o]), std::get<1>(poligono.vertices[v_o]), std::get<2>(poligono.vertices[v_o]));
		glVertex3f(std::get<0>(poligono.vertices[v_d]), std::get<1>(poligono.vertices[v_d]), std::get<2>(poligono.vertices[v_d]));
		glEnd();
	}
}
