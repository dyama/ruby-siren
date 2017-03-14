require 'test_helper'

class SirenTest < Minitest::Test

  include Siren

  def test_that_it_has_a_version_number
    refute_nil ::Siren::VERSION
  end

  def test_vec
    p Vec.new([1.0, 2, 3.to_f])
    assert Vec.new.to_a == [0.0, 0.0, 0.0]
    assert Vec.new.x == 0.0
    assert Vec.new.y == 0.0
    assert Vec.new.z == 0.0
    assert Vec.new([1.0, 2, 3.to_f]).to_a == [1.0, 2.0, 3.0]
    v1 = Vec.new
    v1.x = 25.to_i
    v1.y = 30.to_f
    assert v1.to_a == [25.0, 30.0, 0.0]
  end

  def test_bndbox
    p BndBox.new
  end

end
