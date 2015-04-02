#include <QApplication>
#include "diskswissknife.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DiskSwissKnife w;
    w.show();

    return a.exec();
}
