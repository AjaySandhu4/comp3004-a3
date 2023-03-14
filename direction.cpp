#include "direction.h"

QTextStream& operator<<(QTextStream& stream, Direction direction)
{
    QString directionString;
    if(direction == Direction::DOWN) directionString = "DOWN";
    else directionString = "UP";
    stream << directionString;
    return stream;
}
