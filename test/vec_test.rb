#!/usr/bin/env ruby
# coding: utf-8

##
# Siren::Vec test module
#

module VecTest

  include Siren

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

    mag_tol = 0.01        # tolerance for magnitude(size of Vector)
    ang_tol = 0.01.to_deg # tolerance for angle bitween two vectors

    assert Vec.x.equal?(Vec.new(1.0 - 1.0e-7, 0, 0), mag_tol, ang_tol)

    ## bug?
    # assert Vec.new(2, 0, 0).normal?(Vec.new(1, 0, 0), ang_tol)

    assert Vec.new(5, 0, 0).normal.to_a == [1, 0, 0]
    vec = Vec.new(5, 0, 0)
    vec.normal!
    assert vec.to_a == [1, 0, 0]

    ## bug?
    # assert Vec.new(10, 1, 10).parallel?(Vec.new(3, 3, 3), ang_tol)

    assert Vec.x.reverse?(-Vec.x, ang_tol)
    refute Vec.y.reverse?(-Vec.x, ang_tol)
    vec = Vec.x
    vec.reverse!
    assert vec.to_a == (-Vec.x).to_a

    assert Vec.x.angle(Vec.y) == Math::PI / 2
    assert Vec.x.angle(-Vec.y) == Math::PI / 2

    assert Vec.x.angleref(Vec.y, Vec.z) == Math::PI / 2
    ## bug?
    # p Vec.x.angleref(Vec.y, -Vec.z))

    assert Vec.new(1, 0, 0).magnitude == 1
    assert Vec.new(2, 0, 0).size == 2
    assert Vec.new(3, 0, 0).length == 3

    assert Vec.x.cross(Vec.y) == Vec.z
    assert Vec.x.cross(-Vec.y) == -Vec.z
    vec = Vec.x
    vec.cross!(Vec.y)
    assert vec == Vec.z

    assert Vec.x.dot(Vec.y) == 0
    assert Vec.x.dot(Vec.new(1, 1, 0)) > 0
    ## bug?
    # p Vec.x.dot(Vec.new(1, -1, 0))

    ## no checked
    # dot_cross
    # cross_cross
    # cross_cross!
    # cross_mag
    # cross_square_mag
    # square_mag

    assert Vec.x.mirror(Vec.y) == -Vec.x
    vec = Vec.x
    vec.mirror!(-Vec.y)
    assert vec == -Vec.x

    ## bug?
    # assert Vec.x.rotate(Vec.z, 90.0.to_rad) == Vec.y
    # vec = Vec.x
    # vec.rotate!(Vec.z, 90.0.to_rad)
    # assert vec == Vec.y

    assert Vec.x.scale(2).to_a == [2, 0, 0]
    vec = Vec.x
    vec.scale!(2)
    assert vec.to_a == [2, 0, 0]

    ## Trans class not supported yet.
    # transform
    # transform!

  end

end
