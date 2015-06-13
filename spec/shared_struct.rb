require "ostruct"

RSpec.shared_examples "struct_equal" do |data|
  it "equal struct version" do
    should eq(OpenStruct.new(data))
  end
  it "equal array version" do
    should eq(data.values)
    should_not eq(data.values * 2) #should not work with array of double size
  end
  it "equal hash version" do
    should eq(data)
  end
end
