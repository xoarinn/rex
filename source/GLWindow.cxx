#include <rex/GL/GLWindow.hxx>
#include <rex/Utility/Logger.hxx>
#include <rex/OpenGL.hxx>
#include <GLFW/glfw3.h>

#pragma warning( disable : 4800 ) // * -> bool conversion

REX_NS_BEGIN

// the total number of windows
uint32 GLWindow::_windowCount = 0;

// create new window w/ width and height
GLWindow::GLWindow( int32 width, int32 height )
    : GLWindow( width, height, "REX", GLWindowHints() )
{
}

// create new window w/ width, height, and title
GLWindow::GLWindow( int32 width, int32 height, const String& title )
    : GLWindow( width, height, title, GLWindowHints() )
{
}

// create new window w/ width, height, title, and hints
GLWindow::GLWindow( int32 width, int32 height, const String& title, const GLWindowHints& hints )
{
    // ensure GLFW can initialize
    if ( !InitializeGlfw() )
    {
        return;
    }


    // set the window hints
    glfwDefaultWindowHints();
    glfwWindowHint( GLFW_RESIZABLE,             hints.Resizable );
    glfwWindowHint( GLFW_VISIBLE,               hints.Visible   );
    glfwWindowHint( GLFW_DEPTH_BITS,            0 );
    glfwWindowHint( GLFW_STENCIL_BITS,          0 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );
#if __DEBUG__
    glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT,  GL_TRUE );
#endif


    // get the window for fullscreen mode
    int32         monitorCount = 0;
    GLFWmonitor** monitors = glfwGetMonitors( &monitorCount );
    GLFWmonitor*  monitor = nullptr;
    if ( hints.Fullscreen )
    {
        monitor = monitors[ 0 ]; // fullscreen on default monitor
    }

    
    // create the window
    GLFWwindow* window = glfwCreateWindow( width, height, title.c_str(), monitor, nullptr );


    // ensure the window was created
    if ( !window )
    {
        Logger::Log( "Failed to create GLFW window" );
        return;
    }


    // make the window current on this thread and set the swap interval
    glfwMakeContextCurrent( window );
    glfwSwapInterval( hints.VSync );


    // initialize GLEW and ensure we can support the graphics card
    if ( ( glewInit() != GLEW_OK ) && !GLEW_VERSION_4_0 )
    {
        Logger::Log( "Failed to load OpenGL 4.0" );
        glfwDestroyWindow( window );
        return;
    }


    // center the window if it's not fullscreen
    if ( !hints.Fullscreen )
    {
        // because we're centering on the default monitor, we don't need to worry about the monitor's position
        const GLFWvidmode* vmode = glfwGetVideoMode( monitors[ 0 ] );
        glfwSetWindowPos( window,
                          ( vmode->width - width ) / 2,
                          ( vmode->height - height ) / 2 );
    }


    // setup our viewport
    int32 viewportWidth  = 0;
    int32 viewportHeight = 0;
    glfwGetFramebufferSize( window, &viewportWidth, &viewportHeight );
    glViewport( 0, 0, viewportWidth, viewportHeight );


    // record our window
    ++_windowCount;
    _handle.reset( window, []( void* hWindow )
    {
        glfwDestroyWindow( static_cast<GLFWwindow*>( hWindow ) );
    } );
}

// copy another window
GLWindow::GLWindow( const GLWindow& other )
{
    // register that a new window exists
    ++_windowCount;

    // copy the handle
    _handle = other._handle;
}

// destroy window
GLWindow::~GLWindow()
{
    // decrement the window count
    --_windowCount;

    // delete our reference to the window
    _handle.reset();

    // if there are no windows, terminate GLFW
    if ( !_windowCount )
    {
        glfwTerminate();
    }
}

// initialize GLFW
bool GLWindow::InitializeGlfw()
{
    // check if we need to initialize GLFW
    if ( !_windowCount )
    {
        // set the error callback
        glfwSetErrorCallback( []( int code, const char* err )
        {
            Logger::Log( "GLFW Error: ", err, " (code: ", code, ")" );
        } );

        // try to initalize GLFW
        if ( glfwInit() != GL_TRUE )
        {
            Logger::Log( "Failed to initialize GLFW" );
            return false;
        }
    }
    return true;
}

// check if window is open
bool GLWindow::IsOpen() const
{
    GLFWwindow* window = static_cast<GLFWwindow*>( _handle.get() );
    return !static_cast<bool>( glfwWindowShouldClose( window ) );
}

// get OpenGL context
GLContext GLWindow::GetContext() const
{
    return GLContext( _handle.get() );
}

// close this window
void GLWindow::Close()
{
    GLFWwindow* window = static_cast<GLFWwindow*>( _handle.get() );
    glfwSetWindowShouldClose( window, GL_TRUE );
}

// hide this window
void GLWindow::Hide()
{
    GLFWwindow* window = static_cast<GLFWwindow*>( _handle.get() );
    glfwHideWindow( window );
}

// poll events
void GLWindow::PollEvents()
{
    glfwPollEvents();
}

// show this window
void GLWindow::Show()
{
    GLFWwindow* window = static_cast<GLFWwindow*>( _handle.get() );
    glfwShowWindow( window );
}

// convert window to bool
GLWindow::operator bool() const
{
    return static_cast<bool>( _handle );
}

REX_NS_END