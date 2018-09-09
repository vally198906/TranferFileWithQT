#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLayout>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ui->mainToolBar->setVisible(false);
    this->ui->menuBar->setVisible(false);
//user Info
    userInfoGroupBox = new QGroupBox("User Info",this);
    userNameLabel = new QLabel(userInfoGroupBox);
    userNameLabel->setText("User Name:");

    userNameLineEdit = new QLineEdit(userInfoGroupBox);

    IPAddrLabel = new QLabel(this);
    IPAddrLabel->setText("IP Address:");
    IpAddrLineEdit = new IPLineEdit(userInfoGroupBox);


    QGridLayout *userGrid = new QGridLayout(userInfoGroupBox);
    userGrid->addWidget(userNameLabel,0,0);
    userGrid->addWidget(userNameLineEdit,0,1);
    userGrid->addWidget(IPAddrLabel,1,0);
    userGrid->addWidget(IpAddrLineEdit,1,1);
    userInfoGroupBox->setLayout(userGrid);
    userInfoGroupBox->setGeometry(500,10,400,250);

//file info
    fileInfoGroupBox = new QGroupBox("File Info",this);
    fileTabView = new QTableView(this);
    addFileButton = new QPushButton("Add File",this);
    removeFileButton = new QPushButton("Remove File",fileInfoGroupBox);
    transferFileButton = new QPushButton("Transfer File",fileInfoGroupBox);

    model = new QStandardItemModel();
    model->setColumnCount(1);
    model->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit(("File Name")));
    //model->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit(("Modify Time")));
    fileTabView->setModel(model);
    fileTabView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    fileTabView->setColumnWidth(0,350);
    connect(addFileButton,SIGNAL(clicked()),this,SLOT(AddFiles()));
    connect(fileTabView,SIGNAL(clicked(QModelIndex)),this,SLOT(SetCurModelIndex(QModelIndex)));
    connect(removeFileButton,SIGNAL(clicked()),this,SLOT(RemoveFiles()));
    //fileTabView->setColumnWidth(0,120);


    QGridLayout *fileLayout = new QGridLayout(fileInfoGroupBox);
    fileLayout->addWidget(fileTabView,0,0,10,5);
    fileLayout->addWidget(addFileButton,10,0,1,1);
    fileLayout->addWidget(removeFileButton,10,2,1,1);
    fileLayout->addWidget(transferFileButton,10,4,1,1);
    fileInfoGroupBox->setLayout(fileLayout);
    fileInfoGroupBox->setGeometry(500,270,400,450);

    QVBoxLayout *vBoxLayout = new QVBoxLayout(this);
    vBoxLayout->addLayout(userGrid);
    vBoxLayout->addLayout(fileLayout);
    this->setLayout(vBoxLayout);

    //Text Edit
    textEdit = new QTextEdit(this);
    textEdit->setGeometry(10,10,450,710);
    textEdit->setEnabled(false);

    proccessBar = new QProgressBar(this);
    proccessBar->setGeometry(10,750,780,20);
    proccessBar->setMinimum(0);  // 最小值
    proccessBar->setMaximum(4800);  // 最大值
    proccessBar->setValue(2000);  // 当前进度
    double dProgress = (proccessBar->value() - proccessBar->minimum()) * 100.0
                    / (proccessBar->maximum() - proccessBar->minimum());
    //proccessBar->setFormat(QString::fromLocal8Bit("Now the percent is：%1").arg(QString::number(dProgress, 'f', 1)));
    proccessBar->setAlignment(Qt::AlignRight | Qt::AlignVCenter);  // 对齐方式
}

void MainWindow::AddFiles()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(tr("选择传输的文件"));
    fileDialog->setDirectory(".");
    fileDialog->setFileMode(QFileDialog::ExistingFiles);

    //FileList.clear();
    QStringList ToAddFiles;
    if(fileDialog->exec())
    {
        ToAddFiles = fileDialog->selectedFiles();
    }
    for(int i = 0;i < ToAddFiles.size();i++)
    {
        bool existFile = false;
        for(int j = 0;j < FileList.size();j++)
        {
            if(FileList[j] == ToAddFiles[i])
            {
                existFile = true;
                break;
            }
        }
        if(existFile == false)
        {
            FileList.push_back(ToAddFiles[i]);
            QString pureFileName = ToAddFiles[i].right(ToAddFiles[i].size() - ToAddFiles[i].lastIndexOf('/') - 1);
            QStandardItem *curItem = new QStandardItem(pureFileName);
            model->setItem(FileList.size() - 1,0,curItem);
            curItem->setToolTip(ToAddFiles[i]);
            //model->item(model->rowCount(),0)->setTextAlignment(Qt::AlignCenter);
        }
    }
}

void MainWindow::SetCurModelIndex(QModelIndex index)
{
    this->curIndex = index;
}

void MainWindow::RemoveFiles(void)
{
    qDebug() << "curIndex:" << curIndex.row();
    int row = curIndex.row();
    if((row >= 0) && (row < FileList.size()))
    {
        model->removeRow(row);
        FileList.removeAt(row);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}
