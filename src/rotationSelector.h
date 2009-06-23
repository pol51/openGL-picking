#ifndef __ROTATIONSELECTOR_H
#define __ROTATIONSELECTOR_H

#include <QDialog>

#include <QLabel>
#include <QSlider>
#include <QGridLayout>

class Cube;

class RotationSelector: public QDialog
{
  Q_OBJECT

  public:
    RotationSelector(Cube* cube, QWidget* parent);
    virtual ~RotationSelector();

  protected:
    QLabel _lblRX;
    QLabel _lblRY;
    QLabel _lblRZ;
    QSlider _sliderRX;
    QSlider _sliderRY;
    QSlider _sliderRZ;
    QGridLayout _mainLayout;
};

#endif
