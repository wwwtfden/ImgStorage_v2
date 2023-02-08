#include "qexlabel.h"

void QExLabel::mouseReleaseEvent(QMouseEvent *e)
{
        if (e->button() == Qt::RightButton) {
            emit clicked((QObject*)this);
 //           qDebug() << "img rightbutton clicked";
            pmnu = new QMenu(this);
            QString nameOfDeleteAction = "&Delete Image ";
            QString nameOfExportAction = "&Export Image ";
            pmnu->addAction(nameOfDeleteAction);
            pmnu->addAction(nameOfExportAction);
            connect(pmnu, SIGNAL(triggered(QAction*)), SLOT(slotMenuActivated(QAction*)));
            pmnu->exec(e->globalPos());
            //emit imgDeleted();
            
        }
}

void QExLabel::mouseDoubleClickEvent(QMouseEvent *e)
{
    emit imgDClicked();
}

void QExLabel::slotMenuActivated(QAction *pAction)
{
    if (pAction->text() == "&Export Image "){
       // qDebug() << pAction->text();
        emit imgExported();
    } else if (pAction->text() == "&Delete Image "){
        emit imgDeleted();
    }
}
