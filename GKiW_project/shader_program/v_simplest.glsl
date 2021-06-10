#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec4 lp1;
uniform vec4 lp2;
//Atrybuty
in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec4 normal; //wektor normalny w przestrzeni modelu
in vec2 texCoord0;

//Zmienne interpolowane
out vec4 l1;
out vec4 l2;
out vec4 l3;
out vec4 l4;
out vec4 n;
out vec4 v;
out vec2 iTexCoord0;

void main(void) {

    iTexCoord0 = texCoord0;
    //vec4 lp1 = vec4(3.6f, 1.74f, 3.49f,1.0f);
    //vec4 lp2 = vec4(-3.6f, 1.74f, 3.49f,1.0f);
    vec4 lp3 = vec4(3.4f, 1.8f, -3.4f,1.0f);
    vec4 lp4 = vec4(-3.4f, 1.8f, -3.4f,1.0f);

    l1 = normalize(V * lp1 - V*M*vertex);
    l2 = normalize(V * lp2 - V*M*vertex);
    l3 = normalize(V * lp3 - V*M*vertex);
    l4 = normalize(V * lp4 - V*M*vertex);

    v = normalize(vec4(0, 0, 0, 1) - V * M * vertex); //wektor do obserwatora w przestrzeni oka
    n = normalize(V * M * normal); //wektor normalny w przestrzeni oka
    
    gl_Position=P*V*M*vertex;
}