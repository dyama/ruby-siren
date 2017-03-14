require 'test_helper'

class SirenTest < Minitest::Test

  include Siren

  def test_that_it_has_a_version_number
    refute_nil ::Siren::VERSION
  end

  def test_vec

    # initialize method
    assert Vec.new.to_a == [0.0, 0.0, 0.0]
    assert Vec.new(1.0, 2.to_i, 3.to_f).to_a ==  Vec.new([1.0, 2.to_i, 3.to_f]).to_a
    assert Vec.new([1.0, 2, 3.to_f]).to_a == [1.0, 2.0, 3.0]

    # class methods
    assert Vec.zero.to_a == [0, 0, 0]
    assert Vec.x.to_a    == [1, 0, 0]
    assert Vec.y.to_a    == [0, 1, 0]
    assert Vec.z.to_a    == [0, 0, 1]
    assert Vec.xdir.to_a == [1, 0, 0]
    assert Vec.ydir.to_a == [0, 1, 0]
    assert Vec.zdir.to_a == [0, 0, 1]
    assert Vec.xy.to_a   == [1, 1, 0]
    assert Vec.xz.to_a   == [1, 0, 1]
    assert Vec.yx.to_a   == [1, 1, 0]
    assert Vec.yz.to_a   == [0, 1, 1]
    assert Vec.zx.to_a   == [1, 0, 1]
    assert Vec.zy.to_a   == [0, 1, 1]
    assert Vec.xyz.to_a  == [1, 1, 1]

    # instance methods
    assert (Vec.new(1, 2, 3) == Vec.new(1, 2, 3)) == true
    assert (Vec.new(1, 2, 3) != Vec.new(1, 2, 3)) == false
    assert (-Vec.new(1, 2, 3)).to_a == [-1, -2, -3]
    assert (Vec.new(1, 0, 0) + Vec.new(1, 0, 0)).to_a == [2, 0, 0]
    assert (Vec.new(1, 0, 0) - Vec.new(1, 0, 0)).to_a == [0, 0, 0]
    assert (Vec.new(1, 0, 0) * 2).to_a == [2, 0, 0]
    assert (Vec.new(1, 0, 0) / 2).to_a == [0.5, 0, 0]

    assert Vec.new.x == 0
    assert Vec.new.y == 0
    assert Vec.new.z == 0
    assert (Vec.new.x = 1) == 1
    assert (Vec.new.y = 2) == 2
    assert (Vec.new.z = 3) == 3

    assert Vec.new(1, 2, 3).to_ary == [1, 2, 3]

    # tolerance for magnitude(size of Vector)
    mag_tol = 0.01
    # tolerance for angle bitween two vectors
    ang_tol = 0.01.to_deg

    assert Vec.new(1, 0, 0).equal?(Vec.new(1.0 - 1.0e-7, 0, 0), mag_tol, ang_tol)
    # bug?
    # assert Vec.new(2, 0, 0).normal?(Vec.new(1, 0, 0), ang_tol)
    assert Vec.new(5, 0, 0).normal.to_a == [1, 0, 0]

    # ...
    v1 = Vec.new
    v1.x = 25.to_i
    v1.y = 30.to_f
    assert v1.to_a == [25.0, 30.0, 0.0]

  end

  # def test_bndbox
  #   p BndBox.new
  # end

end
