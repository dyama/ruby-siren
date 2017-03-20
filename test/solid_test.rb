#!/usr/bin/env ruby
# coding: utf-8

##
# Siren::Solid test module
#

require 'test_helper'

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
    # assert Solid.cylinder.class == Solid
  end

end
