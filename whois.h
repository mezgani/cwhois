#ifndef WHOIS_H
#define WHOIS_H
#include "ui_whois.h"
#include "client.h"
#include "sockets.h"
#include <QSystemTrayIcon>

class QAction;
class QMenu;



class Whois:public QWidget, private Ui::whoisForm 
{
Q_OBJECT
public:
	Whois(QWidget* parent=0);
	void setVisible(bool visible);

protected:
        void closeEvent(QCloseEvent *event);


public slots:
	void doWhois();
	void about();
	void aboutdialog();
	void affich();
        bool doSaveFile();
        void doSave();
        void doClear();
        void doQuit();
        void setIcon();
        void iconActivated(QSystemTrayIcon::ActivationReason reason);
        void showMessage();

private:
        void createActions();
        void createTrayIcon();

        QAction *minimizeAction;
        QAction *maximizeAction;
        QAction *restoreAction;
        QAction *aboutAction;
        QAction *quitAction;

        QSystemTrayIcon *trayIcon;
        QMenu *trayIconMenu;



};
#endif
