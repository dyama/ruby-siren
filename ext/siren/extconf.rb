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
$srcs << Dir.glob("#{dir}/src/*.{c,cpp}")
$srcs << Dir.glob("#{dir}/src/*/*.{c,cpp}")
$srcs.flatten!

$objs = []
$objs << Dir.glob("#{dir}/src/*.{c,cpp}").map{|n| "#{n.gsub(/\..*$/, '')}.o" }
$objs << Dir.glob("#{dir}/src/*/*.{c,cpp}").map{|n| "#{n.gsub(/\..*$/, '')}.o" }
$objs.flatten!

create_makefile("siren/siren")

