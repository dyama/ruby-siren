require 'test_helper'
require 'vec_test'
require 'bndbox_test'
require 'solid_test'
require 'trans_test'
require 'io_test'

class SirenTest < Minitest::Test

  include Siren

  def test_that_it_has_a_version_number
    refute_nil ::Siren::VERSION
  end

  include VecTest
  include BndBoxTest
  # include TransTest
  # include IoTest
  include SolidTest

  def test_classes
    assert Vec.class == Class.class
    assert Trans.class == Class.class
    assert BndBox.class == Class.class
    assert Shape.class == Class.class
    assert Vertex.class == Class.class
    assert Edge.class == Class.class
    assert Wire.class == Class.class
    assert Face.class == Class.class
    assert Shell.class == Class.class
    assert Solid.class == Class.class
    assert Compound.class == Class.class
    assert Curve.class == Class.class
    assert Line.class == Class.class
    assert Circle.class == Class.class
    assert Ellipse.class == Class.class
    assert Hyperbola.class == Class.class
    assert Parabola.class == Class.class
    assert BzCurve.class == Class.class
    assert BSCurve.class == Class.class
    assert OffsetCurve.class == Class.class
    assert Filler.class == Class.class
  end

end
