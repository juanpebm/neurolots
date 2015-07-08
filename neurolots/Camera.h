/**
 * @file    Camera.h
 * @brief
 * @author  Juan José García <juanjosegarciacan@gmail.com>
 * @date
 * @remarks Copyright (c) GMRV/URJC. All rights reserved.
 * Do not distribute without further notice.
 */

#ifndef __NEUROLOTS_CAMERA__
#define __NEUROLOTS_CAMERA__


// Eigen
#include <Eigen/Dense>

#include <vector>
#include <iostream>
#include <string>
#include <ctime>

#ifdef NEUROLOTS_WITH_ZEQ
#include <zeq/zeq.h>
#include <zeq/hbp/hbp.h>
#include <servus/uri.h>

#include <pthread.h>
#include <mutex>
#include <boost/bind.hpp>
#endif

namespace neurolots
{

  class Camera
  {

    public:

      Camera( float fov_ = 45.0f, float ratio_ = ((float)16)/9,
        float nearPlane_ = 0.1f, float farPlane_ = 10000.0f,
        Eigen::Vector3f pivot_ = Eigen::Vector3f( 0.0f, 0.0f, 0.0f ),
        float radius_ = 1000.0f, float yaw_ = 0.0f, float pitch_ = 0.0f );

#ifdef NEUROLOTS_WITH_ZEQ
      Camera( const std::string& uri_, float fov_ = 45.0f,
          float ratio_ = ((float)16)/9, float nearPlane_ = 0.1f,
          float farPlane_ = 10000.0f,
          Eigen::Vector3f pivot_ = Eigen::Vector3f( 0.0f, 0.0f, 0.0f ),
          float radius_ = 1000.0f, float yaw_ = 0.0f, float pitch_ = 0.0f );
#endif

      ~Camera(void);

      void LocalRotation( float yaw_, float pitch_ );
      void Anim( void );

      // GETTERS

      float Fov( void );

      Eigen::Vector3f Pivot( void );
      float Radius( void );

      float* ProjectionMatrix( void );
      float* ViewMatrix( void );
      float* Position( void );

#ifdef NEUROLOTS_WITH_ZEQ
      zeq::Subscriber* Subscriber( void );
#endif

      // SETTERS

      void Ratio( float ratio_ );
      void Pivot( Eigen::Vector3f pivot_ );
      void Radius( float radius_ );
      void Rotation( float yaw_, float pitch_ );

      void TargetPivot( Eigen::Vector3f targetPivot_ );
      void TargetRadius( float targetRadius_ );
      void TargetPivotRadius( Eigen::Vector3f targetPivot_,
          float targetRadius_ );

      void AnimDuration( float animDuration_ );

    private:

      void _PositionVectorized( std::vector<float>& positionVec_ );
      void _Rotation( Eigen::Matrix3f rotation_ );
      void _ViewMatrixVectorized( std::vector<float>& viewVec_ );

      void _BuildProjectionMatrix( void );
      void _BuildViewMatrix( void );

#ifdef NEUROLOTS_WITH_ZEQ
      void _OnCameraEvent( const zeq::Event& event_ );
      static void* _Subscriber( void* camera_ );
#endif

      Eigen::Matrix3f _RotationFromPY( float yaw_, float pitch_ );

      float _f;
      float _fov;
      float _ratio;
      float _nearPlane;
      float _farPlane;

      Eigen::Vector3f _pivot;
      float _radius;

      Eigen::Matrix3f _rotation;

      std::vector<float> _positionVec;
      std::vector<float> _projVec;
      std::vector<float> _viewVec;

      Eigen::Vector3f _targetPivot;
      float _targetRadius;

#ifdef NEUROLOTS_WITH_ZEQ
      bool _zeqConnection;

      servus::URI _uri;
      zeq::Publisher* _publisher;
      zeq::Subscriber* _subscriber;

      std::mutex _positionMutex;
      std::mutex _rotationMutex;
      std::mutex _viewMatrixMutex;

      pthread_t _subscriberThread;
#endif

      bool _isAniming;
      bool _firstStep;
      float _speedPivot;
      float _speedRadius;
      float _animDuration;

      std::clock_t _previusTime;
  };


} //end namespace neurolots

#endif // _NEUROLOTS_CAMERA__

// EOF
