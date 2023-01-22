#include "bludiste.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Bludiste w;
    w.show();
    return a.exec();
}
