# -*- mode: ruby; coding: utf-8 -*-
gem "rdoc"
require "rake"
require "rake/clean"
require "rubygems/package_task"

require 'rake/extensiontask' rescue nil

ENV["RDOCOPT"] = "" if ENV["RDOCOPT"]
CLOBBER.include("doc")

load "rwx.gemspec"

Gem::PackageTask.new(GEMSPEC)do |pkg|
  pkg.need_zip = true
  pkg.need_tar = true
  pkg.need_tar_bz2 = true
  pkg.need_tar_gz = true
end

unless Rake.const_defined?(:ExtensionTask)
  desc "Compiles rwx, outputting ext/rwx.so"
  task :compile do
    cd "ext"
    ruby "extconf.rb"
    sh "make"
    cd ".."
  end
else
  Rake::ExtensionTask.new('rwx',GEMSPEC) do |t|
    t.ext_dir = "ext"
    t.source_pattern = "*.cpp"
  end
  
  Rake::ExtensionTask.new('rwx',GEMSPEC) do |t|
    t.ext_dir = "ext"
    t.source_pattern = "*.cpp"
    
    t.cross_compile = true                # enable cross compilation (requires cross compile toolchain)
    t.cross_platform = 'i386-mswin32'     # forces the Windows platform instead of the default one
  end
end