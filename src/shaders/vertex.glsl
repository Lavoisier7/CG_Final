#version 330 core
in vec3 pos;       // 顶点位置
in vec3 normal;    // 顶点法线

out vec3 l;	// 光源向量 
out vec3 n;	// 法向量
out vec3 v;	// 视线方向

uniform mat4 viewMatrix;    // 视图矩阵
uniform mat4 projMatrix;    // 投影矩阵
uniform vec3 lightPosition; // 光源位置
uniform vec3 camPosition;   // 相机位置

void main()
{
    // 确定在实际视角中的点
    vec4 pos_mv = viewMatrix * vec4(pos, 1.0f);
    gl_Position = projMatrix * pos_mv;
    // 计算各个向量
    vec4 lightPositionExtended = vec4(lightPosition, 1.0f);
    vec4 lEx = lightPositionExtended - pos_mv;
    l = normalize(vec3(lEx[0], lEx[1], lEx[2]));
    mat3 normalMatrix = mat3(viewMatrix);
    n = normalize(normalMatrix * normal);
    v = normalize(vec3(vec4(camPosition,1.0) - pos_mv));
}