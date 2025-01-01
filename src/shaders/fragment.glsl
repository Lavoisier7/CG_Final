#version 330 core
out vec4 FragColor; // �����ɫ

in vec3 l;	// ��Դ���� 
in vec3 n;	// ������
in vec3 v;	// ���߷���

uniform vec3 objectColor;	// ������ɫ

void main()
{
    // ������ La��������� Ld�����淴��� Ls���˴���ʹ�ð׹�
	vec3 La, Ld, Ls;
	La = vec3(1.0f, 1.0f, 1.0f);
	Ld = vec3(1.0f, 1.0f, 1.0f);
	Ls = vec3(1.0f, 1.0f, 1.0f);
	// ������ϵ��
	float Ka = 0.2f, Kd = 0.2f, Ks = 16.0f;
	// �߹�ϵ��
	float shininess = 5.0f;

	// ���㻷�������
	vec3 Ia = Ka * La;
	// ��������������
	vec3 Id = Kd * Ld * (dot(l, n));
	// ���㾵�淴������
	vec3 r = 2.0f * (dot(l, n)) * n - l;	// ���䶨�ɹ�ʽ
	vec3 Is = Ks * Ls * pow(max(dot(r, v), 0.0f), shininess);

    // ������ɫ
    vec3 result = (Ia + Id + Is) * objectColor;
    FragColor = vec4(result, 1.0);
}