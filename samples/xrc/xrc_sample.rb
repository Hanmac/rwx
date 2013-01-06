begin
  require 'rubygems' 
rescue LoadError
end

begin
require 'rwx'
rescue LoadError
require "../../ext/rwx"
end

#STDERR.reopen("file.log","w")
# Basic Frame Class. This creates the dialog window
class SimpleFrame < WX::Frame 
  def initialize
    super nil, :title => "Sample", :position => [50, 50], :size => [300, 300]

    txt = "Choose 'Open Dialog' from the menu to see a dialog made with XRC"
    WX::StaticText.new self, :label => txt, :position => [20, 20]

    # Create a new menu
    self.menubar = WX::MenuBar.new(nil) {|mbar|
			mbar.append("File") {|menu|
				menu.append_normal(:open, "Open Dialog") {p SimpleDialog.new(self).show_modal }
				menu.append_normal(:exit, "Quit") { close }
			}
		}
  end
end

class SimpleDialog < WX::Dialog
  def initialize(parent)
    # To load a layout defined in XRC into a Ruby subclass of Dialog,
    WX::xrc_load_file("samples.xrc") {
    	super(parent,'SAMPLE_DIALOG')
    }
    
    bind(:button_clicked,:ok) { end_modal(:ok) }
    bind(:button_clicked,:cancel) { end_modal(:cancel) }
    bind(:button_clicked,:SAMPLE_MESSAGE) { 
      WX::message_box(nil,"And now a message from our sponsors.")
     }

  end
end

# Application class.
class XrcApp < WX::App

  def on_init
    # Get a new resources object
    # Show the main frame.
    main = SimpleFrame.new
    main.show
  end
end

XrcApp.new.main_loop()

