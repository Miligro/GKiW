//#version 330
//
////Zmienne jednorodne
//uniform mat4 P;
//uniform mat4 V;
//uniform mat4 M;
//
////Atrybuty
//in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
////in vec4 color; //kolor zwi�zany z wierzcho�kiem
//in vec4 normal; //wektor normalny w przestrzeni modelu
//in vec2 texCoord0;
//
////Zmienne interpolowane
//out vec4 ic;
//out vec4 l;
//out vec4 n;
//out vec4 v;
//out vec2 iTexCoord0;
//
//void main(void) {
//
//    iTexCoord0 = texCoord0;
//    vec4 color1 = vec4(0,0,1,1);
//    vec4 lp = vec4(0, 1, 0, 1); //pozcyja �wiat�a, przestrze� �wiata
//    l = normalize(V * lp - V*M*vertex); //wektor do �wiat�a w przestrzeni oka
//    v = normalize(vec4(0, 0, 0, 1) - V * M * vertex); //wektor do obserwatora w przestrzeni oka
//    n = normalize(V * M * normal); //wektor normalny w przestrzeni oka
//
//    ic = color1;
//    
//    gl_Position=P*V*M*vertex;
//}
//






//te lepsze
#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

//Atrybuty
in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
//in vec4 color; //kolor zwi�zany z wierzcho�kiem
in vec4 normal; //wektor normalny w przestrzeni modelu
in vec2 texCoord0;

//Zmienne interpolowane
out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

void main(void) {

    gl_Position = P * V *  M * vertex;
    FragPos = vec3(M * vertex);
    Normal = mat3(transpose(inverse(M))) * normal.xyz;
    TexCoords = texCoord0;
}
