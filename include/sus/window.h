#ifndef _SUS_WINDOW_H_
#define _SUS_WINDOW_H_

#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <GLFW/glfw3.h>

typedef void (*gl_window_update_callback_t)(GLFWwindow *);

/**
 * The default width used.
 */
#define DEFAULT_WIDTH 640
#define DEFAULT_HEIGHT 480

namespace sus
{

  class gl_window;

  /**
   * @brief Manages all window instances.
   *
   * Provides an interface to manage all active windows as well as the GLFW
   * backend.
   *
   * @remarks The user should not need to instantiate this class. It is
   * instantiated in the first instance of a `gl_window`.
   */
  class gl_window_manager
  {
    std::vector<gl_window> _contained_windows;

  public:
    gl_window_manager();
    ~gl_window_manager();

    /**
     * @brief Signals all windows to close.
     *
     * @see gl_window::close
     */
    void close_all();

  private:
    void register_window(gl_window &window);
    void unregister_window(gl_window &window);
  };

  /**
   * @brief Holds information about a window.
   *
   * The `gl_window` class contains details about a window and provides a simple
   * interface to interact with OpenGL.
   */
  class gl_window
  {
    friend constexpr bool operator==(const gl_window &win1,
                                     const gl_window &win2);

  public:
    /**
     * @brief Constructs a new `gl_window` object with the default window size.
     *
     * @param title The title of the window.
     */
    gl_window(std::string title);
    /**
     * @brief Construct a new `gl_window` object using the given integer pair as
     * its size.
     *
     * @param dims The dimensions of the window.
     * @param title The title of the window.
     */
    gl_window(const std::pair<int, int> dims, std::string title);
    ~gl_window();

    /**
     * @brief Sets the function that the window calls upon resizing.
     *
     * @warning If you plan on changing the framebuffer resize callback, note
     * that it will @b replace the existing callback.
     *
     * @param callback The new callback to use.
     */
    void set_resize_callback(GLFWframebuffersizefun callback);

    /**
     * @brief Sets the callback used in the update method.
     *
     * The default update function may not be sufficient in some cases. This
     * method exists so that developers can set more advanced update callbacks
     * when needed.
     *
     * @attention If you plan on changing the update callback, note that it will
     * @b replace the existing callback.
     *
     * @param callback The new callback to be used in the update method.
     */
    void set_update_callback(gl_window_update_callback_t callback);

    /**
     * @brief Gets the internal window manager.
     *
     * @return std::optional<gl_window_manager>& A reference to the window
     * manager.
     */
    std::optional<gl_window_manager> &get_window_manager();

    /**
     * @brief Checks if the current window has been signalled to close.
     *
     * You can use this method as the condition for an event loop and things of
     * that nature.
     *
     * @see gl_window::close
     *
     * @return true The window has been closed.
     * @return false The window is still alive.
     */
    bool is_closed();

    /**
     * @brief Signals the current `gl_window` object to close.
     *
     * @see gl_window#is_closed
     * @see gl_window_manager::close_all
     */
    void close();

  private:
    GLFWwindow *_window;

    static inline std::optional<gl_window_manager> _window_manager{};
  };

}

#endif
