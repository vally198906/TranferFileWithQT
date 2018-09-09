#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "iplineedit.h"
#include <QGroupBox>
#include <QLabel>
#include <QTableView>
#include <QPushButton>
#include <QProgressBar>
#include <QTextEdit>
#include <QStringList>
#include <QStandardItemModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void AddFiles(void);
    void RemoveFiles(void);
    void SetCurModelIndex(QModelIndex);

private:
    Ui::MainWindow *ui;

    //User info group box
    QGroupBox *userInfoGroupBox;
    QLabel *userNameLabel;
    QLineEdit *userNameLineEdit;
    QLabel *IPAddrLabel;
    IPLineEdit *IpAddrLineEdit;

    //File info group box
    QGroupBox *fileInfoGroupBox;
    QStandardItemModel  *model;
    QTableView *fileTabView;
    QPushButton *addFileButton;
    QPushButton *removeFileButton;
    QPushButton *transferFileButton;

    QStringList FileList;
    QModelIndex curIndex;


    //Process bar
    QProgressBar *proccessBar;

    //TextEdit
    QTextEdit *textEdit;

};

#endif // MAINWINDOW_H
