#include "Builder.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	Builder w;
	w.show();
	return a.exec();
}
