RSpec.shared_examples "copyable" do
  it "equal copied version" do
    should eq(subject.dup)
  end
end