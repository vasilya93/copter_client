#include <QPainter>
#include <math.h>
#include "orientationindicator.h"

OrientationIndicator::OrientationIndicator(QWidget *parent) :
    QWidget(parent),
    m_fOrientation(0)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

/*virtual */QSize OrientationIndicator::sizeHint() const
{
    return QSize(150, 150);
}

/*virtual */void OrientationIndicator::paintEvent(QPaintEvent* paintEvent)
{
    (void) paintEvent;
    QPainter painter(this);
    int nHeight = height(),
        nWidth = width();
    QPoint pointCenter = rect().center();
    int nCenterX = pointCenter.x(),
        nCenterY = pointCenter.y();

    int nRadius = (nHeight > nWidth ? nWidth : nHeight) / 2 - 5;
    painter.drawEllipse(nCenterX - nRadius,
                        nCenterY - nRadius,
                        nRadius * 2,
                        nRadius * 2);
    painter.drawLine(nCenterX + nRadius * cos(m_fOrientation),
                     nCenterY - nRadius * sin(m_fOrientation),
                     nCenterX - nRadius * cos(m_fOrientation),
                     nCenterY + nRadius * sin(m_fOrientation));


}
