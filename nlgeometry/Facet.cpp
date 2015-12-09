/**
 * @file    Facet.cpp
 * @brief
 * @author  Juan José García <juanjose.garcia@urjc.es>
 * @date    2015
 * @remarks Copyright (c) 2015 GMRV/URJC. All rights reserved.
 * Do not distribute without further notice.
 */

#include "Facet.h"

namespace nlgeometry
{

  Facet::Facet(  const VertexPtr& v0_, const VertexPtr& v1_,
                 const VertexPtr& v2_ )
    : _v0( v0_ )
    , _v1( v1_ )
    , _v2( v2_ )
  {

  }

  Facet::~Facet( void )
  {

  }

  VertexPtr& Facet::v0( void )
  {
    return _v0;
  }

  VertexPtr& Facet::v1( void )
  {
    return _v1;
  }

  VertexPtr& Facet::v2( void )
  {
    return _v2;
  }

  VertexPtr Facet::v0( void ) const
  {
    return _v0;
  }

  VertexPtr Facet::v1( void ) const
  {
    return _v1;
  }

  VertexPtr Facet::v2( void ) const
  {
    return _v2;
  }


} // end namespace neurolots
