
RSpec.describe WX::Size do
  subject { ::WX::Size.new(30,40) }
  
  include_examples "copyable"
  include_examples "dumpable"

  include_examples "struct_equal", {:width => 30, :height => 40}
  

  describe "with subclass" do
    subject { class SubSize < WX::Size; end; SubSize.new(30,40) }
    include_examples "copyable"
    include_examples "dumpable"

    include_examples "struct_equal", {:width => 30, :height => 40}
  end

  it "equal rect" do
    should eq(WX::Rect.new(1, 2, 30, 40))
  end
end
