RSpec.describe WX::Brush do
  
  it "equal to its used color" do
    c = WX::Color.new(255, 0, 0)
    b = WX::Brush.new(c)
    
    #because brush == color is threated as brush == WX::Brush.new(color)
    expect(b).to eq(c)
  end

#  it "undefined color constants before app-init" do
#    #constants are not defined yet can only use them later
#    expect { WX::Brush::RED }.to raise_error(NameError)
#    expect { WX::Brush::BLUE }.to raise_error(NameError)
#  end
  
  it "equal to color name after app-init" do
    b1 = WX::Brush.new(WX::Color::RED)
    #because brush == color is threated as brush == WX::Brush.new(color)
    expect(b1).to eq(WX::Color::RED)
    #because brush == string is threated as brush == WX::Brush.new(WX::Color.new(string))
    expect(b1).to eq("red")

    b2 = WX::Brush.new(WX::Color::BLUE)
    expect(b2).to eq(WX::Color::BLUE)
    expect(b2).to eq("blue")
  end

  it "defined brush constants after app-init" do
    expect { WX::Brush::RED }.to_not raise_error
    expect { WX::Brush::BLUE }.to_not raise_error
    expect { WX::Brush::NOTHING }.to raise_error(NameError)
  end

  it "equal to color constants after app-init" do
    expect(WX::Brush::RED).to eq(WX::Color::RED)
    expect(WX::Brush::BLUE).to eq(WX::Color::BLUE)
  end

end