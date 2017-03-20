#!/usr/bin/env ruby
# coding: utf-8

##
# Siren::Solid test module
#

dir = File.expand_path(File.dirname(__FILE__))
require "#{dir}/../test_helper"

module SolidTest

  include Siren

  def test_solid
    assert Solid.box.class == Solid
    assert Solid.box2p.class == Solid
    size = [10, 10, 10]
    pos  = Vec.zero
    dir  = [1, 0, 0]
    assert Solid.boxax(size, pos, dir).class == Solid
    assert Solid.sphere.class == Solid
    # bug
    # assert Solid.cylinder.class == Solid
    assert Solid.cylinder([0, 0, 0], [0, 0, 1], 0.5, 2, Math::PI * 2.0).class == Solid
    assert Solid.cone([0, 0, 0], [0, 0, 1], 2, 1, 3, Math::PI * 2.0).class == Solid
    assert Solid.torus([0, 0, 0], [0, 0, 1], 5, 1, Math::PI * 2.0).class == Solid

    # surf = plane(Vec.zero, Vec.z, Vec.y, -10, 10, -10, 10)
    # assert Solid.halfspace(surf, [0, 0, 1]).class == Solid

    # not implemented
    # assert Solid.prism.class == Solid
    # assert Solid.revol.class == Solid
    # assert Solid.revolution.class == Solid
    # assert Solid.wedge.class == Solid

    assert Solid.box.volume.round(2) == 1.0
  end

end
