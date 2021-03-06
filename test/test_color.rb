require "test/unit"
require "ostruct"

require_relative "../ext/rwx"

class ColorTest < Test::Unit::TestCase

  def test_color_overflow
  
    c = WX::Color.new(300, 0, 0)
    assert_equal(c.red, 44)
    assert_equal(c.green, 0)
    assert_equal(c.blue, 0)
    assert_equal(c.alpha, 255)
  end
  
  def test_color_constant_and_names
    assert_raises(NameError) { WX::Color::RED }
    assert_raises(NameError) { WX::Color::BLUE }
    c1 = WX::Color.new(255, 0, 0)
    c2 = WX::Color.new(254, 0, 0)
    assert_nil(c1.name)    
    assert_nil(c2.name)
        
    test = self
    WX::App() {
      test.assert_nothing_raised(NameError) { WX::Color::RED }
      test.assert_nothing_raised(NameError) { WX::Color::BLUE }
      test.assert_raises(NameError) { WX::Color::Nothing }
      test.assert_equal(c1.name, "red")
      test.assert_equal(c1, "red")
      
      test.assert_nil(c2.name)

      break
    }
    
  end
  
  def test_equal
    c1 = WX::Color.new(255, 0, 0)
    c2 = WX::Color.new(254, 0, 0)
    assert_equal(c1, "red")
    assert_not_equal(c1, "blue")
    assert_equal(c1, "rgb(255, 0, 0)")
    assert_equal(c1, "rgba(255, 0, 0, 255)")
    assert_equal(c1, "#FF0000")
    assert_equal(c1, 0x0000FF)
    assert_equal(c1, OpenStruct.new(:red => 255, :green => 0, :blue => 0))
    assert_equal(c1, OpenStruct.new(:red => Rational(255, 256), :green => 0, :blue => 0))
    
    assert_not_equal(c2, "red")
    assert_not_equal(c2, "blue")
    assert_equal(c2, "rgb(254, 0, 0)")
    assert_equal(c2, "rgba(254, 0, 0, 255)")
    assert_equal(c2, "#FE0000")
    assert_equal(c2, 0x0000FE) 
    assert_equal(c2, OpenStruct.new(:red => 254, :green => 0, :blue => 0))
    assert_equal(c2, OpenStruct.new(:red => Rational(254, 256), :green => 0, :blue => 0))
  end
  
  

end