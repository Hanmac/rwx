RSpec.describe WX::Bitmap do

  subject { WX::Bitmap.new(3, 4) }

  include_examples "copyable", true
  include_examples "dumpable", true

  [24, 32].each do |depth|
    describe "with #{depth} bit" do
      subject { WX::Bitmap.new(3, 4, :depth => depth) }

      include_examples "copyable", true
      include_examples "dumpable", true

      it "check values" do
        expect(subject.width).to eq(3)
        expect(subject.height).to eq(4)
        expect(subject.depth).to eq(depth)
      end
    end

    [0.5, 1.0, 2.0].each do |scale|
      describe "with #{depth} bit and #{scale} scale" do
        subject { WX::Bitmap.new(100, 200, :depth => depth, :scale => scale) }

        include_examples "copyable", true
        include_examples "dumpable", true

        it "check values" do
          expect(subject.width).to eq(100 * scale)
          expect(subject.height).to eq(200 * scale)
          expect(subject.depth).to eq(depth)

          if subject.scale_factor == scale
            expect(subject.scaled_width).to eq(100)
            expect(subject.scaled_height).to eq(200)
          end
        end
      end
    end
  end

  it "error on invalid" do
    expect { WX::Bitmap.new(-10, 10) }.to raise_error(ArgumentError)
    expect { WX::Bitmap.new(10, -10) }.to raise_error(ArgumentError)
    expect { WX::Bitmap.new(10, 10, :scale => 0.0) }.to raise_error(ArgumentError)
    expect { WX::Bitmap.new(10, 10, :scale => -1.0) }.to raise_error(ArgumentError)
  end
end
