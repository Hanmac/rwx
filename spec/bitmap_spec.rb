RSpec.describe WX::Bitmap do

  subject { WX::Bitmap.new(3, 4) }

  include_examples "copyable", true
  include_examples "dumpable", true

  describe "with 24 bit" do
    subject { WX::Bitmap.new(3, 4, 24) }

    include_examples "copyable", true
    include_examples "dumpable", true
  end

  describe "with 32 bit" do
    subject { WX::Bitmap.new(3, 4, 32) }

    include_examples "copyable", true
    include_examples "dumpable", true
  end

end
