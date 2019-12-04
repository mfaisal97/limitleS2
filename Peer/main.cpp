#include "../GUI/mainwidget/mainwidget.h"
#include <QApplication>
#include <iostream>
using namespace std;
int main(int argc, char *argv[])
{
    Peer* peer = new Peer(PeerServerPort, ToCharArray(ServiceDirectoryAddr),ServiceDirectoryPort);

    // if(fork() == 0){
    //   peer->serveRequest();
    //   return 0;
    // }
    // else {
      //cout << "userInfo:  " << peer->getUserInfo();
      QApplication a(argc, argv);
      MainWidget w(nullptr,peer);
      //w.setPeer(peer);
      w.show();
      return a.exec();
      // }
}
