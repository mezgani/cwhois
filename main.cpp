//#include <qapplication.h>
#include <ctime>
#include "whois.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QtGui>

void center(QWidget &widget)
{
  int x, y;
  int screenWidth;
  int screenHeight;
  int width, height;
  QSize windowSize;

  QDesktopWidget *desktop = QApplication::desktop();

  width = widget.frameGeometry().width();
  height = widget.frameGeometry().height();  

  screenWidth = desktop->width();
  screenHeight = desktop->height();
 
  x = (screenWidth - width) / 2;
  y = (screenHeight - height) / 2;

  widget.move( x, y );
}



int main(int argc, char *argv[])
{
        QApplication app(argc, argv);
        if (!QSystemTrayIcon::isSystemTrayAvailable()) {
         QMessageBox::critical(0, QObject::tr("Systray"),
                               QObject::tr("I couldn't detect any system tray "
                                           "on this system."));
         return 1;
        }

        Whois widget;
        widget.setWindowIcon(QIcon("web.png"));
        widget.show();
	center(widget);
        return app.exec();
}
