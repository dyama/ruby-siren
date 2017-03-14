require 'test_helper'

class SirenTest < Minitest::Test
  def test_that_it_has_a_version_number
    refute_nil ::Siren::VERSION
  end

  def test_vec
    # p Siren::Vec.new
    # p Siren::Vec.new.x
    # p Siren::Vec.new.y
    # p Siren::Vec.new.z
    # v = Siren::Vec.new(1.0, 2, 3.to_f)
    # v.x = 1.0;
    # v.y = 2;
    # v.z = 3.to_f;
    # p v.x
    # p v.y
    # p v.z
    v2 = Siren::Vec.new([1.0, 2, 3.to_f])
    p v2.x
    p v2.y
    p v2.z
  end

end
