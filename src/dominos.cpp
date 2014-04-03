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
		frontgear->ApplyAngularImpulse( -400,0 );
		break;
		case('s'):
		wheel1->ApplyAngularImpulse( 200,0 );
		wheel2->ApplyAngularImpulse( 200,0 );
		break;
	}
	}
dominos_t::dominos_t()
{
/////////////////////////////////////////////////////////////////////////////
	///Ground
	{
	  b2EdgeShape shape; 
	  shape.Set(b2Vec2(-90.0f, -3.0f), b2Vec2(90.0f, -3.0f)); 
	  b2FixtureDef *grnd = new b2FixtureDef;
	  grnd->filter.groupIndex = -5;//
	  grnd->shape=&shape;
	  grnd->friction=1;
	  b2BodyDef bd;
	  b2Body* b1=m_world->CreateBody(&bd); 
	  b1->CreateFixture(grnd);
	  
	  b2PolygonShape shape1;
	  shape1.SetAsBox(1,4,b2Vec2(50,0),0); 
	  grnd->shape=&shape1;
	  b1->CreateFixture(grnd);
	}
//////////////////////////////////////////////////////////////////////////////
	 ///Add wheels
	 b2CircleShape circleDef;
	 circleDef.m_radius = 8;
	 b2FixtureDef *fd2 = new b2FixtureDef;
	 fd2->density = 2.0f;
	 fd2->friction = 1;
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
	 fd1->density = 1.5f;
	 fd1->friction = 0.5f;
	 fd1->restitution = 0.2f;
	 fd1->filter.groupIndex = -1;//
	 fd1->shape = new b2PolygonShape;
	 fd1->shape = &boxshape;
	 b2BodyDef* rodDef=new b2BodyDef;
	 rodDef->type= b2_dynamicBody;
	 ///0* rod between gears
	 rodDef->position.Set(wheel1->GetWorldCenter().x, wheel1->GetWorldCenter().y);
	 b2Body* rodbody=m_world->CreateBody(rodDef);
	 rodbody->CreateFixture(fd1);
	 ///60* back wheel rod
	 boxshape.SetAsBox(8,0.6,b2Vec2(8*cosf(b2_pi/3),8*sinf(b2_pi/3)),b2_pi/3);
	 rodbody->CreateFixture(fd1);
	 ///-60* front wheel rod
	 boxshape.SetAsBox(8,0.6,b2Vec2(30-8*cosf(b2_pi/3),8*sinf(b2_pi/3)),-b2_pi/3);
	 rodbody->CreateFixture(fd1);
	 ///0* Top cycle rod
	 boxshape.SetAsBox(7,0.6,b2Vec2(15,16*sinf(b2_pi/3)),0);
	 rodbody->CreateFixture(fd1);
	 ///60* front cycle rod
	 boxshape.SetAsBox(8,0.6,b2Vec2(14+8*cosf(b2_pi/3),8*sinf(b2_pi/3)),b2_pi/3);
	 rodbody->CreateFixture(fd1);
	 ///seat rod
	 boxshape.SetAsBox(1.5,0.4,b2Vec2((16-1.5)*cosf(b2_pi/3),(16+1.5)*sinf(b2_pi/3)),-b2_pi/3);
	 rodbody->CreateFixture(fd1);
	 ///-60* front cycle small handle
	 boxshape.SetAsBox(1.5,0.6,b2Vec2(22-1.5f*cosf(b2_pi/3),(16+1.5)*sinf(b2_pi/3)),-b2_pi/3);
	 rodbody->CreateFixture(fd1);
	 ///60* front cycle small handle
	 boxshape.SetAsBox(2.5,0.5,b2Vec2(22-3*cosf(b2_pi/3)+2.0*cosf(b2_pi/4),(16+3)*sinf(b2_pi/3)+2.0*sinf(b2_pi/4)),b2_pi/4);
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
	 vertices[4].Set(-2.0f, -0.1f);
	 b2PolygonShape polygon;
	 boxshape.Set(vertices, 5);
	 seatbody->CreateFixture(fd1);
//////////////////////////////////////////////////////////////////////////////
     ///handles on cycle
	 b2BodyDef* handleDef=new b2BodyDef;
	 handleDef->type= b2_dynamicBody;
	 handleDef->fixedRotation=true;
	 handleDef->position.Set(wheel1->GetWorldCenter().x+22-3*cosf(b2_pi/3)+4.5*cosf(b2_pi/4), wheel1->GetWorldCenter().y+(16+3)*sinf(b2_pi/3)+4.5*sinf(b2_pi/4));
	 b2Body* handlebody=m_world->CreateBody(handleDef);
	 
	 fd1->shape = new b2CircleShape;
	 b2CircleShape handlecircle;
	 handlecircle.m_radius = 1.0;
	 fd1->shape=&handlecircle;
	 handlebody->CreateFixture(fd1);
	 
	 ///Adding Revolute Joints between thigh1 and seat
	 b2WeldJointDef jointDef_handle;
	 jointDef_handle.Initialize(handlebody, rodbody, handlebody->GetWorldCenter());
	 m_world->CreateJoint(&jointDef_handle);
//////////////////////////////////////////////////////////////////////////////
	///Creating the thighs of man on seat
	b2FixtureDef thighfd;
	b2PolygonShape thighshape;
	thighfd.filter.groupIndex = -1;
	thighfd.density=0.1f;  
	thighshape.SetAsBox(1.0f,6.0f,b2Vec2(6*cosf(b2_pi/2-b2_pi/4),-6*sinf(b2_pi/2-b2_pi/4)),b2_pi/4);
    thighfd.shape = &thighshape;
	b2BodyDef thighDef;
	thighDef.type = b2_dynamicBody;
	thighDef.position.Set(seatbody->GetPosition().x,seatbody->GetPosition().y+2.0f);
	///Creating thigh1
	b2Body* thighbody1 = m_world->CreateBody(&thighDef);
	thighbody1->CreateFixture(&thighfd);
	///Adding Revolute Joints between thigh1 and seat
	b2RevoluteJointDef jointDef_thigh1;
	b2Vec2 seattopvec;
	seattopvec.Set(seatbody->GetPosition().x,seatbody->GetPosition().y+2.0f);
	jointDef_thigh1.Initialize(thighbody1, seatbody, seattopvec);
	m_world->CreateJoint(&jointDef_thigh1);
	
	///creating thigh2
	thighshape.SetAsBox(6.0f,1.0f,b2Vec2(6*cosf(0),6*sinf(0)),0);
	b2Body* thighbody2 = m_world->CreateBody(&thighDef);
	thighbody2->CreateFixture(&thighfd);

	///Adding Revolute Joints between thigh1 and seat
	b2RevoluteJointDef jointDef_thigh2;
	seattopvec.Set(seatbody->GetPosition().x+0.5f,seatbody->GetPosition().y+2.0f);
	jointDef_thigh2.Initialize(thighbody2, seatbody, seattopvec);
	m_world->CreateJoint(&jointDef_thigh2);
////////////////////////////////////////////////////////////////////////////// 
	///Creating upper body of the man
	b2FixtureDef ubodyfd;
	b2PolygonShape ubodyshape;
	ubodyfd.filter.groupIndex = -1;
	ubodyfd.density=1.0f;  
	b2Vec2 ubodypts[6];
	ubodypts[0].Set(-2.0f, -0.2f);
	ubodypts[1].Set(-4.0f, 6.0f);
	ubodypts[2].Set(1.0f, 15.0f);
	ubodypts[3].Set(6.0f, 14.0f);
	ubodypts[4].Set(6.0f, 6.0f);
	ubodypts[5].Set(3.0f,-0.2f);
	ubodyshape.Set(ubodypts, 6);
	ubodyfd.shape=&ubodyshape;
	b2BodyDef ubodyDef;
	ubodyDef.type = b2_dynamicBody;
	ubodyDef.position.Set(seatbody->GetPosition().x,seatbody->GetPosition().y+1.5f);
	///Creating upperbody
	b2Body* ubody1 = m_world->CreateBody(&ubodyDef);
	ubody1->CreateFixture(&ubodyfd);
	
	///Adding Revolute Joints between ubody1 and seat
	b2RevoluteJointDef jointDef_ubody1;
	jointDef_ubody1.Initialize(ubody1, seatbody, ubody1->GetPosition());
	jointDef_ubody1.lowerAngle =-0.02*b2_pi; // -90 degrees
    jointDef_ubody1.upperAngle = 0.125f * b2_pi; // 45 degrees
    jointDef_ubody1.enableLimit = true;
	m_world->CreateJoint(&jointDef_ubody1);

	///Distance Joint behind ubody1 and handle
	b2DistanceJointDef jointDef_ubdhandle;
	jointDef_ubdhandle.Initialize(ubody1,handlebody,ubody1->GetWorldCenter(),handlebody->GetWorldCenter());
	jointDef_ubdhandle.frequencyHz = 2.5f;
	jointDef_ubdhandle.dampingRatio = 0.4f;
	m_world->CreateJoint(&jointDef_ubdhandle);
//////////////////////////////////////////////////////////////////////////////  
	///Adding hands to upper body
	b2FixtureDef handfd;
	b2PolygonShape handshape;
	handfd.filter.groupIndex = -1;
	handfd.density=0.1f;  
	handshape.SetAsBox(5.0f,1.0f,b2Vec2(5*cosf(b2_pi/3),-5*sinf(b2_pi/3)),-b2_pi/3	);
	handfd.shape = &handshape;
	b2BodyDef handDef;
	handDef.type = b2_dynamicBody;
	handDef.position.Set(ubody1->GetPosition().x+4.0f,ubody1->GetPosition().y+12.0f);
	
	///Creating hand1 (shoulder arm)
	b2Body* handbody1 = m_world->CreateBody(&handDef);
	handbody1->CreateFixture(&handfd);
	
	///Adding Revolute joint between upper body (ubody1) and handbody1
	b2RevoluteJointDef jointhand1Def;
	b2Vec2 handpos1;
	handpos1.Set(ubody1->GetPosition().x+4.0f,ubody1->GetPosition().y+12.0f);
	jointhand1Def.Initialize(handbody1, ubody1,handpos1);
	m_world->CreateJoint(&jointhand1Def);
	
	///Creating hand2 (hand arm)
	handDef.position.Set(handlebody->GetWorldCenter().x,handlebody->GetWorldCenter().y);
	handshape.SetAsBox(5.0f,1.0f,b2Vec2(-5*cosf(b2_pi/12),5*sinf(b2_pi/12)),-b2_pi/12);
	b2Body* handbody2 = m_world->CreateBody(&handDef);
	handbody2->CreateFixture(&handfd);
	
	///Adding Revolute joint between upper body (ubody1) and handbody2
	b2RevoluteJointDef jointhand2Def;
	b2Vec2 handpos2;
	handpos2.Set(handlebody->GetWorldCenter().x,handlebody->GetWorldCenter().y);
	jointhand2Def.Initialize(handbody2, handlebody,handpos2);
	m_world->CreateJoint(&jointhand2Def);
	
	///Adding Revolute joint between handbody1 and handbody2
	b2RevoluteJointDef jointDef_hands;
	b2Vec2 hands_pos;
	hands_pos.Set(handbody1->GetWorldCenter().x+4*cosf(b2_pi/3),handbody1->GetWorldCenter().y-4*sinf(b2_pi/3));
	jointDef_hands.Initialize(handbody1, handbody2,hands_pos);
	m_world->CreateJoint(&jointDef_hands);
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
	gearfd.friction = 1.0f;
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
	chainfd.density=5.0f;
	chainfd.friction=1.0f;
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
	
	///Distance Joint behind leg1 and pedal1
	b2DistanceJointDef jointleg1Def;
	b2Vec2 footpos1;
	footpos1.Set(pedal1->GetWorldCenter().x,pedal1->GetWorldCenter().y+0.25f);
	jointleg1Def.Initialize(legbody1, pedal1,footpos1,footpos1);
	m_world->CreateJoint(&jointleg1Def);
	
	legDef.position.Set(-6.0f, 12.0f);
	legshape.SetAsBox(1.0f,8.0f,b2Vec2(8*cosf(b2_pi/2-b2_pi/15),8*sinf(b2_pi/2-b2_pi/15)),-b2_pi/15);
	b2Body* legbody2 = m_world->CreateBody(&legDef);
	legbody2->CreateFixture(&legfd);
	
	///Distance Joint behind leg2 and pedal2
	b2DistanceJointDef jointleg2Def;
	b2Vec2 footpos2;
	footpos2.Set(pedal2->GetWorldCenter().x,pedal2->GetWorldCenter().y+0.25f);
	jointleg2Def.Initialize(legbody2, pedal2,footpos2,footpos2);
	m_world->CreateJoint(&jointleg2Def);
//////////////////////////////////////////////////////////////////////////////

	///RevoluteJoint between thigh1 and leg1
	b2RevoluteJointDef jointDef_thleg1;
	b2Vec2 kneepos1;
	kneepos1.Set(legbody1->GetWorldCenter().x,legbody1->GetWorldCenter().y+7.0f);
	jointDef_thleg1.Initialize(thighbody1, legbody1, kneepos1);
	m_world->CreateJoint(&jointDef_thleg1);
	
	///RevoluteJoint between thigh2 and leg2
	b2RevoluteJointDef jointDef_thleg2;
	b2Vec2 kneepos2;
	kneepos2.Set(legbody2->GetWorldCenter().x+7.0f*cosf(b2_pi/2-b2_pi/15),legbody2->GetWorldCenter().y+7.0f*sinf(b2_pi/2-b2_pi/15));
	jointDef_thleg2.Initialize(thighbody2, legbody2, kneepos2);
	m_world->CreateJoint(&jointDef_thleg2);

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
