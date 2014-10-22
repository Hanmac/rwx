#Encoding: UTF-8
require "../../ext/rwx"

class SamplesControlApp < WX::App
  
  def create_button(panel, label, position, both)
    WX::Button.new(panel, :label => label, :position => position, :size => [140,30] ) {|button|
      button.bind(:button) {
        both.each { |list|
          yield list
        }
      }
    }
  end
  
  def create_list(panel, klass, choices)
    normal = klass.new(panel, :choices => choices, :position => [10,10], :size => [120,70], :multiple => true)
    sorted = klass.new(panel, :choices => choices[0,3], :position => [10,90], :size => [120,70], :sort => true)
    both = [nornal,sorted]
    
    create_button(panel, "Select #&2", [180,30], both) { |list|
      list.selection = 2 if list.item_count > 2
    }

    create_button(panel, "&Select 'This'", [340,30], both) { |list|
      list.string_selection = "This" 
    }
    
    create_button(panel, "&Clear", [180,80], both ) { |list|
      list.clear
    }

    create_button(panel, "&Append 'Hi!'", [340,80], both ) { |list|
      list.append "Hi kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk!"
    }
    
    WX::Button.new(panel, :label => "D&elete selected item", :position => [180,130], :size => [140,30] ) {|button|
      button.bind(:button) {
        yield normal, sorted
      }
    }
  end
  
  def create_bookctrl(frame, choices)
    book = WX::NoteBookCtrl.new(frame)
    book.image_list = ["icons/list.xpm","icons/choice.xpm"]
      
    book.add_page(WX::Panel,"wxListBox",true,0) {|panel|
      create_list(panel, WX::ListBox, choices) {|normal, sorted|
        Array(normal.selection).reverse_each {|sel| 
          normal.delete_item(sel) if sel != -1
        }
        
        sel = sorted.selection
        sorted.delete_item(sel) if sel != -1
      }
    }
    
    book.add_page(WX::Panel,"wxChoice",false,1) {|panel|
      create_list(panel, WX::Choice, choices) {|normal, sorted|
        sel = normal.selection 
        normal.delete_item(sel) if sel != -1
        
        sel = sorted.selection
        sorted.delete_item(sel) if sel != -1
      }
    }
  end
  
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
      
      @frame.sizer = WX::BoxSizer.new(:vertical) {|box|
        
        box.add(create_bookctrl(frame, choices),:border => true, :expand => true)
        box.add(@text,:proportion => 1, :border => true,  :expand => true)
      } 
      
    }
    
    @frame.show
  end
end

SamplesControlApp.new.main_loop
