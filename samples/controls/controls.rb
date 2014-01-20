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
      @book.image_list = ["icons/list.xpm","icons/choice.xpm"]
        
      @book.add_page(WX::Panel,"wxListBox",true,0) {|panel|
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
            listbox.string_selection = "This" 
            listbox_sorted.string_selection = "This"
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
            Array(listbox.selection).reverse_each {|sel| 
              listbox.delete_item(sel) if sel != -1
            }
            
            sel = listbox_sorted.selection
            listbox_sorted.delete_item(sel) if sel != -1
          }
        }
      }
      
      
      @book.add_page(WX::Panel,"wxChoice",false,1) {|panel|
        choice = WX::Choice.new(panel, :choices => choices, :position => [10,10])
        choice_sorted = WX::Choice.new(panel, :choices => choices[0,3], :position => [10,90], :size => [120,-1], :sort => true)
        
        WX::Button.new(panel, :label => "Select #&2", :position => [180,30], :size => [140,30] ) {|button|
          button.bind(:button) {
            choice.selection = 2 if choice.item_count > 2
            choice_sorted.selection = 2 if choice_sorted.item_count > 2
          }
        }

        WX::Button.new(panel, :label => "&Select 'This'", :position => [340,30], :size => [140,30] ) {|button|
          button.bind(:button) {
            choice.string_selection = "This" if choice.items.include?("This") 
              choice_sorted.string_selection = "This" if choice_sorted.items.include?("This")
          }
        }
        
        WX::Button.new(panel, :label => "&Clear", :position => [180,80], :size => [140,30] ) {|button|
          button.bind(:button) {
            choice.clear 
            choice_sorted.clear
          }
        }
  
        WX::Button.new(panel, :label => "&Append 'Hi!'", :position => [340,80], :size => [140,30] ) {|button|
          button.bind(:button) {
            choice.append "Hi kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk!"
            choice_sorted.append "Hi hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh!"
          }
        }
        
        WX::Button.new(panel, :label => "D&elete selected item", :position => [180,130], :size => [140,30] ) {|button|
          button.bind(:button) {
            sel = choice.selection 
            choice.delete_item(sel) if sel != -1
            
            sel = choice_sorted.selection
            choice_sorted.delete_item(sel) if sel != -1
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
