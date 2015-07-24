//OpenGL
#include <GL/glew.h>
#ifdef Darwin
#include <gl.h>
#include <glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#endif

//Eigen
#include "Eigen/Dense"

//Neurolots
#include "Camera.h"
#include "Program.h"
#include "NeuronMesh.h"
#include "NeuronsCollection.h"


#include <iostream>

#ifdef NEUROLOTS_WITH_DEFLECT
#include <deflect/deflect.h>
#endif


#define WINDOW_TITLE_PREFIX "NeuroLoTs"

using Eigen::Vector3d;
using namespace std;
using namespace neurolots;


bool rotation = false;
bool translation = false;

bool paintSoma = true;
bool paintNeurites = true;

int m_x, m_y;

bool mode = true;

unsigned int frameCount = 0;

Camera* camera;
NeuronsCollection* neuronsCollection;

#ifdef NEUROLOTS_WITH_DEFLECT

deflect::Stream* deflectStream;

bool deflectCompressImage = true;
unsigned int deflectCompressionQuality = 75;

void handleStreamingError(const char* errorMessage)
{
    std::cerr << errorMessage <<  std::endl;
}

void startStreaming(const char* deflectHostName = "NeuroLOTs",
                    const char* deflectHostAddress = "localhost")
{
  if( deflectStream )
      return;

  deflectStream = new deflect::Stream( deflectHostName,
                                        deflectHostAddress);
  if (!deflectStream->isConnected())
  {
      handleStreamingError("Could not connect to host!");
      return;
  }
  deflectStream->registerForEvents();

}

void stopStreaming( void )
{
  delete deflectStream;
  deflectStream = nullptr;
}

void processDeflectEvents( void )
{
  if( !deflectStream->isRegisteredForEvents( ))
    return;

  deflect::Event lastEvent;

  if ( deflectStream->hasEvent( ))
  std::cout << "------------------------------------------" << std::endl;

  unsigned int windowWidth = glutGet( GLUT_WINDOW_WIDTH ),
               windowHeight = glutGet( GLUT_WINDOW_HEIGHT );

  while( deflectStream->hasEvent( ))
  {

    const deflect::Event& wallEvent = deflectStream->getEvent();

    float mouseX = wallEvent.mouseX * windowWidth;
    float mouseY = wallEvent.mouseY * windowHeight;

    switch (wallEvent.type)
    {
      case deflect::Event::EVT_PRESS:
        rotation = true;
        m_x = mouseX;
        m_y = mouseY;
        break;

      case deflect::Event::EVT_RELEASE:
        rotation = false;
        break;
      case deflect::Event::EVT_MOVE:
        if( rotation )
          {
            camera->LocalRotation( -( m_x - mouseX ) * 0.01,
                                   -( m_y - mouseY ) * 0.01 );
            m_x = mouseX;
            m_y = mouseY;
          }
        break;
      case deflect::Event::EVT_WHEEL:

        if (wallEvent.dy < 0)
          camera->Radius( camera->Radius( ) / 1.1f );
        else
          camera->Radius( camera->Radius( ) * 1.1f );
        break;

      default:
        break;


    }

  }
}


void sendDeflectFrameImage( bool compress = true,
                            unsigned int compressedQuality = 75 )
{

  // Grab the frame from OpenGL
  const int windowWidth = glutGet( GLUT_WINDOW_WIDTH );
  const int windowHeight = glutGet( GLUT_WINDOW_HEIGHT );

  const size_t imageSize = windowWidth * windowHeight * 4;
  unsigned char* imageData = new unsigned char[imageSize];
  glReadPixels( 0, 0, windowWidth, windowHeight, GL_RGBA, GL_UNSIGNED_BYTE,
                (GLvoid*)imageData );

  // Send the frame through the stream
  deflect::ImageWrapper deflectImage( (const void*)imageData, windowWidth,
                                      windowHeight, deflect::RGBA );
  deflectImage.compressionPolicy = compress ?
                  deflect::COMPRESSION_ON : deflect::COMPRESSION_OFF;
  deflectImage.compressionQuality = compressedQuality;
  deflect::ImageWrapper::swapYAxis( (void*)imageData, windowWidth,
                                    windowHeight, 4 );
  const bool success = deflectStream->send( deflectImage );
  deflectStream->finishFrame();

  if ( !success )
  {
      handleStreamingError("Streaming failure, connection closed.");
      return;
  }

  delete [] imageData;
}


void updateStreaming( void )
{
  processDeflectEvents( );
  sendDeflectFrameImage( );
}

#endif

void usageMessage()
{
  std::cerr << std::endl
            << "Usage: "
            << "neurolots" << " "
            << "(-bc blue_config_path | -swc swc_file_list) "
            << "-zeq uri"
            << "-pw host"
            << std::endl << std::endl;
  exit(-1);
}

void sceneInit( void )
{
  glEnable( GL_DEPTH_TEST );
  glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
  glPolygonMode( GL_FRONT_AND_BACK , GL_FILL );
  glEnable( GL_CULL_FACE );

  glLineWidth( 1.5 );

}

void paintFunc(void)
{
  frameCount ++;
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  neuronsCollection->Paint( );

#ifdef NEUROLOTS_WITH_DEFLECT
  updateStreaming();
#endif

  glUseProgram( 0 );
  glFlush( );
  glutSwapBuffers( );
}

void keyboardFunc( unsigned char key, int /* _x */, int /* _y */ )
{
  switch( key )
  {
    case 'm':
      mode = !mode;
      if ( mode )
      {
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        glDisable( GL_POLYGON_OFFSET_LINE );
      }
      else
      {
        glEnable( GL_POLYGON_OFFSET_LINE );
        glPolygonOffset( -1, -1 );
        glLineWidth( 1.5 );
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
      }
      break;
    case 'c':
      camera->Pivot( Eigen::Vector3f( 0.0f, 0.0f, 0.0f ));
      camera->Radius( 1000.0f );
      camera->Rotation( 0.0f, 0.0f );
      break;
    case 'w':
      neuronsCollection->AddLod( 1.0f );
      break;
    case 's':
      neuronsCollection->AddLod( -1.0f );
      break;
    case 'e':
      neuronsCollection->AddTng( 0.1f );
      break;
    case 'd':
      neuronsCollection->AddTng( -0.1f );
      break;
    case 'r':
      neuronsCollection->AddMaxDist( 1 );
      break;
    case 'f':
      neuronsCollection->AddMaxDist( -1 );
      break;
    case 'o':
      paintSoma = !paintSoma;
      neuronsCollection->PaintSoma( paintSoma );
      break;
    case 'p':
      paintNeurites = !paintNeurites;
      neuronsCollection->PaintNeurites( paintNeurites );
      break;
  }
}

void mouseFunc( int boton, int state, int _x, int _y )
{
  if ( boton == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
  {
    rotation = true;
    m_x = _x;
    m_y = _y;
  }
  if ( boton == GLUT_LEFT_BUTTON && state == GLUT_UP )
  {
    rotation=false;
  }
  if ( boton == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN )
  {
    translation = true;
    m_x = _x;
    m_y = _y;
  }
  if ( boton == GLUT_MIDDLE_BUTTON && state == GLUT_UP )
  {
    translation = false;
  }
  if ( boton == 3 && state == GLUT_DOWN )
  {
    camera->Radius( camera->Radius( ) / 1.1f );
  }
  if ( boton == 4 && state == GLUT_DOWN )
  {
    camera->Radius( camera->Radius( ) * 1.1f );
  }
}

void mouseMoveFunc( int _x, int _y )
{
  if( rotation )
  {
    camera->LocalRotation( -( m_x - _x ) * 0.01, -( m_y - _y ) * 0.01 );
    m_x = _x;
    m_y = _y;
  }
  if( translation )
  {
    m_x = _x;
    m_y = _y;
  }
}

void idleFunc( )
{
  glutPostRedisplay( );
}

void resizeFunc( int w, int h )
{
  camera->Ratio((( double ) w ) / h );
  glViewport( 0, 0, w, h );
}

void timerFunc( int value )
{
  if( 0 != value )
  {
    char* tmpString = new char[ 512 + strlen( WINDOW_TITLE_PREFIX )];

    sprintf( tmpString, "%s: %d FPS", WINDOW_TITLE_PREFIX, frameCount );

    glutSetWindowTitle( tmpString );
    free( tmpString );
  }

  frameCount = 0;
  glutTimerFunc( 1000, timerFunc, 1 );
}

int main( int argc, char* argv[ ])
{
  if( argc < 2 )
  {
    usageMessage( );
  }

  glutInit( &argc, argv );
  glutInitContextVersion( 4, 4 );

  glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA );
  glutInitWindowPosition( 0, 0 );
  glutInitWindowSize( 600, 600 );
  glutCreateWindow( WINDOW_TITLE_PREFIX );

  glutDisplayFunc( paintFunc );
  glutKeyboardFunc( keyboardFunc );
  glutMouseFunc( mouseFunc );
  glutMotionFunc( mouseMoveFunc );
  glutIdleFunc( idleFunc );
  glutReshapeFunc( resizeFunc );
  glutTimerFunc( 0, timerFunc, 0);

  glewExperimental = GL_TRUE;
  glewInit( );

  std::string fileName( argv[ 1 ]);

#ifdef NEUROLOTS_WITH_ZEQ
  std::string uri;
  bool connection = false;

  for( int i = 2; i < argc; i++ )
  {
    if( std::strcmp( argv[ i ], "-zeq" ) == 0 )
    {
      if( ++i < argc )
      {
        uri = std::string( argv[ i ]);
        connection = true;
      }
    }
  }

  if ( connection )
  {
    camera = new Camera( uri );
    neuronsCollection = new NeuronsCollection( uri, fileName, camera );
  }
  else
  {
    camera = new Camera( );
    neuronsCollection = new NeuronsCollection( fileName, camera );
  }
#else
  std::cerr << "ZEQ not supported" << std::endl;
  camera = new Camera( );
  neuronsCollection = new NeuronsCollection( fileName, camera );
#endif

#ifdef NEUROLOTS_WITH_DEFLECT

  std::string deflectHost ("localhost");

  for( int i = 2; i < argc; i++ )
    {
      if( std::strcmp( argv[ i ], "-pw" ) == 0 )
      {
        if( ++i < argc )
        {
          deflectHost = std::string( argv[ i ]);
        }
      }
    }

  startStreaming("NeuroLOTs", deflectHost.c_str());
#endif

  sceneInit( );

  glutMainLoop( );

  return 0;
}
