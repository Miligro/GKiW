//#version 330
//
//uniform sampler2D textureMap0;
//
//out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela
//
//in vec4 ic;
//in vec4 n;
//in vec4 l;
//in vec4 v;
//in vec2 iTexCoord0;
//
//void main(void) {
//
//	vec4 texColor = texture(textureMap0,iTexCoord0);
//
//	vec4 ml = normalize(l);
//	vec4 mn = normalize(n);
//	vec4 mv = normalize(v);
//
//	vec4 mr = reflect(-ml,mn);
//
//	vec4 kd = texColor;
//	vec4 ks = vec4(1,1,1,1);
//	float nl = clamp(dot(mn,ml),0,1);
//	float rv = pow(clamp(dot(mr,mv),0,1),130);
//	pixelColor=vec4(kd.rgb * nl, kd.a) + vec4(ks.rgb*rv, 1);
//	//pixelColor=ic;
//}

#version 330

uniform sampler2D textureMap0;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

void main(void) {

    vec3 lightPos = vec3(0.0,1.0,0.0);
    vec3 lightDiff = vec3(0.5f);
	vec4 texColor = texture(textureMap0,TexCoords);


	vec4 kd = texColor;
	vec3 ks = vec3(1);

	//ambient
	vec3 ambient = vec3(0.2f) * kd.rgb;

	//diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightDiff * diff * kd.xyz;
    
    // Specular
    //tutaj wektor pozycji
    vec3 viewDir = normalize(vec3(0.0,0.0,0.0) - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 130);
    vec3 specular = ks * spec * ks;
    
    // Attenuation
    float distance    = length(lightPos - FragPos);
    float attenuation = 1.0f / (1.0f + 0.09 * distance + 0.002 * (distance * distance));
    
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    
    pixelColor = vec4(ambient + diffuse + specular, 1.0f);
}