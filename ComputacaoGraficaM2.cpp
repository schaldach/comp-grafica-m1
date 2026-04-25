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
float rot_ele;


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
            // cout << "Tipo: " << tipo << "\n";
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

                int fp = stoi(x.substr(0, x.find("/"))) - 1;
                int fs = stoi(y.substr(0, y.find("/"))) - 1;
                int ft = stoi(z.substr(0, z.find("/"))) - 1;
                face.push_back(fp);
                face.push_back(fs);
                face.push_back(ft);

                ultima_face.clear();

                ultima_face.push_back(face[0]);
                ultima_face.push_back(face[1]);
                ultima_face.push_back(face[2]);

                faces.push_back(face);
            }

            else if(tipo.find("/") != -1) { // o que seria o "tipo" veio um outro vértice, pois é uma face com mais de 3 vértices
                // face com 4 vértices, criaremos o 2º triângulo
                // se ele especifica um quadrilátero em qualquer ordem, é o 2º vértice que fica oposto ao último.
                // então iremos fazer o triângulo com o 1º e 3º vértices
                // e pensando nos casos com 5 vértices, que eu não havia percebido antes, esse comportamento se repete
                vector<int> face;
                // cout << "tipo: " << tipo << "\n";
                int f = stoi(tipo.substr(0, tipo.find("/"))) - 1;

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

            glVertex3f(vertices[face[0]][0], vertices[face[0]][1], vertices[face[0]][2]);
            glVertex3f(vertices[face[1]][0], vertices[face[1]][1], vertices[face[1]][2]);
            glVertex3f(vertices[face[2]][0], vertices[face[2]][1], vertices[face[2]][2]);

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
void drawElephant()
{
    glPushMatrix();
    glTranslatef(0, -40.00, -105);
    glColor3f(1.0, 0.23, 0.27);
    glScalef(0.4, 0.4, 0.4);
    glRotatef(rot_ele, 0, 1, 0);
    glCallList(elefante);
    glPopMatrix();
    rot_ele = rot_ele + 0.6;
    if (rot_ele > 360) rot_ele = rot_ele - 360;
}
void display(void)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    drawElephant();
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
    glutTimerFunc(10, timer, 0);
    loadObj("data/porsche.obj");
    glutMainLoop();
    return 0;
}
