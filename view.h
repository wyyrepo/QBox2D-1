#ifndef VIEW_H
#define VIEW_H
#include <QGraphicsView>
#include <QMouseEvent>

class QBox2DView : public QGraphicsView
{
    Q_OBJECT
public:
    QBox2DView(QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

public slots:
    void zoomIn();
    void zoomOut();

signals:
    void mouseRightButtonPressed(const QPointF&);
    void mouseLeftButtonPressed(const QPointF&);
    void mouseRightButtonReleased();
    void mouseMoved(const QPointF&);
};

#endif // VIEW_H