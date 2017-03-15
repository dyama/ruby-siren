require 'test_helper'
require 'vec_test'
require 'bndbox_test'

class SirenTest < Minitest::Test

  include Siren

  def test_that_it_has_a_version_number
    refute_nil ::Siren::VERSION
  end

  include VecTest
  include BndBoxTest

end
