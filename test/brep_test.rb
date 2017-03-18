#!/usr/bin/env ruby
# coding: utf-8

##
# BRep methods test module
#

require 'test_helper'

module BRepTest

  include Siren

  def test_brep

    path = "/tmp/siren-brep-test.brep"
    save_brep box, path
    mybox = load_brep path
    assert mybox.class == Siren::Solid

    p mybox.dump

  end

end
