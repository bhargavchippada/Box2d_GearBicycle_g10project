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
	///front wheel
	 b2Body* wheel2;
	void dominos_t::keyboard(unsigned char key)
	{
	  switch(key){
		case('w'):
		frontgear->ApplyAngularImpulse( -100,0 );
		break;
		case('s'):
		wheel1->ApplyAngularImpulse( 100,0 );
		wheel2->ApplyAngularImpulse( 100,0 );
		break;
	}
	}
dominos_t::dominos_t()
{
/////////////////////////////////////////////////////////////////////////////
	///Ground
	{
	  b2EdgeShape shape; 
	  shape.Set(b2Vec2(-90.0f, 0.0f), b2Vec2(90.0f, 0.0f)); 
	  b2FixtureDef *grnd = new b2FixtureDef;
	  grnd->filter.groupIndex = -5;//
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
	 fd2->filter.groupIndex = -1;//
	 fd2->shape=new b2CircleShape;
	 fd2->shape=&circleDef;

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
	 fd1->density = 2.0f;
	 fd1->friction = 0.5f;
	 fd1->restitution = 0.2f;
	 fd1->filter.groupIndex = -1;//
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
	 
	 boxshape.SetAsBox(1.5,0.4,b2Vec2((16-1.5)*cosf(b2_pi/3),(16+1.5)*sinf(b2_pi/3)),-b2_pi/3);
	 rodbody->CreateFixture(fd1);
//////////////////////////////////////////////////////////////////////////////
	 /// seat on cycle
	 b2BodyDef* seatDef=new b2BodyDef;
	 seatDef->type= b2_dynamicBody;
	 seatDef->position.Set(wheel1->GetWorldCenter().x+(16-3)*cosf(b2_pi/3), wheel1->GetWorldCenter().y+(16+3)*sinf(b2_pi/3));
	 b2Body* seatbody=m_world->CreateBody(seatDef);
	 
	 b2Vec2 vertices[5];
	 vertices[0].Set(-3.0f, 2.0f);
	 vertices[1].Set(4.0f, 2.0f);
	 vertices[2].Set(4.0f, 1.0f);
	 vertices[3].Set(0.0f, -0.1f);
	 vertices[3].Set(-2.0f, -0.1f);
	 b2PolygonShape polygon;
	 boxshape.Set(vertices, 5);
	 seatbody->CreateFixture(fd1);
	 
	 
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
	gearfd.filter.groupIndex = -1;//
	gearfd.filter.categoryBits = 0x0002;//
	gearfd.filter.maskBits = 0x0004;//
	gearfd.shape = &gearshape;
	gearfd.density = 1.0f;
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
	///Adding the Chain
	b2Vec2 vs[44];
	b2Body* conveyer[44];
	
	b2FixtureDef chainfd;
    chainfd.filter.categoryBits = 0x0004;
    chainfd.filter.maskBits = 0x0002;
	b2PolygonShape chainshape;
	chainshape.SetAsBox(0.5f, 0.25f);
	chainfd.shape = &chainshape;
	chainfd.density=1.0f;
	b2BodyDef chainDef;
	chainDef.type = b2_dynamicBody;
	
	///The top chain units
 for (int i = 0; i < 16; ++i)
	{
	vs[i].Set(-21.0f+1.0f*i,11.0f);
	chainDef.position.Set(-20.5f+1.0f*i,11.0f);
	conveyer[i]=m_world->CreateBody(&chainDef);
	conveyer[i]->CreateFixture(&chainfd);
	}
	
	///The right chain units
	chainshape.SetAsBox(0.25f, 0.5f);
 for (int i = 0; i < 6; ++i)
	{
	vs[i+16].Set(-5.0f,11.0f-i*1.0f);
	chainDef.position.Set(-5.0f,10.5f-i*1.0f);
	conveyer[i+16]=m_world->CreateBody(&chainDef);
	conveyer[i+16]->CreateFixture(&chainfd);
	}
	
	///The Bottom chain units
	chainshape.SetAsBox(0.5f, 0.25f);
 for (int i = 0; i < 16; ++i)
	{
	vs[i+22].Set(-5.0f-1.0f*i,5.0f);
	chainDef.position.Set(-5.5f-1.0f*i,5.0f);
	conveyer[i+22]=m_world->CreateBody(&chainDef);
	conveyer[i+22]->CreateFixture(&chainfd);		
	}
	
	///The left chain units
	chainshape.SetAsBox(0.25f, 0.5f);
 for (int i = 0; i < 6; ++i)
	{
	vs[i+38].Set(-21.0f,5.0f+1.0f*i);
	chainDef.position.Set(-21.0f,5.0f+1.0f*i+.5f);
	conveyer[i+38] = m_world->CreateBody(&chainDef);
	conveyer[i+38]->CreateFixture(&chainfd);
	}
//////////////////////////////////////////////////////////////////////////////
	///Adding Revolute joint between chain units
	b2RevoluteJointDef jointDef3;
 for(int i=1;i<44;i++)
	{
	jointDef3.Initialize(conveyer[i-1], conveyer[i],vs[i]);
	m_world->CreateJoint(&jointDef3);
	}
	
	jointDef3.Initialize(conveyer[0], conveyer[43],vs[0]);
	m_world->CreateJoint(&jointDef3);
//////////////////////////////////////////////////////////////////////////////	
	///Creating connecting pedal rod and pedals
    b2FixtureDef pedalfd;
    pedalfd.filter.groupIndex = -1;
	pedalfd.density=1.0f;  
	b2PolygonShape pedrodshape;
	pedrodshape.SetAsBox(.25f, 4.5f);
	pedalfd.shape = &pedrodshape;
	b2BodyDef pedalDef;
	pedalDef.type = b2_dynamicBody;
	pedalDef.position.Set(-6.0f, 8.0f);
	///Creating the rod connecting the pedals which i call pedalrod
	b2Body* pedalbody = m_world->CreateBody(&pedalDef);
	pedalbody->CreateFixture(&pedalfd);
	
	///welding pedalrod to cycle frame front gear
	b2WeldJointDef jointDef4;
	jointDef4.Initialize(frontgear, pedalbody, frontgear->GetWorldCenter());
	m_world->CreateJoint(&jointDef4);
//////////////////////////////////////////////////////////////////////////////
	///pedal1: Creating the first pedal
	pedrodshape.SetAsBox(1.25f, .25f);  
	pedalDef.fixedRotation=true;
	pedalDef.position.Set(-6.0f, 4.0f);
	b2Body* pedal1 = m_world->CreateBody(&pedalDef);
	pedal1->CreateFixture(&pedalfd);
	
	///Creating a revolute joint between pedal1 and pedal rod
	b2RevoluteJointDef jointDef5;
	jointDef5.Initialize(pedalbody, pedal1, pedal1->GetWorldCenter());
	m_world->CreateJoint(&jointDef5);
	
//////////////////////////////////////////////////////////////////////////////
	///pedal2: Creating the second pedal
	pedalDef.position.Set(-6.0f, 12.0f);
	b2Body* pedal2 = m_world->CreateBody(&pedalDef);
	pedal2->CreateFixture(&pedalfd);

	///Creating a revolute joint between pedal2 and pedal rod
	b2RevoluteJointDef jointDef6;
	jointDef6.Initialize(pedalbody, pedal2, pedal2->GetWorldCenter());
	m_world->CreateJoint(&jointDef6);
//////////////////////////////////////////////////////////////////////////////
	///Creating the legs of man on pedal
	b2FixtureDef legfd;
	b2PolygonShape legshape;
	legfd.filter.groupIndex = -1;
	legfd.density=0.1f;  
	legshape.SetAsBox(1.0f,8.0f);
	legfd.shape = &legshape;
	b2BodyDef legDef;
	legDef.type = b2_dynamicBody;
	legDef.position.Set(-6.0f, 12.0f);
	
	b2Body* legbody1 = m_world->CreateBody(&legDef);
	legbody1->CreateFixture(&legfd);
	
	///welding behind leg1 to pedal1
	b2WeldJointDef jointleg1Def;
	jointleg1Def.Initialize(legbody1, pedal1, pedal1->GetWorldCenter());
	m_world->CreateJoint(&jointleg1Def);
	
	legDef.position.Set(-6.0f, 20.0f);
	b2Body* legbody2 = m_world->CreateBody(&legDef);
	legbody2->CreateFixture(&legfd);
	
	///welding behind leg1 to pedal1
	b2WeldJointDef jointleg2Def;
	jointleg2Def.Initialize(legbody2, pedal2, pedal2->GetWorldCenter());
	m_world->CreateJoint(&jointleg2Def);
//////////////////////////////////////////////////////////////////////////////
	
//////////////////////////////////////////////////////////////////////////////
	///welding behind gear to behind tire
	b2WeldJointDef jointDef7;
	jointDef7.Initialize(gearbody1, wheel1, gearbody1->GetWorldCenter());
	m_world->CreateJoint(&jointDef7);
		 
	///welding behind seat to cycle frame
	b2WeldJointDef jointDef8;
	jointDef8.Initialize(seatbody, rodbody, seatbody->GetWorldCenter());
	m_world->CreateJoint(&jointDef8);
}
  sim_t *sim = new sim_t("Dominos", dominos_t::create);
}
