/*
* Copyright (c) 2006-2009 Erin Catto http://www.box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

/* 
 * Base code for CS 296 Software Systems Lab 
 * Department of Computer Science and Engineering, IIT Bombay
 * Instructor: Parag Chaudhuri
 */

#include <iostream>
#include <cmath>
#include "cs296_base.hpp"
#include "render.hpp"

#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include "GL/freeglut.h"
#endif

#include <cstring>
using namespace std;

#include "dominos.hpp"

namespace cs296
{
  /**  The is the constructor 
   * This is the documentation block for the constructor.
   */ 
  
  dominos_t::dominos_t()
  {
    //Base Ground 
    {
      b2EdgeShape shape; 
      shape.Set(b2Vec2(-90.0f, -3.0f), b2Vec2(90.0f, -3.0f));
      b2BodyDef bd; 
      b2Body* ground = m_world->CreateBody(&bd); 
      ground->CreateFixture(&shape, 0.0f);
    }
    
	  {
		 //add wheels
		 b2CircleShape circleDef;
         circleDef.m_radius = 8;
         b2FixtureDef *fd2 = new b2FixtureDef;
         fd2->density = 1;
         fd2->friction = 5;
         fd2->restitution = 0.2;
         fd2->filter.groupIndex = -1;
		 fd2->shape=new b2CircleShape;
		 fd2->shape=&circleDef;
		 b2Body* wheel1;
		 b2Body* wheel2;
         for (int i = 0; i < 2; i++) {
            
            b2BodyDef* bodyDef = new b2BodyDef;
            bodyDef->type = b2_dynamicBody;
            if (i == 0) bodyDef->position.Set(-20,8);
            else bodyDef->position.Set(20,8);
            bodyDef->allowSleep = false;
            
            if (i == 0) {
				wheel1 = m_world->CreateBody(bodyDef);
				wheel1->CreateFixture(fd2);
						}
            else {
				wheel2 = m_world->CreateBody(bodyDef);
				wheel2->CreateFixture(fd2);
				}
         }
         //add cycle frame
         b2PolygonShape boxshape;
		 boxshape.SetAsBox(12,0.6,b2Vec2(12,0),0);
         b2FixtureDef *fd1 = new b2FixtureDef;
		 fd1->density = 1;
		 fd1->friction = 0.5f;
		 fd1->restitution = 0.2f;
		 fd1->filter.groupIndex = -1;
		 fd1->shape = new b2PolygonShape;
		 fd1->shape = &boxshape;
		 b2BodyDef* rodDef=new b2BodyDef;
		 rodDef->type= b2_dynamicBody;
		 rodDef->fixedRotation=true;
		 rodDef->position.Set(wheel1->GetWorldCenter().x, wheel1->GetWorldCenter().y);
		 b2Body* rodbody=m_world->CreateBody(rodDef);
		 rodbody->CreateFixture(fd1);
		 
		 boxshape.SetAsBox(8,0.6,b2Vec2(8*cosf(b2_pi/3),8*sinf(b2_pi/3)),b2_pi/3);
		 rodbody->CreateFixture(fd1);
		 
		 boxshape.SetAsBox(8,0.6,b2Vec2(40-8*cosf(b2_pi/3),8*sinf(b2_pi/3)),-b2_pi/3);
		 rodbody->CreateFixture(fd1);
		 
		 boxshape.SetAsBox(12,0.6,b2Vec2(20,16*sinf(b2_pi/3)),0);
		 rodbody->CreateFixture(fd1);
		 
		 boxshape.SetAsBox(8,0.6,b2Vec2(24+8*cosf(b2_pi/3),8*sinf(b2_pi/3)),b2_pi/3);
		 rodbody->CreateFixture(fd1);
         
         //Add Rovolute Joints
         b2RevoluteJointDef revoluteJointDef;
         revoluteJointDef.enableMotor = true;
         
         revoluteJointDef.Initialize(rodbody, wheel1, wheel1->GetWorldCenter());
         b2RevoluteJoint* motor1 = (b2RevoluteJoint*)m_world->CreateJoint(&revoluteJointDef);
         
         revoluteJointDef.Initialize(rodbody, wheel2, wheel2->GetWorldCenter());
         b2RevoluteJoint* motor2 = (b2RevoluteJoint*)m_world->CreateJoint(&revoluteJointDef);
         
}
{
	 //The triangle wedge
      b2Body* sbody;
      b2PolygonShape poly;
      b2Vec2 vertices[3];
      vertices[0].Set(-1,0);
      vertices[1].Set(1,0);
      vertices[2].Set(0,1.5);
      poly.Set(vertices, 3);
      b2FixtureDef wedgefd;
      wedgefd.shape = &poly;
      wedgefd.density = 10.0f;
      wedgefd.friction = 0.0f;
      wedgefd.restitution = 0.0f;
      b2BodyDef wedgebd;
      wedgebd.type=b2_dynamicBody;
      wedgebd.position.Set(-22.0f, 30.0f);
      sbody = m_world->CreateBody(&wedgebd);
      sbody->CreateFixture(&wedgefd);
}
}
  sim_t *sim = new sim_t("Gear Cycle", dominos_t::create);
}
