#
# Edge クラス拡張メソッド
#
class Siren::Edge

  def length
    r = 0.0
    self.to_pts.each_cons(2) do |prev, curr|
      r += prev.dist curr
    end
    r
  end

end

