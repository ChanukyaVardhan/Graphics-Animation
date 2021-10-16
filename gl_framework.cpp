#include "gl_framework.hpp"
#include "hierarchy_node.hpp"

extern GLfloat c_xrot, c_yrot, c_zrot, c_zpos, c_ypos, c_xpos;
extern bool enable_perspective, play_animation;
extern csX75::HNode *node_h1, *node_h2, *node_h3, *node_h4, *node_h5, *node_h6, *curr_node;
extern csX75::HNode *node_h7, *node_h8, *node_h9, *node_h10, *node_h11, *node_h12;
extern csX75::HNode *node_h13, *node_h14, *node_h15, *node_h16, *node_h17, *node_h18;
extern csX75::HNode *node_h19, *node_h20, *node_h21, *node_h22, *node_h23;
extern csX75::HNode *node_nh1, *node_nh2, *node_nh3, *node_nh4, *node_nh5, *node_nh6;
extern csX75::HNode *node_nh7, *node_nh8, *node_nh9, *node_nh10, *node_nh11, *node_e1, *node_e2;
extern int light1_value, light2_value, spotlight1_value, humanoid_indicator;

extern std::ofstream keyframes;
extern std::ifstream keyframes_in;
extern float start_time, initialization_done, time_interval_between_frames;
extern bool end_of_file, recording;
extern std::vector<float> frame1, frame2;

extern void load_frame_vector(std::vector<float>& frame_vector);

bool toggle_time_input = 0; // Initially default value will be used
bool first_save = 1;

namespace csX75
{
  //! Initialize GL State
  void initGL(void)
  {
    //Set framebuffer clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //Set depth buffer furthest depth
    glClearDepth(1.0);
    //Set depth test to less-than
    glDepthFunc(GL_LESS);
    //Enable depth testing
    glEnable(GL_DEPTH_TEST);
  }

  //!GLFW Error Callback
  void error_callback(int error, const char* description)
  {
    std::cerr<<description<<std::endl;
  }

  //!GLFW framebuffer resize callback
  void framebuffer_size_callback(GLFWwindow* window, int width, int height)
  {
    //!Resize the viewport to fit the window size - draw to entire window
    glViewport(0, 0, width, height);
  }

  //!GLFW keyboard callback
  void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
  {
    //!Close the window if the ESC key was pressed
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
    else if (key == GLFW_KEY_P && action == GLFW_PRESS)
      enable_perspective = !enable_perspective;
    else if (key == GLFW_KEY_A)
      c_yrot -= 1.0;
    else if (key == GLFW_KEY_D)
      c_yrot += 1.0;
    else if (key == GLFW_KEY_W)
      c_xrot -= 1.0;
    else if (key == GLFW_KEY_S)
      c_xrot += 1.0;
    else if (key == GLFW_KEY_Q)
      c_zrot -= 1.0;
    else if (key == GLFW_KEY_E)
      c_zrot += 1.0;
    else if (key == GLFW_KEY_R)
    {
      c_xrot = 0.0;
      c_yrot = 0.0;
      c_zrot = 0.0;
    }
    else if (key == GLFW_KEY_H && action == GLFW_PRESS)
    {
      humanoid_indicator = 1 - humanoid_indicator;
      if(humanoid_indicator)
      {
        std::cout<<"Current Model : Humanoid"<<std::endl;
        curr_node = node_h1;
      }
      else
      {
        std::cout<<"Current Model : Non-Humanoid"<<std::endl;
        curr_node = node_nh1;
      }
    }
    else if (key == GLFW_KEY_KP_ADD)
      c_zpos -= 0.1;
    else if (key == GLFW_KEY_KP_SUBTRACT)
      c_zpos += 0.1;
    else if (key == GLFW_KEY_B && action == GLFW_PRESS)
      light1_value = 1 - light1_value;
    else if (key == GLFW_KEY_N && action == GLFW_PRESS)
      light2_value = 1 - light2_value;
    else if (key == GLFW_KEY_M && action == GLFW_PRESS)
      spotlight1_value = 1 - spotlight1_value;
    else if (key == GLFW_KEY_0 && action == GLFW_PRESS)
    {
      if(humanoid_indicator)
        curr_node = node_h1;
      else
        curr_node = node_nh1;
    }
    else if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
      if(humanoid_indicator)
        curr_node = node_h2;
      else
        curr_node = node_nh2;
    }
    else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
      if(humanoid_indicator)
        curr_node = node_h4;
      else
        curr_node = node_nh7;
    }
    else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
      if(humanoid_indicator)
        curr_node = node_h6;
    }
    else if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    {
      if(humanoid_indicator)
        curr_node = node_h9;
      else
        curr_node = node_nh8;
    }
    else if (key == GLFW_KEY_5 && action == GLFW_PRESS)
    {
      if(humanoid_indicator)
        curr_node = node_h11;
    }
    else if (key == GLFW_KEY_6 && action == GLFW_PRESS)
    {
      if(humanoid_indicator)
        curr_node = node_h14;
      else
        curr_node = node_nh9;
    }
    else if (key == GLFW_KEY_7 && action == GLFW_PRESS)
    {
      if(humanoid_indicator)
        curr_node = node_h16;
    }
    else if (key == GLFW_KEY_8 && action == GLFW_PRESS)
    {
      if(humanoid_indicator)
        curr_node = node_h19;
      else
        curr_node = node_nh10;
    }
    else if (key == GLFW_KEY_9 && action == GLFW_PRESS)
    {
      if(humanoid_indicator)
        curr_node = node_h21;
    }
    else if (key == GLFW_KEY_LEFT)
    {
      if(curr_node != NULL)
      {
        if(curr_node == node_h2)
        {
          if(curr_node->get_ry() > -45.0)
          {
            curr_node->dec_ry();
          }
        }
        else
          if(curr_node == node_nh1)
          {
            curr_node->dec_rz();
          }
          else
          {
            curr_node->dec_ry();
          }
      }
    }
    else if (key == GLFW_KEY_RIGHT)
    {
      if(curr_node != NULL)
      {
        if(curr_node == node_h2)
        {
          if(curr_node->get_ry() < 45.0)
          {
            curr_node->inc_ry();
          }
        }
        else
        {
          if(curr_node == node_nh1)
          {
            curr_node->inc_rz();
          }
          else
          {
            curr_node->inc_ry();
          }
        }
      }
    }
    else if (key == GLFW_KEY_UP)
    {
      if(curr_node != NULL)
      {
        if(curr_node == node_h2)
        {
          if(curr_node->get_rx() > -30.0)
          {
            curr_node->dec_rx();
          }
        }
        else
        {
          if(curr_node == node_nh1)
          {
            curr_node->dec_rx();
          }
          else
          {
            curr_node->dec_rx();
          }
        }
      }
    }
    else if (key == GLFW_KEY_DOWN)
    {
      if(curr_node != NULL)
      {
        if(curr_node == node_h2)
        {
          if(curr_node->get_rx() < 30.0)
          {
            curr_node->inc_rx();
          }
        }
        else
        {
          if(curr_node == node_nh1)
          {
            curr_node->inc_rx();
          }
          else
          {
            curr_node->inc_rx();
          }
        }
      }
    }
    else if (key == GLFW_KEY_PAGE_UP)
    {
      if(curr_node != NULL)
      {
        if(curr_node == node_h2)
        {
          if(curr_node->get_rz() > -20.0)
          {
            curr_node->dec_rz();
          }
        }
        else
        {
          if(curr_node == node_nh1)
          {
            curr_node->dec_ry();
          }
          else
          {
            curr_node->dec_rz();
          }
        }
      }
    }
    else if (key == GLFW_KEY_PAGE_DOWN)
    {
      if(curr_node != NULL)
      {
        if(curr_node == node_h2)
        {
          if(curr_node->get_rz() < 20.0)
          {
            curr_node->inc_rz();
          }
        }
        else
        {
          if(curr_node == node_nh1)
          {
            curr_node->inc_ry();
          }
          else
          {
            curr_node->inc_rz();
          }
        }
      }
    }
    else if (key == GLFW_KEY_J)
    {
      if(curr_node != NULL && (curr_node == node_h1 || curr_node == node_nh1))
        curr_node->dec_tx();
    }
    else if (key == GLFW_KEY_L)
    {
      if(curr_node != NULL && (curr_node == node_h1 || curr_node == node_nh1))
        curr_node->inc_tx();
    }
    else if (key == GLFW_KEY_I)
    {
      if(curr_node != NULL && (curr_node == node_h1 || curr_node == node_nh1))
        curr_node->inc_ty();
    }
    else if (key == GLFW_KEY_K)
    {
      if(curr_node != NULL && (curr_node == node_h1 || curr_node == node_nh1))
        curr_node->dec_ty();
    }
    else if (key == GLFW_KEY_U)
    {
      if(curr_node != NULL && (curr_node == node_h1 || curr_node == node_nh1))
        curr_node->dec_tz();
    }
    else if (key == GLFW_KEY_O)
    {
      if(curr_node != NULL && (curr_node == node_h1 || curr_node == node_nh1))
        curr_node->inc_tz();
    }
    else if (key == GLFW_KEY_G && action == GLFW_PRESS)
    {
      keyframes.open("keyframes.txt", std::ios::out | std::ios::app); // to append

      keyframes<<light1_value<<" "; // information about light 1
      keyframes<<light2_value<<" "; // information about light 2
      keyframes<<spotlight1_value<<" "; // information about spotlight
      keyframes<<c_xpos<<" "<<c_ypos<<" "<<c_zpos<<" "; // information about position of camera
      keyframes<<c_xrot<<" "<<c_yrot<<" "<<c_zrot<<" "; // information about rotation of camera

      node_h1->save_current_frame(keyframes); // information about humanoid
      node_nh1->save_current_frame(keyframes); // information about non-humanoid
      node_e1->save_current_frame(keyframes); // information about environment
      node_e2->save_current_frame(keyframes); // information about environment

      keyframes<<"\n";
      if(first_save)
      {
        first_save = 0;
        keyframes<<light1_value<<" "; // information about light 1
        keyframes<<light2_value<<" "; // information about light 2
        keyframes<<spotlight1_value<<" "; // information about spotlight
        keyframes<<c_xpos<<" "<<c_ypos<<" "<<c_zpos<<" "; // information about position of camera
        keyframes<<c_xrot<<" "<<c_yrot<<" "<<c_zrot<<" "; // information about rotation of camera

        node_h1->save_current_frame(keyframes); // information about humanoid
        node_nh1->save_current_frame(keyframes); // information about non-humanoid
        node_e1->save_current_frame(keyframes); // information about environment
        node_e2->save_current_frame(keyframes); // information about environment

        keyframes<<"\n";
        keyframes<<0<<" ";
        keyframes<<"\n";
      }
      else
      {
        if(toggle_time_input)
        {
          float frame_interval_time;
          std::cout<<"Enter the time interval between the frames"<<std::endl;
          std::cin>>frame_interval_time;

          keyframes<<frame_interval_time<<" ";
          keyframes<<"\n";
        }
        else
        {
          keyframes<<time_interval_between_frames<<" ";
          keyframes<<"\n";
        }
      }
      keyframes.close();
      std::cout<<"Saved current frame into keyframes.txt"<<std::endl;
    }
    else if (key == GLFW_KEY_T && action == GLFW_PRESS)
    {
      toggle_time_input = 1 - toggle_time_input;
      if(toggle_time_input)
      {
        std::cout<<"Currently time between frames is expected as input after each frame"<<std::endl;
      }
      else
      {
        std::cout<<"Default value of 1.0 sec time interval is chosen"<<std::endl;
      }
    }
    else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {

      play_animation = 1;
      initialization_done = 0;
      end_of_file = 0;
      keyframes_in.open("keyframes.txt", std::ios::in);

    }
    else if (key == GLFW_KEY_Y && action == GLFW_PRESS)
    {
      recording = 1 - recording;
      std::cout<<"Recording Value "<<recording<<std::endl;
    }
  }
};