#include "EnemyController.h"
#include "cinder/app/AppNative.h"
#include "cinder/ObjLoader.h"
#include "cinder/Rand.h"
#include "Resources.h"

using namespace ci;
using namespace ci::app;

static const Vec3f asteroidScaleA = Vec3f( 80.0f, 80.0f, 80.0f );
static const Vec3f asteroidScaleB = Vec3f( 30.0f, 30.0f, 30.0f );
static const Vec3f enemyScaleA = Vec3f( 20.0f, 20.0f, 20.0f );
static const Vec3f enemyScaleB = Vec3f( 15.0f, 15.0f, 15.0f );
static const Vec3f bossBodyScale = Vec3f( 55.0f, 55.0f, 55.0f );
static const Vec3f bossWingScaleA = Vec3f( 55.0f, 55.0f, 55.0f );
static const Vec3f bossWingScaleB = Vec3f( -55.0f, 55.0f, 55.0f );
static const float bossHealth = 1500.0f;
static const float defaultHitSize = 20.0f;

EnemyController::EnemyController()
{
    mStarted        = false;
    
    std::shared_ptr<ObjLoader> objLoadPtr( new ObjLoader( (DataSourceRef) loadResource( RES_OBJ_ENEMYA ), (DataSourceRef) loadResource( RES_MAT_ENEMYA ) ) );
    objLoadPtr->load( &mEnemyATriMesh);
    mEnemyAVbo = gl::VboMesh( mEnemyATriMesh );
    
    objLoadPtr.reset( new ObjLoader( (DataSourceRef) loadResource( RES_OBJ_ASTEROIDA ), (DataSourceRef) loadResource( RES_MAT_ASTEROIDA ) ) );
    objLoadPtr->load( &mAsteroidATriMesh );
    mAsteroidAVbo = gl::VboMesh( mAsteroidATriMesh );
    
    objLoadPtr.reset( new ObjLoader( (DataSourceRef) loadResource( RES_OBJ_ENEMYB ), (DataSourceRef) loadResource( RES_MAT_ENEMYB ) ) );
    objLoadPtr->load( &mEnemyBTriMesh );
    mEnemyBVbo = gl::VboMesh( mEnemyBTriMesh );
    
    objLoadPtr.reset( new ObjLoader( (DataSourceRef) loadResource( RES_OBJ_BOSSWING ), (DataSourceRef) loadResource( RES_MAT_BOSSWING ) ) );
    objLoadPtr->load( &mBossWingTriMesh );
    mBossWingVbo = gl::VboMesh( mBossWingTriMesh );
    
    objLoadPtr.reset( new ObjLoader( (DataSourceRef) loadResource( RES_OBJ_BOSSBODY ), (DataSourceRef) loadResource( RES_MAT_BOSSBODY ) ) );
    objLoadPtr->load( &mBossBodyTriMesh );
    mBossBodyVbo = gl::VboMesh( mBossBodyTriMesh );
    
    objLoadPtr.reset( new ObjLoader( (DataSourceRef) loadResource( RES_OBJ_ASTEROIDB ), (DataSourceRef) loadResource( RES_MAT_ASTEROIDB ) ) );
    objLoadPtr->load( &mAsteroidBTriMesh );
    mAsteroidBVbo = gl::VboMesh( mAsteroidBTriMesh );
    
    objLoadPtr.reset();

/*
    mEnemies.push_back( Enemy(
                              Vec4f( -300.0f,  150.0f, -500.0f, defaultHitSize ),
                              0.0,
                              0,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f(    0.0f,  150.0f, -500.0f, defaultHitSize ),
                              0.0f,
                              0,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f(  300.0f,  150.0f, -500.0f, defaultHitSize ),
                              0.0f,
                              0,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( -300.0f,     0.0f, -500.0f, defaultHitSize ),
                              0.0f,
                              0,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f(   0.0f,     0.0f, -500.0f, defaultHitSize ),
                              0.0f,
                              0,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f(  300.0f,    0.0f, -500.0f, defaultHitSize ),
                              0.0f,
                              0,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( -300.0f, -150.0f, -500.0f, defaultHitSize ),
                              0.0f,
                              0,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f(   0.0f,  -150.0f, -500.0f, defaultHitSize ),
                              0.0f,
                              0,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( 300.0f,  -150.0f, -500.0f, defaultHitSize ),
                              0.0f,
                              0,
                              false) );
 */
}

void EnemyController::init( float startTime )
{
    /*
     \Load the enemies
     Enemy types:
     0 - Asteroid flying towards screen
     1 - Type A flying left negative logarithmic
     2 - Type A flying right negative logarithmic
     3 - Type A flying diagonal left up and stopping
     4 - Type A flying diagonal right up and stopping
     5 - Type A flying towards the screen
     6 - Type B flying towards the screen
     7 - Boss body
     8 - Boss wing left
     9 - Boss wing right
     10 - Invulnerable Asteroid
     11 - Type B flying towards the screen with short pause
     */
    
    // Phase 0, 5s Asteroids (2.5s per meteor)
    mEnemies.push_back( Enemy(
                              Vec4f( -400.0f, 0.0f, -500.0f, defaultHitSize ),
                              startTime + 5.0f,
                              10,
                              true) );
    mEnemies.push_back( Enemy(
                              Vec4f( 0.0f, 0.0f, -500.0f, defaultHitSize ),
                              startTime + 7.3f,
                              10,
                              true) );
    mEnemies.push_back( Enemy(
                              Vec4f( 200.0f, 0.0f, -500.0f, defaultHitSize ),
                              startTime + 9.6f,
                              10,
                              true) );
    mEnemies.push_back( Enemy(
                              Vec4f( 100.0f, 0.0f, -500.0f, defaultHitSize ),
                              startTime + 11.9f,
                              10,
                              true) );
    mEnemies.push_back( Enemy(
                              Vec4f( -300.0f, 0.0f, -500.0f, defaultHitSize ),
                              startTime + 14.2f,
                              10,
                              true) );
    
    // Phase 1, 17.3s Enemy A Group 1 (6.3 secs)
    mEnemies.push_back( Enemy(
                              Vec4f( -850.0f, 450.0f, -500.0f, defaultHitSize ),
                              startTime + 17.3f,
                              1,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( -850.0f, 300.0f, -500.0f, defaultHitSize ),
                              startTime + 17.80f,
                              1,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( -850.0f, 150.0f, -500.0f, defaultHitSize ),
                              startTime + 18.30f,
                              1,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( -850.0f, 0.0f, -500.0f, defaultHitSize ),
                              startTime + 18.80f,
                              1,
                              false) );
    
    // Phase 2, 25.1 Enemy A Group 2 (6.3 secs)
    mEnemies.push_back( Enemy(
                              Vec4f( 850.0f, 450.0f, -500.0f, defaultHitSize ),
                              startTime + 25.10f,
                              2,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( 850.0f, 300.0f, -500.0f, defaultHitSize ),
                              startTime + 25.6f,
                              2,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( 850.0f, 150.0f, -500.0f, defaultHitSize ),
                              startTime + 26.1f,
                              2,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( 850.0f, 0.0f, -500.0f, defaultHitSize ),
                              startTime + 26.6f,
                              2,
                              false) );
    
    // Phase 3, 32.9s Enemy A Group 3 (6.3 secs)
    mEnemies.push_back( Enemy(
                              Vec4f( -850.0f, -250.0f, -500.0f, defaultHitSize ),
                              startTime + 32.9f,
                              3,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( -850.0f, -100.0f, -500.0f, defaultHitSize ),
                              startTime + 33.4f,
                              3,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( -850.0f, 50.0f, -500.0f, defaultHitSize ),
                              startTime + 33.9f,
                              3,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( 850.0f, -250.0f, -500.0f, defaultHitSize ),
                              startTime + 32.9f,
                              4,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( 850.0f, -100.0f, -500.0f, defaultHitSize ),
                              startTime + 33.4f,
                              4,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( 850.0f, 50.0f, -500.0f, defaultHitSize ),
                              startTime + 33.9f,
                              4,
                              false) );
    
    // Phase 4, 40.2 Enemy A Group 2 (6.3 secs)
    mEnemies.push_back( Enemy(
                              Vec4f( 850.0f, 450.0f, -500.0f, defaultHitSize ),
                              startTime + 40.2f,
                              2,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( 850.0f, 300.0f, -500.0f, defaultHitSize ),
                              startTime + 40.7f,
                              2,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( 850.0f, 150.0f, -500.0f, defaultHitSize ),
                              startTime + 41.2f,
                              2,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( 850.0f, 0.0f, -500.0f, defaultHitSize ),
                              startTime + 41.7f,
                              2,
                              false) );
    
    // Phase 5, 49s Asteroids& Group 1 (2.3 secs)
    mEnemies.push_back( Enemy(
                              Vec4f( 400.0f, 300.0f, -500.0f, defaultHitSize ),
                              startTime + 49.0f,
                              10,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( 200.0f, 150.0f, -500.0f, defaultHitSize ),
                              startTime + 49.0f,
                              10,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( 0.0f, 0.0f, -500.0f, defaultHitSize ),
                              startTime + 49.0f,
                              6,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( -200.0f, -150.0f, -500.0f, defaultHitSize ),
                              startTime + 49.0f,
                              10,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( -400.0f, -300.0f, -500.0f, defaultHitSize ),
                              startTime + 49.0f,
                              10,
                              false) );
    
    // Phase 6, 51.3 Asteroids& Group 2 (2.3 secs)
    mEnemies.push_back( Enemy(
                              Vec4f( -400.0f, 300.0f, -500.0f, defaultHitSize ),
                              startTime + 51.8f,
                              10,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( -200.0f, 150.0f, -500.0f, defaultHitSize ),
                              startTime + 51.8f,
                              6,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( 0.0f, 0.0f, -500.0f, defaultHitSize ),
                              startTime + 51.8f,
                              10,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( 200.0f, -150.0f, -500.0f, defaultHitSize ),
                              startTime + 51.8f,
                              6,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( 400.0f, -300.0f, -500.0f, defaultHitSize ),
                              startTime + 51.8f,
                              10,
                              false) );
    
    // Phase 6, 53.6 Asteroids plain 1 (2.3 secs)
    mEnemies.push_back( Enemy(
                              Vec4f( -400.0f, 150.0f, -500.0f, defaultHitSize ),
                              startTime + 53.6f,
                              10,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( -200.0f, 150.0f, -500.0f, defaultHitSize ),
                              startTime + 53.6f,
                              10,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( 0.0f,    150.0f, -500.0f, defaultHitSize ),
                              startTime + 53.6f,
                              10,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( 200.0f,  150.0f, -500.0f, defaultHitSize ),
                              startTime + 53.6f,
                              10,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( 400.0f,  150.0f, -500.0f, defaultHitSize ),
                              startTime + 53.6f,
                              10,
                              false) );
    
    // Phase 7, 55.9 Asteroids plain 2 (2.3 secs)
    mEnemies.push_back( Enemy(
                              Vec4f( -400.0f, -150.0f, -500.0f, defaultHitSize ),
                              startTime + 55.9f,
                              10,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( -200.0f, -150.0f, -500.0f, defaultHitSize ),
                              startTime + 55.9f,
                              10,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( 0.0f,    -150.0f, -500.0f, defaultHitSize ),
                              startTime + 55.9f,
                              10,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( 200.0f,  -150.0f, -500.0f, defaultHitSize ),
                              startTime + 55.9f,
                              10,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( 400.0f,  -150.0f, -500.0f, defaultHitSize ),
                              startTime + 55.9f,
                              10,
                              false) );
    
    // Phase 8, 58.2 Enemy A + B mixed (8.5 secs)
    mEnemies.push_back( Enemy(
                              Vec4f( -300.0f,  150.0f, -500.0f, defaultHitSize ),
                              startTime + 58.2f,
                              11,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f(    0.0f,  150.0f, -500.0f, defaultHitSize ),
                              startTime + 58.2f,
                              5,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f(  300.0f,  150.0f, -500.0f, defaultHitSize ),
                              startTime + 58.2f,
                              11,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( -300.0f,     0.0f, -500.0f, defaultHitSize ),
                              startTime + 58.7f,
                              5,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f(   0.0f,     0.0f, -500.0f, defaultHitSize ),
                              startTime + 58.7f,
                              11,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f(  300.0f,    0.0f, -500.0f, defaultHitSize ),
                              startTime + 58.7f,
                              5,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( -300.0f, -150.0f, -500.0f, defaultHitSize ),
                              startTime + 59.2f,
                              11,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f(   0.0f,  -150.0f, -500.0f, defaultHitSize ),
                              startTime + 59.2f,
                              5,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( 300.0f,  -150.0f, -500.0f, defaultHitSize ),
                              startTime + 59.2f,
                              11,
                              false) );
    
    // Phase 9, 67.7s Enemy A Group 4 (6.3 secs)
    mEnemies.push_back( Enemy(
                              Vec4f( -850.0f, -250.0f, -500.0f, defaultHitSize ),
                              startTime + 67.7,
                              3,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( -850.0f, -100.0f, -500.0f, defaultHitSize ),
                              startTime + 67.7f,
                              3,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( -850.0f, 50.0f, -500.0f, defaultHitSize ),
                              startTime + 67.7f,
                              3,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( 850.0f, -250.0f, -500.0f, defaultHitSize ),
                              startTime + 67.7f,
                              4,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( 850.0f, -100.0f, -500.0f, defaultHitSize ),
                              startTime + 67.7f,
                              4,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( 850.0f, 50.0f, -500.0f, defaultHitSize ),
                              startTime + 67.7f,
                              4,
                              false) );
    
    // Phase 10, 74.0s Enemy A + B mixed (8.5 secs)
    mEnemies.push_back( Enemy(
                              Vec4f( -300.0f,  150.0f, -500.0f, defaultHitSize ),
                              startTime + 74.0f,
                              5,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f(    0.0f,  150.0f, -500.0f, defaultHitSize ),
                              startTime + 74.5f,
                              11,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f(  300.0f,  150.0f, -500.0f, defaultHitSize ),
                              startTime + 74.0f,
                              5,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( -300.0f,     0.0f, -500.0f, defaultHitSize ),
                              startTime + 74.5f,
                              11,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f(   0.0f,     0.0f, -500.0f, defaultHitSize ),
                              startTime + 74.0f,
                              5,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f(  300.0f,    0.0f, -500.0f, defaultHitSize ),
                              startTime + 74.5f,
                              11,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( -300.0f, -150.0f, -500.0f, defaultHitSize ),
                              startTime + 74.0f,
                              5,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f(   0.0f,  -150.0f, -500.0f, defaultHitSize ),
                              startTime + 74.5f,
                              11,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( 300.0f,  -150.0f, -500.0f, defaultHitSize ),
                              startTime + 74.0f,
                              5,
                              false) );
    
    // Phase 11, 82.5 Enemy A Group 5, time segmented (6.3 secs)
    mEnemies.push_back( Enemy(
                              Vec4f( -850.0f, -250.0f, -500.0f, defaultHitSize ),
                              startTime + 82.5f,
                              3,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( -850.0f, -100.0f, -500.0f, defaultHitSize ),
                              startTime + 83.0f,
                              3,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( -850.0f, 50.0f, -500.0f, defaultHitSize ),
                              startTime + 82.5f,
                              3,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( 850.0f, -250.0f, -500.0f, defaultHitSize ),
                              startTime + 83.0f,
                              4,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( 850.0f, -100.0f, -500.0f, defaultHitSize ),
                              startTime + 82.5f,
                              4,
                              false) );
    mEnemies.push_back( Enemy(
                              Vec4f( 850.0f, 50.0f, -500.0f, defaultHitSize ),
                              startTime + 83.0f,
                              4,
                              false) );
    
    // Phase 12, 89.3 Boss (No time restriction)
    mEnemies.push_back( Enemy(
                              Vec4f( 0.0f, -1000.0f, -500.0f, 20.0f ),
                              startTime + 90.0f,
                              7,
                              false,
                              (int)bossHealth) );
    

}

void EnemyController::update( const ci::Vec4f& bPos, const bool bulletLive, const bool started )
{
    if( started )
    {
        if( !mStarted )
        {
            mStarted = true;
            init( getElapsedSeconds() );
        }
        
        for( std::list<Enemy>::iterator e = mEnemies.begin(); e != mEnemies.end(); e++ )
        {
            // Calculate the distance only if the enemy is active
            if( e->isActive() )
            {
                float distance = sqrtf( powf( e->getPos().x - bPos.x, 2.0f ) +
                                        powf( e->getPos().y - bPos.y, 2.0f ) );
                float radii = e->getPos().w + bPos.w;
                
                e->update( distance <= radii + 35.0f, bulletLive, getElapsedSeconds() );
            }
            // Otherwise, make hit detection false
            else
            {
                e->update( false, false, getElapsedSeconds() );
            }
        }
    }
}

void EnemyController::draw()
{    
    for( std::list<Enemy>::iterator e = mEnemies.begin(); e != mEnemies.end(); e++ )
    {
        if( !e->isActive() || (e->getPos().z >= 350.0f) )
            continue;
        
        if( e->isAlive() )
        {
            gl::pushMatrices();
            gl::translate( e->getPos().xyz() );
            
            
            switch( e->getType() )
            {
                case 0:
//                    gl::multModelView(e->getMatrix());
//                    gl::scale( asteroidScaleA);
//                    gl::draw( mAsteroidAVbo );
                    if(e->getHealth() > 0 )
                        gl::drawSolidRect( Rectf( 100.0f, -50.0f, 105.0f, -50.0f + (e->getHealth()/100.0f) * 100.0f) );
                    gl::drawStrokedCube( Vec3f::zero(), Vec3f( 100.0f, 100.0f, 100.f ) );
                    gl::multModelView(e->getMatrix());
                    gl::scale( enemyScaleA );
                    gl::draw( mEnemyAVbo );
                    break;
                    
                case 1:
                    if(e->getHealth() > 0 )
                        gl::drawSolidRect( Rectf( 100.0f, -50.0f, 105.0f, -50.0f + (e->getHealth()/100.0f) * 100.0f) );
                    gl::multModelView(e->getMatrix());
                    gl::scale( enemyScaleA );
                    gl::draw( mEnemyAVbo );
                    break;
                    
                case 2:
                    if(e->getHealth() > 0 )
                        gl::drawSolidRect( Rectf( 100.0f, -50.0f, 105.0f, -50.0f + (e->getHealth()/100.0f) * 100.0f) );
                    gl::multModelView(e->getMatrix());
                    gl::scale( enemyScaleA );
                    gl::draw( mEnemyAVbo );
                    break;
                
                case 3:
                    if(e->getHealth() > 0 )
                        gl::drawSolidRect( Rectf( 100.0f, -50.0f, 105.0f, -50.0f + (e->getHealth()/100.0f) * 100.0f) );
                    gl::multModelView(e->getMatrix());
                    gl::scale( enemyScaleA );
                    gl::draw( mEnemyAVbo );
                    break;
                
                case 4:
                    if(e->getHealth() > 0 )
                        gl::drawSolidRect( Rectf( 100.0f, -50.0f, 105.0f, -50.0f + (e->getHealth()/100.0f) * 100.0f) );
                    gl::multModelView(e->getMatrix());
                    gl::scale( enemyScaleA );
                    gl::draw( mEnemyAVbo );
                    break;
                    
                case 5:
                    if(e->getHealth() > 0 )
                        gl::drawSolidRect( Rectf( 100.0f, -50.0f, 105.0f, -50.0f + (e->getHealth()/100.0f) * 100.0f) );
                    gl::multModelView(e->getMatrix());
                    gl::scale( enemyScaleA );
                    gl::draw( mEnemyAVbo );
                    break;
                    
                case 6:
                    if(e->getHealth() > 0 )
                        gl::drawSolidRect( Rectf( 100.0f, -50.0f, 105.0f, -50.0f + (e->getHealth()/100.0f) * 100.0f) );
                    gl::multModelView(e->getMatrix());
                    gl::scale( enemyScaleB );
                    gl::draw( mEnemyBVbo );
                    break;
                
                case 7:
                    gl::pushMatrices();
                        if(e->getHealth() > 0 )
                            gl::drawSolidRect( Rectf( 400.0f, 50.0f, 410.0f, 50.0f + (e->getHealth()/bossHealth) * 200.0f) );
                        gl::rotate( Quaternion<float>(  1.0f - e->getVRot(), 0.0f, 1.0f, 0.0f ) );
                        gl::scale( bossBodyScale );
                        gl::draw( mBossBodyVbo );
                    gl::popMatrices();
                    
                    gl::pushMatrices();
                        gl::translate( Vec3f( -100.0f, 200.0f, 0.0f ) );
                        gl::rotate( Quaternion<float>( 0.995f + ( cos( 2.0f * getElapsedSeconds()) * 0.005f ), 0.0f, 0.0f, -1.0f ) );
                        gl::scale( bossWingScaleA );
                        gl::draw( mBossWingVbo );
                    gl::popMatrices();
                    
                    gl::pushMatrices();
                        gl::translate( Vec3f( -200.0f, 150.0f, 0.0f ) );
                        gl::rotate( Quaternion<float>( 0.995f + ( cos( 2.0f * getElapsedSeconds()) * 0.005f ), 0.0f, 0.0f, -1.0f ) );
                        gl::scale( bossWingScaleA );
                        gl::draw( mBossWingVbo );
                    gl::popMatrices();
                    
                    gl::pushMatrices();
                        gl::translate( Vec3f( 100.0f, 200.0f, 0.0f ) );
                        gl::rotate( Quaternion<float>( 0.995f + ( cos( 2.0f * getElapsedSeconds()) * 0.005f ), 0.0f, 0.0f, 1.0f ) );
                        gl::scale( bossWingScaleB );
                        gl::draw( mBossWingVbo );
                    gl::popMatrices();
                    
                    gl::pushMatrices();
                        gl::translate( Vec3f( 200.0f, 150.0f, 0.0f ) );
                        gl::rotate( Quaternion<float>( 0.995f + ( cos( 2.0f * getElapsedSeconds()) * 0.005f ), 0.0f, 0.0f, 1.0f ) );
                        gl::scale( bossWingScaleB );
                        gl::draw( mBossWingVbo );
                    gl::popMatrices();
                    
                    break;
                
//                case 8:
//                    gl::rotate( Quaternion<float>( 0.995f + ( cos( 2.0f * getElapsedSeconds()) * 0.005f ), 0.0f, 0.0f, -1.0f ) );
//                    gl::scale( bossWingScaleA );
//                    gl::draw( mBossWingVbo );
//                    break;
//                
//                case 9:
//                    gl::rotate( Quaternion<float>( 0.995f + ( cos( 2.0f * getElapsedSeconds()) * 0.005f ), 0.0f, 0.0f, 1.0f ) );
//                    gl::scale( bossWingScaleB );
//                    gl::draw( mBossWingVbo );
//                    break;
                
                case 10:
                    gl::multModelView(e->getMatrix());
                    gl::scale( asteroidScaleB );
                    gl::draw( mAsteroidBVbo );
                    break;
                
                case 11:
                    if(e->getHealth() > 0 )
                        gl::drawSolidRect( Rectf( 100.0f, -50.0f, 105.0f, -50.0f + (e->getHealth()/100.0f) * 100.0f) );
                    gl::multModelView(e->getMatrix());
                    gl::scale( enemyScaleB );
                    gl::draw( mEnemyBVbo );
                    break;
            }
            
            gl::popMatrices();
        }
        else if( getElapsedSeconds() < e->getDeathTime() )
        {
//            float rad = pow((getElapsedSeconds()/e->getDeathTime()), 8.0f) * 100.0f;
//            float alp = 1.0f - (getElapsedSeconds()/e->getDeathTime());
            switch( e->getType() )
            {
                case 7:
                    gl::pushMatrices();
                    gl::translate( e->getPos().xyz() );
                    gl::color( 1.0f, 1.0f, 1.0f, ( 1.0f - (getElapsedSeconds()/e->getDeathTime() ) ) );
                    gl::drawStrokedCircle( e->getPos().xy(), pow((getElapsedSeconds()/e->getDeathTime()), 8.0f) * 300.0f );
                    gl::popMatrices();
                    break;
                    
                default:
                    gl::pushMatrices();
                    gl::translate( e->getPos().xyz() );
                    gl::color( 1.0f, 1.0f, 1.0f, ( 1.0f - (getElapsedSeconds()/e->getDeathTime() ) ) );
                    gl::drawStrokedCircle( e->getPos().xy(), pow((getElapsedSeconds()/e->getDeathTime()), 8.0f) * 100.0f );
                    gl::popMatrices();
                    break;
            }
        }
    }
}

const std::list<Enemy>& EnemyController::enemyList()
{
    return mEnemies;
}

EnemyController::~EnemyController()
{
    mEnemies.clear();
    mEnemyAVbo.reset();
    mEnemyATriMesh.clear();
    mEnemyBVbo.reset();
    mEnemyBTriMesh.clear();
    mAsteroidAVbo.reset();
    mAsteroidATriMesh.clear();
    mBossBodyVbo.reset();
    mBossBodyTriMesh.clear();
    mBossWingVbo.reset();
    mBossWingTriMesh.clear();
}