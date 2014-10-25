RSpec.shared_examples "dumpable" do
  it "equal dumped version" do
    should eq(Marshal.load(Marshal.dump(subject)))
  end
end