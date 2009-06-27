#include "glWidget.h"

#include <QtOpenGL>
#include <QDebug>

#include <cube.h>
#include <rotationSelector.h>

GlWidget::GlWidget(QWidget *parent)
  :QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
  _selection(-1), _rx(0), _ry(0), _rz(0)
{
  _refreshTimer.setSingleShot(false);
  QObject::connect(
    &_refreshTimer, SIGNAL(timeout()),
    this, SLOT(update())
  );
  _refreshTimer.start(20);

  _cubes.append(new Cube(-.5f,  .5f, 0.f, this));
  _cubes.last()->setColor(0xff7f7fff);
  _cubes.append(new Cube(-.5f, -.5f, 0.f, this));
  _cubes.last()->setColor(0xff7fff7f);
  _cubes.append(new Cube( .5f, -.5f, 0.f, this));
  _cubes.last()->setColor(0xffff7f7f);
  _cubes.append(new Cube( .5f,  .5f, 0.f, this));
  _cubes.last()->setColor(0xffffff7f);

  setAttribute(Qt::WA_NoSystemBackground);
  setMinimumSize(480, 480);
  setMouseTracking(true);
}

GlWidget::~GlWidget()
{
  makeCurrent();

  QVectorIterator<Cube*> cube(_cubes);
  while (cube.hasNext())
    delete cube.next();
}

QSize GlWidget::sizeHint() const
{
  return QSize(480, 480);
}

void GlWidget::initializeGL()
{
  qglClearColor(Qt::black);
  glShadeModel(GL_SMOOTH);
}

void GlWidget::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  resizeGL(width(), height());

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  qglClearColor(Qt::black);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  static GLfloat lightPosition[4] = { 0.2, 0.3, 6.0, 3.0 };
  glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

  glPushMatrix();

  draw();

  glPopMatrix();

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glDisable(GL_LIGHT0);
  glDisable(GL_LIGHTING);

  glFlush();
}

void GlWidget::draw()
{
  glPushMatrix();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glTranslated(0.0, 0.0, -10.0);
  glRotated(_rx / 16.0, 1.0, 0.0, 0.0);
  glRotated(_ry / 16.0, 0.0, 1.0, 0.0);
  glRotated(_rz / 16.0, 0.0, 0.0, 1.0);

  QVectorIterator<Cube*> cube(_cubes);
  while (cube.hasNext())
    cube.next()->Draw();

  glPopMatrix();
}

void GlWidget::resizeGL(int width, int height)
{
  int side = qMin(width, height);
  glViewport((width - side)>>1, (height - side)>>1, side, side);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1, +1, +1, -1, -1.0, 15.0);
  glMatrixMode(GL_MODELVIEW);
}

void GlWidget::paintEvent(QPaintEvent */*event*/)
{
  QPainter painter;
  painter.begin(this);

  paintGL();

  painter.end();
}

void GlWidget::mouseMoveEvent(QMouseEvent *event)
{
  int dx = event->x() - _lastPos.x();
  int dy = event->y() - _lastPos.y();

  if (event->buttons() & Qt::LeftButton)
  {
    _rx = normalizeAngle(_rx + 8*dy);
    _ry = normalizeAngle(_ry + 8*dx);
  }
  else if (event->buttons() & Qt::RightButton)
  {
    _rx = normalizeAngle(_rx + 8*dy);
    _rz = normalizeAngle(_rz + 8*dx);
  }
  _lastPos = event->pos();
  pick(event->pos());
}

void GlWidget::mousePressEvent(QMouseEvent *event)
{
  _lastPos = event->pos();

  pick(event->pos());
  if (_selection >= 0)
  {
    if (event->button() == Qt::RightButton)
    {
      Cube *cube = _cubes[_selection];
      QColor color = QColorDialog::getColor(cube ->color(), this);
      if (color.isValid())
        cube->setColor(color);
    }
    if (event->button() == Qt::LeftButton)
    {
      Cube *cube = _cubes[_selection];
      RotationSelector selector(cube, this);
      selector.exec();
    }
  }
}

void GlWidget::pick(const QPoint &pos)
{
  qreal x = pos.x();
  qreal y = pos.y();

  GLuint selectBuf[1024];
  GLint hits;
  GLint viewport[4];

  glGetIntegerv(GL_VIEWPORT, viewport);
  glSelectBuffer(1024, selectBuf);
  glRenderMode(GL_SELECT);

  glInitNames();
  glPushName(0);

  glPushMatrix();
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPickMatrix((GLdouble)x, (GLdouble)(viewport[3] - y), 1.f, 1.f, viewport);
  glOrtho(-1, +1, +1, -1, -1.0, 15.0);

  draw();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  hits = glRenderMode(GL_RENDER);

  //On traite les sommets cliqués
  processHits(hits, selectBuf); //Fonction appelée ci-dessous

  glPopMatrix();

  updateGL();
}

void GlWidget::processHits(GLint hits, GLuint buffer[])
{
  int selection = -1;

  if (hits > 0)
  {
    GLuint nb_names, name, *ptr;
    GLdouble z1, z2, zInter = 0.0;

    qDebug() << "Hits:" << hits;

    ptr = (GLuint *)buffer;

    /*  Pour chaque hit (collision)  */
    for (GLint i = 0; i < hits; i++)
    {
      nb_names = ptr[0];
      if (nb_names != 1)
      {
        QMessageBox::critical(this, "Erreur pile noms", "Erreur sur la pile des noms !\nLe programme va quitter.");
        exit(1);
      }

      z1 = (double) ptr[1]/0x7fffffff;
      z2 = (double) ptr[2]/0x7fffffff;

      name = ptr[3];
      ptr +=4;

      /* On ne garde que le nom de l'objet le plus proche (ici on garde le zmin le plus petit) */
      if (i == 0 || z1 < zInter)
      {
        selection = name;
        zInter=z1;
      }
    }
  }
  else
    selection = -1;

  if (selection != _selection)
  {
    if (_selection >= 0) _cubes[_selection]->setHighlighted(false);
    _selection = selection;
    if (_selection >= 0) _cubes[_selection]->setHighlighted(true);
    qDebug() << "Selection changed:" << _selection;
  }
}

int GlWidget::normalizeAngle(int angle)
{
  while (angle < 0) angle += 360 * 16;
  while (angle > 360 * 16) angle -= 360 * 16;
  return angle;
}
