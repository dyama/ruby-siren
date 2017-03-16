#!/usr/bin/env ruby
# coding: utf-8

##
# Siren::Solid test module
#

require 'test_helper'

module SolidTest

  include Siren

  def test_solid
    p box.volume
  end

end
