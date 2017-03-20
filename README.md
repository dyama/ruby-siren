siren - 3D operation gem
========================

![siren logo](http://siren.xyz/res/siren_logo.png)

**siren** is a 3D operation gem powered by Open CASCADE Technorogy. siren is integrated to Ruby interpreter environment, so it gives high-level 3D geometrical and topological operation with easy and simple scripting.
For example, you can get volume of shape, intersection lines of some shapes, center of gravity and a result of boolean operation. It supports IGES, STEP file with NURBS surface and curve.

You can get more information at siren official page.

* siren official page: http://siren.xyz/

Example
-------

    # Make two boxes
    box1 = Siren.box [10, 10, 10]
    box2 = Siren.box [10, 10, 10]

    # Move box2
    box2.translate! [5, 5, 5]

    # Fuse two boxes
    box3 = box1.fuse box2

    # Print volume of box
    p box3.volume # => 1875

    box3.faces do |face|
      # Print area of face
      p face.area
    end

![Preview](http://siren.xyz/wp-content/uploads/2016/01/twoboxes.jpg)

You can find more examples code at the Github page.

https://github.com/dyama/mruby-siren/wiki/Example

Requirement
-----------

* Linux

Dependencies
------------

* Open CASCADE Technorogy 7.1.0
  * Official page: http://www.opencascade.org/
  * Source code: http://www.opencascade.com/content/latest-release
  * License: http://www.opencascade.com/content/licensing

How to build
------------

Add this line to your application's Gemfile:

```ruby
gem 'siren2'
```

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install siren2

Usage
-----

```ruby
require 'siren'
include Siren
```

Documentation
-------------

* Official web page: http://siren.xyz/
* Tutorial: http://siren.xyz/tutorial.html
* Wiki: https://github.com/dyama/mruby-siren/wiki.
  * Reference Manual: https://github.com/dyama/mruby-siren/wiki/Reference-manual
  * Examples: https://github.com/dyama/mruby-siren/wiki/Example

Development
-----------

After checking out the repo, run `bin/setup` to install dependencies. Then, run `rake test` to run the tests. You can also run `bin/console` for an interactive prompt that will allow you to experiment.

To install this gem onto your local machine, run `bundle exec rake install`. To release a new version, update the version number in `version.rb`, and then run `bundle exec rake release`, which will create a git tag for the version, push git commits and tags, and push the `.gem` file to [rubygems.org](https://rubygems.org).

Contributing
------------

Bug reports and pull requests are welcome on GitHub at https://github.com/dyama/ruby-siren.

Lincense
--------
MIT License

Show `LICENSE` file.

Author
------
* dyama <dyama@member.fsf.org>

