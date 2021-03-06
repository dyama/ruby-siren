#ifndef _SHAPE_CHUNK_H_
#define _SHAPE_CHUNK_H_

#ifdef SR_ENABLE_CHUNK

#include "siren.h"

#include <TopoDS.hxx>
#include <TopoDS_CompSolid.hxx>

extern VALUE sr_cChunk;

bool siren_chunk_install();
TopoDS_CompSolid siren_chunk_get(VALUE self);
bool siren_chunk_p(const VALUE&);
void siren_chunk_check(const VALUE&);

VALUE siren_chunk_new( const TopoDS_Shape* src);

VALUE siren_chunk_init(int, VALUE*, VALUE);
VALUE siren_chunk_to_solid(int, VALUE*, VALUE);

#endif

#endif
