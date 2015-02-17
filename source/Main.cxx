#include "Rex.hxx"
#include "Debug.hxx"
#include <chrono>
#include <thread>

// Windows includes to open the output image
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#define NOMINMAX
#include <Windows.h>
#include <shellapi.h>


// a simple ray tracer for drawing multiple objects
struct MultiObjectTracer : public rex::Tracer
{
    MultiObjectTracer( rex::Scene* scene )
        : rex::Tracer( scene )
    {
    }
    virtual rex::Color Trace( const rex::Ray& ray ) const
    {
        return Trace( ray, 0 );
    }
    virtual rex::Color Trace( const rex::Ray& ray, int depth ) const
    {
        rex::ShadePoint sp = _pScene->HitObjects( ray );
        if ( sp.HasHit )
        {
            return sp.Color;
        }
        return _pScene->GetBackgroundColor();
    }
};


int main( int argc, char** argv )
{
    using namespace rex;

    // seed the random number generator
    Random::Seed( static_cast<uint32>( time( 0 ) ) );


    // create the scene
    WriteLine( "Building scene..." );
    Scene scene;
    scene.SetSamplerType<RegularSampler>( 4 );
    scene.SetTracerType<MultiObjectTracer>();
    scene.SetCameraType<PerspectiveCamera>();
    {
        PerspectiveCamera* camera = reinterpret_cast<PerspectiveCamera*>( scene.GetCamera().get() );
        camera->SetPosition( 0.0, 0.0, 750.0 );
        camera->SetTarget( 0.0, 0.0, 0.0 );
        camera->SetUp( 0.0, 1.0, 0.0 );
        camera->SetViewDistance( 1000.0f );
    }
    scene.Build( 400, 400, 0.5f );


    // render the scene and time it
    Timer timer;
    timer.Start();
    {
        scene.Render();
    }
    timer.Stop();


    // save and open the image
    rex::WriteLine( "Saving image..." );
    scene.GetImage()->Save( "output.png" );
    ShellExecute( 0, 0, TEXT( "output.png" ), 0, 0, SW_SHOW );
    

    // now write out how long the rendering took
    rex::WriteLine( "Render stats:" );
#if __DEBUG__
    rex::WriteLine( "  Build mode: Debug" );
#else
    rex::WriteLine( "  Build mode: Release" );
#endif
    rex::WriteLine( "  Image size: ", scene.GetImage()->GetWidth(), "x", scene.GetImage()->GetHeight() );
    rex::WriteLine( "  Duration:   ", timer.GetElapsed(), " seconds" );


    rex::ReadLine();
    return 0;
}
