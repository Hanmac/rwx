RSpec.shared_examples "copyable" do
  it "equal copied version" do
    copy = subject.dup
    should eq(copy)
    expect(subject.class).to eq(copy.class)
  end
end
