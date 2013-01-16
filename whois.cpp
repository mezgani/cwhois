#include <QtGui>
#include <iostream>
#include <ctime>
#include <string>
#include <string.h>
#include "sockets.h"
#include "whois.h"
#include "client.h"
#include "exception.h"
#include <QTextStream>
#include <QFile>


Whois::Whois(QWidget* parent):QWidget(parent)
{
setupUi(this);
createActions();
createTrayIcon();
setIcon();
trayIcon->show();

connect(whoisButton, SIGNAL(clicked()), this, SLOT(doWhois()));
connect(clearButton, SIGNAL(clicked()), this, SLOT(doClear()));
connect(saveButton,  SIGNAL(clicked()), this, SLOT(doSave()));
connect(quitButton,  SIGNAL(clicked()), this, SLOT(doQuit()));

connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), 
	this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

}



void Whois::setVisible(bool visible)
{
     minimizeAction->setEnabled(visible);
     maximizeAction->setEnabled(!visible);
     restoreAction->setEnabled(!visible);
     QWidget::setVisible(visible);
}

void Whois::closeEvent(QCloseEvent *event)
{
     if (trayIcon->isVisible()) {
         showMessage();
         hide();
         event->ignore();
     }
}

void Whois::setIcon()
 {
     QIcon icon;
     icon.addFile("/usr/share/pixmaps/cwhois.png", QSize(16,16));
     trayIcon->setIcon(icon);
     setWindowIcon(icon);

}

void Whois::iconActivated(QSystemTrayIcon::ActivationReason reason)
 {
     switch (reason) {
     case QSystemTrayIcon::Trigger:
     case QSystemTrayIcon::DoubleClick:
           this->show();
           break;
     case QSystemTrayIcon::MiddleClick:
           showMessage();
           break;
     default:
         ;
     }
 }


void Whois::showMessage()
{
     QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(1);
     trayIcon->showMessage("cwhois", "The program will keep running.\n"
				      "To terminate the program choose quit in the systray menu.", icon, 2000);
}

void Whois::about()
{
     QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(0);
     trayIcon->showMessage("About cwhois",     
                "The cwhois app was coded by MEZGANI Ali <handrix@users.sourceforge.net> \n"
                "This application is licenced under General Public Licence.\n",
                icon, 2000);
}


void Whois::createActions()
{
     minimizeAction = new QAction(tr("Mi&nimize"), this);
     connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

     maximizeAction = new QAction(tr("Ma&ximize"), this);
     connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

     restoreAction = new QAction(tr("&Restore"), this);
     connect(restoreAction, SIGNAL(triggered()), this, SLOT(show()));

     aboutAction = new QAction(tr("&About"), this);
     connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

     quitAction = new QAction(tr("&Quit"), this);
     connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void Whois::createTrayIcon()
{
     trayIconMenu = new QMenu(this);
     trayIconMenu->addAction(minimizeAction);
     trayIconMenu->addAction(maximizeAction);
     trayIconMenu->addAction(restoreAction);
     trayIconMenu->addSeparator();
     trayIconMenu->addAction(aboutAction);
     trayIconMenu->addSeparator();
     
     trayIconMenu->addAction(quitAction);

     trayIcon = new QSystemTrayIcon(this);
     trayIcon->setContextMenu(trayIconMenu);
}



void Whois::doWhois() 
{

std::string reply, banner;
QString myaddress=input->text().trimmed();
QString myserver=serverBox->currentText().trimmed();

if ((! myaddress.isEmpty()) && (! myserver.isEmpty()))  {


	//Converting QString to string, first convert it to the right encoding:
	QByteArray enca=myaddress.toUtf8(); //or toLocal8Bit or whatever
	QByteArray encs=myserver.toUtf8(); //or toLocal8Bit or whatever

	//then allocate enough memory:
	char *address=new char[enca.size()+1];
	char *server=new char[encs.size()+1];

	//and copy it
	strcpy(address,enca.data());
	strcpy(server,encs.data());

	ClientSocket client_socket = ClientSocket(server, 43, 4);

	try
		{
	      	if (ipv4Button->isChecked())
		      	ClientSocket client_socket = ClientSocket(server, 43, 4);
		else ClientSocket client_socket = ClientSocket(server, 43, 6);
		try {
			output->clear();
	       	        client_socket << address;
        	    	client_socket << "\r\n";
               		do {
                       		client_socket >> reply;
		               	output->insertPlainText(QString(reply.c_str()));
				} while (reply != "" );
       		}
	
		catch ( SocketException& e ) {}
		}
	catch ( SocketException& e )
	{

	     QMessageBox::information(this,"warning",
               		"Exception the whois server is unreachable\n"
               		"maybe you have to check the network connectivity\n");

	     //std::cout << "Exception was caught:" << e.description() << "\n";
	}	

	}
else {
    		QMessageBox::information(this,"warning",
               		"Please insert a correct value. \n");
	}	

}

void Whois::affich()
{
	QString server, domain;
	server=serverBox->currentText();
	domain=input->text();
	output->setText(domain+" "+server);
	if (ipv4Button->isChecked())
		output->append("IPv4");
	if (ipv6Button->isChecked())
		output->append("IPv6");
}


void Whois::doClear(){
        input->clear();
        output->clear();
    }

void Whois::doQuit(){
	QMessageBox::StandardButton button;
        button = QMessageBox::question(this, tr("Quit cWhois"),
                                       QString(tr("Are you sure you want to " \
                                                  "quit the application?")),
                                      QMessageBox::Yes | QMessageBox::No);

        if (button == QMessageBox::Yes) {
            qApp->quit();
        }
        else if (button == QMessageBox::No) {
            hide();
        }
}

void Whois::aboutdialog() 
{
	QMessageBox::about(this,"About cwhois",
                "This cwhois app was coded by MEZGANI Ali\n"
                "This application is licenced under General Public Licence.\n"
                "handrix@users.sourceforge.net.\n");
}
bool Whois::doSaveFile()
{
    QString data = output->toPlainText().trimmed();

    if (data.isEmpty()) {
	return false;
	}
     
    QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty())
        return false;

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << output->toPlainText();
    QApplication::restoreOverrideCursor();
    return true;
}




void Whois::doSave()
{
if (doSaveFile()){
    QMessageBox::information(this,"information",
                                "Information: File saved \n");
    }
}



