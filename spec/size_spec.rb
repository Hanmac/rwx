RSpec.shared_examples "shared_size" do

  include_examples "copyable", true
  include_examples "dumpable", true

  include_examples "struct_equal", {:width => 30, :height => 40}

  it "equal rect" do
    should eq(WX::Rect.new(1, 2, 30, 40))
  end

  def compare_helper(obj, data)
    should_not eq(obj)
    expect(obj).to_not eq(subject)
    expect(obj.class).to eq(subject.class)

    expect(obj).to eq(WX::Size.new(*data.values))
    expect(obj).to eq(data.values)
    expect(obj).to eq(data)
  end

  it "scale" do
    obj = subject.scale(2)

    compare_helper(obj, {:width => 30 * 2, :height => 40 * 2})
  end
  
  it "scale!" do
    obj = subject.dup
    obj.scale!(2)

    compare_helper(obj, {:width => 30 * 2, :height => 40 * 2})
  end
  
  it "inc_by" do
    obj = subject.inc_by(2)

    compare_helper(obj, {:width => 30 + 2, :height => 40 + 2})
  end
  
  it "inc_by!" do
    obj = subject.dup
    obj.inc_by!(2)

    compare_helper(obj, {:width => 30 + 2, :height => 40 + 2})
  end
  
  
  it "dec_by" do
    obj = subject.dec_by(2)

    compare_helper(obj, {:width => 30 - 2, :height => 40 - 2})
  end
  
  it "dec_by!" do
    obj = subject.dup
    obj.dec_by!(2)

    compare_helper(obj, {:width => 30 - 2, :height => 40 - 2})
  end
  
end

RSpec.describe WX::Size do
  subject { ::WX::Size.new(30,40) }
  include_examples "shared_size"

  describe "with subclass" do
    subject { class SubSize < WX::Size; end; SubSize.new(30,40) }
    include_examples "shared_size"
  end

end
