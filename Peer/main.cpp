#include "../GUI/mainwidget/mainwidget.h"
#include <QApplication>
#include <iostream>
using namespace std;    
int main(int argc, char *argv[])
{
    Peer* peer = new Peer(PeerServerPort, ToCharArray(ServiceDirectoryAddr),ServiceDirectoryPort);
    cout << "userInfo:  " << peer->getUserInfo();
    QApplication a(argc, argv);
    MainWidget w;
    //w.setPeer(peer);
    w.show();
    return a.exec();
}
