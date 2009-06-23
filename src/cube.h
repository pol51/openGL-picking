#include <QtOpenGL>

class Cube : public QObject
{
  Q_OBJECT

  protected:
    static int _nextId;

    int _id;

    qreal _size;

    qreal _x, _y, _z;
    qreal _tx, _ty, _tz;
    qreal _rx, _ry, _rz;

    /*!
    * @brief The color of object
    */
    QColor _color;
    Q_PROPERTY(QColor _color READ color WRITE setColor);

    /*!
    * @brief The selection flag
    */
    bool _selected;
    Q_PROPERTY(bool _selected READ selected WRITE setSelected);

    /*!
    * @brief The highlight flag
    */
    bool _highlighted;
    Q_PROPERTY(bool _highlighted READ highlighted WRITE setHighlighted);


  public:
    /*!
    * @brief Constructor
    */
    Cube(qreal x, qreal y, qreal z, QObject *parent = NULL);

    /*!
    * @brief Destructor
    */
    virtual ~Cube() {}

    /*!
    * @brief Returns the color of object
    */
    const QColor &color() const { return _color; }

    /*!
    * @brief Sets the color of object
    */
    void setColor(const QColor &color) { _color = color; }

    /*!
    * @brief Returns true if object is selected, false otherwise
    */
    bool selected() const { return _selected; }

    /*!
    * @brief Sets the object selected or not
    */
    void setSelected(const bool selected) { _selected = selected; }

    /*!
    * @brief Returns true if object is highlighted, false otherwise
    */
    bool highlighted() const { return _highlighted; }

    /*!
    * @brief Sets the object highlighted or not
    */
    void setHighlighted(const bool highlighted) { _highlighted = highlighted; }

    /*!
    * @brief Draw the object
    */
    void Draw() const;

    /*!
    * @brief Translate the object
    */
    void Translate(qreal tx, qreal ty, qreal tz) { _tx = tx; _ty = ty; _tz = tz; }

    /*!
    * @brief Rotate the object
    */
    void Rotate(qreal rx, qreal ry, qreal rz) { _rx = rx; _ry = ry; _rz = rz; }

    qreal rx() const { return _rx; }
    qreal ry() const { return _ry; }
    qreal rz() const { return _rz; }
};
