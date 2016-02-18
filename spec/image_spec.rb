RSpec.describe WX::Image do

  subject { WX::Image.new(3, 4) }

  include_examples "copyable", true
  include_examples "dumpable", true

  it "error on invalid" do
    expect { WX::Image.new(-10, 10) }.to raise_error(ArgumentError)
    expect { WX::Image.new(10, -10) }.to raise_error(ArgumentError)
  end
end
