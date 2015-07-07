RSpec.shared_examples "dumpable" do |hashable = false|
  it "equal dumped version" do
    dumped = Marshal.load(Marshal.dump(subject))
    should eq(dumped)
    expect(subject.class).to eq(dumped.class)
  end
  
  if hashable
    it "hash equal dumped version" do
      dumped = Marshal.load(Marshal.dump(subject))
      should eql(dumped)
      expect(subject.hash).to eq(dumped.hash)
    end
  else
    it "not hash equal dumped version" do
      dumped = Marshal.load(Marshal.dump(subject))
      should_not eql(dumped)
      expect(subject.hash).not_to eq(dumped.hash)
    end
  end
end
