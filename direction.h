#ifndef DIRECTION_H
#define DIRECTION_H

#include <QTextStream>
#include <QString>

//Purpose of class: Used to represent the possible directions an elevator travels
enum class Direction { DOWN, UP, UNKNOWN };

QTextStream& operator<<(QTextStream& stream, Direction direction);

#endif // DIRECTION_H
