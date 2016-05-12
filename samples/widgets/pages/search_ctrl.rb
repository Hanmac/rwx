#Encoding: UTF-8


class SearchCtrlPage < CommonPage
  def create_boxleft
    WX::BoxSizer.new(:vertical) {|boxleft|
      boxleft.add(
        add_statbox("&Options", :vertical) {|box|
          @search_button = add_checkbox(box, "Search button", value: true ) {
            @search_ctrl.search_button_visible = @search_button.value
          }
          @cancel_button = add_checkbox(box, "Cancel button") {
            @search_ctrl.cancel_button_visible = @cancel_button.value
          }
          @search_menu = add_checkbox(box, "Search menu") {
            @search_ctrl.menu = @search_menu.value ? create_menu : nil
          }
        },
        :expand => true
      )
    }
  end
  
  def create_menu
    menu = WX::Menu.new
    1.upto(10) {|i| menu.append_normal(nil, "item %d" % i, "tip %d" % i) }
    menu
  end
  
  def create_widget
    @search_ctrl = WX::SearchCtrl.new(self)

    @search_ctrl.bind(:searchctrl_search_btn) {
      p "seach after: #{@search_ctrl.value}"
    }
    @search_ctrl.bind(:searchctrl_cancel_btn) { p "cancel" }
  end
  
end
