
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
   b2Body* sbody1;
  void dominos_t::keyboard(unsigned char key)
  {
	  switch(key){
		case('w'):
		sbody1->ApplyAngularImpulse( -20,0 );
		break;
		case('s'):
		sbody1->ApplyAngularImpulse( 20,0 );
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
      shape.Set(b2Vec2(-90.0f, -2.5f), b2Vec2(90.0f, -2.5f));
      b2BodyDef bd; 
      b1 = m_world->CreateBody(&bd); 
      b1->CreateFixture(&shape, 0.0f);
    }
          

           b2Body* sbody;
      b2CircleShape circle;
      circle.m_radius = 3.0;
	
      b2FixtureDef ballfd;
      //ballfd.filter.groupIndex = -2;
      ballfd.filter.categoryBits = 0x0002;
      ballfd.filter.maskBits = 0x0004;
      ballfd.shape = &circle;
      ballfd.density = 50.0f;
      ballfd.friction = 100.0f;
      ballfd.restitution = 0.0f;
      b2BodyDef ballbd;
      ballbd.type = b2_dynamicBody;
      ballbd.position.Set(9.5f, 3.0f);
      sbody = m_world->CreateBody(&ballbd);
      sbody->CreateFixture(&ballfd);
      
	             
      b2CircleShape circle1;
      circle1.m_radius = 3.0;
	
      b2FixtureDef ballfd1;
      //ballfd1.filter.groupIndex = -2;
      ballfd1.filter.categoryBits = 0x0002;
      ballfd1.filter.maskBits = 0x0004;
      ballfd1.shape = &circle1;
      ballfd1.density = 50.0f;
      ballfd1.friction = 100.0f;
      ballfd1.restitution = 0.0f;
      b2BodyDef ballbd1;
      ballbd1.type = b2_dynamicBody;
      
      ballbd1.position.Set(18.5f, 3.0f);
      sbody1 = m_world->CreateBody(&ballbd1);
      sbody1->CreateFixture(&ballfd1);
      //sbody1->SetAngularVelocity(1.2f);
      //sbody->SetAngularVelocity(1.0f);
      
      b2Body* b2;
      b2FixtureDef ballfd2;
      //ballfd2.filter.groupIndex = -2;
	ballfd2.filter.categoryBits = 0x0002;
      ballfd2.filter.maskBits = 0x0004;
      b2PolygonShape shape;
	shape.SetAsBox(4.5f, 0.25f);
	ballfd2.shape = &shape;  
	b2BodyDef bd;
	//bd.type = b2_dynamicBody;
	bd.position.Set(14.0f, 3.0f);
	b2 = m_world->CreateBody(&bd);
	b2->CreateFixture(&ballfd2);
	b2RevoluteJointDef jointDef;
	jointDef.Initialize(sbody, b2, sbody->GetWorldCenter());
		m_world->CreateJoint(&jointDef);


	b2RevoluteJointDef jointDef1;
	jointDef1.Initialize(sbody1, b2, sbody1->GetWorldCenter());
m_world->CreateJoint(&jointDef1);
b2Vec2 vs[72];
b2Body* conveyer[72];

 for (int i = 0; i < 24; ++i)
	{
	vs[i].Set(8.25f+.5f*i,6.0f);
    b2FixtureDef ballfd4;
    ballfd4.filter.categoryBits = 0x0004;
      ballfd4.filter.maskBits = 0x0002;
      
    //ballfd2.filter.groupIndex = -2;
	b2PolygonShape shape;
	shape.SetAsBox(.25f, 0.25f);
	ballfd4.shape = &shape;
	ballfd4.density=0.1f;
	ballfd.friction=100.0f;  
	b2BodyDef bd4;
	bd4.type = b2_dynamicBody;
	bd4.position.Set(8.25f+.5f*i,6.0f);
	conveyer[i] = m_world->CreateBody(&bd4);
	conveyer[i]->CreateFixture(&ballfd4);
	}
 for (int i = 0; i < 12; ++i)
	{
		vs[24+i].Set(20.0f,6.0f-i*.5f);
		b2FixtureDef ballfd4;
		
    ballfd4.filter.categoryBits = 0x0004;
      ballfd4.filter.maskBits = 0x0002;
    //ballfd2.filter.groupIndex = -2;
	b2PolygonShape shape;
	shape.SetAsBox(.25f, 0.25f);
	ballfd4.shape = &shape;  
		ballfd4.density=0.1f;
	ballfd.friction=100.0f;
	b2BodyDef bd4;
	bd4.type = b2_dynamicBody;
	bd4.position.Set(20.0f,5.75f-i*.5f);
	conveyer[i+24] = m_world->CreateBody(&bd4);
	conveyer[i+24]->CreateFixture(&ballfd4);
	}
 for (int i = 0; i < 24; ++i)
	{
		vs[36+i].Set(20.0f-.5f*i,0.0f);
		
    b2FixtureDef ballfd4;
    ballfd4.filter.categoryBits = 0x0004;
      ballfd4.filter.maskBits = 0x0002;
    //ballfd2.filter.groupIndex = -2;
	b2PolygonShape shape;
	shape.SetAsBox(.25f, 0.25f);
	ballfd4.shape = &shape;  
	b2BodyDef bd4;
	ballfd4.density=0.1f;
	ballfd.friction=100.0f;  
	bd4.type = b2_dynamicBody;
	bd4.position.Set(19.75f-.5f*i,0.0f);
	conveyer[i+36] = m_world->CreateBody(&bd4);
	conveyer[i+36]->CreateFixture(&ballfd4);		
	}
 for (int i = 0; i < 12; ++i)
	{
		vs[60+i].Set(8.0f,0.0f+.5f*i);
    b2FixtureDef ballfd4;
    ballfd4.filter.categoryBits = 0x0004;
      ballfd4.filter.maskBits = 0x0002;
    //ballfd2.filter.groupIndex = -2;
	b2PolygonShape shape;
	shape.SetAsBox(.25f, 0.25f);
	ballfd4.shape = &shape;  
	b2BodyDef bd4;
	ballfd4.density=0.1f;
	ballfd.friction=100.0f;  
	bd4.type = b2_dynamicBody;
	bd4.position.Set(8.0f,0.0f+.5f*i+.25f);
	conveyer[i+60] = m_world->CreateBody(&bd4);
	conveyer[i+60]->CreateFixture(&ballfd4);
	}

for(int i=1;i<72;i++)
{
	b2RevoluteJointDef jointDef3;
	jointDef3.Initialize(conveyer[i-1], conveyer[i],vs[i]);
	m_world->CreateJoint(&jointDef3);
}
b2RevoluteJointDef jointDef3;
	jointDef3.Initialize(conveyer[0], conveyer[71],vs[0]);
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
	bd5.position.Set(18.5f, 3.0f);
	b5 = m_world->CreateBody(&bd5);
	b5->CreateFixture(&ballfd5);
	//b5->SetAngularVelocity(0.2f);
	b2WeldJointDef jointDef5;
	jointDef5.Initialize(sbody1, b5, sbody1->GetWorldCenter());//,sbody1->GetWorldCenter());//,sbody1->GetWorldCenter());
	m_world->CreateJoint(&jointDef5);
	b2DistanceJointDef jointDef51;
	b2Vec2 a;
	a.Set(18.5f,1.5f);
	jointDef51.Initialize(sbody1, b5,a,a); //(18.5f,1.5f),(18.5f,1.5f));sbody1->GetWorldCenter());//,sbody1->GetWorldCenter());
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
	bd6.position.Set(18.5f, -1.0f);
	b6 = m_world->CreateBody(&bd6);
	b6->CreateFixture(&ballfd6);
	//b6->SetAngularVelocity(1.0f);
	b2RevoluteJointDef jointDef6;
	jointDef6.Initialize(b5, b6, b6->GetWorldCenter());//,sbody1->GetWorldCenter());//,sbody1->GetWorldCenter());
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
	bd7.position.Set(18.5f, 7.0f);
	b7 = m_world->CreateBody(&bd7);
	b7->CreateFixture(&ballfd7);
	//b6->SetAngularVelocity(1.0f);
	b2RevoluteJointDef jointDef7;
	jointDef7.Initialize(b5, b7, b7->GetWorldCenter());//,sbody1->GetWorldCenter());//,sbody1->GetWorldCenter());
	m_world->CreateJoint(&jointDef7);
}
  sim_t *sim = new sim_t("Dominos", dominos_t::create);
}
