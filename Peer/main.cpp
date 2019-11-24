#include "../GUI/mainwidget/mainwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    Peer* peer = new Peer(PeerServerPort, ToCharArray(ServiceDirectoryAddr),ServiceDirectoryPort);
    QApplication a(argc, argv);
    MainWidget w;
    w.setPeer(peer);
    w.show();
    return a.exec();
}
