#ifndef DIRECTION_H
#define DIRECTION_H

#include <QTextStream>
#include <QString>

enum class Direction { DOWN, UP };

QTextStream& operator<<(QTextStream& stream, Direction direction);


#endif // DIRECTION_H
