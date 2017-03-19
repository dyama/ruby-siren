#!/usr/bin/env ruby
# coding: utf-8

##
# I/O methods test module
#

require 'test_helper'

module IoTest

  include Siren

  def test_brep
    @path = "/tmp/siren-io-test.brep"
    save_brep box, @path
    mybox = load_brep @path
    assert mybox.class == Siren::Solid
  end

  def test_iges
    @path = "/tmp/siren-io-test.iges"
    save_iges box, @path
    mybox = load_iges @path
    assert mybox.class == Siren::Solid
  end

  def test_step
    @path = "/tmp/siren-io-test.step"
    save_step box, @path
    mybox = load_step @path
    assert mybox.class == Siren::Solid
  end

  def test_stl
    @path = "/tmp/siren-io-test.stl"
    save_stl box, @path
    mybox = load_stl @path
    assert mybox.class == Siren::Compound
  end

  def test_ply

  end

end
