#include "direction.h"

QTextStream& operator<<(QTextStream& stream, Direction direction)
{
    QString directionString;
    if(direction == Direction::DOWN) directionString = "DOWN";
    else if(direction == Direction::UP) directionString = "UP";
    else if(direction == Direction::UNKNOWN) directionString = "UNKNOWN";
    stream << directionString;
    return stream;
}
