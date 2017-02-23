#ifndef _COMMON_H_
#define _COMMON_H_

#include "ruby.h"
#include "shape.h"

#include <gp_Vec.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>
#include <gp_Ax3.hxx>

void siren_ary_to_xyz( VALUE ary, Standard_Real& x, Standard_Real& y, Standard_Real& z);

inline gp_Pnt siren_ary_to_pnt( VALUE val)
{
  Standard_Real x, y, z;
  siren_ary_to_xyz(mrb, val, x, y, z);
  return gp_Pnt(x, y, z);
}

inline gp_Vec siren_ary_to_vec( VALUE val)
{
  Standard_Real x, y, z;
  siren_ary_to_xyz(mrb, val, x, y, z);
  return gp_Vec(x, y, z);
}

inline gp_Dir siren_ary_to_dir( VALUE val)
{
  Standard_Real x, y, z;
  siren_ary_to_xyz(mrb, val, x, y, z);
  return gp_Dir(x, y, z);
}

inline gp_Ax1 siren_ary_to_ax1( VALUE pos, VALUE norm)
{
  return gp_Ax1(siren_ary_to_pnt(mrb, pos), siren_ary_to_dir(mrb, norm));
}

inline gp_Ax2 siren_ary_to_ax2( VALUE pos, VALUE norm, VALUE vdir)
{
  return gp_Ax2(siren_ary_to_pnt(mrb, pos), siren_ary_to_dir(mrb, norm), siren_ary_to_dir(mrb, vdir));
}

inline gp_Ax2 siren_ary_to_ax2( VALUE pos, VALUE norm)
{
  return gp_Ax2(siren_ary_to_pnt(mrb, pos), siren_ary_to_dir(mrb, norm));
}

inline gp_Ax3 siren_ary_to_ax3( VALUE pos, VALUE norm, VALUE vdir)
{
  return gp_Ax3(siren_ary_to_pnt(mrb, pos), siren_ary_to_dir(mrb, norm), siren_ary_to_dir(mrb, vdir));
}

inline gp_Ax3 siren_ary_to_ax3( VALUE pos, VALUE norm)
{
  return gp_Ax3(siren_ary_to_pnt(mrb, pos), siren_ary_to_dir(mrb, norm));
}

inline VALUE siren_pnt_to_ary( const gp_Pnt& pnt)
{
  VALUE res[3];
  res[0] = mrb_float_value(mrb, pnt.X());
  res[1] = mrb_float_value(mrb, pnt.Y());
  res[2] = mrb_float_value(mrb, pnt.Z());
  return mrb_ary_new_from_values(mrb, 3, res);
}

inline VALUE siren_dir_to_ary( const gp_Dir& dir)
{
  VALUE res[3];
  res[0] = mrb_float_value(mrb, dir.X());
  res[1] = mrb_float_value(mrb, dir.Y());
  res[2] = mrb_float_value(mrb, dir.Z());
  return mrb_ary_new_from_values(mrb, 3, res);
}

inline VALUE siren_vec_to_ary( const gp_Vec& vec)
{
  VALUE res[3];
  res[0] = mrb_float_value(mrb, vec.X());
  res[1] = mrb_float_value(mrb, vec.Y());
  res[2] = mrb_float_value(mrb, vec.Z());
  return mrb_ary_new_from_values(mrb, 3, res);
}

inline VALUE siren_pnt_new( double x, double y, double z)
{
  VALUE res[3];
  res[0] = mrb_float_value(mrb, x);
  res[1] = mrb_float_value(mrb, y);
  res[2] = mrb_float_value(mrb, z);
  return mrb_ary_new_from_values(mrb, 3, res);
}

VALUE mrb_instance_alloc( VALUE cv);

#endif
