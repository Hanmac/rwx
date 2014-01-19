#Encoding: UTF-8
require "../../ext/rwx"

class A < WX::App
  
  def on_init
  
    @frame = WX::Frame.new(nil,
      :icon => "../sample.xpm",:title => "Controls wxWidgets App",
      :position => [50, 50], :size => [700, 450]
    )
  
    @frame.menubar = WX::MenuBar.new(nil) {|m|
      m.append(:File) {|menu|
        
      }
    }
     
    choices = ["This",
      "is",
      "one of my long and",
      "wonderful",
      "examples."]
    
    @frame.tap {|frame|
    #WX::Panel.new(@frame,:position => [10, 10], :size => [300, 100]) {|panel|
           
      @text = WX::TextCtrl.new(frame,
        :value => "This is the log window.\n",
        :position => [0, 250], :size => [100, 50],
        :style => WX::TextCtrl::MULTILINE
      ) 
      
      
      @book = WX::NoteBookCtrl.new(frame)
           
      @book.add_page(WX::Panel,"wxListBox") {|panel|
        listbox = WX::ListBox.new(panel, :choices => choices, :position => [10,10], :size => [120,70], :multiple => true)
        listbox_sorted = WX::ListBox.new(panel, :choices => choices[0,3], :position => [10,90], :size => [120,70], :sort => true)
        
        WX::Button.new(panel, :label => "Select #&2", :position => [180,30], :size => [140,30] ) {|button|
          button.bind(:button) {
            listbox.selection = 2 if listbox.item_count > 2
            listbox_sorted.selection = 2 if listbox_sorted.item_count > 2
          }
        }

        WX::Button.new(panel, :label => "&Select 'This'", :position => [340,30], :size => [140,30] ) {|button|
          button.bind(:button) {
            listbox.string_selection = "This" if listbox.items.include?("This") 
            listbox_sorted.string_selection = "This" if listbox_sorted.items.include?("This")
          }
        }
        
        WX::Button.new(panel, :label => "&Clear", :position => [180,80], :size => [140,30] ) {|button|
          button.bind(:button) {
            listbox.clear 
            listbox_sorted.clear
          }
        }
  
        WX::Button.new(panel, :label => "&Append 'Hi!'", :position => [340,80], :size => [140,30] ) {|button|
          button.bind(:button) {
            listbox.append "Hi kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk!"
            listbox_sorted.append "Hi hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh!"
          }
        }
        
        WX::Button.new(panel, :label => "D&elete selected item", :position => [180,130], :size => [140,30] ) {|button|
          button.bind(:button) {
            sel = listbox.selection 
            listbox.delete_item(sel) if sel != -1
            
            sel = listbox_sorted.selection
            listbox_sorted.delete_item(sel) if sel != -1
          }
        }
      }
      
      @frame.sizer = WX::BoxSizer.new {|box|
        box.orientation = true
        
        box.add(@book,:border => true, :expand => true)
        box.add(@text,:proportion => 1, :border => true,  :expand => true)
      } 
      
    }
    
    @frame.show
  end
end

A.new.main_loop
