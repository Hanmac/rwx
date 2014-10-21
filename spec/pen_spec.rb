RSpec.describe WX::Pen do
  
  it "equal to its used color" do
    c = WX::Color.new(255, 0, 0)
    pen = WX::Pen.new(c)
    
    #because Pen == color is threated as Pen == WX::Pen.new(color)
    expect(pen).to eq(c)
  end

#  it "undefined color constants before app-init" do
#    #constants are not defined yet can only use them later
#    expect { WX::Pen::RED }.to raise_error(NameError)
#    expect { WX::Pen::BLUE }.to raise_error(NameError)
#  end
  
  it "equal to color name after app-init" do
    pen1 = WX::Pen.new(WX::Color::RED)
    #because Pen == color is threated as Pen == WX::Pen.new(color)
    expect(pen1).to eq(WX::Color::RED)
    #because Pen == string is threated as Pen == WX::Pen.new(WX::Color.new(string))
    expect(pen1).to eq("red")

    pen2 = WX::Pen.new(WX::Color::BLUE)
    expect(pen2).to eq(WX::Color::BLUE)
    expect(pen2).to eq("blue")
  end

  it "defined Pen constants after app-init" do
    expect { WX::Pen::RED }.to_not raise_error
    expect { WX::Pen::BLUE }.to_not raise_error
    expect { WX::Pen::NOTHING }.to raise_error(NameError)
  end

  it "equal to color constants after app-init" do
    expect(WX::Pen::RED).to eq(WX::Color::RED)
    expect(WX::Pen::BLUE).to eq(WX::Color::BLUE)
  end

end