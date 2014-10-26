RSpec.describe WX::Brush do
  subject { WX::Brush::RED }
    
  include_examples "copyable"
  include_examples "dumpable"

  describe "with stipple" do
    subject { 
      b = WX::Brush::new(WX::Color::RED)
      b.stipple = WX::Image.new(3, 4)
      b
    }
    include_examples "copyable"
    include_examples "dumpable"
  end
  
  include_examples "gdiobject"

end