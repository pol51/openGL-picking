#include "rotationSelector.h"

#include <cube.h>

RotationSelector::RotationSelector(Cube* cube, QWidget* parent):
  QDialog(parent)
{
  _mainLayout.addWidget(&_lblRX, 0, 0);
  _mainLayout.addWidget(&_lblRY, 1, 0);
  _mainLayout.addWidget(&_lblRZ, 2, 0);

  _mainLayout.addWidget(&_sliderRX, 0, 1);
  _mainLayout.addWidget(&_sliderRY, 1, 1);
  _mainLayout.addWidget(&_sliderRZ, 2, 1);

  setLayout(&_mainLayout);

  _lblRX.setText("rX:");
  _lblRY.setText("rY:");
  _lblRZ.setText("rZ:");

  _sliderRX.setMinimum(-180); _sliderRX.setMaximum(180);
  _sliderRY.setMinimum(-180); _sliderRY.setMaximum(180);
  _sliderRZ.setMinimum(-180); _sliderRZ.setMaximum(180);

  _sliderRX.setOrientation(Qt::Horizontal);
  _sliderRY.setOrientation(Qt::Horizontal);
  _sliderRZ.setOrientation(Qt::Horizontal);

  _sliderRX.setValue(cube->rx());
  _sliderRY.setValue(cube->ry());
  _sliderRZ.setValue(cube->rz());

  connect(&_sliderRX, SIGNAL(valueChanged(int)),
          cube, SLOT(setRx(int)));
  connect(&_sliderRY, SIGNAL(valueChanged(int)),
          cube, SLOT(setRy(int)));
  connect(&_sliderRZ, SIGNAL(valueChanged(int)),
          cube, SLOT(setRz(int)));
}

RotationSelector::~RotationSelector()
{
}
