RSpec.shared_examples "copyable" do |hashable = false|
  it "equal copied version" do
    copy = subject.dup
    should eq(copy)
    expect(subject.class).to eq(copy.class)
  end

  if hashable
    it "hash equal copied version" do
      copy = subject.dup
      should eql(copy)
      expect(subject.hash).to eq(copy.hash)
    end
  else
    it "not hash equal copied version" do
      copy = subject.dup
      should_not eql(copy)
      expect(subject.hash).not_to eq(copy.hash)
    end
  end


end
