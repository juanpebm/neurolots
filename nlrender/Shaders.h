// File generated by reto_generate_shaders.py. Do not edit.
#ifndef NLRENDER_SHADERS_H
#define NLRENDER_SHADERS_H

namespace nlrender
{
  const char* const quad_frag = "#version 400\n"
  "out vec4 oColor;\n"
  "\n"
  "in vec3 normal;\n"
  "in vec3 L;\n"
  "\n"
  "uniform vec3 color;\n"
  "\n"
  "vec4 shading( )\n"
  "{\n"
  "  vec3 N=normalize( normal );\n"
  "  float dif = dot( N, L );\n"
  "  dif = clamp( dif, 0.0, 1.0 );\n"
  "  return vec4( color * dif, 1.0 ) + vec4( color * 0.3, 1.0 );\n"
  "}\n\n"
  "\n"
  "void main( )\n"
  "{\n"
  "  oColor = shading( );\n"
  "}";

  const char* const triangle_vert = "#version 400\n"
  "subroutine float levelDistType( vec3 position );\n"
  "subroutine uniform levelDistType levelDist;\n"
  "\n"
  "in vec3 inVertex;\n"
  "in vec3 inCenter;\n"
  "in vec3 inTangent;\n"
  "\n"
  "out vec3 vPosition;\n"
  "out vec3 vCenter;\n"
  "out float vlot;\n"
  "\n"
  "uniform mat4 model;\n"
  "uniform vec3 cameraPos;\n"
  "uniform float lod;\n"
  "uniform float maxDist;\n"
  "\n"
  "subroutine( levelDistType )\n"
  "float homogeneous( vec3 postion )\n"
  "{\n"
  "  return lod;\n"
  "}\n\n"
  "subroutine( levelDistType )\n"
  "float linear( vec3 position )\n"
  "{\n"
  "  return ( lod - 1.0 ) * clamp( ( 1.0 - length( position\n"
  "    - cameraPos ) / maxDist ), 0.0, 1.0 );\n"
  "}\n\n"
  "\n"
  "void main( void )\n"
  "{\n"
  "  vPosition = ( model * vec4(inVertex, 1.0 )).xyz;\n"
  "  vCenter = ( model * vec4( inCenter, 1.0 )).xyz;\n"
  "  vlot = levelDist( vCenter ) * length( inTangent ) + 1.0;\n"
  "}";

  const char* const quad_tes = "#version 400\n"
  "layout(quads) in;\n"
  "\n"
  "in vec3 tcCenter[];\n"
  "in float r[];\n"
  "in vec3 tcNormal[];\n"
  "in vec3 m[];\n"
  "\n"
  "out vec3 position;\n"
  "out vec3 normal;\n"
  "out vec3 L;\n"
  "\n"
  "uniform mat4 proy;\n"
  "uniform mat4 view;\n"
  "uniform mat4 model;\n"
  "uniform vec3 cameraPos;\n"
  "\n"
  "void main( )\n"
  "{\n"
  "  vec3 tePosition;\n"
  "  vec3 teCenter;\n"
  "  vec3 teNormal;\n"
  "  float u=gl_TessCoord.x;\n"
  "  float u_2=u*u;\n"
  "  float u_3=u_2*u;\n"
  "  float v=gl_TessCoord.y;\n"
  "  float v_2=v*v;\n"
  "  float v_3=v_2*v;\n"
  "  teCenter = (2*v_3-3*v_2+1)*tcCenter[0]+(v_3-2*v_2+v)*m[0]+\n"
  "    (v_3-v_2)*m[2]+(-2*v_3+3*v_2)*tcCenter[2];\n"
  "  vec3 a_t=mix(tcNormal[0],tcNormal[1],u);\n"
  "  vec3 b_t=mix(tcNormal[2],tcNormal[3],u);\n"
  "  teNormal = normalize(mix(a_t,b_t,v));\n"
  "  float radius=mix(r[0],r[2],v);\n"
  "  tePosition = (radius*teNormal+teCenter);\n"
  "  normal = teNormal;\n"
  "  L = normalize(cameraPos-tePosition);\n"
  "  position = tePosition;\n"
  "  gl_Position = proy*view*vec4(tePosition,1.0);\n"
  "}";

  const char* const triangle_geom = "#version 400\n"
  "layout( triangles ) in;\n"
  "layout( triangle_strip, max_vertices = 3 ) out;\n"
  "\n"
  "in vec3 position[3];\n"
  "in vec3 normal[3];\n"
  "in vec3 L[3];\n"
  "\n"
  "out vec3 gnormal;\n"
  "out vec3 gL;\n"
  "out vec3 outValue0;\n"
  "out vec3 outValue1;\n"
  "\n"
  "void createGeom( )\n"
  "{\n"
  "  outValue0 = position[0];\n"
  "  outValue1 = normal[0];\n"
  "  gnormal = normal[0];\n"
  "  gL = L[0];\n"
  "  gl_Position = gl_in[0].gl_Position;\n"
  "  EmitVertex( );\n"
  "\n"
  "  outValue0 = position[1];\n"
  "  outValue1 = normal[1];\n"
  "  gnormal = normal[1];\n"
  "  gL = L[1];\n"
  "  gl_Position = gl_in[1].gl_Position;\n"
  "  EmitVertex( );\n"
  "  \n"
  "  outValue0 = position[2];\n"
  "  outValue1 = normal[2];\n"
  "  gnormal = normal[2];\n"
  "  gL = L[2];\n"
  "  gl_Position = gl_in[2].gl_Position;\n"
  "  EmitVertex( );\n"
  "  EndPrimitive( );\n"
  "}\n\n"
  "\n"
  "void main( void )\n"
  "{\n"
  "  createGeom( );\n"
  "}";

  const char* const triangle_frag = "#version 400\n"
  "out vec4 oColor;\n"
  "\n"
  "in vec3 normal;\n"
  "in vec3 L;\n"
  "\n"
  "uniform vec3 color;\n"
  "\n"
  "vec4 shading( )\n"
  "{\n"
  "  vec3 N=normalize( normal );\n"
  "  float dif = dot( N, L );\n"
  "  dif = clamp( dif, 0.0, 1.0 );\n"
  "  return vec4( color * dif, 1.0 ) + vec4( color * 0.3, 1.0 );\n"
  "}\n\n"
  "\n"
  "void main( )\n"
  "{\n"
  "  oColor = shading( );\n"
  "}";

  const char* const quad_vert = "#version 400\n"
  "subroutine float levelDistType( vec3 position );\n"
  "subroutine uniform levelDistType levelDist;\n"
  "\n"
  "in vec3 inVertex;\n"
  "in vec3 inCenter;\n"
  "in vec3 inTangent;\n"
  "\n"
  "out vec3 vPosition;\n"
  "out vec3 vCenter;\n"
  "out vec3 vTangent;\n"
  "out float vlot;\n"
  "\n"
  "uniform mat4 model;\n"
  "uniform vec3 cameraPos;\n"
  "uniform float lod;\n"
  "uniform float maxDist;\n"
  "\n"
  "subroutine( levelDistType )\n"
  "float homogeneous( vec3 postion )\n"
  "{\n"
  "  return lod;\n"
  "}\n\n"
  "subroutine( levelDistType )\n"
  "float linear( vec3 position )\n"
  "{\n"
  "  return ( lod - 1.0 ) * clamp( ( 1.0 - length( position\n"
  "    - cameraPos ) / maxDist ), 0.0, 1.0 );\n"
  "}\n\n"
  "\n"
  "void main( void )\n"
  "{\n"
  "  vPosition = ( model * vec4(inVertex, 1.0 )).xyz;\n"
  "  vCenter = ( model * vec4( inCenter, 1.0 )).xyz;\n"
  "  vTangent = ( model * vec4( inTangent, 0.0 )).xyz;\n"
  "  vlot = levelDist( vCenter ) + 1.0;\n"
  "}";

  const char* const quad_geom = "#version 400\n"
  "layout( triangles ) in;\n"
  "layout( triangle_strip, max_vertices = 3 ) out;\n"
  "\n"
  "in vec3 position[3];\n"
  "in vec3 normal[3];\n"
  "in vec3 L[3];\n"
  "\n"
  "out vec3 gnormal;\n"
  "out vec3 gL;\n"
  "out vec3 outValue0;\n"
  "out vec3 outValue1;\n"
  "\n"
  "void createGeom( )\n"
  "{\n"
  "  outValue0 = position[0];\n"
  "  outValue1 = normal[0];\n"
  "  gnormal = normal[0];\n"
  "  gL = L[0];\n"
  "  gl_Position = gl_in[0].gl_Position;\n"
  "  EmitVertex( );\n"
  "\n"
  "  outValue0 = position[1];\n"
  "  outValue1 = normal[1];\n"
  "  gnormal = normal[1];\n"
  "  gL = L[1];\n"
  "  gl_Position = gl_in[1].gl_Position;\n"
  "  EmitVertex( );\n"
  "  \n"
  "  outValue0 = position[2];\n"
  "  outValue1 = normal[2];\n"
  "  gnormal = normal[2];\n"
  "  gL = L[2];\n"
  "  gl_Position = gl_in[2].gl_Position;\n"
  "  EmitVertex( );\n"
  "  EndPrimitive( );\n"
  "}\n\n"
  "\n"
  "void main( void )\n"
  "{\n"
  "  createGeom( );\n"
  "}";

  const char* const triangle_tes = "#version 400\n"
  "layout(triangles) in;\n"
  "\n"
  "in vec3 tcCenter[];\n"
  "in vec3 tcNormal[];\n"
  "in float tcRadius[];\n"
  "\n"
  "out vec3 position;\n"
  "out vec3 normal;\n"
  "out vec3 L;\n"
  "\n"
  "uniform mat4 proy;\n"
  "uniform mat4 view;\n"
  "uniform vec3 cameraPos;\n"
  "\n"
  "void main( )\n"
  "{\n"
  "  vec3 tePosition;\n"
  "  vec3 teCenter;\n"
  "  float teRadius;\n"
  "  float u=gl_TessCoord.x;\n"
  "  float v=gl_TessCoord.y;\n"
  "  float w=gl_TessCoord.z;\n"
  "  normal = normalize(tcNormal[0]*u+tcNormal[1]*v+tcNormal[2]*w);\n"
  "  teCenter = tcCenter[0]*u+tcCenter[1]*v+tcCenter[2]*w;\n"
  "  teRadius = tcRadius[0]*u+tcRadius[1]*v+tcRadius[2]*w;\n"
  "  tePosition = teCenter+normal*teRadius;\n"
  "  L = normalize(cameraPos-tePosition);\n"
  "  position = tePosition;\n"
  "  gl_Position = proy*view*vec4(tePosition,1);\n"
  "}";

  const char* const quad_tcs = "#version 400\n"
  "layout(vertices=4) out;\n"
  "\n"
  "in vec3 vPosition[];\n"
  "in vec3 vCenter[];\n"
  "in vec3 vTangent[];\n"
  "in float vlot[];\n"
  "\n"
  "out vec3 tcCenter[];\n"
  "out float r[];\n"
  "out vec3 tcNormal[];\n"
  "out vec3 m[];\n"
  "\n"
  "uniform float tng;\n"
  "\n"
  "#define ID gl_InvocationID\n"
  "\n"
  "void main( )\n"
  "{\n"
  "  tcCenter[ID] = vCenter[ID];\n"
  "  tcNormal[ID] = normalize( vPosition[ID]-vCenter[ID] );\n"
  "  r[ID]=distance(vPosition[ID],vCenter[ID]);\n"
  "  m[ID]=( tng + 1.0 )* normalize(vTangent[ID]) *\n"
  "    distance(vCenter[0],vCenter[2]) / 8;\n"
  "  float tcLod = (vlot[0]+vlot[1]+vlot[2]+vlot[3])/4;\n"
  "  gl_TessLevelInner[0] = tcLod;\n"
  "  gl_TessLevelInner[1] = tcLod;\n"
  "  gl_TessLevelOuter[0] = (vlot[0]+vlot[2])/2;\n"
  "  gl_TessLevelOuter[1] = (vlot[0]+vlot[1])/2;\n"
  "  gl_TessLevelOuter[2] = (vlot[1]+vlot[3])/2;\n"
  "  gl_TessLevelOuter[3] = (vlot[2]+vlot[3])/2;\n"
  "}";

  const char* const triangle_tcs = "#version 400\n"
  "layout(vertices = 3) out;\n"
  "\n"
  "in vec3 vPosition[];\n"
  "in vec3 vCenter[];\n"
  "in float vlot[];\n"
  "\n"
  "out vec3 tcCenter[];\n"
  "out vec3 tcNormal[];\n"
  "out float tcRadius[];\n"
  "\n"
  "#define ID gl_InvocationID\n"
  "\n"
  "void main( void )\n"
  "{\n"
  "  tcCenter[ID] = vCenter[ID];\n"
  "  tcNormal[ID] = normalize( vPosition[ID]-vCenter[ID] );\n"
  "  tcRadius[ID] = distance( vPosition[ID], vCenter[ID] );\n"
  "  float tcLod = (vlot[0]+vlot[1]+vlot[2])/3;\n"
  "  gl_TessLevelInner[0] = tcLod;\n"
  "  gl_TessLevelOuter[0] = (vlot[1]+vlot[2])/2;\n"
  "  gl_TessLevelOuter[1] = (vlot[2]+vlot[0])/2;\n"
  "  gl_TessLevelOuter[2] = (vlot[0]+vlot[1])/2;\n"
  "}";
};

#endif /* NLRENDER_SHADERS_H */
