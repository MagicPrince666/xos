#include "widget.h"
#include <QApplication>
#include <QDesktopWidget>
 
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.adjustSize();
    w.move(QApplication::desktop()->screen()->rect().center() - w.rect().center());
    w.show();
 
    return a.exec();
}
