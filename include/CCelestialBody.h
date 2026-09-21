#pragma once

#include "raylib/raylib.h"
#include <vector>

class CCelestialBody {
public:
  float mass;
  Vector3 velocity; // need to make a choice here, use this velocity or the
                    // velocity in the CTransform. or there might be a way to
                    // make both of them refer to the same thing.
  float resultantForceMagnitude; // this is as the name the resultant force. my plan is
                          // that we calculate the force between every objects
                          // in the scene. and then for every body, calcualte
                          // the resultant force. need to do some vector maths.
                          // REFER a level physics vector questions. will clear
                          // this up very much.
                          //
                          // we will use F = GMm/r^2
                          //
  // align the velocuity to the resyult force. How? I need to refer to a level
  // physics vector too.
  //
  //
  //
  //we will create the resultant force from this vector of vector. 
  //and these forces should be calculated as F = GMm/r^2 
  std::vector<Vector3> forcesActingOnTheBody;


  //and then i think
  //
  //F = ma 
  //ma = GMm/r^2
  //a = GM/r^2
  //u + at = v


  CCelestialBody(const float m, const Vector3 &v) : mass(m), velocity(v) {}
  // resultatFOrce needs to be updated ebery frame
};
