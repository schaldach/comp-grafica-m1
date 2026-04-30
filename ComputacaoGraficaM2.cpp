#include <iostream>
#include <fstream>
#include <vector>
#include <GL/freeglut.h>
#include <string>
using namespace std;

//globals

unsigned int elefante;
vector<vector<float>> vertices;
vector<vector<float>> normals;
vector<vector<float>> textures;
vector<vector<int>> faces;

void keyboard(unsigned char key, int x, int y);

void loadObj(string fname)
{
    int read;
    float x, y, z;
    ifstream arquivo(fname);
    if (!arquivo.is_open()) {
        cout << "arquivo nao encontrado";
        exit(1);
    }
    else {
        string tipo;
        vector<float> ultima_face;
        while (arquivo >> tipo)
        {
            if (tipo == "v")
            {
                vector<float> vertice;
                float x, y, z;
                arquivo >> x >> y >> z;
                vertice.push_back(x);
                vertice.push_back(y);
                vertice.push_back(z);
                vertices.push_back(vertice);
            }

            else if (tipo == "vn")
            {
                vector<float> normal;
                float x, y, z;
                arquivo >> x >> y >> z;
                normal.push_back(x);
                normal.push_back(y);
                normal.push_back(z);
                normals.push_back(normal);
            }

            else if (tipo == "vt")
            {
                vector<float> texture;
                float x, y;
                arquivo >> x >> y;
                texture.push_back(x);
                texture.push_back(y);
                textures.push_back(texture);
            }

            else if (tipo == "f")
            {
                vector<int> face;
                string x, y, z;
                arquivo >> x >> y >> z;

                int fp = stoi(x.substr(0, x.find("/")));
                if (fp >= 0) fp = fp - 1; // o index negativo estará correto (-1 é de fato o último), enquanto que o index positivo, por começar em 1, tem que ser subtraido 1

                int fs = stoi(y.substr(0, y.find("/")));
                if (fs >= 0) fs = fs - 1;

                int ft = stoi(z.substr(0, z.find("/")));
                if (ft >= 0) ft = ft - 1;

                face.push_back(fp);
                face.push_back(fs);
                face.push_back(ft);

                ultima_face.clear();

                ultima_face.push_back(face[0]);
                ultima_face.push_back(face[1]);
                ultima_face.push_back(face[2]);

                faces.push_back(face);
            }
            // como detectar melhor quando continua a face? pois no radar existe uma linha com '/' que não é face
            else if (tipo.find("/") != -1 && tipo.substr(tipo.find("/") + 1, tipo.length()).find("/") != -1) { // ao menos 2 caracteres '/'
                // o que seria o "tipo" veio um outro vértice, pois é uma face com mais de 3 vértices
                // face com 4 vértices, criaremos o 2º triângulo
                // se ele especifica um quadrilátero em qualquer ordem, é o 2º vértice que fica oposto ao último.
                // então iremos fazer o triângulo com o 1º e 3º vértices
                // e pensando nos casos com 5 vértices, que eu não havia percebido antes, esse comportamento se repete
                vector<int> face;
                // cout << "tipo: " << tipo << "\n";
                int f = stoi(tipo.substr(0, tipo.find("/")));
                if (f >= 0) f = f - 1; // o index negativo estará correto (-1 é de fato o último), enquanto que o index positivo, por começar em 1, tem que ser subtraido 1

                face.push_back(ultima_face[0]);
                face.push_back(ultima_face[2]);
                face.push_back(f);

                ultima_face.clear();

                ultima_face.push_back(face[0]);
                ultima_face.push_back(face[1]);
                ultima_face.push_back(face[2]);

                faces.push_back(face);
            }
        }
    }




    elefante = glGenLists(1);
    glPointSize(2.0);
    glNewList(elefante, GL_COMPILE);
    {
        glPushMatrix();
        glBegin(GL_TRIANGLES);

        for (int i = 0; i < faces.size(); i++)
        {
            vector<int> face = faces[i];

            int v1_index = face[0];
            if (v1_index < 0) v1_index = vertices.size() + v1_index;

            int v2_index = face[1];
            if (v2_index < 0) v2_index = vertices.size() + v2_index;

            int v3_index = face[2];
            if (v3_index < 0) v3_index = vertices.size() + v3_index;

            glVertex3f(vertices[v1_index][0], vertices[v1_index][1], vertices[v1_index][2]);
            glVertex3f(vertices[v2_index][0], vertices[v2_index][1], vertices[v2_index][2]);
            glVertex3f(vertices[v3_index][0], vertices[v3_index][1], vertices[v3_index][2]);

        }
        glEnd();

    }
    glPopMatrix();
    glEndList();
    arquivo.close();

}


void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)w / (GLfloat)h, 0.1, 1000.0);

    glMatrixMode(GL_MODELVIEW);
}

float rotation_x = 0;
float rotation_y = 0;
float rotation_z = 0;

float movimentacao_x = 0;
float movimentacao_y = -40;
float movimentacao_z = -105;

float escala = 0.4;

void drawElephant(float rotation)
{
    glPushMatrix();
    glColor3f(1.0, 0.23, 0.27);

    // translação (x,y,z)
    glTranslatef(movimentacao_x, movimentacao_y, movimentacao_z);

    // escala (mesmo número para todos)
    glScalef(escala, escala, escala);

    // rotação (x,y,z)
    glRotatef(rotation_x, 1, 0, 0);
    glRotatef(rotation_y, 0, 1, 0);
    glRotatef(rotation_z, 0, 0, 1);

    glCallList(elefante);
    glPopMatrix();
}
void display(void)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    drawElephant(0);
    glutSwapBuffers();
}

void timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(10, timer, 0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 450);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Carregar OBJ");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(10, timer, 0);
    loadObj("data/porsche.obj");
    glutMainLoop();
    return 0;
}

void keyboard(unsigned char key, int x, int y) {
    std::cout << key;
    switch (key) {
    case 27:
        exit(0);
        break;
    case 'o':
        escala = escala * 0.9;
        break;
    case 'p':
        escala = escala * 1.1;
        break;
    case 'w':
		rotation_x -= 2;
        break;
    case 's':
        rotation_x += 2;
        break;
    case 'd':
        rotation_y -= 2;
        break;
    case 'a':
        rotation_y += 2;
        break;
    case 'e':
        rotation_z -= 2;
        break;
    case 'q':
        rotation_z += 2;
        break;
    case 'g':
        movimentacao_x -= 2;
        break;
    case 'j':
        movimentacao_x += 2;
        break;
    case 'n':
        movimentacao_y -= 2;
        break;
    case 'h':
        movimentacao_y += 2;
        break;
    case 'b':
        movimentacao_z -= 2;
        break;
    case 'm':
        movimentacao_z += 2;
        break;
    }
}
