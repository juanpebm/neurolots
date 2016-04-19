/**
 * @file    NeuronsCollection.h
 * @brief
 * @author  Juan José García <juanjose.garcia@urjc.es>
 * @date
 * @remarks Copyright (c) 2015 GMRV/URJC. All rights reserved.
 * Do not distribute without further notice.
 */

#ifndef __NLRENDER_NEURONS_COLLECTION__
#define __NLRENDER_NEURONS_COLLECTION__


#include "Program.h"
#include "Camera.h"

#include <nsol/nsol.h>
#include <nsol/DataSet.h>

#include <Eigen/Dense>

#include <string>

#include <set>

#ifdef NEUROLOTS_USE_ZEQ

#include <zeq/zeq.h>
#include <zeq/hbp/hbp.h>
#include <servus/uri.h>

#include <pthread.h>
#include <mutex>
#include <boost/bind.hpp>

#endif

#include <nlrender/api.h>
#include "Neuron.h"
#include "../nlgeometry/Facet.h"

namespace nlrender
{

  typedef nsol::Columns* ColumnsPtr;

  class NeuronsCollection
  {

  public:

    typedef enum
    {
      HOMOGENEOUS = 0,
      LINEAR
    }TTessMethod;

    NLRENDER_API
    NeuronsCollection( Camera* camera_ );

    NLRENDER_API
    ~NeuronsCollection( void );

    NLRENDER_API
    void loadBlueConfig( const std::string& blueConfig_,
                         const std::string& target_ = std::string( "" ),

                         int loadFlags_ = nsol::MORPHOLOGY |
                         nsol::CORTICAL_HIERARCHY );

    NLRENDER_API
    void loadSwc( const std::string& swcFile_ );

    NLRENDER_API
    void loadScene( const std::string& xmlFile_ );

    NLRENDER_API
    void setZeqUri( const std::string& uri_ );

    NLRENDER_API
    void Paint( void );

    NLRENDER_API
    void PaintNeuron( const unsigned int& id_ );

    NLRENDER_API
    void PaintNeuron( const NeuronPtr& neuron );

    NLRENDER_API
    void focusOnNeuron( unsigned int id );

    NLRENDER_API
    void focusOnNeuron( NeuronPtr neuron_ );

    NLRENDER_API
    void focusAll( void );

    NLRENDER_API
    void extractMesh( NeuronPtr neuron_,
                      const std::string& outFileName = std::string("out.obj"));

    //Getters

    NLRENDER_API
    Eigen::Vector3f NeuronColor( void );

    NLRENDER_API
    Eigen::Vector3f SelectedNeuronColor( void );

    NLRENDER_API
    ColumnsPtr Columns( void );

    NLRENDER_API
    std::vector< unsigned int > neuronIDs( void );

    NLRENDER_API
    NeuronPtr neuronById( unsigned int id );

#ifdef NEUROLOTS_USE_ZEQ

    NLRENDER_API
    zeq::Subscriber* Subscriber( void );

#endif

    NLRENDER_API
    bool SelectionChange( void );

    //Setters

    NLRENDER_API
    void lod( float lod_ );

    NLRENDER_API
    void tng( float tng_ );

    NLRENDER_API
    void maxDist( float maxDist_ );

    NLRENDER_API
    void NeuronColor( const Eigen::Vector3f& neuronColor_ );

    NLRENDER_API
    void SelectedNeuronColor( const Eigen::Vector3f& selectedNeuronColor_ );

    NLRENDER_API
    void PaintSoma( bool paintSoma_ );

    NLRENDER_API
    void PaintNeurites( bool paintNeurites_ );

    NLRENDER_API
    void PaintSelectedSoma( bool paintSelectedSoma_ );

    NLRENDER_API
    void PaintSelectedNeurites( bool paintSelectedNeurites_ );

    NLRENDER_API
    void tessMethod( const TTessMethod& tessMethod_ )
    {
      _tessMethod = ( unsigned int ) tessMethod_;
    }

  private:

    void _Init( void );

    bool _IsSelected( nsol::NeuronPtr neuron_ );

    void _DefaultCamera( void );

    void _VectorToMesh( const std::vector< float >& vecVertices_,
                        const std::vector< float >& vecNormals_,
                        nlgeometry::Vertices& vertices_,
                        nlgeometry::Facets& facets_ ) const;

#ifdef NEUROLOTS_USE_ZEQ

    void _OnFocusEvent( const zeq::Event& event_ );
    void _OnSelectionEvent( const zeq::Event& event_ );
    void _OnSelectionFocusEvent( const zeq::Event& event_ );
    static void* _Subscriber( void* collection_ );

#endif

    Program* _programTriangles;
    Program* _programQuads;

    Program* _programTrianglesFB;
    Program* _programQuadsFB;

    unsigned int _tfo;
    std::vector< unsigned int > _tbos;

    Camera* _camera;

    float _lod;
    float _tng;
    float _maxDist;

    std::vector< float > _neuronColor;
    std::vector< float > _selectedNeuronColor;

    bool _paintSoma;
    bool _paintNeurites;
    bool _paintSelectedSoma;
    bool _paintSelectedNeurites;

    nsol::DataSet _dataSet;

    std::set<unsigned int> _selectedNeurons;

    bool _selectionChange;

#ifdef NEUROLOTS_USE_ZEQ
    bool _zeqConnection;

    servus::URI _uri;
    zeq::Subscriber* _subscriber;

    pthread_t _subscriberThread;
#endif

    Eigen::Vector3f _defaultPivot;
    float _defaultRadius;

    unsigned int _tessMethod;
  };

} // end namespace nlrender

#endif // __NLRENDER_NEURONS_COLLECTION__

//EOF
