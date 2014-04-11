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
   b2Body* sbody;
   b2Body* sbody1;
   b2Body* wheel1;
   b2CircleShape circle4;
   	b2WeldJointDef jointDef8;
   	b2Vec2 vec;
   	b2BodyDef ballbd;
	float i=6;
         b2FixtureDef ballfd;
  void dominos_t::keyboard(unsigned char key)
  {
	  switch(key){
		case('w'):
		sbody1->ApplyAngularImpulse( -250,0 );
		break;
		case('s'):
		wheel1->ApplyAngularImpulse( 150,0 );
		break;
		case('a'):
		if(i>2)i=i-1;
      circle4.m_radius = i*.5f;
		ballfd.shape=&circle4;
		vec=b2Vec2(sbody->GetWorldCenter());
		m_world->DestroyBody(sbody);
      ballbd.type = b2_dynamicBody;
      ballbd.position=vec;
      sbody = m_world->CreateBody(&ballbd);
		sbody->CreateFixture(&ballfd);
		jointDef8.Initialize(sbody, wheel1, wheel1->GetWorldCenter());
	m_world->CreateJoint(&jointDef8);
		break;
			case('d'):
		if(i<6)i=i+1;
      circle4.m_radius = i*.5f;
		ballfd.shape=&circle4;
		vec=b2Vec2(sbody->GetWorldCenter());
		m_world->DestroyBody(sbody);
      ballbd.type = b2_dynamicBody;
      ballbd.position=vec;
      sbody = m_world->CreateBody(&ballbd);
		sbody->CreateFixture(&ballfd);
		jointDef8.Initialize(sbody, wheel1, wheel1->GetWorldCenter());
	m_world->CreateJoint(&jointDef8);
		break;
	
	}
  }
  dominos_t::dominos_t()
  {
    //Ground
    /** \par The Ground<br> 
     * Variable name : b1<br>
     * Type : b2Body<br>
     * Function : Ground<br>
     * Fixture Shape : Edge <br><br>
     */ 
    b2Body* b1;  
    {
      b2EdgeShape shape; 
      shape.Set(b2Vec2(-90.0f, 0.0f), b2Vec2(90.0f, 0.0f));
      b2BodyDef bd; 
      b2FixtureDef *grnd = new b2FixtureDef;
      grnd->filter.categoryBits = 0x00F4;
		grnd->filter.maskBits = 0x00F2;
		 grnd->shape=&shape;
      b1 = m_world->CreateBody(&bd); 
      b1->CreateFixture(grnd);
    }

		 //add wheels
		 b2CircleShape circleDef;
         circleDef.m_radius = 8;
         b2FixtureDef *fd2 = new b2FixtureDef;
         fd2->density = 0.1f;
         fd2->friction = 5;
         fd2->restitution = 0.2;
         fd2->filter.groupIndex = -1;
		 fd2->shape=new b2CircleShape;
		 fd2->shape=&circleDef;
		 fd2->filter.categoryBits = 0x00F2;
		 fd2->filter.maskBits = 0x00F4;
		 //b2Body* wheel1;
		 b2Body* wheel2;
         for (int i = 0; i < 2; i++) {
            
            b2BodyDef* bodyDef = new b2BodyDef;
            bodyDef->type = b2_dynamicBody;
            if (i == 0) bodyDef->position.Set(-20,8);
            else bodyDef->position.Set(10,8);
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
		 boxshape.SetAsBox(7,0.6,b2Vec2(7,0),0);
         b2FixtureDef *fd1 = new b2FixtureDef;
		 fd1->density = 0.1f;
		 fd1->filter.categoryBits = 0x00F2;
		 fd1->filter.maskBits = 0x00F4;
		 fd1->friction = 0.5f;
		 fd1->restitution = 0.2f;
		 fd1->filter.groupIndex = -1;
		 b2BodyDef* rodDef=new b2BodyDef;
		 rodDef->type= b2_dynamicBody;
		 rodDef->position.Set(wheel1->GetWorldCenter().x, wheel1->GetWorldCenter().y);
		 b2Body* rodbody=m_world->CreateBody(rodDef);
		 for(int i=0;i<5;i++)
		 {	
			 circleDef.m_radius = 1+i*.5f;
			 fd1->shape=&circleDef;
			 rodbody->CreateFixture(fd1);
		 }
		 fd1->shape = new b2PolygonShape;
		 fd1->shape = &boxshape;
		 rodbody->CreateFixture(fd1);
		 boxshape.SetAsBox(8,0.6,b2Vec2(8*cosf(b2_pi/3),8*sinf(b2_pi/3)),b2_pi/3);
		 rodbody->CreateFixture(fd1);
		 boxshape.SetAsBox(2.5,0.6,b2Vec2(0+(2*cosf(b2_pi/4)),(2*sinf(-b2_pi/4))),-b2_pi/4);
		 rodbody->CreateFixture(fd1);
		 boxshape.SetAsBox(8,0.6,b2Vec2(30-8*cosf(b2_pi/3),8*sinf(b2_pi/3)),-b2_pi/3);
		 rodbody->CreateFixture(fd1);
		 boxshape.SetAsBox(7,0.6,b2Vec2(15,16*sinf(b2_pi/3)),0);
		 rodbody->CreateFixture(fd1);
		 boxshape.SetAsBox(8,0.6,b2Vec2(14+8*cosf(b2_pi/3),8*sinf(b2_pi/3)),b2_pi/3);
		 rodbody->CreateFixture(fd1);
         b2RevoluteJointDef revoluteJointDef;
         revoluteJointDef.Initialize(rodbody, wheel1, wheel1->GetWorldCenter());
         b2RevoluteJoint* motor1 = (b2RevoluteJoint*)m_world->CreateJoint(&revoluteJointDef);
         revoluteJointDef.Initialize(rodbody, wheel2, wheel2->GetWorldCenter());
         b2RevoluteJoint* motor2 = (b2RevoluteJoint*)m_world->CreateJoint(&revoluteJointDef);
		b2Body* lowergearbar;
		ballbd.type = b2_dynamicBody;
        lowergearbar = m_world->CreateBody(&ballbd);
		boxshape.SetAsBox(1.5,0.6,b2Vec2((4.5*cosf(b2_pi/4))-(1.5*cosf(b2_pi/4.0)),(4.5*sinf(-b2_pi/4))-(1.5*sinf(b2_pi/4.0))),b2_pi/4.0);
		lowergearbar->CreateFixture(fd1);
      b2CircleShape circle;
      circle.m_radius = 3;
      ballfd.filter.categoryBits = 0x0002;
      ballfd.filter.maskBits = 0x0004;
      ballfd.shape = &circle;
      ballfd.density = 5.0f;
      ballfd.friction = 100.0f;
      ballfd.restitution = 0.0f;
      ballbd.type = b2_dynamicBody;
      ballbd.position.Set(-20.0f, 8.0f);
      sbody = m_world->CreateBody(&ballbd);
      sbody->CreateFixture(&ballfd);
      b2CircleShape circle1;
      circle1.m_radius = 3.0;
     b2FixtureDef ballfd1;
      ballfd1.filter.categoryBits = 0x0002;
      ballfd1.filter.maskBits = 0x0004;
      ballfd1.shape = &circle1;
      ballfd1.density = 5.0f;
      ballfd1.friction = 100.0f;
      ballfd1.restitution = 0.0f;
      b2BodyDef ballbd1;
      ballbd1.type = b2_dynamicBody;
      ballbd1.position.Set(-6.0f, 8.0f);
      sbody1 = m_world->CreateBody(&ballbd1);
      sbody1->CreateFixture(&ballfd1);
      b2Body* sbody3;
      b2CircleShape circle3;
      circle3.m_radius = 0.75;
      b2FixtureDef ballfdc3;
      ballfdc3.filter.categoryBits = 0x0002;
      ballfdc3.filter.maskBits = 0x0004;
      ballfdc3.shape = &circle3;
      ballfdc3.density = 5.0f;
      ballfdc3.friction = 100.0f;
      ballfdc3.restitution = 0.0f;
      b2BodyDef ballbdc3;
      ballbdc3.type = b2_dynamicBody;
      ballbdc3.position.Set(-20.0f+(4.5*cosf(b2_pi/4)), 8.0f+(4.5*sinf(-b2_pi/4)));
      sbody3 = m_world->CreateBody(&ballbdc3);
      sbody3->CreateFixture(&ballfdc3);
      b2Body* sbody4;
      ballbdc3.position.Set(-20.0f+(4.5*cosf(b2_pi/4))-(3*cosf(b2_pi/4.0f)), 8.0f+(4.5*sinf(-b2_pi/4))-(3*sinf(b2_pi/4.0f)));
      sbody4 = m_world->CreateBody(&ballbdc3);
      sbody4->CreateFixture(&ballfdc3);
      b2Body* b2;
      b2FixtureDef ballfd2;
	ballfd2.filter.categoryBits = 0x0002;
      ballfd2.filter.maskBits = 0x0004;
      b2PolygonShape shape;
	shape.SetAsBox(12.25f, 0.6f);
	ballfd2.shape = &shape;  
	b2BodyDef bd;
	bd.position.Set(-8.0f, 8.0f);
	b2RevoluteJointDef jointDef;
	jointDef.Initialize(sbody, rodbody, sbody->GetWorldCenter());
		m_world->CreateJoint(&jointDef);
	b2RevoluteJointDef jointDef1;
	jointDef1.Initialize(sbody1, rodbody, sbody1->GetWorldCenter());
m_world->CreateJoint(&jointDef1);
b2RevoluteJointDef jointDefc3;
	jointDefc3.Initialize(sbody3, rodbody, sbody3->GetWorldCenter());
m_world->CreateJoint(&jointDefc3);
b2RevoluteJointDef jointDefc4;
	jointDefc4.Initialize(sbody4, lowergearbar, sbody4->GetWorldCenter());
m_world->CreateJoint(&jointDefc4);
b2RevoluteJointDef jointDefc5;
	jointDefc5.Initialize(rodbody, lowergearbar, sbody3->GetWorldCenter());
m_world->CreateJoint(&jointDefc5);
b2Vec2 vs[100];
b2Body* conveyer[100];
    b2FixtureDef ballfd4;
   ballfd4.filter.categoryBits = 0x0004;
      ballfd4.filter.maskBits = 0x0002;ballfd4.filter.groupIndex = -1;
      ballfd4.density=5.1f;
	ballfd4.friction=100.0f;  
 for (int i = 0; i < 16; ++i)
	{
	vs[i].Set(-21.0f+1.0f*i,9.0f);
	b2PolygonShape shape;
	shape.SetAsBox(.5f, 0.25f);
	ballfd4.shape = &shape;
	b2BodyDef bd4;
	bd4.type = b2_dynamicBody;
	bd4.position.Set(-20.5f+1.0f*i,9.0f);
	conveyer[i] = m_world->CreateBody(&bd4);
	conveyer[i]->CreateFixture(&ballfd4);
	}
 for (int i = 0; i < 4; ++i)
	{
		vs[16+i].Set(-5.0f,9.0f-i*1.0f);
b2PolygonShape shape;
	shape.SetAsBox(.25f, 0.5f);
	ballfd4.shape = &shape;  
	b2BodyDef bd4;
	bd4.type = b2_dynamicBody;
	bd4.position.Set(-5.0f,8.5f-i*1.0f);
	conveyer[i+16] = m_world->CreateBody(&bd4);
	conveyer[i+16]->CreateFixture(&ballfd4);
	}
 for (int i = 0; i < 10; ++i)
	{
		vs[20+i].Set(-5.0f-1.0f*i,5.0f);
	b2PolygonShape shape;
	shape.SetAsBox(.5f, 0.25f);
	ballfd4.shape = &shape;  
	b2BodyDef bd4;
	bd4.type = b2_dynamicBody;
	bd4.position.Set(-5.5f-1.0f*i,5.0f);
	conveyer[i+20] = m_world->CreateBody(&bd4);
	conveyer[i+20]->CreateFixture(&ballfd4);		
	}
 for (int i = 0; i < 3; ++i)
	{
		vs[30+i].Set(-15.0f,5.0f-1.0f*i);
	b2PolygonShape shape;
	shape.SetAsBox(.25f, 0.55f);
	ballfd4.shape = &shape;  
	b2BodyDef bd4;
	bd4.type = b2_dynamicBody;
	bd4.position.Set(-15.0f,4.5f-1.0f*i);
	conveyer[i+30] = m_world->CreateBody(&bd4);
	conveyer[i+30]->CreateFixture(&ballfd4);
	}
 for (int i = 0; i < 4; ++i)
	{
		vs[33+i].Set(-15.0f-1.0f*i,2.0f);
	b2PolygonShape shape;
	shape.SetAsBox(.55f, 0.25f);
	ballfd4.shape = &shape;  
	b2BodyDef bd4;
	bd4.type = b2_dynamicBody;
	bd4.position.Set(-15.5f-1.0f*i,2.0f);
	conveyer[i+33] = m_world->CreateBody(&bd4);
	conveyer[i+33]->CreateFixture(&ballfd4);
	} 
	for (int i = 0; i < 2; ++i)
	{
		vs[37+i].Set(-19.0f,2.0f+1.0f*i);
	b2PolygonShape shape;
	shape.SetAsBox(.25f, 0.55f);
	ballfd4.shape = &shape;  
	b2BodyDef bd4;
	bd4.type = b2_dynamicBody;
	bd4.position.Set(-19.0f,2.5f+1.0f*i);
	conveyer[i+37] = m_world->CreateBody(&bd4);
	conveyer[i+37]->CreateFixture(&ballfd4);
	} 
	for (int i = 0; i < 3; ++i)
	{
		vs[39+i].Set(-19.0f+1.0f*i,4.0f);
	b2PolygonShape shape;
	shape.SetAsBox(.55f, 0.25f);
	ballfd4.shape = &shape;  
	b2BodyDef bd4;
	bd4.type = b2_dynamicBody;
	bd4.position.Set(-18.5f+1.0f*i,4.0f);
	conveyer[i+39] = m_world->CreateBody(&bd4);
	conveyer[i+39]->CreateFixture(&ballfd4);
	}
	for (int i = 0; i < 2; ++i)
	{
		vs[42+i].Set(-16.0f,4.0f+1.0f*i);
	b2PolygonShape shape;
	shape.SetAsBox(.25f, 0.55f);
	ballfd4.shape = &shape;  
	b2BodyDef bd4;
	bd4.type = b2_dynamicBody;
	bd4.position.Set(-16.0f,4.5f+1.0f*i);
	conveyer[i+42] = m_world->CreateBody(&bd4);
	conveyer[i+42]->CreateFixture(&ballfd4);
	}
	for (int i = 0; i < 5; ++i)
	{
		vs[44+i].Set(-16.0f-1.0f*i,6.0f);
	b2PolygonShape shape;
	shape.SetAsBox(.55f, 0.25f);
	ballfd4.shape = &shape;  
	b2BodyDef bd4;
	bd4.type = b2_dynamicBody;
	bd4.position.Set(-16.5f-1.0f*i,6.0f);
	conveyer[i+44] = m_world->CreateBody(&bd4);
	conveyer[i+44]->CreateFixture(&ballfd4);
	}
		for (int i = 0; i < 3; ++i)
	{
		vs[49+i].Set(-21.0f,6.0f+1.0f*i);
	b2PolygonShape shape;
	shape.SetAsBox(.25f, 0.55f);
	ballfd4.shape = &shape;  
	b2BodyDef bd4;
	bd4.type = b2_dynamicBody;
	bd4.position.Set(-21.0f,6.5f+1.0f*i);
	conveyer[i+49] = m_world->CreateBody(&bd4);
	conveyer[i+49]->CreateFixture(&ballfd4);
	}
	for(int i=0;i<0;i++)
	{
			vs[53+i].Set(-21.0f,10.0f);
	b2PolygonShape shape;
	shape.SetAsBox(.25f, 0.55f);
	ballfd4.shape = &shape;  
	b2BodyDef bd4;
	bd4.type = b2_dynamicBody;
	bd4.position.Set(-21.0f,9.5f);
	conveyer[i+53] = m_world->CreateBody(&bd4);
	conveyer[i+53]->CreateFixture(&ballfd4);
	}
for(int i=1;i<52;i++)
{
	b2RevoluteJointDef jointDef3;
	jointDef3.Initialize(conveyer[i-1], conveyer[i],vs[i]);
	m_world->CreateJoint(&jointDef3);
}
b2RevoluteJointDef jointDef3;
	jointDef3.Initialize(conveyer[0], conveyer[51],vs[0]);
	m_world->CreateJoint(&jointDef3);
	b2Body* b5;
      b2FixtureDef ballfd5;
	ballfd5.filter.categoryBits = 0x0003;
      ballfd5.filter.maskBits = 0x0001;
      ballfd5.filter.groupIndex=-1;
      b2PolygonShape shape5;
	shape5.SetAsBox(.25f, 4.5f);
	ballfd5.shape = &shape5;
	ballfd5.density=2.0f;  
	b2BodyDef bd5;
	bd5.type = b2_dynamicBody;
	bd5.position.Set(-6.0f, 8.0f);
	b5 = m_world->CreateBody(&bd5);
	b5->CreateFixture(&ballfd5);
	b2WeldJointDef jointDef5;
	jointDef5.Initialize(sbody1, b5, sbody1->GetWorldCenter());
	m_world->CreateJoint(&jointDef5);
	b2Body* b6;
      b2FixtureDef ballfd6;
	ballfd5.filter.categoryBits = 0x0008;
      ballfd5.filter.maskBits = 0x0008;
      ballfd6.filter.groupIndex=-1;
      b2PolygonShape shape6;
	shape6.SetAsBox(1.25f, .25f);
	ballfd6.shape = &shape6;  
	b2BodyDef bd6;
	bd6.fixedRotation=true;
	bd6.type = b2_dynamicBody;
	bd6.position.Set(-6.0f, 4.0f);
	b6 = m_world->CreateBody(&bd6);
	b6->CreateFixture(&ballfd6);
	b2RevoluteJointDef jointDef6;
	jointDef6.Initialize(b5, b6, b6->GetWorldCenter());
	m_world->CreateJoint(&jointDef6);
	b2Body* b7;
      b2FixtureDef ballfd7;
	ballfd5.filter.categoryBits = 0x0008;
      ballfd5.filter.maskBits = 0x0008;
      ballfd7.filter.groupIndex=-1;
      b2PolygonShape shape7;
	shape7.SetAsBox(1.25f, .25f);
	ballfd7.shape = &shape7;  
	b2BodyDef bd7;
	bd7.fixedRotation=true;
	bd7.type = b2_dynamicBody;
	bd7.position.Set(-6.0f, 12.0f);
	b7 = m_world->CreateBody(&bd7);
	b7->CreateFixture(&ballfd7);
	b2RevoluteJointDef jointDef7;
	jointDef7.Initialize(b5, b7, b7->GetWorldCenter());
	m_world->CreateJoint(&jointDef7);
	jointDef8.Initialize(sbody, wheel1, sbody->GetWorldCenter());
	m_world->CreateJoint(&jointDef8);
	b2DistanceJointDef jointDef9;
	jointDef9.Initialize(lowergearbar, rodbody, lowergearbar->GetWorldCenter(),lowergearbar->GetWorldCenter());	
	m_world->CreateJoint(&jointDef9);
		b2DistanceJointDef jointDef10;
	jointDef10.Initialize(lowergearbar, rodbody, lowergearbar->GetWorldCenter(),lowergearbar->GetWorldCenter());	
	m_world->CreateJoint(&jointDef10);
		b2DistanceJointDef jointDef11;
	jointDef11.Initialize(lowergearbar, rodbody, lowergearbar->GetWorldCenter(),lowergearbar->GetWorldCenter());
	
	m_world->CreateJoint(&jointDef11);
			b2DistanceJointDef jointDef14;
	jointDef14.Initialize(lowergearbar, rodbody, lowergearbar->GetWorldCenter(),lowergearbar->GetWorldCenter());
	
	m_world->CreateJoint(&jointDef14);
			b2DistanceJointDef jointDef13;
	jointDef13.Initialize(lowergearbar, rodbody, lowergearbar->GetWorldCenter(),lowergearbar->GetWorldCenter());
	
	m_world->CreateJoint(&jointDef13);
	
	
	
	///bhargava
	
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
ubodypts[1].Set(-3.0f, 6.0f);
ubodypts[2].Set(2.0f, 15.0f);
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
jointDef_ubody1.lowerAngle =-0.02*b2_pi;
    jointDef_ubody1.upperAngle = 0.125f * b2_pi;
    jointDef_ubody1.enableLimit = true;
m_world->CreateJoint(&jointDef_ubody1);

///Distance Joint behind ubody1 and handle
b2DistanceJointDef jointDef_ubdhandle;
jointDef_ubdhandle.Initialize(ubody1,handlebody,ubody1->GetWorldCenter(),handlebody->GetWorldCenter());
jointDef_ubdhandle.frequencyHz = 2.5f;
jointDef_ubdhandle.dampingRatio = 0.4f;
m_world->CreateJoint(&jointDef_ubdhandle);
ubodyDef.position.Set(seatbody->GetPosition().x,seatbody->GetPosition().y+1.5f);
//////////////////////////////////////////////////////////////////////////////
///Adding head to upper body
ubodyfd.shape = new b2CircleShape;
b2CircleShape headcircle;
headcircle.m_radius = 2.5;
ubodyfd.shape=&headcircle;
///Creating head of man
ubodyDef.position.Set(ubody1->GetPosition().x+3.5+0.5,ubody1->GetPosition().y+14.5f+2.5);
b2Body* uhead = m_world->CreateBody(&ubodyDef);
uhead->CreateFixture(&ubodyfd);
///Adding Revolute Joints between head and ubody1
b2RevoluteJointDef jointDef_uhead;
jointDef_uhead.Initialize(uhead, ubody1, b2Vec2(ubody1->GetPosition().x+3.5,ubody1->GetPosition().y+14.5f));
jointDef_uhead.lowerAngle =-0.02*b2_pi;
    jointDef_uhead.upperAngle = 0.125f * b2_pi;
    jointDef_uhead.enableLimit = true;
m_world->CreateJoint(&jointDef_uhead);
///Distance Joint behind ubody1 and handle
b2DistanceJointDef jointDef_ubdhead;
b2Vec2 disthead;
disthead.Set(ubody1->GetWorldCenter().x+6,ubody1->GetWorldCenter().y+4);
jointDef_ubdhead.Initialize(uhead,ubody1,uhead->GetWorldCenter(),disthead);
jointDef_ubdhead.frequencyHz = 1.0f;
jointDef_ubdhead.dampingRatio = 0.4f;
m_world->CreateJoint(&jointDef_ubdhead);
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

///Creating 2nd optional hand11 (shoulder arm)
handDef.position.Set(ubody1->GetPosition().x+4.0f+1.0f,ubody1->GetPosition().y+12.0f+0.5f);
b2Body* handbody11 = m_world->CreateBody(&handDef);
handbody11->CreateFixture(&handfd);
///Adding Revolute joint between upper body (ubody1) and handbody1
b2RevoluteJointDef jointhand1Def;
b2Vec2 handpos1;
handpos1.Set(ubody1->GetPosition().x+4.0f,ubody1->GetPosition().y+12.0f);
jointhand1Def.Initialize(handbody1, ubody1,handpos1);
m_world->CreateJoint(&jointhand1Def);

///Creating 2nd optional Revolute joint between upper body (ubody1) and handbody11
handpos1.Set(ubody1->GetPosition().x+4.0f+1.0f,ubody1->GetPosition().y+12.0f+0.5f);
b2RevoluteJointDef jointhand11Def;
jointhand11Def.Initialize(handbody11, ubody1,handpos1);
m_world->CreateJoint(&jointhand11Def);
///Creating hand2 (hand arm)
handDef.position.Set(handlebody->GetWorldCenter().x,handlebody->GetWorldCenter().y);
handshape.SetAsBox(5.0f,1.0f,b2Vec2(-5*cosf(b2_pi/12),5*sinf(b2_pi/12)),-b2_pi/12);
b2Body* handbody2 = m_world->CreateBody(&handDef);
handbody2->CreateFixture(&handfd);

///Creating 2nd optional hand22 (hand arm)
handDef.position.Set(handlebody->GetWorldCenter().x+1.0f,handlebody->GetWorldCenter().y+0.5f);
b2Body* handbody22 = m_world->CreateBody(&handDef);
handbody22->CreateFixture(&handfd);
///Adding Revolute joint between upper body (ubody1) and handbody2
b2RevoluteJointDef jointhand2Def;
b2Vec2 handpos2;
handpos2.Set(handlebody->GetWorldCenter().x,handlebody->GetWorldCenter().y);
jointhand2Def.Initialize(handbody2, handlebody,handpos2);
m_world->CreateJoint(&jointhand2Def);

///Creating 2nd optional Revolute joint between upper body (ubody1) and handbody22
handpos2.Set(handlebody->GetWorldCenter().x+1.0f,handlebody->GetWorldCenter().y+0.5f);
b2RevoluteJointDef jointhand22Def;
jointhand22Def.Initialize(handbody22, handlebody,handpos2);
m_world->CreateJoint(&jointhand22Def);
///Adding Revolute joint between handbody1 and handbody2
b2RevoluteJointDef jointDef_hands;
b2Vec2 hands_pos;
hands_pos.Set(handbody1->GetWorldCenter().x+4*cosf(b2_pi/3),handbody1->GetWorldCenter().y-4*sinf(b2_pi/3));
jointDef_hands.Initialize(handbody1, handbody2,hands_pos);
m_world->CreateJoint(&jointDef_hands);

///Creating 2nd optional Revolute joint between handbody11 and handbody22
b2RevoluteJointDef jointDef_hands1;
b2Vec2 hands_pos1;
hands_pos1.Set(handbody11->GetWorldCenter().x+4*cosf(b2_pi/3),handbody11->GetWorldCenter().y-4*sinf(b2_pi/3));
jointDef_hands1.Initialize(handbody11, handbody22,hands_pos1);
m_world->CreateJoint(&jointDef_hands1);
	
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
footpos1.Set(b6->GetWorldCenter().x,b6->GetWorldCenter().y+0.25f);
jointleg1Def.Initialize(legbody1, b6,footpos1,footpos1);
m_world->CreateJoint(&jointleg1Def);

legDef.position.Set(-6.0f, 12.0f);
legshape.SetAsBox(1.0f,8.0f,b2Vec2(8*cosf(b2_pi/2-b2_pi/15),8*sinf(b2_pi/2-b2_pi/15)),-b2_pi/15);
b2Body* legbody2 = m_world->CreateBody(&legDef);
legbody2->CreateFixture(&legfd);

///Distance Joint behind leg2 and pedal2
b2DistanceJointDef jointleg2Def;
b2Vec2 footpos2;
footpos2.Set(b7->GetWorldCenter().x,b7->GetWorldCenter().y+0.25f);
jointleg2Def.Initialize(legbody2, b7,footpos2,footpos2);
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
	
		b2WeldJointDef jointDef12;
jointDef12.Initialize(seatbody, rodbody, seatbody->GetWorldCenter());
m_world->CreateJoint(&jointDef12);
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}
  sim_t *sim = new sim_t("Dominos", dominos_t::create);
}
