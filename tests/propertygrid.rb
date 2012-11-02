#Encoding: UTF-8
require "../ext/rwx"

STDERR.reopen("file.log","w")

class A < WX::App
	def on_init
	
	
#	
#		#f = WX::Font.new(10,:script)
#		#p f.faceName
		@frame = WX::Frame.new(nil)
		@frame.title = "hallo"

		pg = WX::PropertyGridManager.new(@frame,
			:style => WX::PropertyGridManager::TOOLBAR,
			:extra_style => WX::PropertyGridManager::EX_MODE_BUTTONS
		)

		fp = pg.append(WX::FontProperty)
		fp.value_image = "ruby32x32.png"
		p fp.value
		p fp.each_child.map {|pr| pr.type}
		p fp.each_child.map {|pr| pr.wxclass}
		p fp.each_child.map {|pr| pr.each_choices.to_a}
		
		@frame.show
		
	end
end
	
A.new.main_loop

#		"ruby32x32.png"