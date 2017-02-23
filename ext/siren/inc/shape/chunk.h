#ifndef _SHAPE_CHUNK_H_
#define _SHAPE_CHUNK_H_

#ifdef SIREN_ENABLE_CHUNK

#include "siren.h"

#include <TopoDS.hxx>
#include <TopoDS_CompSolid.hxx>

static rb_data_type_t siren_chunk_type = { "Chunk", siren_shape_final };
bool siren_chunk_install( struct RClass* sr_mSiren);
TopoDS_CompSolid siren_chunk_get( VALUE self);
VALUE siren_chunk_new( const TopoDS_Shape* src);
VALUE siren_chunk_obj();
struct RClass* siren_chunk_rclass();

VALUE siren_chunk_init( VALUE self);
VALUE siren_chunk_to_solid( VALUE self);

#endif

#endif
