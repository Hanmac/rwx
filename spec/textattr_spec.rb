RSpec.describe WX::TextAttr do

  subject { WX::TextAttr.new(:text_color => WX::Color::BLACK, :font_point_size => 13) }

  include_examples "copyable", true
  include_examples "dumpable", true

  it "switch between point and pixel size" do
    a = WX::TextAttr.new( :font_point_size => 13 )
    
    expect(a.font_point_size).to eq(13)
    expect(a.font_pixel_size).to be_nil

    a.font_pixel_size = 24

    expect(a.font_point_size).to be_nil
    expect(a.font_pixel_size).to eq(24)

  end

  it "equal hash version" do
    should eq(:text_color => WX::Color::BLACK, :font_point_size => 13)
  end

end
