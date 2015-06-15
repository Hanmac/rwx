
RSpec.describe WX::Point do
  subject { ::WX::Point.new(3,4) }
  
  include_examples "copyable"
  include_examples "dumpable"

  include_examples "struct_equal", {:x => 3, :y => 4}
  

  describe "with subclass" do
    subject { class SubPoint < WX::Point; end; SubPoint.new(3,4) }
    include_examples "copyable"
    include_examples "dumpable"

    include_examples "struct_equal", {:x => 3, :y => 4}
  end

end
