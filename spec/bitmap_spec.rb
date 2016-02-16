RSpec.describe WX::Bitmap do

  subject { WX::Bitmap.new(3, 4) }

  include_examples "copyable", true
  include_examples "dumpable", true

  [24, 32].each do |depth|
    describe "with #{depth} bit" do
      subject { WX::Bitmap.new(3, 4, :depth => depth) }

      include_examples "copyable", true
      include_examples "dumpable", true
    end

    [0.5, 1.0, 2.0].each do |scale|
      describe "with #{depth} bit and #{scale} scale" do
        subject { WX::Bitmap.new(100, 200, :depth => depth, :scale => scale) }

        include_examples "copyable", true
        include_examples "dumpable", true
      end
    end
  end
end
