# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'siren2/version'

Gem::Specification.new do |spec|
  spec.name          = "siren2"
  spec.version       = Siren::VERSION
  spec.authors       = ["dyama"]
  spec.email         = ["dyama@member.fsf.org"]

  spec.summary       = "3D operation library"
  spec.description   = "3D operation libray"
  spec.homepage      = "http://siren.xyz/"

  spec.files         = `git ls-files -z`.split("\x0").reject do |f|
    f.match(%r{^(test|spec|features)/})
  end
  spec.bindir        = "exe"
  spec.executables   = spec.files.grep(%r{^exe/}) { |f| File.basename(f) }
  spec.require_paths = ["lib"]
  spec.extensions    = ["ext/siren/extconf.rb"]

  spec.add_development_dependency "bundler", "~> 1.14"
  spec.add_development_dependency "rake", "~> 10.0"
  spec.add_development_dependency "rake-compiler"
  spec.add_development_dependency "minitest", "~> 5.0"
end
