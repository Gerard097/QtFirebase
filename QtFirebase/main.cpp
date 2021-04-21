#include "QtFirebase.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtFirebase w;
    w.show();
    return a.exec();
}
