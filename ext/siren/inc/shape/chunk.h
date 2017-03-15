#ifndef _SHAPE_CHUNK_H_
#define _SHAPE_CHUNK_H_

#ifdef SIREN_ENABLE_CHUNK

#include "siren.h"

#include <TopoDS.hxx>
#include <TopoDS_CompSolid.hxx>

extern VALUE sr_cChunk;

static rb_data_type_t siren_chunk_type = { "Chunk", siren_shape_final };
bool siren_chunk_install();
TopoDS_CompSolid siren_chunk_get(VALUE self);
VALUE siren_chunk_new( const TopoDS_Shape* src);

VALUE siren_chunk_init(int, VALUE*, VALUE);
VALUE siren_chunk_to_solid(int, VALUE*, VALUE);

#endif

#endif
