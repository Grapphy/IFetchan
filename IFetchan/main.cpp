#include "ifetchan.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    IFetchan w;
    w.setAttribute(Qt::WA_TranslucentBackground, true);
    w.show();
    return a.exec();
}
