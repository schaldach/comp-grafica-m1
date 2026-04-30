#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <GL/freeglut.h>
#include <string>
using namespace std;

//globals

struct Face {
    vector<float> vertices;
    vector<float> normals;
    vector<float> textures;
};

struct Luz {
    int id;
    std::array<GLfloat, 4> posicao;
    std::array<GLfloat, 4> ambiente;
    std::array<GLfloat, 4> difusa;
    std::array<GLfloat, 4> especular;
    bool ativada;
};

unsigned int elefante;
vector<vector<float>> vertices;
vector<vector<float>> normals;
vector<vector<float>> textures;
vector<Face> faces;

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
        string face_vertice;
        string v_str, vt_str, vn_str;
        Face ultima_face;
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
                Face face;

                for (int i = 0; i < 3; i++)
                {
                    arquivo >> face_vertice;
                    istringstream vss(face_vertice);

                    // vértice
                    getline(vss, v_str, '/');
                    int v = stoi(v_str);
                    if (v >= 0) v--; // o index negativo estará correto (-1 é de fato o último), enquanto que o index positivo, por começar em 1, tem que ser subtraido 1
                    face.vertices.push_back(v);

                    // textura
                    if (getline(vss, vt_str, '/') && !vt_str.empty())
                    {
                        int vt = stoi(vt_str);
                        if (vt >= 0) vt--;
                        face.textures.push_back(vt);
                    }

                    // normal
                    if (getline(vss, vn_str, '/') && !vn_str.empty())
                    {
                        int vn = stoi(vn_str);
                        if (vn >= 0) vn--;
                        face.normals.push_back(vn);
                    }
                }

                ultima_face.vertices = face.vertices;
                ultima_face.textures = face.textures;
                ultima_face.normals = face.normals;

                faces.push_back(face);
            }
            // como detectar melhor quando continua a face? pois no radar existe uma linha com '/' que não é face
            else if (tipo.find("/") != -1 && tipo.substr(tipo.find("/") + 1, tipo.length()).find("/") != -1) { // ao menos 2 caracteres '/'
                // o que seria o "tipo" veio um outro vértice, pois é uma face com mais de 3 vértices
                // face com 4 vértices, criaremos o 2º triângulo
                // se ele especifica um quadrilátero em qualquer ordem, é o 2º vértice que fica oposto ao último.
                // então iremos fazer o triângulo com o 1º e 3º vértices
                // e pensando nos casos com 5 vértices, que eu não havia percebido antes, esse comportamento se repete
                Face face;

                face.vertices.push_back(ultima_face.vertices[0]);
                face.vertices.push_back(ultima_face.vertices[2]);

                face.textures.push_back(ultima_face.textures[0]);
                face.textures.push_back(ultima_face.textures[2]);

                face.normals.push_back(ultima_face.normals[0]);
                face.normals.push_back(ultima_face.normals[2]);

                // cout << "tipo: " << tipo << "\n";
                istringstream vss(tipo);

                // vértice
                getline(vss, v_str, '/');
                int v = stoi(v_str);
                if (v >= 0) v--;
                face.vertices.push_back(v);

                // textura
                if (getline(vss, vt_str, '/') && !vt_str.empty())
                {
                    int vt = stoi(vt_str);
                    if (vt >= 0) vt--;
                    face.textures.push_back(vt);
                }

                // normal
                if (getline(vss, vn_str, '/') && !vn_str.empty())
                {
                    int vn = stoi(vn_str);
                    if (vn >= 0) vn--;
                    face.normals.push_back(vn);
                }

                ultima_face.vertices = face.vertices;
                ultima_face.textures = face.textures;
                ultima_face.normals = face.normals;

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
            Face face = faces[i];

            int v1_index = face.vertices[0];
            if (v1_index < 0) v1_index = vertices.size() + v1_index;

            int v2_index = face.vertices[1];
            if (v2_index < 0) v2_index = vertices.size() + v2_index;

            int v3_index = face.vertices[2];
            if (v3_index < 0) v3_index = vertices.size() + v3_index;

            glVertex3f(vertices[v1_index][0], vertices[v1_index][1], vertices[v1_index][2]);
            glVertex3f(vertices[v2_index][0], vertices[v2_index][1], vertices[v2_index][2]);
            glVertex3f(vertices[v3_index][0], vertices[v3_index][1], vertices[v3_index][2]);

            if (!face.normals.empty())
            {
                int n1_index = face.normals[0];
                if (n1_index < 0) n1_index = normals.size() + n1_index;

                int n2_index = face.normals[1];
                if (n2_index < 0) n2_index = normals.size() + n2_index;

                int n3_index = face.normals[2];
                if (n3_index < 0) n3_index = normals.size() + n3_index;

                glNormal3f(normals[n1_index][0], normals[n1_index][1], normals[n1_index][2]);
                glNormal3f(normals[n2_index][0], normals[n2_index][1], normals[n2_index][2]);
                glNormal3f(normals[n3_index][0], normals[n3_index][1], normals[n3_index][2]);
            }
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

vector<Luz> luzes = {
    {
        GL_LIGHT0,
        { 500.0, 500.0, 0.0, 1.0 },
        { 0.2, 0.15, 0.1, 1.0 },
        { 0.8, 0.7, 0.6, 1.0 },
        { 1.0, 1.0, 1.0, 1.0 },
        true
    }, {
        GL_LIGHT1,
        { -500.0, -500.0, 0.0, 1.0 },
        { 0.1, 0.2, 0.1, 1.0 },
        { 0.7, 0.8, 0.7, 1.0 },
        { 1.0, 1.0, 1.0, 1.0 },
        true
    }, {
        GL_LIGHT2,
        { 0.0, 0.0, 1000.0, 1.0 },
        { 0.1, 0.15, 0.2, 1.0 },
        { 0.4, 0.6, 0.8, 1.0 },
        { 1.0, 1.0, 1.0, 1.0 },
        true
    }
};

void setupLighting()
{
    for (const auto& luz : luzes)
    {
        if (luz.ativada)
            glEnable(luz.id);
        else
            glDisable(luz.id);
    }
}

void display(void)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    drawElephant(0);
    setupLighting();
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
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
    case '1':
        luzes[0].ativada = !luzes[0].ativada;
        break;
    case '2':
        luzes[1].ativada = !luzes[1].ativada;
        break;
    case '3':
        luzes[2].ativada = !luzes[2].ativada;
        break;
    }
}

void timer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(10, timer, 0);
}

void initGL()
{
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_LIGHTING);

    for (const auto& luz : luzes)
    {
        glLightfv(luz.id, GL_POSITION, luz.posicao.data());

        glLightfv(luz.id, GL_AMBIENT, luz.ambiente.data());
        glLightfv(luz.id, GL_DIFFUSE, luz.difusa.data());
        glLightfv(luz.id, GL_SPECULAR, luz.especular.data());
    }
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
    initGL();
    loadObj("data/porsche.obj");
    glutMainLoop();
    return 0;
}
