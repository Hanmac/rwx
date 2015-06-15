
RSpec.describe WX::Rect do
  subject { ::WX::Rect.new(3, 4, 30, 40) }
  
  include_examples "copyable"
  include_examples "dumpable"

  include_examples "struct_equal", {:x => 3, :y => 4, :width => 30, :height => 40}
  
  describe "with subclass" do
    subject { class SubRect < WX::Rect; end; SubRect.new(3, 4, 30, 40) }
    include_examples "copyable"
    include_examples "dumpable"

    include_examples "struct_equal", {:x => 3, :y => 4, :width => 30, :height => 40}
  end
end
