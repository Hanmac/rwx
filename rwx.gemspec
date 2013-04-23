# -*- mode: ruby; coding: utf-8 -*-

GEMSPEC = Gem::Specification.new do |spec|

  # Project information
  spec.name        = "rwx"
  spec.summary     = "Ruby newest wxWidgets Binding"
  spec.description =<<-DESCRIPTION
newest Binding for wxWidgets 2.9+, it was made after wxRuby died.
  DESCRIPTION
  spec.version     = File.read("VERSION").strip.gsub("-", ".")
  spec.author      = "Hanmac"
  spec.email       = "hanmac@gmx.de"

  # Requirements
  spec.platform              = Gem::Platform::RUBY
  spec.required_ruby_version = ">= 1.9.2"

  # Gem contents
  spec.files         = `git ls-files`.split("\n")
  spec.has_rdoc         = true
  spec.extra_rdoc_files = ["README.rdoc", "COPYING"]
  spec.rdoc_options << "-t" << "The rwx RDocs" << "-m" << "README.rdoc"

end
