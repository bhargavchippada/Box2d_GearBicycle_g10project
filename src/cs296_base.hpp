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


#ifndef _CS296BASE_HPP_
#define _CS296BASE_HPP_

#include "render.hpp"
#include <Box2D/Box2D.h>
#include <cstdlib>

#define	RAND_LIMIT 32767

namespace cs296
{

  //! What is the difference between a class and a struct in C++?
  class base_sim_t;
  struct settings_t;
  
  //! Why do we use a typedef
  typedef base_sim_t* sim_create_fcn(); 

  //! Simulation settings. Some can be controlled in the GUI by clicking on the boxes.<br>
  //! Can be changed while running the simulation
 
 
  struct settings_t
  {
    //!constructor
    settings_t() :
      view_center(0.0f, 20.0f),			//!< Sets the centre of screen to (0.0f,20.0f)
      hz(60.0f),						//!< Sets the frame rate to 60
      velocity_iterations(8),			//!< Sets the velocity iterations to 8
	  position_iterations(3),			//!< Sets the position interations to 3
      draw_shapes(1),					//!< Initalizes the draw_shapes to true
      draw_joints(1),					//!< Initalizes the draw joints tool to true
      draw_AABBs(0),					//!< Initalizes the draw_AABB to false
      draw_pairs(0),					//!< Initalizes the draw_pairs to false
      draw_contact_points(0),			//!< Initalizes the draw_contact_points to false
      draw_contact_normals(0),			//!< Initalizes the draw_normals to false
      draw_contact_forces(0),			//!< Initalizes the draw_contact_forces to false
      draw_friction_forces(0),			//!< Initalizes the draw_friction_forces to false
      draw_COMs(0),						//!< Initalizes the draw_COMs to false
      draw_stats(0),					//!< Initalizes the draw_stats to false
      draw_profile(0),					//!< Initalizes the draw_profiles to false
      enable_warm_starting(1),			//!< Initalizes the enable_warm_settings to true
      enable_continuous(1),				//!< Initalizes the enable_continous to true
      enable_sub_stepping(0),			//!< Initalizes the enable_sub_setting to false
      pause(0),							//!< Initalizes the pause to false
      single_step(0)					//!< Initalizes the single_step to false
    {}
    
    b2Vec2 view_center;					//!<Type:b2Vec2 used to set view_center[Center of the screen]
    float32 hz;							//!<Type:float32 used to set hz[frame rate]
    int32 velocity_iterations;			//!<Type:int32 used to set velocity_iterations
    int32 position_iterations;			//!<Type:int32 used to set position_iterations
    int32 draw_shapes;					//!<Type:int32 used to set draw_shapes[for drawing shapes of b2body]
    int32 draw_joints;					//!<Type:int32 used to set draw_joints[for drawing joints connecting different bodies]
    int32 draw_AABBs;					//!<Type:int32 used to set draw_AABBs[for drawing bounding boxes]
    int32 draw_pairs;					//!<Type:int32 used to set draw_pairs[]
    int32 draw_contact_points;			//!<Type:int32 used to set draw_contact_points[for showing the contact points]
    int32 draw_contact_normals;			//!<Type:int32 used to set draw_contact_normals[for drawing contact normals during collision]
    int32 draw_contact_forces;			//!<Type:int32 used to set draw_contact_forces[for drawing contact forces]
    int32 draw_friction_forces;			//!<Type:int32 used to set draw_friction_forces[for drawing friction forces]
    int32 draw_COMs;					//!<Type:int32 used to set draw_COMs[Centre of mass]
    int32 draw_stats;					//!<Type:int32 used to set draw_stats[for showing statistics of simulation]
    int32 draw_profile;					//!<Type:int32 used to set draw_profile[for showing information about calculations being done]
    int32 enable_warm_starting;			//!<Type:int32 used to set enable_warm_starting[for smooth start of the simulation]
    int32 enable_continuous;			//!<Type:int32 used to set enable_continuous[method for calculating time of impacts]
    int32 enable_sub_stepping;			//!<Type:int32 used to set enable_sub_stepping[moves object directly to the time of impact]
    int32 pause;						//!<Type:int32 used to set pause[for pausing the simulation]
    int32 single_step;					//!<Type:int32 used to set single_step[for going to the next iteration]
  };	
  
  /**  Initializes the simulation
   */ 
  struct sim_t
  {
    const char *name;  				//!<this represents the name of the simulation
    sim_create_fcn *create_fcn;		//!<this creates the simulation and is of typedef base_sim_t*

    sim_t(const char *_name, sim_create_fcn *_create_fcn): 			//!<creates simulation by taking in two arguments
      name(_name), create_fcn(_create_fcn) {;}
  };
  
  extern sim_t *sim;
  
  
  const int32 k_max_contact_points = 2048;  
/**  Stores information about the colliding bodies and their properties,positions<br>
 * velocities,etc
   */
  struct contact_point_t
  {
    b2Fixture* fixtureA;				//!<type:b2Fixture It is the fixture of bodyA
    b2Fixture* fixtureB;				//!<type:b2Fixture It is the fixture of bodyB
    b2Vec2 normal;						//!<type:b2Vec2 Stores the direction of normal force
    b2Vec2 position;					//!<type:b2Vec2 Store the position of point of contact
    b2PointState state;					//!<type:b2PointState Information about the state of collision
  };
  
/**  This class holds infromation about the simulation. It has functions which enable<br> input from users via keybord or mouse.
 * And also has<br> various call back functions. Inherited from b2ContactListener<br>
 * It also define the simulation world
   */

  class base_sim_t : public b2ContactListener
  {
  public:
    
    base_sim_t();   //!< constructor        

    
    virtual ~base_sim_t();//!< Virtual destructors for deallocating the occupied memory
    
    void set_text_line(int32 line) { m_text_line = line; } //!<for setting m_text_line based on line no
    void draw_title(int x, int y, const char *string); //!<for drawing titles
    
  
    
    virtual void step(settings_t* settings); //!< assigns settings to the world





    virtual void keyboard(unsigned char key) { B2_NOT_USED(key); }
    //!< Functions enabling the user to give input. Not used currently.
    virtual void keyboard_up(unsigned char key) { B2_NOT_USED(key); }
//!< Functions enabling the user to give input. Not used currently.
    void shift_mouse_down(const b2Vec2& p) { B2_NOT_USED(p); }
    //!< Functions enabling the user to give input. Not used currently.
    virtual void mouse_down(const b2Vec2& p) { B2_NOT_USED(p); }
    //!< Functions enabling the user to give input. Not used currently.
    virtual void mouse_up(const b2Vec2& p) { B2_NOT_USED(p); }
    //!< Functions enabling the user to give input. Not used currently.
    void mouse_move(const b2Vec2& p) { B2_NOT_USED(p); }
//!< Functions enabling the user to give input. Not used currently.
    
    virtual void joint_destroyed(b2Joint* joint) { B2_NOT_USED(joint); }
    //!< Let derived tests know that a joint was destroyed.
    
   
    virtual void begin_contact(b2Contact* contact) { B2_NOT_USED(contact); }  //!< called when two bodies begin to touch
    virtual void end_contact(b2Contact* contact) { B2_NOT_USED(contact); }		//!< called after bodies cease to touch
    virtual void pre_solve(b2Contact* contact, const b2Manifold* oldManifold);	//!< called befor solver starts solving
    
    virtual void post_solve(const b2Contact* contact, const b2ContactImpulse* impulse)	
    {
      B2_NOT_USED(contact);
      B2_NOT_USED(impulse);
    }//!< called after solver is done

  //!How are protected members different from private memebers of a class in C++ ?
  protected:

    //! What are Friend classes?
    friend class contact_listener_t;
    
    b2Body* m_ground_body;        //!< type:b2Body creates ground body
    b2AABB m_world_AABB;			//!<type:b2AABB creates the simulation world of AABB
    contact_point_t m_points[k_max_contact_points];     //!< stores contact points
    int32 m_point_count;       //!< max no of contact points

    debug_draw_t m_debug_draw;  //!<for drawing objects in world
    int32 m_text_line;			//!<type:int32 stores no of text lines
    b2World* m_world;          //!<type:b2World  This declares the simulation world

    int32 m_step_count;			//!<type:int32 max no of steps
    
    b2Profile m_max_profile;	//!<type:b2Profile max no of profiles 
    b2Profile m_total_profile;	//!<type:b2Profile total no of profiles
  };
}

#endif
