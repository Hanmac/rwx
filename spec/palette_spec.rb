RSpec.describe WX::Palette do

  subject { WX::Palette.new("black") }

  include_examples "copyable", true
  include_examples "dumpable", true

end
