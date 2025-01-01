#include "myglwidget.h"
#include <GL/glew.h>
#include <algorithm>

MyGLWidget::MyGLWidget(QWidget *parent)
	:QOpenGLWidget(parent)
{
}

MyGLWidget::~MyGLWidget()
{
}

void MyGLWidget::initializeGL()
{
	initializeOpenGLFunctions();
	WindowSizeW = width();
	WindowSizeH = height();
	glViewport(0, 0, WindowSizeW, WindowSizeH);
	camera.setPerspective(45.0f, WindowSizeW / (float)WindowSizeH, 0.1f, 5000.0f);

	teapot.init();
	teapot.loadModel("./objs/teapot_8000.obj");
	teapot.setImage("./objs/brown.png");

}

// WASD 控制移动，空格上升，Z 下降
void MyGLWidget::keyPressEvent(QKeyEvent* e) {
	switch (e->key()) {
	case Qt::Key_W: camera.moveForward(); break;
	case Qt::Key_S: camera.moveBackward(); break;
	case Qt::Key_A: camera.moveLeft(); break;
	case Qt::Key_D: camera.moveRight(); break;
	case Qt::Key_Space: camera.moveUp(); break;
	case Qt::Key_Z: camera.moveDown(); break;
	}
	update();
}

// 按下左键并移动鼠标以移动视角
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
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glShadeModel(GL_SMOOTH); // 启用平滑着色

	// 公用绘制参数
	vec3 l = vec3(0, 1000, 0);
	QVector3D lightPosition(l[0], l[1], l[2]);
	QVector3D camPosition = camera.getPosition();
	QMatrix4x4 viewMatrix = camera.getViewMatrix();
	QMatrix4x4 projMatrix = camera.getProjectionMatrix();
	
	// 绘制模型
	teapot.draw(lightPosition, camPosition, viewMatrix, projMatrix);
}


void MyGLWidget::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
	WindowSizeW = w;
	WindowSizeH = h;
}


