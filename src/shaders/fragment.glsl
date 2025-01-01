#version 330 core
out vec4 FragColor; // 输出颜色

in vec3 l;	// 光源向量 
in vec3 n;	// 法向量
in vec3 v;	// 视线方向

uniform vec3 objectColor;	// 物体颜色

void main()
{
    // 环境光 La、漫反射光 Ld、镜面反射光 Ls，此处均使用白光
	vec3 La, Ld, Ls;
	La = vec3(1.0f, 1.0f, 1.0f);
	Ld = vec3(1.0f, 1.0f, 1.0f);
	Ls = vec3(1.0f, 1.0f, 1.0f);
	// 各反射系数
	float Ka = 0.2f, Kd = 0.2f, Ks = 16.0f;
	// 高光系数
	float shininess = 5.0f;

	// 计算环境光分量
	vec3 Ia = Ka * La;
	// 计算漫反射光分量
	vec3 Id = Kd * Ld * (dot(l, n));
	// 计算镜面反射光分量
	vec3 r = 2.0f * (dot(l, n)) * n - l;	// 反射定律公式
	vec3 Is = Ks * Ls * pow(max(dot(r, v), 0.0f), shininess);

    // 最终颜色
    vec3 result = (Ia + Id + Is) * objectColor;
    FragColor = vec4(result, 1.0);
}