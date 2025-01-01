#include "myglwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyGLWidget w;
	w.setWindowTitle("Group Homework");
    w.show();
    return a.exec();
}
