RSpec.describe WX::Image do

  subject { WX::Image.new(100, 200) }

  include_examples "copyable", true
  include_examples "dumpable", true

  it "scale by ratio" do
    [3, -3].product([2, -2]).each do |x, y|
      scaled = subject.scale(x, y)

      expect(scaled.width).to eq(x.abs * 100)
      expect(scaled.height).to eq(y.abs * 200)
    end
  end

  it "scale by size" do
    scaled = subject.scale(WX::Size.new(200, 100))

    expect(scaled.width).to eq(200)
    expect(scaled.height).to eq(100)

    expect { subject.scale(WX::Size.new(3, -3)) }.to raise_error(ArgumentError)
    expect { subject.scale(WX::Size.new(-3, 3)) }.to raise_error(ArgumentError)
  end

  it "error on invalid" do
    expect { WX::Image.new(-10, 10) }.to raise_error(ArgumentError)
    expect { WX::Image.new(10, -10) }.to raise_error(ArgumentError)
  end
end
