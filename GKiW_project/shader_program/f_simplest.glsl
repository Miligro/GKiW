#version 330

uniform sampler2D textureMap0;

out vec4 pixelColor;

in vec4 n;
in vec4 l1;
in vec4 l2;
in vec4 v;
in vec2 iTexCoord0;

void main(void) {

	vec4 texColor = texture(textureMap0,iTexCoord0);
	vec4 mn = normalize(n);
	vec4 mv = normalize(v);

	vec4 ml1 = normalize(l1);
	vec4 mr1 = reflect(-ml1,mn);
	float nl1 = clamp(dot(mn,ml1),0,1);
	float rv1 = pow(clamp(dot(mr1,mv),0,1),30);

	vec4 ml2 = normalize(l2);
	vec4 mr2 = reflect(-ml2,mn);
	float nl2 = clamp(dot(mn,ml2),0,1);
	float rv2 = pow(clamp(dot(mr2,mv),0,1),30);

	vec4 kd = texColor;
	vec4 ks = vec4(1,1,1,1);

	pixelColor = vec4(kd.rgb * nl1, kd.a) + vec4(ks.rgb*rv1, 0);
	pixelColor += vec4(kd.rgb * nl2, kd.a) + vec4(ks.rgb*rv2, 0);
}