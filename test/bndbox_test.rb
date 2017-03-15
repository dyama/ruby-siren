#!/usr/bin/env ruby
# coding: utf-8

##
# Siren::BndBox test module
#

require 'test_helper'

module BndBoxTest

  include Siren

  def test_bndbox

    b = BndBox.new
    assert b.add([1, 2, 3]).nil?

  end

end
