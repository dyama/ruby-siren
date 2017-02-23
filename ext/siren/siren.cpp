#include "siren.h"

class Test { };

extern "C" {

VALUE rb_mSiren;

void
Init_siren(void)
{
  rb_mSiren = rb_define_module("Siren");
}

}
