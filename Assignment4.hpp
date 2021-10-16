/*
  CS475 Assignment 4

*/

#ifndef _COLORCUBE_HPP_
#define _COLORCUBE_HPP_

// Defining the ESCAPE Key Code
#define ESCAPE 27
// Defining the DELETE Key Code
#define DELETE 127

#include <vector>
#include "gl_framework.hpp"
#include "shader_util.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "hierarchy_node.hpp"


// Camera position and rotation Parameters
GLfloat c_xpos = 0.0, c_ypos = 0.0, c_zpos = 10.0;
GLfloat c_up_x = 0.0, c_up_y = 1.0, c_up_z = 0.0;
GLfloat c_xrot=2.0,c_yrot=0.0,c_zrot=0.0;

//Running variable to toggle culling on/off
bool enable_culling=true;
//Running variable to toggle wireframe/solid modelling
bool solid=true;
//Enable/Disable perspective view
bool enable_perspective=true;
//Shader program attribs
GLuint vPosition, vColor, vNormal, texCoord;

int light1_value = 0, light2_value = 0, spotlight1_value = 0;

int humanoid_indicator = 1;

//global matrix stack for hierarchical modelling
std::vector<glm::mat4> matrixStack;

std::ofstream keyframes;
std::ifstream keyframes_in;
bool recording = 0;
// bool recording = 1;
bool play_animation = 0;
float start_time, present_time;
std::vector<float> frame1, frame2, current_frame;
bool end_of_file = 0;
int camera_light_count = 9;
float time_interval_between_frames = 1.0;
int frame_count = 0;
bool initialization_done = 0;
float total_time_till_now = 0.0;
float present_between_time_interval;

int number_ctrl_pts = 4;
glm::vec3 cpt1(0, 0, 9.1);
glm::vec3 cpt2(-1, 3, 9.1);
glm::vec3 cpt3(-3, 3, 9.1);
glm::vec3 cpt4(-5, 0, 9.1);

bool bezier_interpolate = 0;
// float bezier_start_time = 19.3;
float bezier_start_time = 17.3;
float total_time_animate = 22.4;
float bezier_divide = total_time_animate - bezier_start_time;

int capture_toggle = 2;

csX75::HNode* root_node;
csX75::HNode* curr_node;

// Environment
csX75::HNode* node_e1;
csX75::HNode* node_e2;

// Humanoid
csX75::HNode* node_h1;
csX75::HNode* node_h2;
csX75::HNode* node_h3;
csX75::HNode* node_h4;
csX75::HNode* node_h5;
csX75::HNode* node_h6;
csX75::HNode* node_h7;
csX75::HNode* node_h8;
csX75::HNode* node_h9;
csX75::HNode* node_h10;
csX75::HNode* node_h11;
csX75::HNode* node_h12;
csX75::HNode* node_h13;
csX75::HNode* node_h14;
csX75::HNode* node_h15;
csX75::HNode* node_h16;
csX75::HNode* node_h17;
csX75::HNode* node_h18;
csX75::HNode* node_h19;
csX75::HNode* node_h20;
csX75::HNode* node_h21;
csX75::HNode* node_h22;
csX75::HNode* node_h23;
csX75::HNode* node_h1_top;
csX75::HNode* node_h1_bottom;
csX75::HNode* node_h3_hat_frustum;
csX75::HNode* node_h3_hat_cylinder;
csX75::HNode* node_h3_hat_circle;

// Non Humanoid
csX75::HNode* node_nh1;
csX75::HNode* node_nh2;
csX75::HNode* node_nh3;
csX75::HNode* node_nh3_ears;
csX75::HNode* node_nh3_specs;
csX75::HNode* node_nh4;
csX75::HNode* node_nh5;
csX75::HNode* node_nh6;
csX75::HNode* node_nh7;
csX75::HNode* node_nh8;
csX75::HNode* node_nh9;
csX75::HNode* node_nh10;
csX75::HNode* node_nh11;
csX75::HNode* node_nh1_top;
csX75::HNode* node_nh1_bottom;

float increment_trans = 0.1;


//-------------------------------------------------------------------------

#endif
