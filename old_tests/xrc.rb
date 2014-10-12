require "../ext/rwx"

STDERR.reopen("file.log","w")

class A < WX::App
	def on_init
	
		WX::xrc_load_file("wizard.xrc") {
			@frame = WX::Wizard.new(nil,"MyWizard1")
		}
#		@frame.each_child.select {|w| w.is_a?(WX::WizardPage)}.each_with_index {|w,i|
##			p w.bitmap.save_file("#{i}.png")
#		}

#		@frame = WX::Wizard.new(nil)

#		pages =[]

#		pages << @frame.add_page.tap{|p| WX::Button.new(p)}
#		pages << @frame.add_page.tap{|p| WX::Button.new(p)}

#		pages << @frame.add_page#.tap{|p|p.bitmap = "0.png"}
#		pages << @frame.add_page#.tap{|p|p.bitmap = "1.png"}

#		@frame.chain_pages(*pages)

#		@frame.sizer
		
		
#		@frame.layout
#		@frame.refresh
		
		@frame.run_wizard#@frame.each_child.find {|w| w.is_a?(WX::WizardPage)})
		false
	end
end

A.new.main_loop
