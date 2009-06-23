#include "cube.h"

int Cube::_nextId = 0;

Cube::Cube(qreal x, qreal y, qreal z, QObject *parent)
  :QObject(parent),
  _id(_nextId++),
  _size(.5f),
  _x(x), _y(y), _z(z),
  _tx(0.f), _ty(0.f), _tz(0.f),
  _rx(0.f), _ry(0.f), _rz(0.f),
  _color(0xffffffff),
  _selected(false), _highlighted(false)
{
}

void Cube::Draw() const
{
  glPushMatrix();

  glLoadName(_id);

  if (!highlighted())
    glColor4f(_color.redF(), _color.greenF(), _color.blueF(), _color.alphaF());
  else
    glColor4f(1.f, 1.f, 1.f, 1.f);

  qreal hSize = _size / 2.f;

  glTranslatef(_x, _y, _z);
  glTranslatef(_tx, _ty, _tz);
  glRotatef(_rx, 1.f, 0.f, 0.f);
  glRotatef(_ry, 0.f, 1.f, 0.f);
  glRotatef(_rz, 0.f, 0.f, 1.f);

  glBegin(GL_QUADS);
    glVertex3f( hSize,  hSize, -hSize);
    glVertex3f(-hSize,  hSize, -hSize);
    glVertex3f(-hSize, -hSize, -hSize);
    glVertex3f( hSize, -hSize, -hSize);
  glEnd();
  glBegin(GL_QUADS);
    glVertex3f(-hSize,  hSize,  hSize);
    glVertex3f( hSize,  hSize,  hSize);
    glVertex3f( hSize, -hSize,  hSize);
    glVertex3f(-hSize, -hSize,  hSize);
  glEnd();
  glBegin(GL_QUADS);
    glVertex3f( hSize, -hSize,  hSize);
    glVertex3f(-hSize, -hSize,  hSize);
    glVertex3f(-hSize, -hSize, -hSize);
    glVertex3f( hSize, -hSize, -hSize);
  glEnd();
  glBegin(GL_QUADS);
    glVertex3f(-hSize,  hSize,  hSize);
    glVertex3f( hSize,  hSize,  hSize);
    glVertex3f( hSize,  hSize, -hSize);
    glVertex3f(-hSize,  hSize, -hSize);
  glEnd();
glBegin(GL_QUADS);
    glVertex3f(-hSize, -hSize,  hSize);
    glVertex3f(-hSize,  hSize,  hSize);
    glVertex3f(-hSize,  hSize, -hSize);
    glVertex3f(-hSize, -hSize, -hSize);
  glEnd();
  glBegin(GL_QUADS);
    glVertex3f( hSize, -hSize,  hSize);
    glVertex3f( hSize,  hSize,  hSize);
    glVertex3f( hSize,  hSize, -hSize);
    glVertex3f( hSize, -hSize, -hSize);
  glEnd();


  glPopMatrix();
}
