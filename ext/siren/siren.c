#include "siren.h"

VALUE rb_mSiren;

void
Init_siren(void)
{
  rb_mSiren = rb_define_module("Siren");
}
