# -*- mode: ruby; coding: utf-8 -*-
gem "rdoc"
require "rake"
require "rake/clean"
require "rubygems/package_task"
require "rdoc/task"

begin
require 'rake/extensiontask'
rescue LoadError
end

ENV["RDOCOPT"] = "" if ENV["RDOCOPT"]
TMPSRCDIR = "tmpsrc"
CLEAN.include(TMPSRCDIR)
CLOBBER.include("rdoc")


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

RDoc::Task.new do |rdoc|
  rdoc.title = "rwx RDocs"
  rdoc.main = "README.rdoc"
  rdoc.rdoc_files.include("*.rdoc", TMPSRCDIR)
  rdoc.rdoc_dir = "rdoc"

  # We will have another 'rdoc' task that depends on this task
  rdoc.name = :docs
end

# This task removes the DLL_LOCAL statements temporarily to
# make RDoc work properly. The stripped sources are to be
# found in TMPSRCDIR afterwards.
task :prepare_sources do
  rm_r "tmpsrc" if File.directory?(TMPSRCDIR)

  mkdir TMPSRCDIR
  
  #cp "ext/*.cpp", TMPSRCDIR
  Dir["ext/*.cpp"].each do |path|
    path = path[/\/(.+)/,1]
    puts "Removing DLL_LOCAL in #{path}"

    File.write("#{TMPSRCDIR}/#{path}",File.read("ext/#{path}").gsub(/DLL_LOCAL\s?/, ""))
  end
end

# Removes the temporary directory created by :prepare_sources.
task :clean_sources do
  rm_r TMPSRCDIR
end

desc "Generate the RDoc documentation."
task :rdoc => [:prepare_sources, :docs, :clean_sources]