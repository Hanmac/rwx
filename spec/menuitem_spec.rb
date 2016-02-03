RSpec.describe WX::Menu::Item do

	[:separator, :normal, :check, :radio].each do |k|
		describe "with #{k}" do
			subject do
				item = WX::Menu::Item.new
				item.kind = k
				item
			end

			include_examples "copyable", true
			include_examples "dumpable", true
		end
	end
end
