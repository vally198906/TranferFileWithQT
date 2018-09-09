#include "iplineedit.h"
#include <QPainter>
#include <QPen>
#include <QRegExp>
#include <QValidator>
#include <QDebug>
#include <QMessageBox>
#include <QLayout>
#include <QEvent>
#include <QKeyEvent>

ChildIPLineClient::ChildIPLineClient(QWidget *parent):QLineEdit (parent)
{
    setAlignment(Qt::AlignHCenter);
    setMaxLength(3);
    setMinimumSize(25,20);

    QRegExp rx("(2[0-5]{2}|2[0-4][0-9]|1?[0-9]{1,2})");

    setValidator(new QRegExpValidator(rx, this));
}

ChildIPLineClient::~ChildIPLineClient()
{

}

IPLineEdit::IPLineEdit(QWidget *parent):QLineEdit(parent)
{
    int nWidth = width() - 2 - 4 * 3;
    int averageWidth = (nWidth) / 4;
    QHBoxLayout *pHBox = new QHBoxLayout(this);
    pHBox->setSpacing(10);
    pHBox->setContentsMargins(0, 0, 0, 0);
    this->setFrame(false);
    for (int i = 0; i < 4; i++)
    {
        m_ChildIPLineEdit[i] = new ChildIPLineClient(this);
        m_ChildIPLineEdit[i]->setFrame(false);
        m_ChildIPLineEdit[i]->setMaxLength(3);
        m_ChildIPLineEdit[i]->setAlignment(Qt::AlignCenter);
        m_ChildIPLineEdit[i]->installEventFilter(this);
        //m_ChildIPLineEdit[i]->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        pHBox->addWidget(m_ChildIPLineEdit[i]);
    }
    this->setReadOnly(true);
}

IPLineEdit::~IPLineEdit()
{

}

void IPLineEdit::paintEvent(QPaintEvent *e)
{
    QLineEdit::paintEvent(e);
    QPainter painter(this);
    QBrush brush;
    brush.setStyle(Qt::BrushStyle::SolidPattern);
    brush.setColor(Qt::black);
    painter.setBrush(brush);

    int width = 0;

    for (int i = 0; i < 3; i++)

    {
        width += m_ChildIPLineEdit[i]->width() + (i == 0 ? 3 : 10);//布局的间隔
        painter.drawEllipse(width, height() / 2 , 4, 4);
    }
}

void IPLineEdit::setGeometry(int x, int y, int w, int h)
{
    QLineEdit::setGeometry(x,y,w,h);

    /*int nWidth = width() - 2 - 4 * 3;
    int averageWidth = (nWidth) / 4;

    for(int i = 0;i < 4;i++)
    {
        m_ChildIPLineEdit[i] = new ChildIPLineClient(this);
        m_ChildIPLineEdit[i]->setFrame(false);
        m_ChildIPLineEdit[i]->setGeometry(i * 4 + 1 + (i)*averageWidth + 1,1,averageWidth - 1 ,h - 2);
    }*/
}

int IPLineEdit::getIndex(ChildIPLineClient *lineEdit)
{
    int idx = -1;
    for(int i =0;i < 4;i++)
    {
        if(lineEdit == m_ChildIPLineEdit[i])
            idx = i;
    }
    return idx;
}

bool IPLineEdit::eventFilter(QObject *obj, QEvent *ev)
{
    if(QEvent::KeyPress == ev->type())
    {
        QKeyEvent *keyEvent = dynamic_cast<QKeyEvent*>(ev);
        ChildIPLineClient *lineEdit = qobject_cast<ChildIPLineClient*>(obj);
        switch(keyEvent->key())
        {
        case Qt::Key_0:
        case Qt::Key_1:
        case Qt::Key_2:
        case Qt::Key_3:
        case Qt::Key_4:
        case Qt::Key_5:
        case Qt::Key_6:
        case Qt::Key_7:
        case Qt::Key_8:
        case Qt::Key_9:
        {
            QString strText = lineEdit->text();
            if (lineEdit->selectedText().length())
            {
                lineEdit->text().replace(lineEdit->selectedText(), QChar(keyEvent->key()));
            }
            else if (strText.length() == 3 || strText.length() < 3 && ((strText.toInt() * 10 > 255) || (strText.toInt() * 10 +  (int)(keyEvent->key() - Qt::Key_0)> 255)))
            {
                int index = getIndex(lineEdit);
                if (index != -1 && index != 3)
                {
                    m_ChildIPLineEdit[index + 1]->setFocus();
                    m_ChildIPLineEdit[index + 1]->selectAll();
                }
            }
            else if (strText.length() == 2 && strText.toInt() * 10 < 255)
            {
                if (Qt::Key_0 == keyEvent->key() && strText.toInt())
                {
                    lineEdit->setText(strText.insert(lineEdit->cursorPosition(), QChar(Qt::Key_0)));
                }
            }
            return QLineEdit::eventFilter(obj, ev);
        }
        break;
            case Qt::Key_Backspace:
            {
                QString str = lineEdit->text();
                if(!str.length() || (str.length() && !lineEdit->cursorPosition()))
                {
                    int index = getIndex(lineEdit);
                    if(index != -1 && index != 0)
                    {
                        m_ChildIPLineEdit[index - 1]->setFocus();
                        int length = m_ChildIPLineEdit[index - 1]->text().length();
                        m_ChildIPLineEdit[index - 1]->setCursorPosition(length);
                    }
                }
                return QLineEdit::eventFilter(obj,ev);
            }
            case Qt::Key_Left:
            {
                if (!lineEdit->cursorPosition())
                {

                    int index = getIndex(lineEdit);
                    if (index != -1 && index != 0)
                    {
                        m_ChildIPLineEdit[index - 1]->setFocus();
                        int length = m_ChildIPLineEdit[index - 1]->text().length();
                        m_ChildIPLineEdit[index - 1]->setCursorPosition(length ? length : 0);
                    }
                }
                return QLineEdit::eventFilter(obj,ev);
            }
            case Qt::Key_Right:
            {
                if (lineEdit->cursorPosition() == lineEdit->text().length())
                {
                    int index = getIndex(lineEdit);
                    if (index != -1 && index != 3)
                    {
                        m_ChildIPLineEdit[index + 1]->setFocus();
                        m_ChildIPLineEdit[index + 1]->setCursorPosition(0);
                    }
                }
                return QLineEdit::eventFilter(obj,ev);
            }
            case Qt::Key_Tab:
            {
                int index = getIndex(lineEdit);
                if(index != -1)
                {
                    m_ChildIPLineEdit[(index) % 4]->setFocus();
                    m_ChildIPLineEdit[(index) % 4]->setCursorPosition(0);
                }
                return QLineEdit::eventFilter(obj,ev);
            }
            default:
                return QLineEdit::eventFilter(obj,ev);
        }
    }
}

QString IPLineEdit::getIPText(void)
{
    QString str = "";
    for(int i = 0;i < 4;i++)
    {
        if(m_ChildIPLineEdit[i]->text() == "")
        {
             //QMessageBox::information(this, "Warning", "Invaid IP", QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);
            return "";
        }
        else
        {
            str += m_ChildIPLineEdit[i]->text();
            if(i != 3)
            {
                str += ".";
            }
        }
    }
    return str;
}


















