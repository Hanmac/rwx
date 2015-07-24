RSpec.describe WX::Menu::Item do

  subject { i = WX::Menu::Item.new; i.id = :open; i.label = nil; i }

  include_examples "copyable", true
  include_examples "dumpable", true

end
