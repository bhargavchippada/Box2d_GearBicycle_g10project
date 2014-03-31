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
   b2Body* frontgear;
   ///back wheel
   b2Body* wheel1;
  void dominos_t::keyboard(unsigned char key)
  {
	  switch(key){
		case('w'):
		frontgear->ApplyAngularImpulse( -50,0 );
		break;
		case('s'):
		wheel1->ApplyAngularImpulse( 50,0 );
		break;
	}
  }
  dominos_t::dominos_t()
  {
/////////////////////////////////////////////////////////////////////////////
    ///Ground
    {
      //ground floor
      b2EdgeShape shape; 
      shape.Set(b2Vec2(-90.0f, 0.0f), b2Vec2(90.0f, 0.0f)); 
      b2FixtureDef *grnd = new b2FixtureDef;
      grnd->filter.categoryBits = 0x00F4;//
	  grnd->filter.maskBits = 0x00F2;//
	  grnd->shape=&shape;
	  b2BodyDef bd;
	  b2Body* b1=m_world->CreateBody(&bd); 
      b1->CreateFixture(grnd);
    }
//////////////////////////////////////////////////////////////////////////////
	 ///Add wheels
	 b2CircleShape circleDef;
	 circleDef.m_radius = 8;
	 b2FixtureDef *fd2 = new b2FixtureDef;
	 fd2->density = 0.1f;
	 fd2->friction = 5;
	 fd2->restitution = 0.2;
	 fd2->filter.groupIndex = -1;
	 fd2->shape=new b2CircleShape;
	 fd2->shape=&circleDef;
	 fd2->filter.categoryBits = 0x00F2;//
	 fd2->filter.maskBits = 0x00F4;//

	 ///front wheel
	 b2Body* wheel2;
	 b2BodyDef* wheelDef = new b2BodyDef;
	 wheelDef->type = b2_dynamicBody;
	 wheelDef->allowSleep = false;
	 
	 wheelDef->position.Set(-20,8);
	 wheel1 = m_world->CreateBody(wheelDef);
	 wheel1->CreateFixture(fd2);
	 
	 wheelDef->position.Set(10,8);
	 wheel2 = m_world->CreateBody(wheelDef);
	 wheel2->CreateFixture(fd2);
//////////////////////////////////////////////////////////////////////////////
	 ///Add cycle frame
	 b2PolygonShape boxshape;
	 boxshape.SetAsBox(7,0.6,b2Vec2(7,0),0);
	 b2FixtureDef *fd1 = new b2FixtureDef;
	 fd1->density = 0.1f;
	 
	 fd1->filter.categoryBits = 0x00F2;//
	 fd1->filter.maskBits = 0x00F4;//
	 fd1->friction = 0.5f;
	 fd1->restitution = 0.2f;
	 fd1->filter.groupIndex = -1;
	 fd1->shape = new b2PolygonShape;
	 fd1->shape = &boxshape;
	 b2BodyDef* rodDef=new b2BodyDef;
	 rodDef->type= b2_dynamicBody;
	 
	 rodDef->position.Set(wheel1->GetWorldCenter().x, wheel1->GetWorldCenter().y);
	 b2Body* rodbody=m_world->CreateBody(rodDef);
	 rodbody->CreateFixture(fd1);
	 
	 boxshape.SetAsBox(8,0.6,b2Vec2(8*cosf(b2_pi/3),8*sinf(b2_pi/3)),b2_pi/3);
	 rodbody->CreateFixture(fd1);
	 
	 boxshape.SetAsBox(8,0.6,b2Vec2(30-8*cosf(b2_pi/3),8*sinf(b2_pi/3)),-b2_pi/3);
	 rodbody->CreateFixture(fd1);
	 
	 boxshape.SetAsBox(7,0.6,b2Vec2(15,16*sinf(b2_pi/3)),0);
	 rodbody->CreateFixture(fd1);
	 
	 boxshape.SetAsBox(8,0.6,b2Vec2(14+8*cosf(b2_pi/3),8*sinf(b2_pi/3)),b2_pi/3);
	 rodbody->CreateFixture(fd1);
		 
//////////////////////////////////////////////////////////////////////////////        
	 ///Adding Rovolute Joints between tires and cycle frame
	 b2RevoluteJointDef revoluteJointDef;
	 revoluteJointDef.enableMotor = true;
	 
	 revoluteJointDef.Initialize(rodbody, wheel1, wheel1->GetWorldCenter());
	 m_world->CreateJoint(&revoluteJointDef);
	 
	 revoluteJointDef.Initialize(rodbody, wheel2, wheel2->GetWorldCenter());
	 m_world->CreateJoint(&revoluteJointDef);

//////////////////////////////////////////////////////////////////////////////         

	  ///back gear
	  b2CircleShape gearshape;
	  gearshape.m_radius = 3.0;
	  b2FixtureDef gearfd;
	  //ballfd.filter.groupIndex = -2;
	  gearfd.filter.categoryBits = 0x0002;//
	  gearfd.filter.maskBits = 0x0004;//
	  gearfd.shape = &gearshape;
	  gearfd.density = 5.0f;
	  gearfd.friction = 100.0f;
	  gearfd.restitution = 0.0f;
	  b2BodyDef gearbd;
	  gearbd.type = b2_dynamicBody;
	  gearbd.position.Set(-20.0f, 8.0f);
	  b2Body* gearbody1 = m_world->CreateBody(&gearbd);
	  gearbody1->CreateFixture(&gearfd);
	  
	  ///Front gear
	  gearbd.position.Set(-6.0f, 8.0f);
	  frontgear = m_world->CreateBody(&gearbd);
	  frontgear->CreateFixture(&gearfd);
      
////////////////////////////////////////////////////////////////////////////// 
	///Adding Revolute joints between gears and cycle frame
	b2RevoluteJointDef jointDef1;
	jointDef1.Initialize(gearbody1, rodbody, gearbody1->GetWorldCenter());
	m_world->CreateJoint(&jointDef1);

	b2RevoluteJointDef jointDef2;
	jointDef2.Initialize(frontgear, rodbody, frontgear->GetWorldCenter());
	m_world->CreateJoint(&jointDef2);
//////////////////////////////////////////////////////////////////////////////

b2Vec2 vs[44];
b2Body* conveyer[44];

 for (int i = 0; i < 16; ++i)
	{
	vs[i].Set(-21.0f+1.0f*i,11.0f);
    b2FixtureDef ballfd4;
    ballfd4.filter.categoryBits = 0x0004;
      ballfd4.filter.maskBits = 0x0002;ballfd4.filter.groupIndex = -1;
      
    //ballfd2.filter.groupIndex = -2;
	b2PolygonShape shape;
	shape.SetAsBox(.5f, 0.25f);
	ballfd4.shape = &shape;
	ballfd4.density=5.1f;
	gearfd.friction=100.0f;  
	b2BodyDef bd4;
	bd4.type = b2_dynamicBody;
	bd4.position.Set(-20.5f+1.0f*i,11.0f);
	conveyer[i] = m_world->CreateBody(&bd4);
	conveyer[i]->CreateFixture(&ballfd4);
	}
 for (int i = 0; i < 6; ++i)
	{
		vs[16+i].Set(-5.0f,11.0f-i*1.0f);
		b2FixtureDef ballfd4;
		
    ballfd4.filter.categoryBits = 0x0004;
      ballfd4.filter.maskBits = 0x0002;ballfd4.filter.groupIndex = -1;
    //ballfd2.filter.groupIndex = -2;
	b2PolygonShape shape;
	shape.SetAsBox(.25f, 0.5f);
	ballfd4.shape = &shape;  
		ballfd4.density=5.1f;
	gearfd.friction=100.0f;
	b2BodyDef bd4;
	bd4.type = b2_dynamicBody;
	bd4.position.Set(-5.0f,10.5f-i*1.0f);
	conveyer[i+16] = m_world->CreateBody(&bd4);
	conveyer[i+16]->CreateFixture(&ballfd4);
	}
 for (int i = 0; i < 16; ++i)
	{
		vs[22+i].Set(-5.0f-1.0f*i,5.0f);
		
    b2FixtureDef ballfd4;
    ballfd4.filter.categoryBits = 0x0004;
      ballfd4.filter.maskBits = 0x0002;ballfd4.filter.groupIndex = -1;
    //ballfd2.filter.groupIndex = -2;
	b2PolygonShape shape;
	shape.SetAsBox(.5f, 0.25f);
	ballfd4.shape = &shape;  
	b2BodyDef bd4;
	ballfd4.density=5.1f;
	gearfd.friction=100.0f;  
	bd4.type = b2_dynamicBody;
	bd4.position.Set(-5.5f-1.0f*i,5.0f);
	conveyer[i+22] = m_world->CreateBody(&bd4);
	conveyer[i+22]->CreateFixture(&ballfd4);		
	}
 for (int i = 0; i < 6; ++i)
	{
		vs[38+i].Set(-21.0f,5.0f+1.0f*i);
    b2FixtureDef ballfd4;
    ballfd4.filter.categoryBits = 0x0004;
      ballfd4.filter.maskBits = 0x0002;ballfd4.filter.groupIndex = -1;
    //ballfd2.filter.groupIndex = -2;
	b2PolygonShape shape;
	shape.SetAsBox(.25f, 0.55f);
	ballfd4.shape = &shape;  
	b2BodyDef bd4;
	ballfd4.density=5.1f;
	gearfd.friction=100.0f;  
	bd4.type = b2_dynamicBody;
	bd4.position.Set(-21.0f,5.0f+1.0f*i+.5f);
	conveyer[i+38] = m_world->CreateBody(&bd4);
	conveyer[i+38]->CreateFixture(&ballfd4);
	}

for(int i=1;i<44;i++)
{
	b2RevoluteJointDef jointDef3;
	jointDef3.Initialize(conveyer[i-1], conveyer[i],vs[i]);
	m_world->CreateJoint(&jointDef3);
}
b2RevoluteJointDef jointDef3;
	jointDef3.Initialize(conveyer[0], conveyer[43],vs[0]);
	m_world->CreateJoint(&jointDef3);
	
	b2Body* b5;
      b2FixtureDef ballfd5;
      //ballfd2.filter.groupIndex = -2;
	ballfd5.filter.categoryBits = 0x0003;
      ballfd5.filter.maskBits = 0x0001;
      b2PolygonShape shape5;
	shape5.SetAsBox(.25f, 4.5f);
	ballfd5.shape = &shape5;
	ballfd5.density=2.0f;  
	b2BodyDef bd5;
	bd5.type = b2_dynamicBody;
	bd5.position.Set(-6.0f, 8.0f);
	b5 = m_world->CreateBody(&bd5);
	b5->CreateFixture(&ballfd5);
	//b5->SetAngularVelocity(0.2f);
	b2WeldJointDef jointDef5;
	jointDef5.Initialize(frontgear, b5, frontgear->GetWorldCenter());//,frontgear->GetWorldCenter());//,frontgear->GetWorldCenter());
	m_world->CreateJoint(&jointDef5);
	//b2DistanceJointDef jointDef51;
	//b//2Vec2 a;
	//a.Set(18.5f,1.5f);
	//jointDef51.Initialize(frontgear, b5,a,a); //(18.5f,1.5f),(18.5f,1.5f));frontgear->GetWorldCenter());//,frontgear->GetWorldCenter());
	//m_world->CreateJoint(&jointDef51);
	
	b2Body* b6;
      b2FixtureDef ballfd6;
      //ballfd2.filter.groupIndex = -2;
	ballfd5.filter.categoryBits = 0x0008;
      ballfd5.filter.maskBits = 0x0008;
      b2PolygonShape shape6;
	shape6.SetAsBox(1.25f, .25f);
	ballfd6.shape = &shape6;  
	b2BodyDef bd6;
	bd6.fixedRotation=true;
	bd6.type = b2_dynamicBody;
	bd6.position.Set(-6.0f, 4.0f);
	b6 = m_world->CreateBody(&bd6);
	b6->CreateFixture(&ballfd6);
	//b6->SetAngularVelocity(1.0f);
	b2RevoluteJointDef jointDef6;
	jointDef6.Initialize(b5, b6, b6->GetWorldCenter());//,frontgear->GetWorldCenter());//,frontgear->GetWorldCenter());
	m_world->CreateJoint(&jointDef6);
	b2Body* b7;
      b2FixtureDef ballfd7;
      //ballfd2.filter.groupIndex = -2;
	ballfd5.filter.categoryBits = 0x0008;
      ballfd5.filter.maskBits = 0x0008;
      b2PolygonShape shape7;
	shape7.SetAsBox(1.25f, .25f);
	ballfd7.shape = &shape7;  
	b2BodyDef bd7;
	bd7.fixedRotation=true;
	bd7.type = b2_dynamicBody;
	bd7.position.Set(-6.0f, 12.0f);
	b7 = m_world->CreateBody(&bd7);
	b7->CreateFixture(&ballfd7);
	//b6->SetAngularVelocity(1.0f);
	b2RevoluteJointDef jointDef7;
	jointDef7.Initialize(b5, b7, b7->GetWorldCenter());//,frontgear->GetWorldCenter());//,frontgear->GetWorldCenter());
	m_world->CreateJoint(&jointDef7);
	//weld joining gear1 to wheel1
	b2WeldJointDef jointDef8;
	jointDef8.Initialize(gearbody1, wheel1, gearbody1->GetWorldCenter());//,frontgear->GetWorldCenter());//,frontgear->GetWorldCenter());
	m_world->CreateJoint(&jointDef8);
}
  sim_t *sim = new sim_t("Dominos", dominos_t::create);
}
