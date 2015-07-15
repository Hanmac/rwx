RSpec.describe WX::Image do

  subject { WX::Image.new(3, 4) }

  include_examples "copyable", true
  include_examples "dumpable", true

end
