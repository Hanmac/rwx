RSpec.shared_examples "dumpable" do
  it "equal dumped version" do
    dumped = Marshal.load(Marshal.dump(subject))
    should eq(dumped)
    expect(subject.class).to eq(dumped.class)
  end
end
