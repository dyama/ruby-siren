#!/usr/bin/env ruby
# coding: utf-8

##
# I/O methods test module
#

require 'test_helper'

module IoTest

  include Siren

  def test_brep
    @path = "/tmp/siren-io-test"
    save_brep box, @path
    mybox = load_brep @path
    assert mybox.class == Siren::Solid
    p mybox.dump
  end

  def test_iges

  end

  def test_step

  end

  def test_stl

  end

  def test_ply

  end

end
