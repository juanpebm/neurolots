/**
 * @file    Quad.h
 * @brief
 * @author  Juan José García <juanjose.garcia@urjc.es>
 * @date
 * @remarks Copyright (c) 2015 GMRV/URJC. All rights reserved.
 * Do not distribute without further notice.
 */

#ifndef __NLGENERATOR_QUAD__
#define __NLGENERATOR_QUAD__

#include "../nlphysics/Node.h"
#include <cfloat>

#include <nlgenerator/api.h>

namespace nlgenerator
{
  class Quad
  {
    public:

      NLGENERATOR_API
      Quad( nlphysics::NodePtr node0_, nlphysics::NodePtr node1_,
            nlphysics::NodePtr node2_, nlphysics::NodePtr node3_ );

      NLGENERATOR_API
      ~Quad( void );

      //Getters
      NLGENERATOR_API
      nlphysics::NodePtr Node0( void );

      NLGENERATOR_API
      nlphysics::NodePtr Node1( void );

      NLGENERATOR_API
      nlphysics::NodePtr Node2( void );

      NLGENERATOR_API
      nlphysics::NodePtr Node3( void );

      NLGENERATOR_API
      Eigen::Vector3f Center( void );

      NLGENERATOR_API
      bool Free( void );

      //Setters
      NLGENERATOR_API
      void Free( bool free_ );

    private:

      nlphysics::NodePtr _node0;
      nlphysics::NodePtr _node1;
      nlphysics::NodePtr _node2;
      nlphysics::NodePtr _node3;

      Eigen::Vector3f _center;

      bool _free;



  };

  typedef Quad* QuadPtr;

  class Quads
  {
    public:
      NLGENERATOR_API
      Quads( void );

      NLGENERATOR_API
      ~Quads( void );

      NLGENERATOR_API
      static void FacesToVectors( std::vector< QuadPtr >& quads,
                                  std::vector< nlphysics::NodePtr >& nodes,
                                  std::vector< float >& vertices,
                                  std::vector< float >& centers,
                                  std::vector< float >& tangents,
                                  std::vector< unsigned int >& mesh );

      NLGENERATOR_API
      static QuadPtr NearerQuad( std::vector< QuadPtr >& quads,
                                 Eigen::Vector3f position );

  };

} // end namespace nlgenerator

#endif // __NLGENERATOR_QUAD__

// EOF
