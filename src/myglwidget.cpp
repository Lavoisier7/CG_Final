#include "myglwidget.h"
#include <GL/glew.h>
#include <algorithm>

MyGLWidget::MyGLWidget(QWidget *parent)
	:QOpenGLWidget(parent)
{
}

MyGLWidget::~MyGLWidget()
{
	makeCurrent();
	VAO.destroy();
	VBO.destroy();
	doneCurrent();
}


void MyGLWidget::initializeGL()
{
	initializeOpenGLFunctions();
	WindowSizeW = width();
	WindowSizeH = height();
	glViewport(0, 0, WindowSizeW, WindowSizeH);
	camera.setPerspective(45.0f, width() / (float)height(), 0.1f, 1000.0f);
	VAO.create();
	VBO.create();
	// 开启深度测试
	glEnable(GL_DEPTH_TEST);
	// 编译和链接着色器
	if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "./shaders/vertex.glsl")) {
		qDebug() << "Vertex shader compile error:" << shaderProgram.log();
		return;
	}
	if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "./shaders/fragment.glsl")) {
		qDebug() << "Fragment shader compile error:" << shaderProgram.log();
		return;
	}
	if (!shaderProgram.link()) {
		qDebug() << "Shader program link error:" << shaderProgram.log();
		return;
	}

	// 选择要加载的model
	//objModel.loadModel("./objs/teapot_600.obj");
	//objModel.loadModel("./objs/teapot_8000.obj");
	//objModel.loadModel("./objs/rock.obj");
	objModel.loadModel("./objs/cube.obj");
	//objModel.loadModel("./objs/singleTriangle.obj");

	// 申请内存储存三角形三个顶点的位置和法向量
	// 每个三角形有三个顶点，每个顶点有一个位置坐标和一个法向量，均使用三维向量表示
	// 故一个三角形中共有 3 * (3 + 3) = 18 个数
	int totalVertexCount = objModel.triangleCount * 18;
	GLfloat* Vertices = new GLfloat[totalVertexCount];
	for (int i = 0; i < objModel.triangleCount; i++) {
		Triangle nowTriangle = objModel.getTriangleByID(i);
		//第一个顶点
		Vertices[i * 18 + 0] = nowTriangle.triangleVertices[0][0];
		Vertices[i * 18 + 1] = nowTriangle.triangleVertices[0][1];
		Vertices[i * 18 + 2] = nowTriangle.triangleVertices[0][2];
		Vertices[i * 18 + 3] = nowTriangle.triangleNormals[0][0];
		Vertices[i * 18 + 4] = nowTriangle.triangleNormals[0][1];
		Vertices[i * 18 + 5] = nowTriangle.triangleNormals[0][2];
		//第二个顶点
		Vertices[i * 18 + 6] = nowTriangle.triangleVertices[1][0];
		Vertices[i * 18 + 7] = nowTriangle.triangleVertices[1][1];
		Vertices[i * 18 + 8] = nowTriangle.triangleVertices[1][2];
		Vertices[i * 18 + 9] = nowTriangle.triangleNormals[1][0];
		Vertices[i * 18 + 10] = nowTriangle.triangleNormals[1][1];
		Vertices[i * 18 + 11] = nowTriangle.triangleNormals[1][2];
		//第三个顶点
		Vertices[i * 18 + 12] = nowTriangle.triangleVertices[2][0];
		Vertices[i * 18 + 13] = nowTriangle.triangleVertices[2][1];
		Vertices[i * 18 + 14] = nowTriangle.triangleVertices[2][2];
		Vertices[i * 18 + 15] = nowTriangle.triangleNormals[2][0];
		Vertices[i * 18 + 16] = nowTriangle.triangleNormals[2][1];
		Vertices[i * 18 + 17] = nowTriangle.triangleNormals[2][2];
	}

	// 将数据绑定到 VBO 上
	VAO.bind();
	VBO.bind();
	VBO.allocate(Vertices, sizeof(GLfloat) * totalVertexCount);
	// 指明数据的存取方式
	shaderProgram.bind();
	shaderProgram.setAttributeBuffer("pos", GL_FLOAT, 0, 3, sizeof(GLfloat) * 6);
	shaderProgram.enableAttributeArray("pos");
	shaderProgram.setAttributeBuffer("normal", GL_FLOAT, sizeof(GLfloat) * 3, 3, sizeof(GLfloat) * 6);
	shaderProgram.enableAttributeArray("normal");

	VBO.release();
	VAO.release();
	shaderProgram.release();
}

void MyGLWidget::keyPressEvent(QKeyEvent* e) {

	switch (e->key()) {
	case Qt::Key_W: camera.moveForward(); break;
	case Qt::Key_S: camera.moveBackward(); break;
	case Qt::Key_A: camera.moveLeft(); break;
	case Qt::Key_D: camera.moveRight(); break;
	case Qt::Key_Space: camera.moveUp(); break;
	case Qt::Key_Shift: camera.moveDown(); break;
	}
	update();
}

void MyGLWidget::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons() & Qt::LeftButton){
		QPoint delta = event->pos() - lastMousePos;
		camera.rotate(delta.x(), delta.y());
		lastMousePos = event->pos();
		update();
	}
}

void MyGLWidget::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton){
		lastMousePos = event->pos();
	}
}

void MyGLWidget::paintGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	vec3 l = objModel.centralPoint + vec3(0, 100, 100);
	QVector3D lightPosition(l[0], l[1], l[2]);
	QVector3D camPosition = camera.getPosition();
	QVector3D objectColor = QVector3D(0.0f, 0.0f, 1.0f);
	QMatrix4x4 viewMatrix = camera.getViewMatrix();
	QMatrix4x4 projMatrix = camera.getProjectionMatrix();

	// 设置 uniform 变量
	shaderProgram.bind();
	shaderProgram.setUniformValue("viewMatrix", viewMatrix);
	shaderProgram.setUniformValue("projMatrix", projMatrix);
	shaderProgram.setUniformValue("lightPosition", lightPosition);
	shaderProgram.setUniformValue("camPosition", camPosition);
	shaderProgram.setUniformValue("objectColor", objectColor);

	// 绘制物体
	VAO.bind();
	glDrawArrays(GL_TRIANGLES, 0, objModel.triangleCount * 3);

	shaderProgram.release();
	VAO.release();
}


void MyGLWidget::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
	WindowSizeW = w;
	WindowSizeH = h;
}


