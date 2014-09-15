#pragma once
#include "Star.h"
#include <list>

class StarController
{
  public:
    StarController( int windowWidth, int windowHeight, int numStars );
    void update( float xInfluence, float yInfluence );
    void draw();
    
    std::list<Star> mStars;
    
    ~StarController();
};