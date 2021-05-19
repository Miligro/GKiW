#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec4 lp;

//Atrybuty
in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec4 normal;
in vec4 color;

out vec4 ic;

void main(void) {

    vec4 l = normalize(V * lp - V * M * vertex);
    vec4 n = normalize(V*M*normal);

    float nl = clamp(dot(n, l),0,1);

    ic = vec4(color.rgb * nl, color.a);

    gl_Position=P*V*M*vertex;
}
