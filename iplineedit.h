#ifndef IPLINEEDIT_H
#define IPLINEEDIT_H
#include <QLineEdit>

class ChildIPLineClient:public QLineEdit
{
    Q_OBJECT
public:
    ChildIPLineClient(QWidget *parent);
    ~ChildIPLineClient();
};

class IPLineEdit:public QLineEdit
{
    Q_OBJECT
public:
    IPLineEdit(QWidget *parent);
    ~IPLineEdit();
    void setGeometry(int x,int y,int w,int h);
    bool SetIPText(int nIP,int nIdex);
    QString getIPText(void);
    int getIndex(ChildIPLineClient *lineEdit);

    bool eventFilter(QObject *obj, QEvent *ev);

private:
    void paintEvent(QPaintEvent *e);

private:
    ChildIPLineClient *m_ChildIPLineEdit[4];
};
#endif // IPLINEEDIT_H
