#ifndef ORIENTATIONINDICATOR_H
#define ORIENTATIONINDICATOR_H

#include <QWidget>

class OrientationIndicator : public QWidget
{
    Q_OBJECT
public:
    explicit OrientationIndicator(QWidget *parent = 0);

    virtual QSize sizeHint() const;

    inline double GetOrientation()
    {
        return m_fOrientation;
    }
    inline void SetOrientation(double fNewOrientation)
    {
        m_fOrientation = fNewOrientation;
        update();
    }

signals:

public slots:

protected:
    virtual void paintEvent(QPaintEvent*);

private:
    double m_fOrientation;
};

#endif // ORIENTATIONINDICATOR_H
