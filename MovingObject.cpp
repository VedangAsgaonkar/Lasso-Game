#include <simplecpp>
#include "MovingObject.h"

using namespace simplecpp;

void MovingObject::nextStep(double t) {     // *** t is the duration of a step
  if(paused) { return; }
  //cerr << "x=" << getXPos() << ",y=" << getYPos() << endl;
  //cerr << "vx=" << vx << ",vy=" << vy << endl;
  //cerr << "ax=" << ax << ",ay=" << ay << endl;

  for(size_t i=0; i<parts.size(); i++){ // *** size_t is equivalent to short , but is returned by size() method which number of elements i.e. size of vector
    parts[i]->move(vx*t, vy*t);
  }
  vx += ax*t;
  vy += ay*t;
} // End MovingObject::nextStep()

double MovingObject::getXPos() {
  return (parts.size() > 0) ? parts[0]->getX() : -1; // *** Its ?: the conditional operator . Return X coordinate of the sprite whose address is parts[0]
}

double MovingObject::getYPos() {
  return (parts.size() > 0) ? parts[0]->getY() : -1;
}

void MovingObject::reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) {
  for(size_t i=0; i<parts.size(); i++){ // *** indices in vectors go from 0 to size-1
    parts[i]->moveTo(argx, argy);
  }
  double angle_rad = angle_deg*PI/180.0;
  double argvx = speed*cos(angle_rad);
  double argvy = -speed*sin(angle_rad);
  vx = argvx; vy = argvy; ax = argax; ay = argay; paused = argpaused;
} // End MovingObject::reset_all()

void MovingObject::getAttachedTo(MovingObject *m) { // *** argument is of type address of moving object to which we have to attach our object
  double xpos = m->getXPos(); // *** The -> operator is a combination of a de referencing operator and a dot operator so m->getXPos() is same as *m.getXPos()
  double ypos = m->getYPos();
  for(size_t i=0; i<parts.size(); i++){ // *** parts of our object
    parts[i]->moveTo(xpos, ypos);
  }
  initMO(m->vx, m->vy, m->ax, m->ay, m->paused);
}
