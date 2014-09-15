#pragma once
#include "Bullet.h"
#include "cinder/TriMesh.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/gl.h"
#include "cinder/Matrix44.h"
#include "Resources.h"

class BulletController
{
  public:
    // Methods
    BulletController();
    void                update( bool firing, float xPos, float yPos, float xRot, float yRot );
    void                draw();
    const bool          getBulletStatus();
    const ci::Vec4f&    getBulletPos();
    ~BulletController();
    
  private:
    // Mechanics
    Bullet              mBullet;
    ci::Matrix44f       mTrans1, mTrans2;
    
    // Model
    ci::TriMesh         mBulletTriMesh;
    ci::gl::VboMesh     mBulletVbo;
};