#ifndef ADDIMAGEWINDOW_H
#define ADDIMAGEWINDOW_H

#include <QDialog>
#include <vector>
#include <string>
#include <QTableWidgetItem>
#include "../../header/Peer.h"
#include "../../header/StegImage.h"

namespace Ui {
class AddImageWindow;
}

class AddImageWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddImageWindow(QWidget *parent = nullptr);
    void setPeer(Peer* _peer){
        //std::cout<<"Peer Passed\n";
        peer = _peer;
    }
    ~AddImageWindow();

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();

private:
    Peer* peer;
    Ui::AddImageWindow *ui;
    std::vector<std::vector<std::string>> pickedUsers;
    StegImage* imageToBeAdded;
};

#endif // ADDIMAGEWINDOW_H
