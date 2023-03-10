#include "note.h"

Note::Note()
{
    text = "";
    img = QPixmap(0,0);
  //  qDebug() << "Note::Note() Created img" << img.size();
}


Note::Note(QString text, QPixmap img)
{
    this->text = text;
    this->img = img;
}


void Note::addText(QString text)
{
    this->text = text;
}


QString Note::getText() const
{
    return this->text;
}

QPixmap Note::getImg() const
{
    return this->img;
}

void Note::setImg(const QPixmap &value, QString filename)
{
    text = filename;
    img = value;
//    qDebug() << "pixSize " << img.size();
}

void Note::clrImg()
{
   QPixmap tmp(QPixmap(0,0));
   img.swap(tmp);
}

//unsigned int Note::getKey() const
//{
//    return key;
//}

//void Note::setKey(unsigned int value)
//{
//    key = value;
//}


QDataStream &operator<<(QDataStream &d,const Note &n)
{
    d << n.getText();
    return d;
}

QDataStream &operator>>(QDataStream &d, Note &n)
{
    d >> n.text;
    return d;
}

