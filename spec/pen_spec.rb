RSpec.describe WX::Pen do
  subject { WX::Pen::RED }

  include_examples "copyable"
  include_examples "dumpable"

  include_examples "gdiobject"

end