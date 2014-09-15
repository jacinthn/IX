#include "StarController.h"
#include "cinder/Vector.h"
#include "cinder/Rand.h"


using namespace ci;

StarController::StarController( int windowWidth, int windowHeight, int numStars )
{
    Color colorChoices[] = {
        Color( 0.9725f, 0.9275f, 0.9255f ),
        Color( 0.7333f, 0.2784f, 0.2196f ),
        Color( 0.4784f, 0.1843f, 0.2039f ),
        Color( 0.8034f, 0.6863f, 0.4824f )
    };
    for( int i = 0; i < numStars; i++ )
    {
        float x = randFloat( (-1.0f/3.0f) * windowWidth, (1.0f/3.0f) * windowWidth );
        float y = randFloat( (-1.0f/3.0f) * windowHeight, (1.0f/3.0f) * windowHeight );
        float z = randFloat( 0.0f, 400.0f);
        int colorIdx = randInt( 0, 4 );
        
        if( x > 0.0f )
            x += 50.0f;
        else
            x -= 50.0f;
        
        if( y > 0.0f )
            y += 50.0f;
        else
            y -= 50.0f;
        
        mStars.push_back( Star( Vec3f( x, y, z ), colorChoices[ colorIdx ] ) );
    }
}

void StarController::update( float xInfluence, float yInfluence )
{
    for(std::list<Star>::iterator s1 = mStars.begin(); s1 != mStars.end(); s1++ )
    {
        s1->update( xInfluence, yInfluence );
    }
}

void StarController::draw()
{
    for(std::list<Star>::iterator s1 = mStars.begin(); s1 != mStars.end(); s1++ )
    {
        s1->draw();
    }
}

StarController::~StarController()
{
    mStars.clear();
}