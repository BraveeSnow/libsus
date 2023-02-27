#include "sus/window.h"

#include <algorithm>
#include <stdexcept>

namespace sus
{

//
// FORWARD DECLARATIONS
//

void default_resize_callback (GLFWwindow *window, int width, int height);
void default_update_callback (GLFWwindow *window);

//
// CLASS DEFINITION: gl_window_manager
//

gl_window_manager::gl_window_manager ()
{
  const char *err;

  if (!glfwInit ())
    {
      glfwGetError (&err);
      throw std::runtime_error (err);
    }
}

gl_window_manager::~gl_window_manager () { glfwTerminate (); }

void
gl_window_manager::register_window (gl_window &window)
{
  _contained_windows.push_back (window);
}

void
gl_window_manager::unregister_window (gl_window &window)
{
  _contained_windows.erase (std::find (_contained_windows.begin (),
                                       _contained_windows.end (), window));
}

void
gl_window_manager::close_all ()
{
  for (gl_window &window : _contained_windows)
    {
      window.close ();
      unregister_window (window);
    }
}

//
// CLASS DEFINITION: gl_window
//

gl_window::gl_window (std::string title)
    : gl_window (std::pair<int, int> (DEFAULT_WIDTH, DEFAULT_HEIGHT), title)
{
}

gl_window::gl_window (const std::pair<int, int> dims, std::string title)
{
  const char *err;

  if (!glfwInit ())
    {
      glfwGetError (&err);
      throw std::runtime_error (err);
    }

  _window = glfwCreateWindow (dims.first, dims.second, title.c_str (), nullptr,
                              nullptr);
  if (_window == nullptr)
    {
      glfwGetError (&err);
      throw std::runtime_error (err);
    }

  glfwSetFramebufferSizeCallback (_window, default_resize_callback);

  if (!_window_manager)
    {
      _window_manager = gl_window_manager ();
    }
}

gl_window::~gl_window () { glfwDestroyWindow (_window); }

void
gl_window::set_resize_callback (GLFWframebuffersizefun callback)
{
  glfwSetFramebufferSizeCallback (_window, callback);
}

std::optional<gl_window_manager> &
gl_window::get_window_manager ()
{
  return _window_manager;
}

bool
gl_window::is_closed ()
{
  return glfwWindowShouldClose (_window);
}

void
gl_window::close ()
{
  glfwSetWindowShouldClose (_window, GLFW_TRUE);
}

constexpr bool
operator== (const gl_window &win1, const gl_window &win2)
{
  // TODO: this is probably a bad idea, make a better solution
  return win1._window == win2._window;
}

//
// MISCELLANEOUS FUNCTIONS
//

void
default_resize_callback (GLFWwindow *window, int width, int height)
{
  glViewport (0, 0, width, height);
}

void
default_update_callback (GLFWwindow *window)
{
  glfwPollEvents ();
  glfwSwapBuffers (window);
}

}
