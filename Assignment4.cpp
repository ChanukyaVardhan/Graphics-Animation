/*
  CS475 Assignment 4

*/


#include "Assignment4.hpp"
#include "texture.cpp"

GLuint shaderProgram;

glm::mat4 rotation_matrix;
glm::mat4 projection_matrix;
glm::mat4 c_rotation_matrix;
glm::mat4 lookat_matrix;

glm::mat4 model_matrix;
glm::mat4 view_matrix;

glm::mat4 modelview_matrix;
glm::mat3 normal_matrix;

GLuint uModelViewMatrix, normalMatrix, viewMatrix, light1, light2, spotlight1;
int num_vertices = 36;

int tesselation=50;
double PI=3.14159265;

glm::vec4 v_positions[10000];
glm::vec4 v_colors[10000];
glm::vec4 v_normals[10000];
glm::vec2 tex_coords[10000];
glm::vec4 red(1.0, 0.0, 0.0, 1.0);
glm::vec4 white(1.0, 1.0, 1.0, 1.0);

//-----------------------------------------------------------------
unsigned int framenum=0;
unsigned char *pRGB;

int SCREEN_WIDTH = 850;
int SCREEN_HEIGHT = 850;
int SCREEN_WIDTH_code = 850;
int SCREEN_HEIGHT_code = 850;

void capture_frame(unsigned int framenum)
{
  //global pointer float *pRGB
  pRGB = new unsigned char [3 * (SCREEN_WIDTH+1) * (SCREEN_HEIGHT + 1) ];


  // set the framebuffer to read
  //default for double buffered
  glReadBuffer(GL_BACK);

  glPixelStoref(GL_PACK_ALIGNMENT,1);//for word allignment
  
  glReadPixels(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, pRGB);
  char filename[200];
  sprintf(filename,"./frames/frame_%04d.ppm",framenum);
  std::ofstream out(filename, std::ios::out);
  out<<"P6"<<std::endl;
  out<<SCREEN_WIDTH<<" "<<SCREEN_HEIGHT<<" 255"<<std::endl;
  out.write(reinterpret_cast<char const *>(pRGB), (3 * (SCREEN_WIDTH+1) * (SCREEN_HEIGHT + 1)) * sizeof(int));
  out.close();

  //function to store pRGB in a file named count
  delete pRGB;
}

void circle(float radius, float direction)
{
  num_vertices = 0;

  float increment = 0.01;
  for(float i=0; i<1; i+=increment)
  {
    float theta = i*2.0*PI, theta_2 = theta + increment*2.0*PI;
    
    v_positions[num_vertices] = glm::vec4(0.0, 0.0, 0.0, 1.0);
    v_colors[num_vertices] = white;
    v_normals[num_vertices] = glm::vec4(0.0, direction, 0.0, 1.0);
    tex_coords[num_vertices] = glm::vec2(0.5, 0.5);
    num_vertices++;

    v_positions[num_vertices] = glm::vec4(radius*cos(theta), 0.0, radius*sin(theta), 1.0);
    v_colors[num_vertices] = white;
    v_normals[num_vertices] = glm::vec4(0.0, direction, 0.0, 1.0);
    tex_coords[num_vertices] = glm::vec2(0.5 + 0.5*cos(theta), 0.5 + 0.5*sin(theta));
    num_vertices++;

    v_positions[num_vertices] = glm::vec4(radius*cos(theta_2), 0.0, radius*sin(theta_2), 1.0);
    v_colors[num_vertices] = white;
    v_normals[num_vertices] = glm::vec4(0.0, direction, 0.0, 1.0);
    tex_coords[num_vertices] = glm::vec2(0.5 + 0.5*cos(theta_2), 0.5 + 0.5*sin(theta_2));
    num_vertices++;
  }
}

void rectangle(float length, float breadth)
{
  num_vertices = 0;

  v_positions[num_vertices] = glm::vec4(-length/2, 0.0, -breadth/2, 1.0);
  v_colors[num_vertices] = white;
  v_normals[num_vertices] = glm::vec4(0.0, 1.0, 0.0, 1.0);
  tex_coords[num_vertices] = glm::vec2(0.0, 0.0);
  num_vertices++;

  v_positions[num_vertices] = glm::vec4(length/2, 0.0, -breadth/2, 1.0);
  v_colors[num_vertices] = white;
  v_normals[num_vertices] = glm::vec4(0.0, 1.0, 0.0, 1.0);
  tex_coords[num_vertices] = glm::vec2(1.0, 0.0);
  num_vertices++;

  v_positions[num_vertices] = glm::vec4(length/2, 0.0, breadth/2, 1.0);
  v_colors[num_vertices] = white;
  v_normals[num_vertices] = glm::vec4(0.0, 1.0, 0.0, 1.0);
  tex_coords[num_vertices] = glm::vec2(1.0, 1.0);
  num_vertices++;

  v_positions[num_vertices] = glm::vec4(-length/2, 0.0, -breadth/2, 1.0);
  v_colors[num_vertices] = white;
  v_normals[num_vertices] = glm::vec4(0.0, 1.0, 0.0, 1.0);
  tex_coords[num_vertices] = glm::vec2(0.0, 0.0);
  num_vertices++;

  v_positions[num_vertices] = glm::vec4(-length/2, 0.0, breadth/2, 1.0);
  v_colors[num_vertices] = white;
  v_normals[num_vertices] = glm::vec4(0.0, 1.0, 0.0, 1.0);
  tex_coords[num_vertices] = glm::vec2(0.0, 1.0);
  num_vertices++;

  v_positions[num_vertices] = glm::vec4(length/2, 0.0, breadth/2, 1.0);
  v_colors[num_vertices] = white;
  v_normals[num_vertices] = glm::vec4(0.0, 1.0, 0.0, 1.0);
  tex_coords[num_vertices] = glm::vec2(1.0, 1.0);
  num_vertices++;
}

void cylinder(float radius, float height)
{
  num_vertices = 0;

  float increment = 0.01;
  for(float i=0; i<1; i+=increment) 
  {
    float theta = i*2.0*PI, theta_2 = theta + increment*2.0*PI;
    v_positions[num_vertices] = glm::vec4(radius*cos(theta), -height, radius*sin(theta), 1.0);
    v_colors[num_vertices] = white;
    v_normals[num_vertices] = glm::vec4(radius*cos(theta), 0.0, radius*sin(theta), 1.0);
    if(i <= 0.75)
      tex_coords[num_vertices] = glm::vec2(i + 0.25, 0);
    else
      tex_coords[num_vertices] = glm::vec2(i - 0.75, 0);
    num_vertices++;

    v_positions[num_vertices] = glm::vec4(radius*cos(theta_2), -height, radius*sin(theta_2), 1.0);
    v_colors[num_vertices] = white;
    v_normals[num_vertices] = glm::vec4(radius*cos(theta_2), 0.0, radius*sin(theta_2), 1.0);
    if(i + increment <= 0.75)
      tex_coords[num_vertices] = glm::vec2(i + increment + 0.25, 0);
    else
      tex_coords[num_vertices] = glm::vec2(i + increment - 0.75, 0);
    num_vertices++;

    v_positions[num_vertices] = glm::vec4(radius*cos(theta), 0, radius*sin(theta), 1.0);
    v_colors[num_vertices] = white;
    v_normals[num_vertices] = glm::vec4(radius*cos(theta), 0, radius*sin(theta), 1.0);
    if(i <= 0.75)
      tex_coords[num_vertices] = glm::vec2(i + 0.25, 1);
    else
      tex_coords[num_vertices] = glm::vec2(i - 0.75, 1);
    num_vertices++;

    v_positions[num_vertices] = glm::vec4(radius*cos(theta), 0, radius*sin(theta), 1.0);
    v_colors[num_vertices] = white;
    v_normals[num_vertices] = glm::vec4(radius*cos(theta), 0, radius*sin(theta), 1.0);
    if(i <= 0.75)
      tex_coords[num_vertices] = glm::vec2(i + 0.25, 1);
    else
      tex_coords[num_vertices] = glm::vec2(i - 0.75, 1);
    num_vertices++;

    v_positions[num_vertices] = glm::vec4(radius*cos(theta_2), 0, radius*sin(theta_2), 1.0);
    v_colors[num_vertices] = white;
    v_normals[num_vertices] = glm::vec4(radius*cos(theta_2), 0, radius*sin(theta_2), 1.0);
    if(i + increment <= 0.75)
      tex_coords[num_vertices] = glm::vec2(i + increment + 0.25, 1);
    else
      tex_coords[num_vertices] = glm::vec2(i + increment - 0.75, 1);
    num_vertices++;

    v_positions[num_vertices] = glm::vec4(radius*cos(theta_2), -height, radius*sin(theta_2), 1.0);
    v_colors[num_vertices] = white;
    v_normals[num_vertices] = glm::vec4(radius*cos(theta_2), 0.0, radius*sin(theta_2), 1.0);
    if(i + increment <= 0.75)
      tex_coords[num_vertices] = glm::vec2(i + increment + 0.25, 0);
    else
      tex_coords[num_vertices] = glm::vec2(i + increment - 0.75, 0);
    num_vertices++;
  }
}

void sphere(float radius, int Lats, int Longs)
{
  num_vertices = 0;
  glm::vec4 temp_v_positions[10000];
  glm::vec4 temp_v_colors[10000];
  glm::vec4 temp_v_normals[10000];
  glm::vec2 temp_tex_coords[10000];
  int temp_num_vertices = 0;

  float lats,longs;

  float slices=(180/(float(Lats)*10))/2;
  float sectors=(180/(float(Longs)*10))/2;

  float l;

  for (lats = 0.0; lats <= PI; lats+=sectors)  
  {
    for(longs = 0.0; longs <= 2.0*PI; longs+=slices)
    {
      float x = radius * sin(lats) * cos(longs);
      float y = radius * sin(lats) * sin(longs);
      float z = radius * cos(lats);
      glm::vec4 pt(x, y, z, 1.0);
      glm::vec2 pt_tex(lats/PI, longs/(2*PI));

      temp_v_positions[temp_num_vertices] = pt;
      temp_v_colors[temp_num_vertices] = red;
      temp_v_normals[temp_num_vertices] = pt;
      temp_tex_coords[temp_num_vertices] = pt_tex;
      temp_num_vertices++;
      
      if(lats+sectors>PI)
        l=PI;
      else
        l=lats+sectors;
      x = radius * sin(l) * cos(longs);
      y = radius * sin(l) * sin(longs);
      z = radius * cos(l);
      pt = glm::vec4(x, y, z, 1.0);
      pt_tex = glm::vec2(l/PI, longs/(2*PI));

      temp_v_positions[temp_num_vertices] = pt; 
      temp_v_colors[temp_num_vertices] = red;
      temp_v_normals[temp_num_vertices] = pt;
      temp_tex_coords[temp_num_vertices] = pt_tex;
      temp_num_vertices++;
    }
  }

  for(int i=0; i<temp_num_vertices-3; i++)
  {
    v_positions[num_vertices] = temp_v_positions[i]; 
    v_colors[num_vertices] = temp_v_colors[i];
    v_normals[num_vertices] = temp_v_normals[i];
    tex_coords[num_vertices] = temp_tex_coords[i];
    num_vertices++;

    v_positions[num_vertices] = temp_v_positions[i+1]; 
    v_colors[num_vertices] = temp_v_colors[i+1];
    v_normals[num_vertices] = temp_v_normals[i+1];
    tex_coords[num_vertices] = temp_tex_coords[i+1];
    num_vertices++;

    v_positions[num_vertices] = temp_v_positions[i+2];
    v_colors[num_vertices] = temp_v_colors[i+2];
    v_normals[num_vertices] = temp_v_normals[i+2];
    tex_coords[num_vertices] = temp_tex_coords[i+2];
    num_vertices++;
  }
}

void frustum(float radius1, float radius2, float height)
{
  num_vertices = 0;

  float increment = 0.01;
  for(float i=0; i<1; i+=increment)
  {
    float theta = i*2.0*PI, theta_2 = theta + increment*2.0*PI;

    glm::vec4 direction1 = glm::vec4(glm::cross(glm::vec3((radius2 - radius1)*cos(theta), -height, (radius2 - radius1)*sin(theta)), glm::vec3((radius2*cos(theta_2)) - (radius1*cos(theta)), -height, (radius2*sin(theta_2)) - (radius1*(sin(theta))))), 1.0);

    v_positions[num_vertices] = glm::vec4(radius1*cos(theta), 0.0, radius1*sin(theta), 1.0);
    v_colors[num_vertices] = white;
    v_normals[num_vertices] = direction1;
    tex_coords[num_vertices] = glm::vec2(0.5, 0.5);
    num_vertices++;

    v_positions[num_vertices] = glm::vec4(radius2*cos(theta), -height, radius2*sin(theta), 1.0);
    v_colors[num_vertices] = white;
    v_normals[num_vertices] = direction1;
    tex_coords[num_vertices] = glm::vec2(0.5 + 0.5*cos(theta), 0.5 + 0.5*sin(theta));
    num_vertices++;

    v_positions[num_vertices] = glm::vec4(radius2*cos(theta_2), -height, radius2*sin(theta_2), 1.0);
    v_colors[num_vertices] = white;
    v_normals[num_vertices] = direction1;
    tex_coords[num_vertices] = glm::vec2(0.5 + 0.5*cos(theta_2), 0.5 + 0.5*sin(theta_2));
    num_vertices++;

    glm::vec4 direction2 = glm::vec4(glm::cross(glm::vec3((radius2 - radius1)*cos(theta_2), -height, (radius2 - radius1)*sin(theta_2)), glm::vec3((radius2*cos(theta_2)) - (radius1*cos(theta)), -height, (radius2*sin(theta_2)) - (radius1*(sin(theta))))), 1.0);

    v_positions[num_vertices] = glm::vec4(radius1*cos(theta), 0.0, radius1*sin(theta), 1.0);
    v_colors[num_vertices] = white;
    v_normals[num_vertices] = direction2;
    tex_coords[num_vertices] = glm::vec2(0.5, 0.5);
    num_vertices++;

    v_positions[num_vertices] = glm::vec4(radius1*cos(theta_2), 0.0, radius1*sin(theta_2), 1.0);
    v_colors[num_vertices] = white;
    v_normals[num_vertices] = direction2;
    tex_coords[num_vertices] = glm::vec2(0.5 + 0.5*cos(theta_2), 0.5 + 0.5*sin(theta_2));
    num_vertices++;

    v_positions[num_vertices] = glm::vec4(radius2*cos(theta_2), -height, radius2*sin(theta_2), 1.0);
    v_colors[num_vertices] = white;
    v_normals[num_vertices] = direction2;
    tex_coords[num_vertices] = glm::vec2(0.5 + 0.5*cos(theta_2), 0.5 + 0.5*sin(theta_2));
    num_vertices++;
  }
}

void specs(float radius)
{
  num_vertices = 0;

  float increment = 0.01;
  for(float i=0; i<1; i+=increment)
  {
    float theta = i*1.0*PI, theta_2 = theta + increment*1.0*PI;
    
    v_positions[num_vertices] = glm::vec4(radius, 0.0, 0.0, 1.0);
    v_colors[num_vertices] = white;
    v_normals[num_vertices] = glm::vec4(0.0, 1.0, 0.0, 1.0);
    tex_coords[num_vertices] = glm::vec2(0.5, 0.5);
    num_vertices++;

    v_positions[num_vertices] = glm::vec4(radius + radius*cos(theta), 0.0, radius*sin(theta), 1.0);
    v_colors[num_vertices] = white;
    v_normals[num_vertices] = glm::vec4(0.0, 1.0, 0.0, 1.0);
    tex_coords[num_vertices] = glm::vec2(0.5 + 0.5*cos(theta), 0.5 + 0.5*sin(theta));
    num_vertices++;

    v_positions[num_vertices] = glm::vec4(radius + radius*cos(theta_2), 0.0, radius*sin(theta_2), 1.0);
    v_colors[num_vertices] = white;
    v_normals[num_vertices] = glm::vec4(0.0, 1.0, 0.0, 1.0);
    tex_coords[num_vertices] = glm::vec2(0.5 + 0.5*cos(theta_2), 0.5 + 0.5*sin(theta_2));
    num_vertices++;
  }

  for(float i=0; i<1; i+=increment)
  {
    float theta = i*1.0*PI, theta_2 = theta + increment*1.0*PI;
    
    v_positions[num_vertices] = glm::vec4(-radius, 0.0, 0.0, 1.0);
    v_colors[num_vertices] = white;
    v_normals[num_vertices] = glm::vec4(0.0, 1.0, 0.0, 1.0);
    tex_coords[num_vertices] = glm::vec2(0.5, 0.5);
    num_vertices++;

    v_positions[num_vertices] = glm::vec4(-radius + radius*cos(theta), 0.0, radius*sin(theta), 1.0);
    v_colors[num_vertices] = white;
    v_normals[num_vertices] = glm::vec4(0.0, 1.0, 0.0, 1.0);
    tex_coords[num_vertices] = glm::vec2(0.5 + 0.5*cos(theta), 0.5 + 0.5*sin(theta));
    num_vertices++;

    v_positions[num_vertices] = glm::vec4(-radius + radius*cos(theta_2), 0.0, radius*sin(theta_2), 1.0);
    v_colors[num_vertices] = white;
    v_normals[num_vertices] = glm::vec4(0.0, 1.0, 0.0, 1.0);
    tex_coords[num_vertices] = glm::vec2(0.5 + 0.5*cos(theta_2), 0.5 + 0.5*sin(theta_2));
    num_vertices++;
  }
}

//-----------------------------------------------------------------

void initBuffersGL(void)
{

  // Load shaders and use the resulting shader program
  std::string vertex_shader_file("vshader.glsl");
  std::string fragment_shader_file("fshader.glsl");

  std::vector<GLuint> shaderList;
  shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  shaderProgram = csX75::CreateProgramGL(shaderList);
  glUseProgram( shaderProgram );

  // getting the attributes from the shader program
  vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
  vColor = glGetAttribLocation( shaderProgram, "vColor" ); 
  vNormal = glGetAttribLocation( shaderProgram, "vNormal" );
  texCoord = glGetAttribLocation( shaderProgram, "texCoord" );
  uModelViewMatrix = glGetUniformLocation( shaderProgram, "uModelViewMatrix" );
  normalMatrix =  glGetUniformLocation( shaderProgram, "normalMatrix" );
  viewMatrix = glGetUniformLocation( shaderProgram, "viewMatrix" );
  light1 = glGetUniformLocation( shaderProgram, "light1" );
  light2 = glGetUniformLocation( shaderProgram, "light2" );
  spotlight1 = glGetUniformLocation( shaderProgram, "spotlight1" );

  // Textures 
  GLuint texture_skin = LoadTexture("images/skin.bmp", 256, 256);
  GLuint texture_shirt = LoadTexture("images/shirt.bmp", 512, 512);
  GLuint luffy_face_256 = LoadTexture("images/luffy_face_256.bmp", 256, 256);
  GLuint texture_shorts = LoadTexture("images/shorts.bmp", 512, 512);
  GLuint texture_sky = LoadTexture("images/sky-dome.bmp", 3000, 750);
  GLuint texture_hat = LoadTexture("images/basket.bmp", 325, 340);
  GLuint dog_skin = LoadTexture("images/dog_skin.bmp", 340, 270);
  GLuint dog_specs = LoadTexture("images/specs.bmp", 500, 375);
  GLuint texture_black = LoadTexture("images/black.bmp", 300, 168);
  GLuint earth_from_satellite = LoadTexture("images/earth_from_satellite.bmp", 3468, 1892);

  // Environment
  sphere(100, 75, 75);
  node_e1 = new csX75::HNode(NULL, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, texture_sky);
  node_e1->change_parameters(0.0, 0.0, 0.0, 90.0, 90.0, 0.0);

  rectangle(200, 200);
  node_e2 = new csX75::HNode(NULL, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, earth_from_satellite);

  float humanoid_factor = 0.3; // Don't change
  float humanoid_shift = 0.0;
  // Humanoid

  // Torso
  float torso_radius;
  torso_radius = 0.3;
  float torso_height;
  torso_height = 1.15 * torso_radius / humanoid_factor;
  cylinder(torso_radius, torso_height);
  node_h1 = new csX75::HNode(NULL, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, texture_shirt);
  humanoid_shift += torso_height;

  circle(torso_radius, 1.0);
  node_h1_top = new csX75::HNode(node_h1, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, texture_skin);
  node_h1_top->change_parameters(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

  circle(torso_radius, 1.0);
  node_h1_bottom = new csX75::HNode(node_h1, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, texture_skin);
  node_h1_bottom->change_parameters(0.0, -torso_height, 0.0, 0.0, 0.0, 0.0);

  // neck
  float neck_height;
  float neck_radius;
  neck_radius = 0.115 * torso_radius / humanoid_factor;
  neck_height = 0.231 * torso_radius / humanoid_factor;
  cylinder(neck_radius, neck_height);
  node_h2 = new csX75::HNode(node_h1, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, texture_skin);
  node_h2->change_parameters(0.0, (0.9)*neck_height, 0.0, 0.0, 0.0, 0.0);

  // Head
  float head_radius;
  head_radius = 0.323 * torso_radius / humanoid_factor;
  tesselation = 75;
  sphere(head_radius, tesselation, tesselation); 
  node_h3 = new csX75::HNode(node_h2, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, luffy_face_256);
  node_h3->change_parameters(0.0, head_radius/2, 0.0, 180.0, -70.0, 0.0);

  float hat_radius1, hat_radius2, hat_radius3, hat_height, hat_frustum_height;
  hat_radius1 = 1.5*head_radius;
  hat_radius2 = 0.6*head_radius;
  hat_frustum_height = 0.05 * torso_radius / humanoid_factor;
  frustum(hat_radius1, hat_radius2, hat_frustum_height);
  node_h3_hat_frustum = new csX75::HNode(node_h3, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, texture_hat);
  node_h3_hat_frustum->change_parameters(0.0, -head_radius+2*hat_frustum_height, 0.0, 0.0, 0.0, 0.0);

  hat_radius3 = 0.9*hat_radius2;
  hat_height = 3*neck_radius;
  frustum(hat_radius2, hat_radius3, hat_height);
  node_h3_hat_cylinder = new csX75::HNode(node_h3_hat_frustum, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, texture_hat);
  node_h3_hat_cylinder->change_parameters(0.0, -hat_frustum_height, 0.0, 0.0, 0.0, 0.0);

  circle(hat_radius3, 1.0);
  node_h3_hat_circle = new csX75::HNode(node_h3_hat_cylinder, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, texture_hat);
  node_h3_hat_circle->change_parameters(0.0, -hat_height, 0.0, 0.0, 0.0, 0.0);

  // Left hand shoulder joint
  float joint_radius;
  joint_radius = 0.115 * torso_radius / humanoid_factor;
  sphere(joint_radius, tesselation, tesselation);
  node_h4 = new csX75::HNode(node_h1, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, texture_skin);
  node_h4->change_parameters(-(torso_radius + (0.85)*joint_radius), -joint_radius - 0.02*(torso_height), 0.0, 0.0, 0.0, 0.0);

  // left upper arm
  float upper_arm_radius;
  upper_arm_radius = 0.115 * torso_radius / humanoid_factor;
  float upper_arm_height;
  upper_arm_height = 0.577 * torso_radius / humanoid_factor;
  cylinder(upper_arm_radius, upper_arm_height);
  node_h5 = new csX75::HNode(node_h4, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, texture_skin);
  node_h5->change_parameters(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

  // Left upper lower joint
  joint_radius = 0.115 * torso_radius / humanoid_factor;
  sphere(joint_radius, tesselation, tesselation);
  node_h6 = new csX75::HNode(node_h5, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, texture_skin);
  node_h6->change_parameters(0.0, -upper_arm_height, 0.0, 0.0, 0.0, 0.0);

  // left lower arm
  upper_arm_radius = 0.115 * torso_radius / humanoid_factor;
  upper_arm_height = 0.531 * torso_radius / humanoid_factor;
  cylinder(upper_arm_radius, upper_arm_height);
  node_h7 = new csX75::HNode(node_h6, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, texture_skin);
  node_h7->change_parameters(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

  // left wrist
  float upper_wrist_radius;
  upper_wrist_radius = 0.138 * torso_radius / humanoid_factor;
  sphere(upper_wrist_radius, tesselation, tesselation);
  node_h8 = new csX75::HNode(node_h7, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, texture_skin);
  node_h8->change_parameters(0.0, -upper_arm_height, 0.0, 0.0, 0.0, 0.0);

  // right hand shoulder joint
  joint_radius = 0.115 * torso_radius / humanoid_factor;
  sphere(joint_radius, tesselation, tesselation);
  node_h9 = new csX75::HNode(node_h1, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, texture_skin);
  node_h9->change_parameters(torso_radius + (0.85)*joint_radius, -joint_radius - 0.02*(torso_height), 0.0, 0.0, 0.0, 0.0);

  // right upper arm
  upper_arm_radius = 0.115 * torso_radius / humanoid_factor;
  upper_arm_height = 0.577 * torso_radius / humanoid_factor;
  cylinder(upper_arm_radius, upper_arm_height);
  node_h10 = new csX75::HNode(node_h9, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, texture_skin);
  node_h10->change_parameters(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

  // right upper lower joint
  joint_radius = 0.115 * torso_radius / humanoid_factor;
  sphere(joint_radius, tesselation, tesselation);
  node_h11 = new csX75::HNode(node_h10, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, texture_skin);
  node_h11->change_parameters(0.0, -upper_arm_height, 0.0, 0.0, 0.0, 0.0);

  // right lower arm
  upper_arm_radius = 0.115 * torso_radius / humanoid_factor;
  upper_arm_height = 0.531 * torso_radius / humanoid_factor;
  cylinder(upper_arm_radius, upper_arm_height);
  node_h12 = new csX75::HNode(node_h11, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, texture_skin);
  node_h12->change_parameters(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

  // right wrist
  upper_wrist_radius = 0.138 * torso_radius / humanoid_factor;
  sphere(upper_wrist_radius, tesselation, tesselation);
  node_h13 = new csX75::HNode(node_h12, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, texture_skin);
  node_h13->change_parameters(0.0, -upper_arm_height, 0.0, 0.0, 0.0, 0.0);

  // Left leg hip joint
  joint_radius = 0.115 * torso_radius / humanoid_factor;
  sphere(joint_radius, tesselation, tesselation);
  node_h14 = new csX75::HNode(node_h1, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, texture_shirt);
  node_h14->change_parameters(-torso_radius + joint_radius, -(0.2)*joint_radius - torso_height, 0.0, 0.0, 0.0, 0.0);
  humanoid_shift += 0.7*joint_radius;

  // left upper leg
  upper_arm_radius = 0.115 * torso_radius / humanoid_factor;
  upper_arm_height = 0.762 * torso_radius / humanoid_factor;
  cylinder(upper_arm_radius, upper_arm_height);
  node_h15 = new csX75::HNode(node_h14, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, texture_shorts);
  node_h15->change_parameters(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
  humanoid_shift += upper_arm_height;

  // Left upper lower leg joint
  joint_radius = 0.115 * torso_radius / humanoid_factor;
  sphere(joint_radius, tesselation, tesselation);
  node_h16 = new csX75::HNode(node_h15, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, texture_skin);
  node_h16->change_parameters(0.0, -upper_arm_height, 0.0, 0.0, 0.0, 0.0);

  // left lower arm
  upper_arm_radius = 0.115 * torso_radius / humanoid_factor;
  upper_arm_height = 0.715 * torso_radius / humanoid_factor;
  cylinder(upper_arm_radius, upper_arm_height);
  node_h17 = new csX75::HNode(node_h16, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, texture_skin);
  node_h17->change_parameters(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
  humanoid_shift += upper_arm_height;

  // left wrist
  upper_wrist_radius = 0.162 * torso_radius / humanoid_factor;
  sphere(upper_wrist_radius, tesselation, tesselation);
  node_h18 = new csX75::HNode(node_h17, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, texture_skin);
  node_h18->change_parameters(0.0, -upper_arm_height, upper_wrist_radius/3, 0.0, 0.0, 0.0);
  humanoid_shift += upper_wrist_radius/2;

  // right leg hip joint
  joint_radius = 0.115 * torso_radius / humanoid_factor;
  sphere(joint_radius, tesselation, tesselation);
  node_h19 = new csX75::HNode(node_h1, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, texture_shirt);
  node_h19->change_parameters(torso_radius - joint_radius, -(0.2)*joint_radius - torso_height, 0.0, 0.0, 0.0, 0.0);

  // right upper arm
  upper_arm_radius = 0.115 * torso_radius / humanoid_factor;
  upper_arm_height = 0.762 * torso_radius / humanoid_factor;
  cylinder(upper_arm_radius, upper_arm_height);
  node_h20 = new csX75::HNode(node_h19, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, texture_shorts);
  node_h20->change_parameters(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

  // right upper lower joint
  joint_radius = 0.115 * torso_radius / humanoid_factor;
  sphere(joint_radius, tesselation, tesselation);
  node_h21 = new csX75::HNode(node_h20, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, texture_skin);
  node_h21->change_parameters(0.0, -upper_arm_height, 0.0, 0.0, 0.0, 0.0);

  // right lower arm
  upper_arm_radius = 0.115 * torso_radius / humanoid_factor;
  upper_arm_height = 0.715 * torso_radius / humanoid_factor;
  cylinder(upper_arm_radius, upper_arm_height);
  node_h22 = new csX75::HNode(node_h21, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, texture_skin);
  node_h22->change_parameters(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

  // right wrist
  upper_wrist_radius = 0.162 * torso_radius / humanoid_factor;
  sphere(upper_wrist_radius, tesselation, tesselation);
  node_h23 = new csX75::HNode(node_h22, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, texture_skin);
  node_h23->change_parameters(0.0, -upper_arm_height, upper_wrist_radius/3, 0.0, 0.0, 0.0);

  node_h1->change_parameters(0.0, humanoid_shift, -0.5, 0.0, 0.0, 0.0);


  float non_humanoid_factor = 0.2; // Don't change this
  float non_humanoid_shift = 0.0;

  // Non Humanoid
  float nh_torso_radius, nh_torso_width;
  nh_torso_radius = 0.2;
  nh_torso_width = 1.0 * nh_torso_radius / non_humanoid_factor;
  cylinder(nh_torso_radius, nh_torso_width);
  node_nh1 = new csX75::HNode(NULL, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, dog_skin);
  non_humanoid_shift += nh_torso_radius;

  circle(nh_torso_radius, 1.0);
  node_nh1_top = new csX75::HNode(node_nh1, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, dog_skin);
  node_nh1_top->change_parameters(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

  circle(nh_torso_radius, 1.0);
  node_nh1_bottom = new csX75::HNode(node_nh1, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, dog_skin);
  node_nh1_bottom->change_parameters(0.0, -nh_torso_width, 0.0, 0.0, 0.0, 0.0);

  float nh_neck_radius, nh_neck_height;
  nh_neck_radius = 0.07 * nh_torso_radius / non_humanoid_factor;
  nh_neck_height = 0.2 * nh_torso_radius / non_humanoid_factor;
  cylinder(nh_neck_radius, nh_neck_height);
  node_nh2 = new csX75::HNode(node_nh1, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, dog_skin);
  node_nh2->change_parameters(0.0, (0.4)*nh_neck_height, 0.1, 45.0, 0.0, 0.0);

  float nh_face_radius;
  nh_face_radius = (1.0)*nh_torso_radius;
  sphere(nh_face_radius, tesselation, tesselation);
  node_nh3 = new csX75::HNode(node_nh2, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, dog_skin);
  node_nh3->change_parameters(0.0, nh_face_radius, 0.0, 0.0, 0.0, 0.0);

  float nh_ears_radius;
  nh_ears_radius = nh_face_radius;
  specs(nh_ears_radius);
  node_nh3_ears = new csX75::HNode(node_nh3, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, texture_black);
  node_nh3_ears->change_parameters(0.0, 0.0, 0.0, -45.0, 0.0, 0.0);

  float nh_specs_radius;
  nh_specs_radius = nh_face_radius/2;
  specs(nh_specs_radius);
  node_nh3_specs = new csX75::HNode(node_nh3, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, dog_specs);
  node_nh3_specs->change_parameters(0.0, (0.9)*nh_face_radius, -(0.5)*nh_face_radius, 135.0, 0.0, 0.0);

  float nh_ear_radius, nh_ear_height, nh_ear_radius2;
  nh_ear_radius = 0.08 * nh_torso_radius / non_humanoid_factor;
  nh_ear_radius2 = 0.04 * nh_torso_radius / non_humanoid_factor;
  nh_ear_height = 0.15 * nh_torso_radius / non_humanoid_factor;
  frustum(nh_ear_radius, nh_ear_radius2, nh_ear_height);
  node_nh4 = new csX75::HNode(node_nh3, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, texture_black);
  node_nh4->change_parameters(0.0, 0.0, -(0.8)*nh_face_radius, 90.0, 0.0, 0.0);

  float nh_front_left_radius, nh_front_left_height;
  nh_front_left_radius = 0.05 * nh_torso_radius / non_humanoid_factor;
  nh_front_left_height = 0.4 * nh_torso_radius / non_humanoid_factor;
  cylinder(nh_front_left_radius, nh_front_left_height);
  node_nh8 = new csX75::HNode(node_nh1, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, dog_skin);
  node_nh8->change_parameters(-nh_torso_radius + (1.5)*nh_front_left_radius, -2*nh_front_left_radius, -nh_front_left_height/3, 90.0, 0.0, 0.0);
  non_humanoid_shift += nh_front_left_height;

  float nh_front_right_radius, nh_front_right_height;
  nh_front_right_radius = 0.05 * nh_torso_radius / non_humanoid_factor;
  nh_front_right_height = 0.4 * nh_torso_radius / non_humanoid_factor;
  cylinder(nh_front_right_radius, nh_front_right_height);
  node_nh7 = new csX75::HNode(node_nh1, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, dog_skin);
  node_nh7->change_parameters(nh_torso_radius - (1.5)*nh_front_right_radius, -2*nh_front_right_radius, -nh_front_right_height/3, 90.0, 0.0, 0.0);

  float nh_back_left_radius, nh_back_left_height;
  nh_back_left_radius = 0.05 * nh_torso_radius / non_humanoid_factor;
  nh_back_left_height = 0.4 * nh_torso_radius / non_humanoid_factor;
  cylinder(nh_back_left_radius, nh_back_left_height);
  node_nh10 = new csX75::HNode(node_nh1, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, dog_skin);
  node_nh10->change_parameters(-nh_torso_radius + (1.5)*nh_back_left_radius, -nh_torso_width +2*nh_back_left_radius, -nh_back_left_height/3, 90.0, 0.0, 0.0);

  float nh_back_right_radius, nh_back_right_height;
  nh_back_right_radius = 0.05 * nh_torso_radius / non_humanoid_factor;
  nh_back_right_height = 0.4 * nh_torso_radius / non_humanoid_factor;
  cylinder(nh_back_right_radius, nh_back_right_height);
  node_nh9 = new csX75::HNode(node_nh1, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, dog_skin);
  node_nh9->change_parameters(nh_torso_radius - (1.5)*nh_back_right_radius, -nh_torso_width +2*nh_back_right_radius, -nh_back_right_height/3, 90.0, 0.0, 0.0);

  float nh_tail_radius, nh_tail_height;
  nh_tail_radius = 0.05 * nh_torso_radius / non_humanoid_factor;
  nh_tail_height = 0.4 * nh_torso_radius / non_humanoid_factor;
  frustum(nh_tail_radius, 0.0, nh_tail_height);
  node_nh11 = new csX75::HNode(node_nh1, num_vertices, v_positions, v_colors, v_normals, tex_coords, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec4) * num_vertices, sizeof(glm::vec2) * num_vertices, dog_skin);
  node_nh11->change_parameters(0.0, -nh_torso_width+0.05, 0.0, -30.0, 0.0, 0.0);

  root_node = curr_node = node_h1;
  node_nh1->change_parameters(2.0, non_humanoid_shift, 0.0, 90.0, 180.0, -45.0);
}

void load_frame_vector(std::vector<float>& frame_vector)
{
  frame_vector.clear();
  float input;
  for(int i=0; i<camera_light_count; i++)
  {
    if(!(keyframes_in>>input))
    {
      std::cout<<"File reading done"<<std::endl;
      end_of_file = 1;
      return;
    }
    else
    {
      frame_vector.push_back(input);
    }
  }

  node_h1->load_frame_vector_hierarchy(frame_vector);
  node_nh1->load_frame_vector_hierarchy(frame_vector);
  node_e1->load_frame_vector_hierarchy(frame_vector);
  node_e2->load_frame_vector_hierarchy(frame_vector);
}

void load_values_from_vectorframe()
{
  // Loading camera and light values
  light1_value = current_frame[0];
  light2_value = current_frame[1];
  spotlight1_value = current_frame[2];
  if(!bezier_interpolate)
  {
    c_xpos = current_frame[3];
    c_ypos = current_frame[4];
    c_zpos = current_frame[5];
  }
  c_xrot = current_frame[6];
  c_yrot = current_frame[7];
  c_zrot = current_frame[8];

  int next_index = node_h1->load_values_from_vectorframe_hierarchy(camera_light_count);
  next_index = node_nh1->load_values_from_vectorframe_hierarchy(next_index);
  next_index = node_e1->load_values_from_vectorframe_hierarchy(next_index);
  next_index = node_e2->load_values_from_vectorframe_hierarchy(next_index);
}

void renderGL(void)
{


  // This if for time interval also in the ketframes.txt
  if(play_animation)
  {
    if(!end_of_file)
    {
      // Assuming initialization of frames is already done for the first time in gl_framework.cpp itself
      if(!initialization_done)
      {
        start_time = glfwGetTime();

        // Initializing the starting frames
        if(!end_of_file)
        {
          load_frame_vector(frame1);
        }
        if(!end_of_file)
        {
          load_frame_vector(frame2);
        }
        keyframes_in>>present_between_time_interval;
        total_time_till_now += present_between_time_interval;

        frame_count = 0;
        initialization_done = 1;
      }

      present_time = glfwGetTime();

      // if frames needs to be updated for the next keyframes
      float between_frame_time = present_time - start_time;
      if(between_frame_time >= total_time_till_now)
      {
        between_frame_time -= total_time_till_now;
        // A key frame gap has been covered
        frame1 = frame2;
        load_frame_vector(frame2);
        if(end_of_file)
        {
          frame2 = frame1;
          // No need of keeping any value to present_between_time_interval as current_frame will any how become frame1
        }
        else
        {
          keyframes_in>>present_between_time_interval;
          total_time_till_now += present_between_time_interval;
          frame_count++;
        }
      }
      else if(frame_count > 0)
      {
        between_frame_time = between_frame_time - total_time_till_now + present_between_time_interval;
      }

      current_frame.clear();

      for(int i=0; i<frame1.size(); i++)
      {
        current_frame.push_back((1 - (between_frame_time/present_between_time_interval))*frame1[i] + (between_frame_time/present_between_time_interval)*frame2[i]);
      }

      // Update the values from current_frame
      load_values_from_vectorframe();
      if(recording)
      {
        if(capture_toggle == 2)
        {
          capture_frame(framenum++);
          capture_toggle = 0;
        }
        else
        {
          capture_toggle++;
        }
      }

      if(bezier_interpolate) // update camera positions accordingly
      {
        float parameter = (total_time_till_now + between_frame_time - bezier_start_time) / bezier_divide;
        if(parameter > 1)
        {
          parameter = 1;
        }
        c_xpos = 1*(pow(1-parameter, 3))*cpt1.x + 3*(pow(1-parameter, 2)*(pow(parameter, 1)))*cpt2.x + 3*(pow(1-parameter, 1)*(pow(parameter, 2)))*cpt3.x + 1*(pow(parameter, 3))*cpt4.x;
        c_ypos = 1*(pow(1-parameter, 3))*cpt1.y + 3*(pow(1-parameter, 2)*(pow(parameter, 1)))*cpt2.y + 3*(pow(1-parameter, 1)*(pow(parameter, 2)))*cpt3.y + 1*(pow(parameter, 3))*cpt4.y;
        c_zpos = 1*(pow(1-parameter, 3))*cpt1.z + 3*(pow(1-parameter, 2)*(pow(parameter, 1)))*cpt2.z + 3*(pow(1-parameter, 1)*(pow(parameter, 2)))*cpt3.z + 1*(pow(parameter, 3))*cpt4.z;
      }

      if(total_time_till_now + between_frame_time >= bezier_start_time)
      {
        bezier_interpolate = 1;
      }
    }
    else
    {
      keyframes_in.close();
      bezier_interpolate = 0;
      play_animation = 0;
      present_between_time_interval = 0.0;
      total_time_till_now = 0.0;
    }
  }
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  matrixStack.clear();

  //Creating the lookat and the up vectors for the camera
  c_rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(c_xrot), glm::vec3(1.0f,0.0f,0.0f));
  c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_yrot), glm::vec3(0.0f,1.0f,0.0f));
  c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_zrot), glm::vec3(0.0f,0.0f,1.0f));

  glm::vec4 c_pos = glm::vec4(c_xpos,c_ypos,c_zpos, 1.0)*c_rotation_matrix;
  glm::vec4 c_up = glm::vec4(c_up_x,c_up_y,c_up_z, 1.0)*c_rotation_matrix;
  //Creating the lookat matrix
  lookat_matrix = glm::lookAt(glm::vec3(c_pos),glm::vec3(0.0),glm::vec3(c_up));

  //creating the projection matrix
  if(enable_perspective)
    // projection_matrix = glm::frustum(-7.0, 7.0, -7.0, 7.0, 1.0, 7.0);
    projection_matrix = glm::perspective(glm::radians(90.0),1.0,0.1,200.0);
  else
    projection_matrix = glm::ortho(-7.0, 7.0, -7.0, 7.0, -10.0, 200.0);

  view_matrix = projection_matrix*lookat_matrix;

  matrixStack.push_back(view_matrix);

  node_h1->render_tree();
  node_nh1->render_tree();
  node_e1->render_tree();
  node_e2->render_tree();
}

int main(int argc, char** argv)
{
  //! The pointer to the GLFW window
  GLFWwindow* window;

  //! Setting up the GLFW Error callback
  glfwSetErrorCallback(csX75::error_callback);

  //! Initialize GLFW
  if (!glfwInit())
    return -1;

  //We want OpenGL 4.0
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //This is for MacOSX - can be omitted otherwise
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  //We don't want the old OpenGL 
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

  //! Create a windowed mode window and its OpenGL context
  window = glfwCreateWindow(SCREEN_WIDTH_code, SCREEN_HEIGHT_code, "CS475/CS675 Assignment 4", NULL, NULL);
  if (!window)
    {
      glfwTerminate();
      return -1;
    }
  
  //! Make the window's context current 
  glfwMakeContextCurrent(window);

  //Initialize GLEW
  //Turn this on to get Shader based OpenGL
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err)
    {
      //Problem: glewInit failed, something is seriously wrong.
      std::cerr<<"GLEW Init Failed : %s"<<std::endl;
    }

  //Print and see what context got enabled
  std::cout<<"Vendor: "<<glGetString (GL_VENDOR)<<std::endl;
  std::cout<<"Renderer: "<<glGetString (GL_RENDERER)<<std::endl;
  std::cout<<"Version: "<<glGetString (GL_VERSION)<<std::endl;
  std::cout<<"GLSL Version: "<<glGetString (GL_SHADING_LANGUAGE_VERSION)<<std::endl;

  //Keyboard Callback
  glfwSetKeyCallback(window, csX75::key_callback);
  //Framebuffer resize callback
  glfwSetFramebufferSizeCallback(window, csX75::framebuffer_size_callback);

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  //Initialize GL state
  csX75::initGL();
  initBuffersGL();

  // Loop until the user closes the window
  while (glfwWindowShouldClose(window) == 0)
    {
       
      // Render here
      renderGL();

      // Swap front and back buffers
      glfwSwapBuffers(window);
      
      // Poll for and process events
      glfwPollEvents();
    }
  
  glfwTerminate();
  return 0;
}

//-------------------------------------------------------------------------

