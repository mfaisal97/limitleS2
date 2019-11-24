#ifndef ADDIMAGEWINDOW_H
#define ADDIMAGEWINDOW_H

#include <QDialog>
#include <vector>
#include <string>
#include <QTableWidgetItem>
#include "../../header/Peer.h"

namespace Ui {
class AddImageWindow;
}

class AddImageWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddImageWindow(QWidget *parent = nullptr);
    void setPeer(Peer* _peer){
        peer = _peer;
    }
    ~AddImageWindow();

private slots:
    void on_pushButton_2_clicked();

private:
    Peer* peer;
    Ui::AddImageWindow *ui;
    std::vector<std::vector<std::string>> pickedUsers;
};

#endif // ADDIMAGEWINDOW_H
