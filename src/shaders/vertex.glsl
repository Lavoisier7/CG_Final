#version 330 core
in vec3 pos;       // ����λ��
in vec3 normal;    // ���㷨��
in vec2 inTex;     // ��������

out vec2 tex;   // ��������
out vec3 l;	// ��Դ���� 
out vec3 n;	// ������
out vec3 v;	// ���߷���

uniform mat4 viewMatrix;    // ��ͼ����
uniform mat4 projMatrix;    // ͶӰ����
uniform vec3 lightPosition; // ��Դλ��
uniform vec3 camPosition;   // ���λ��

void main()
{
    // ȷ����ʵ���ӽ��еĵ�
    vec4 pos_mv = viewMatrix * vec4(pos, 1.0f);
    gl_Position = projMatrix * pos_mv;
    // �����������
    vec4 lightPositionExtended = vec4(lightPosition, 1.0f);
    vec4 lEx = lightPositionExtended - pos_mv;
    l = normalize(vec3(lEx[0], lEx[1], lEx[2]));
    mat3 normalMatrix = mat3(viewMatrix);
    n = normalize(normalMatrix * normal);
    v = normalize(vec3(vec4(camPosition,1.0) - pos_mv));
    tex = inTex;
}