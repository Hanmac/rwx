
RSpec.describe WX::Color do
  it "overflow on bounderies" do
    c = WX::Color.new(40, 0, 0)
    
    expect(c.alpha).to eq(255)
    
    expect { c.red = 300 }.to change { c.red }.
      from( 40 ).
      to( 44 )
  end
  
  it "not equal to color name before app-init" do
    c = WX::Color.new(255, 0, 0)
    expect(c.name).not_to eq("red")
    expect(c.name).to be_nil
  #  expect(c).not_to eq("red")
  end

  it "undefined color constants before app-init" do
    expect { WX::Color::RED }.to raise_error(NameError)
    expect { WX::Color::BLUE }.to raise_error(NameError)
  end
  
  it "start app" do
    WX::App() { break }
  end
  
  it "equal to color name after app-init" do
    c = WX::Color.new(255, 0, 0)
    expect(c.name).to eq("red")
    expect(c).to eq("red")
  end

  it "defined color constants after app-init" do
    expect { WX::Color::RED }.to_not raise_error
    expect { WX::Color::BLUE }.to_not raise_error
    expect { WX::Color::NOTHING }.to raise_error(NameError)
  end

end