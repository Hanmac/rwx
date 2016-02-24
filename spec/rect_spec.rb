RSpec.shared_examples "shared_rect" do

  include_examples "copyable", true
  include_examples "dumpable", true

  include_examples "struct_equal", {:x => 5, :y => 5, :width => 10, :height => 10}


  def compare_helper(obj, data)
    should_not eq(obj)
    expect(obj).to_not eq(subject)
    expect(obj.class).to eq(subject.class)

    expect(obj).to eq(WX::Rect.new(*data.values))
    expect(obj).to eq(data.values)
    expect(obj).to eq(data)
  end

  it "check inflate" do
    obj = subject.inflate(5)

    compare_helper(obj, {:x => 0, :y => 0, :width => 20, :height => 20})
  end

  it "check deflate" do
    obj = subject.deflate(5)

    compare_helper(obj, {:x => 10, :y => 10, :width => 0, :height => 0})
  end

  it "check offset" do
    obj = subject.offset(5, 5)

    compare_helper(obj, {:x => 10, :y => 10, :width => 10, :height => 10})
  end

  it "check inflate!" do
    obj = subject.dup

    obj.inflate!(5)

    compare_helper(obj, {:x => 0, :y => 0, :width => 20, :height => 20})
  end

  it "check deflate!" do
    obj = subject.dup

    obj.deflate!(5)
    
    compare_helper(obj, {:x => 10, :y => 10, :width => 0, :height => 0})
  end

  it "check offset!" do
    obj = subject.dup

    obj.offset!(5, 5)

    compare_helper(obj, {:x => 10, :y => 10, :width => 10, :height => 10})
  end
end

RSpec.describe WX::Rect do
  subject { ::WX::Rect.new(5, 5, 10, 10) }

  include_examples "shared_rect"

  describe "with subclass" do
    subject { class SubRect < WX::Rect; end; SubRect.new(5, 5, 10, 10) }

    include_examples "shared_rect"
  end
end
