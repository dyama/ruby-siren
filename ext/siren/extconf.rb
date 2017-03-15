require "mkmf"
require 'pathname'

dir = File.expand_path(File.dirname(__FILE__))
siren_incdir   = "#{dir}/inc"
siren_incpaths = []
siren_incpaths << siren_incdir
siren_incpaths << Dir.glob("#{siren_incdir}/*/").map.to_a

# Open CASCADE Technology configuration
# Check http://dev.opencascade.org/doc/refman/html/index.html
thirdparty_libs = []
occt_libpaths   = []
occt_incpaths   = []

if occt_libpaths.size == 0 and occt_incpaths.size == 0
  os = RbConfig::CONFIG['host_os'].downcase
  case os
  when /linux|solaris|bsd/
    occt_libpaths = [ '/opt/occ/710/lin64/gcc/lib' ]
    occt_incpaths = [ '/opt/occ/710/inc' ]
  when /darwin|mac os/
    occt_libpaths = [ '/usr/local/opt/opencascade/lib' ]
    occt_incpaths = [ '/usr/local/opt/opencascade/include/opencascade' ]
  when /mswin|mingw/
    occt_libpaths = [ '\occ\700\mingw32\gcc\lib' ]
    occt_incpaths = [ '\occ\700\inc' ]
    spec.cxx.flags << '-D_USE_MATH_DEFINES'
    spec.cxx.flags << '-D__NO_INLINE__'
  end
end

occt_libs = [
  # Foundation classes
  'TKernel', 'TKMath',
  # Modeling data
  'TKG2d', 'TKG3d', 'TKGeomBase', 'TKBRep',
  # Modeling algorithms
  'TKGeomAlgo', 'TKTopAlgo', 'TKBO', 'TKPrim',
  'TKShHealing', 'TKHLR', 'TKMesh', 'TKBool',
  'TKXMesh', 'TKFeat', 'TKFillet', 'TKOffset',
  # Data excange
  'TKXSBase', 'TKSTL', 'TKIGES', 'TKSTEP',
  'TKSTEP209', 'TKSTEPAttr', 'TKSTEPBase',
]

# for Compiler
$CFLAGS   += " -Wno-unused-function -Wno-unused-variable -Wno-unknown-pragmas"
$CPPFLAGS += " " + [occt_incpaths, siren_incpaths].flatten.map{|d| " -I#{d}"}.join
$CXXFLAGS += " -std=gnu++11"

# for Linker
$LDFLAGS  = [occt_libpaths].flatten.map{|d| " -L#{d}"}.join
$LDFLAGS += [occt_libs, thirdparty_libs].flatten.map{|d| " -l#{d}"}.join

$srcs = []
$srcs << Dir.glob("#{dir}/src/*.{c,cpp}").map{|f| File.expand_path(f) }
$srcs << Dir.glob("#{dir}/src/*/*.{c,cpp}").map{|f| File.expand_path(f) }
$srcs.flatten!

$objs = []
if false
  $objs << Dir.glob("#{dir}/src/*.{c,cpp}").map{|n| "#{n.gsub(/\..*$/, '')}.o" }.map{|f| File.expand_path(f) }
  $objs << Dir.glob("#{dir}/src/*/*.{c,cpp}").map{|n| "#{n.gsub(/\..*$/, '')}.o" }.map{|f| File.expand_path(f) }
else
  #$objs << %W(
  #  #{dir}/src/io/step.o
  #  #{dir}/src/io/stl.o
  #  #{dir}/src/io/iges.o
  #  #{dir}/src/curve/offsetcurve.o
  #  #{dir}/src/curve/circle.o
  #  #{dir}/src/curve/line.o
  #  #{dir}/src/curve/ellipse.o
  #  #{dir}/src/curve/bzcurve.o
  #  #{dir}/src/curve/bscurve.o
  #  #{dir}/src/curve/parabola.o
  #  #{dir}/src/curve/hyperbola.o
  #  #{dir}/src/heal.o
  #  #{dir}/src/filler.o
  #  #{dir}/src/offset.o
  #  #{dir}/src/bo.o
  #)
  $objs << %W(
    #{dir}/src/vec.o
    #{dir}/src/common.o
    #{dir}/src/trans.o
    #{dir}/src/bndbox.o
    #{dir}/src/topalgo.o
    #{dir}/src/brep.o
    #{dir}/src/shape/wire.o
    #{dir}/src/shape/face.o
    #{dir}/src/shape/shell.o
    #{dir}/src/shape/chunk.o
    #{dir}/src/shape/edge.o
    #{dir}/src/shape/solid.o
    #{dir}/src/shape/vertex.o
    #{dir}/src/shape/compound.o
    #{dir}/src/shape.o
    #{dir}/src/siren.o
  )
end
$objs.flatten!

create_makefile("siren/siren")

