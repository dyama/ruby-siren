#!/usr/bin/env ruby
# coding: utf-8

##
# Siren::Trans test module
#

require 'test_helper'

module TransTest

  include Siren

  def test_trans

    # p Trans
    # p Trans.new

    assert Trans.new.to_a == [[1,0,0],[0,1,0],[0,0,1],[0,0,0]]
    assert Trans.new.matrix == [[1,0,0],[0,1,0],[0,0,1],[0,0,0]]
    assert Trans.new.mirror(Vec.zero).to_a == [[-1,0,0],[0,-1,0],[0,0,-1],[0,0,0]]
    t = Trans.new
    t.mirror!(Vec.zero)
    assert t.to_a ==  [[-1,0,0],[0,-1,0],[0,0,-1],[0,0,0]]

  end

end
