# -*- mode: ruby; coding: utf-8 -*-

GEMSPEC = Gem::Specification.new do |spec|

  # Project information
  spec.name        = "rwx"
  spec.summary     = "Ruby newest wxWidgets Binding"
  spec.description =<<-DESCRIPTION
newest Binding for wxWidgets 3.0+, it was made after wxRuby died.
  DESCRIPTION
  spec.version     = File.read("VERSION").strip.gsub("-", ".")
  spec.author      = "Hanmac"
  spec.email       = "hanmac@gmx.de"
  spec.homepage    = 'https://github.com/Hanmac/rwx'
  
  spec.license     = 'LGPL-3.0'
  
  # Requirements
  spec.platform              = Gem::Platform::RUBY
  spec.required_ruby_version = ">= 2.0.0"

  spec.add_development_dependency "rdoc", '~> 4.0'
  spec.add_development_dependency "rake", '~> 11.0'
  spec.add_development_dependency "test-unit", '~> 3.0'
  spec.add_development_dependency "rspec", '~> 3.0'
  
  # Gem contents
  begin
    spec.files         = `git ls-files`.split("\n")
  ensure
    spec.files         = Dir["*.rdoc", "COPYING", "Rakefile", "VERSION",
      "rwx.gemspec", "ext/extconf.rb", "ext/*.*pp", "samples/**/**", "tests/**",
      ".rspec", "spec/*"
    ] if spec.files.empty?
  end
  
  spec.extensions << "ext/extconf.rb"
  
  spec.extra_rdoc_files = ["README.rdoc", "COPYING"]
  spec.rdoc_options << "-t" << "The rwx RDocs" << "-m" << "README.rdoc"

  spec.requirements << 'wxWidgets 3.0+'
  spec.requirements << 'git'
  spec.requirements << 'C++ compiler'
end
