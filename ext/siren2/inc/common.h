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

void siren_ary_to_xyz(VALUE ary, Standard_Real& x, Standard_Real& y, Standard_Real& z);

inline gp_Pnt siren_ary_to_pnt(VALUE val)
{
  Standard_Real x, y, z;
  siren_ary_to_xyz(val, x, y, z);
  return gp_Pnt(x, y, z);
}

inline gp_Vec siren_ary_to_vec(VALUE val)
{
  Standard_Real x, y, z;
  siren_ary_to_xyz(val, x, y, z);
  return gp_Vec(x, y, z);
}

inline gp_Dir siren_ary_to_dir(VALUE val)
{
  Standard_Real x, y, z;
  siren_ary_to_xyz(val, x, y, z);
  return gp_Dir(x, y, z);
}

inline gp_Ax1 siren_ary_to_ax1(VALUE pos, VALUE norm)
{
  return gp_Ax1(siren_ary_to_pnt(pos), siren_ary_to_dir(norm));
}

inline gp_Ax2 siren_ary_to_ax2(VALUE pos, VALUE norm, VALUE vdir)
{
  return gp_Ax2(siren_ary_to_pnt(pos), siren_ary_to_dir(norm), siren_ary_to_dir(vdir));
}

inline gp_Ax2 siren_ary_to_ax2(VALUE pos, VALUE norm)
{
  return gp_Ax2(siren_ary_to_pnt(pos), siren_ary_to_dir(norm));
}

inline gp_Ax3 siren_ary_to_ax3(VALUE pos, VALUE norm, VALUE vdir)
{
  return gp_Ax3(siren_ary_to_pnt(pos), siren_ary_to_dir(norm), siren_ary_to_dir(vdir));
}

inline gp_Ax3 siren_ary_to_ax3(VALUE pos, VALUE norm)
{
  return gp_Ax3(siren_ary_to_pnt(pos), siren_ary_to_dir(norm));
}

inline VALUE siren_pnt_to_ary(const gp_Pnt& pnt)
{
  VALUE res[3];
  res[0] = DBL2NUM(pnt.X());
  res[1] = DBL2NUM(pnt.Y());
  res[2] = DBL2NUM(pnt.Z());
  return rb_ary_new_from_values(3, res);
}

inline VALUE siren_dir_to_ary(const gp_Dir& dir)
{
  VALUE res[3];
  res[0] = DBL2NUM(dir.X());
  res[1] = DBL2NUM(dir.Y());
  res[2] = DBL2NUM(dir.Z());
  return rb_ary_new_from_values(3, res);
}

inline VALUE siren_vec_to_ary(const gp_Vec& vec)
{
  VALUE res[3];
  res[0] = DBL2NUM(vec.X());
  res[1] = DBL2NUM(vec.Y());
  res[2] = DBL2NUM(vec.Z());
  return rb_ary_new_from_values(3, res);
}

inline VALUE siren_pnt_new(double x, double y, double z)
{
  VALUE res[3];
  res[0] = DBL2NUM(x);
  res[1] = DBL2NUM(y);
  res[2] = DBL2NUM(z);
  return rb_ary_new_from_values(3, res);
}

bool siren_numeric_p(const VALUE&);
void siren_numeric_check(const VALUE&);

#endif
