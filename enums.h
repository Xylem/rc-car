#ifndef ENUMS_H
#define ENUMS_H

enum Direction {
  RIGHT = 8,
  LEFT,
  STRAIGHT
};

enum Accel {
  FORWARD = 11,
  BACK,
  STOP
};

enum Distance {
  TRIGGER_LEFT = 2,
  ECHO_LEFT,
  TRIGGER_RIGHT,
  ECHO_RIGHT
};

#endif
