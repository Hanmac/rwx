RSpec.describe WX::Font do

  [:ITALIC, :NORMAL, :SMALL, :SWISS].each do |sym|
    describe "constant #{sym}" do
      subject { WX::Font.const_get(sym) }

      include_examples "copyable", true
      include_examples "dumpable", true
    end
  end

  #currently only can check point size, pixel size does fail
  subject { WX::Font.new(13) }

  include_examples "copyable", true
  include_examples "dumpable", true

  it "equal hash version" do
    should eq(:size => 13)
  end

end
