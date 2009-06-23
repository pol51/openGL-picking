#ifndef __GLWIDGET_H
#define __GLWIDGET_H

#include <QGLWidget>
#include <QTimer>

class Cube;

class GlWidget : public QGLWidget
{
  Q_OBJECT

  protected:
    QTimer _refreshTimer;
    QVector<Cube*> _cubes;
    int _selection;

  public:
    GlWidget(QWidget *parent = NULL);
    virtual ~GlWidget();

    QSize sizeHint() const;

  protected:
    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL(int width, int height);
    virtual void paintEvent(QPaintEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void pick(const QPoint &pos);
    virtual void processHits(GLint hits, GLuint buffer[]);
    virtual void draw();

};

#endif
