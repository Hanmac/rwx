RSpec.describe WX::TextAttr do

  subject { WX::TextAttr.new(:text_color => WX::Color::BLACK) }

  include_examples "copyable", true
  #include_examples "dumpable", true

  it "equal hash version" do
    should eq(:text_color => WX::Color::BLACK)
  end

end
