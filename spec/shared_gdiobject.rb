RSpec.shared_examples "gdiobject" do
  it "equal to its used color" do
    c = WX::Color.new(255, 0, 0)
    b = described_class.new(c)
    
    #because brush == color is threated as brush == WX::Brush.new(color)
    expect(b).to eq(c)
    
    #not equal if style is different from default one
    b.style = :cross_hatch
    expect(b).not_to eq(c)
  end
  
  it "equal to color name after app-init" do
    b1 = described_class.new(WX::Color::RED)
    #because brush == color is threated as brush == WX::Brush.new(color)
    expect(b1).to eq(WX::Color::RED)
    #because brush == string is threated as brush == WX::Brush.new(WX::Color.new(string))
    expect(b1).to eq("red")

    b2 = described_class.new(WX::Color::BLUE)
    expect(b2).to eq(WX::Color::BLUE)
    expect(b2).to eq("blue")
  end

  it "defined constants after app-init" do
    expect { described_class::RED }.to_not raise_error
    expect { described_class::BLUE }.to_not raise_error
    expect { described_class::NOTHING }.to raise_error(NameError)
  end

  it "equal to color constants after app-init" do
    expect(described_class::RED).to eq(WX::Color::RED)
    expect(described_class::BLUE).to eq(WX::Color::BLUE)
  end

end