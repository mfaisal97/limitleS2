#include "../GUI/mainwidget/mainwidget.h"
#include <QApplication>
#include <iostream>
#include <signal.h>

using namespace std;

Peer* peer = new Peer(PeerServerPort, ToCharArray(ServiceDirectoryAddr),ServiceDirectoryPort);

int main(int argc, char *argv[])
{
    auto pid = fork();

    if(pid > 0){

      // peer->SetAuthInfo("a", "a");
      // peer->RemoteSignUp();
      // peer->RemoteSignIn();
      // peer->RemoteConnectionInfoUpdate();
      peer->serveRequest();
      return 0;

    }
    else {
      //cout << "userInfo:  " << peer->getUserInfo();
      QApplication a(argc, argv);
      MainWidget w(nullptr,peer);
      //w.setPeer(peer);
      w.show();
      auto k = a.exec();

      kill(pid, SIGKILL);
      return k;
      }
}
