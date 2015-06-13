
RSpec.describe WX::Rect do
  subject { ::WX::Rect.new(3, 4, 30, 40) }
  
  include_examples "copyable"
  include_examples "dumpable"

  include_examples "struct_equal", {:x => 3, :y => 4, :width => 30, :height => 40}
  
end
