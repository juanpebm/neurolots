/**
 * @file    OpenGLWidget.cpp
 * @brief
 * @author  Juan José García <juanjose.garcia@urjc.es>,
 * Pablo Toharia <pablo.toharia@urjc.es>
 * @date    2015
 * @remarks Copyright (c) 2015 GMRV/URJC. All rights reserved.
 * Do not distribute without further notice.
 */

#include "OpenGLWidget.h"
#include <QOpenGLContext>
#include <QMouseEvent>
#include <QColorDialog>
#include <QFileDialog>
#include <sstream>
#include <string>
#include <iostream>

#include "MainWindow.h"
#include "../nlrender/Config.h"

const float OpenGLWidget::_colorFactor = 1 / 255.0f;

OpenGLWidget::OpenGLWidget( QWidget* parent_,
                            Qt::WindowFlags windowsFlags_,
                            const std::string&
#ifdef NEUROLOTS_USE_ZEROEQ
                            zeqSession
#endif
  )
  : QOpenGLWidget( parent_, windowsFlags_ )
  , _fpsLabel( this )
  , _showFps( false )
  , _wireframe( false )
  , _neuronsCollection( nullptr )
  , _frameCount( 0 )
  , _mouseX( 0 )
  , _mouseY( 0 )
  , _rotation( false )
  , _translation( false )
  , _idleUpdate( true )
  , _paint( false )
  , _neuron( nullptr )
  , _translationScale( 0.1f )
  , _rotationScale( 0.01f )
{
#ifdef NEUROLOTS_USE_ZEROEQ
  _camera = new reto::Camera( zeqSession );
#else
  _camera = new reto::Camera( );
#endif

  _cameraTimer = new QTimer( );
  _cameraTimer->start(( 1.0f / 60.f ) * 100 );
  connect( _cameraTimer, SIGNAL( timeout( )), this, SLOT( timerUpdate( )));

  _fpsLabel.setStyleSheet(
    "QLabel { background-color : #333;"
    "color : white;"
    "padding: 3px;"
    "margin: 10px;"
    " border-radius: 10px;}" );

  // This is needed to get key evends
  this->setFocusPolicy( Qt::WheelFocus );

  _lastSavedFileName = QDir::currentPath( );

}


OpenGLWidget::~OpenGLWidget( void )
{
  delete _camera;
}

void OpenGLWidget::createNeuronsCollection( const std::string& zeqSession )
{
  makeCurrent( );
  nlrender::Config::init( );
  _neuronsCollection = new nlrender::NeuronsCollection( _camera );

  _neuronsCollection->setZeqUri( zeqSession );
}

void OpenGLWidget::loadData( const std::string& fileName,
                             const TDataFileType fileType,
                             const std::string& target )
{

  makeCurrent( );

  switch( fileType )
  {
  case TDataFileType::BlueConfig:
    _neuronsCollection->loadBlueConfig( fileName, target );
    break;

  case TDataFileType::SWC:
    _neuronsCollection->loadSwc( fileName );
    break;

  case TDataFileType::NsolScene:
    _neuronsCollection->loadScene( fileName );
    break;

  default:
    throw std::runtime_error( "Data file type not supported" );

  }

  this->_paint = true;
  update( );

  return;
}


void OpenGLWidget::initializeGL( void )
{
  initializeOpenGLFunctions( );

  glEnable( GL_DEPTH_TEST );
  glClearColor(  0.0f, 0.0f, 0.0f, 1.0f );
  glPolygonMode( GL_FRONT_AND_BACK , GL_FILL );
  glEnable( GL_CULL_FACE );

  glLineWidth( 1.5 );

  _then = std::chrono::system_clock::now( );


  QOpenGLWidget::initializeGL( );

}

void OpenGLWidget::paintGL( void )
{

  _frameCount++;
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  if ( _paint )
  {
    if ( _neuron )
      //_neuronsCollection->extractMesh( _neuron );
      _neuronsCollection->paintNeuron( _neuron );
    else if ( _neuronsCollection )
      _neuronsCollection->paint( );

    glUseProgram( 0 );
    glFlush( );

  }

  #define FRAMES_PAINTED_TO_MEASURE_FPS 10
  if ( _frameCount % FRAMES_PAINTED_TO_MEASURE_FPS  == 0 )
  {

    std::chrono::time_point< std::chrono::system_clock > now =
      std::chrono::system_clock::now( );

    auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>( now - _then );
    _then = now;

    MainWindow* mainWindow = dynamic_cast< MainWindow* >( parent( ));
    if ( mainWindow )
    {

      unsigned int ellapsedMiliseconds = duration.count( );

      unsigned int fps = roundf( 1000.0f *
                                 float( FRAMES_PAINTED_TO_MEASURE_FPS ) /
                                 float( ellapsedMiliseconds ));

      // mainWindow->showStatusBarMessage(
      //   QString::number( fps ) + QString( " FPS" ));
      if ( _showFps )
      {
        _fpsLabel.setVisible( true );
        _fpsLabel.setText( QString::number( fps ) + QString( " FPS" ));
        _fpsLabel.adjustSize( );
      }
      else
        _fpsLabel.setVisible( false );
    }

  }

  if ( _idleUpdate )
  {
    // std::cout << _frameCount << std::endl;
    update( );
  }
  else
  {
    _fpsLabel.setVisible( false );
  }

}

void OpenGLWidget::resizeGL( int w , int h )
{
  _camera->ratio((( double ) w ) / h );
  glViewport( 0, 0, w, h );


}


void OpenGLWidget::mousePressEvent( QMouseEvent* event_ )
{

  if ( event_->button( ) == Qt::LeftButton )
  {
    _rotation = true;
    _mouseX = event_->x( );
    _mouseY = event_->y( );
  }

  if ( event_->button( ) ==  Qt::MidButton )
  {
    _translation = true;
    _mouseX = event_->x( );
    _mouseY = event_->y( );
  }

  update( );

}

void OpenGLWidget::mouseReleaseEvent( QMouseEvent* event_ )
{
  if ( event_->button( ) == Qt::LeftButton)
  {
    _rotation = false;
  }

  if ( event_->button( ) ==  Qt::MidButton )
  {
    _translation = false;
  }

  update( );

}

void OpenGLWidget::mouseMoveEvent( QMouseEvent* event_ )
{
  if( _rotation )
  {
      _camera->localRotation( -( _mouseX - event_->x( )) * _rotationScale,
                              -( _mouseY - event_->y( )) * _rotationScale );

      _mouseX = event_->x( );
      _mouseY = event_->y( );
  }
  if( _translation )
  {
      float xDis = ( event_->x() - _mouseX ) * _translationScale;
      float yDis = ( event_->y() - _mouseY ) * _translationScale;

      _camera->localTranslation( Eigen::Vector3f( -xDis, yDis, 0.0f ));
      _mouseX = event_->x( );
      _mouseY = event_->y( );
  }

  this->update( );
}


void OpenGLWidget::wheelEvent( QWheelEvent* event_ )
{

  int delta = event_->angleDelta( ).y( );

  if ( delta > 0 )
    _camera->radius( _camera->radius( ) / 1.1f );
  else
    _camera->radius( _camera->radius( ) * 1.1f );

  update( );

}



void OpenGLWidget::keyPressEvent( QKeyEvent* event_ )
{
  makeCurrent( );

  switch ( event_->key( ))
  {
  case Qt::Key_C:
    _camera->pivot( Eigen::Vector3f( 0.0f, 0.0f, 0.0f ));
    _camera->radius( 1000.0f );
    _camera->rotation( 0.0f, 0.0f );
    update( );
    break;
  }
}


void OpenGLWidget::changeClearColor( QColor color )
{
    makeCurrent( );
    glClearColor( float( color.red( )) * _colorFactor,
                  float( color.green( )) * _colorFactor,
                  float( color.blue( )) * _colorFactor,
                  float( color.alpha( )) * _colorFactor);
    update( );
}

void OpenGLWidget::changeNeuronColor( QColor color )
{
  makeCurrent( );
  _neuronsCollection->neuronColor(
    Eigen::Vector3f( float( color.red( )) * _colorFactor,
                     float( color.green( )) * _colorFactor,
                     float( color.blue( )) * _colorFactor ));
  update( );
}

void OpenGLWidget::changeSelectedNeuronColor(  QColor color )
{
  makeCurrent( );
  _neuronsCollection->selectedNeuronColor(
    Eigen::Vector3f( float( color.red( )) * _colorFactor,
                     float( color.green( )) * _colorFactor,
                     float( color.blue( )) * _colorFactor ));
  update( );
}

void OpenGLWidget::changeNeuronPiece( int index_ )
{
  switch( index_ )
  {
  case 0:
    _neuronsCollection->paintSoma( true );
    _neuronsCollection->paintNeurites( true );
    break;
  case 1:
    _neuronsCollection->paintSoma( true );
    _neuronsCollection->paintNeurites( false );
    break;
  case 2:
    _neuronsCollection->paintSoma( false );
    _neuronsCollection->paintNeurites( true );
    break;
  }
  update( );
}

void OpenGLWidget::changeSelectedNeuronPiece( int index_ )
{
  switch( index_ )
  {
  case 0:
    _neuronsCollection->paintSelectedSoma( true );
    _neuronsCollection->paintSelectedNeurites( true );
    break;
  case 1:
    _neuronsCollection->paintSelectedSoma( true );
    _neuronsCollection->paintSelectedNeurites( false );
    break;
  case 2:
    _neuronsCollection->paintSelectedSoma( false );
    _neuronsCollection->paintSelectedNeurites( true );
    break;
  }
  update( );
}


void OpenGLWidget::toggleUpdateOnIdle( void )
{
  _idleUpdate = !_idleUpdate;
  if ( _idleUpdate )
    update( );
}

void OpenGLWidget::toggleShowFPS( void )
{
  _showFps = !_showFps;
  if ( _idleUpdate )
    update( );
}

void OpenGLWidget::toggleWireframe( void )
{
  makeCurrent( );
  _wireframe = !_wireframe;

  if ( _wireframe )
  {
    glEnable( GL_POLYGON_OFFSET_LINE );
    glPolygonOffset( -1, -1 );
    glLineWidth( 1.5 );
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  }
  else
  {
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glDisable( GL_POLYGON_OFFSET_LINE );
  }

  update( );
}

void OpenGLWidget::timerUpdate( void )
{
  if( _camera->anim( ) || _neuronsCollection->selectionChange( ))
    this->update( );
}

void OpenGLWidget::extractMesh( void )
{
  if( _neuron )
  {

    QString path;
    QString filter( tr( "OBJ (*.obj);; All files (*)" ));
    QFileDialog* fd = new QFileDialog( this,
                                       QString( "Save mesh to OBJ file" ),
                                       _lastSavedFileName,
                                       filter);

    fd->setOption( QFileDialog::DontUseNativeDialog, true );
    fd->setDefaultSuffix( "obj") ;
    fd->setFileMode( QFileDialog/*::FileMode*/::AnyFile );
    fd->setAcceptMode( QFileDialog/*::AcceptMode*/::AcceptSave );
    if ( fd->exec( ))
      path = fd->selectedFiles( )[0];

    if ( path != QString( "" ))
    {
      std::cout << "file " << path.toStdString( ) << std::endl;

      _lastSavedFileName = QFileInfo( path ).path( );
      this->makeCurrent( );
      _neuronsCollection->extractMesh( _neuron, path.toStdString( ));
      glUseProgram( 0 );

    }
  }
}

void OpenGLWidget::onLotValueChanged( int value_ )
{
  _neuronsCollection->lod( ( float ) value_ );
  update( );
}

void OpenGLWidget::onDistanceValueChanged( int value_ )
{
  _neuronsCollection->maxDist(( float ) value_ / 1000.0f );
  update( );
}

void OpenGLWidget::onTangValueChanged( int value_ )
{
  _neuronsCollection->tng(( float ) value_ / 50.0f );
  update( );
}

void OpenGLWidget::onHomogeneousClicked( void )
{
  _neuronsCollection->tessMethod( nlrender::NeuronsCollection::HOMOGENEOUS );
  update( );
}

void OpenGLWidget::onLinearClicked( void )
{
  _neuronsCollection->tessMethod( nlrender::NeuronsCollection::LINEAR );
  update( );
}
