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
    assert (save_brep box, @path) == nil
    mybox = load_brep @path
    assert mybox.class == Siren::Solid
    File.delete @path
  end

  def test_iges
    @path = "/tmp/siren-io-test.iges"
    assert (save_iges box, @path) == nil
    mybox = load_iges @path
    assert mybox.class == Siren::Solid
    File.delete @path
  end

  def test_step
    @path = "/tmp/siren-io-test.step"
    assert (save_step box, @path) == nil
    mybox = load_step @path
    assert mybox.class == Siren::Solid
    File.delete @path
  end

  def test_stl
    @path = "/tmp/siren-io-test.stl"
    assert (save_stl box, @path) == nil
    mybox = load_stl @path
    assert mybox.class == Siren::Shell
    File.delete @path
  end

  def test_ply
    @path = "/tmp/siren-io-test.ply"
    assert (Siren.save_ply box, @path) == nil
    File.delete @path
  end

end
