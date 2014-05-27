#!/usr/bin/ruby

# standard SVG header
puts <<XXX
<?xml version="1.0" standalone="no"?>
<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN"
    "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
<svg viewbox="0 0 1000 1000" version="1.1">
XXX

# draw one line
def line(color, x1, y1, x2, y2)
  d = ['M', x1, y1, 'L', x2, y2].join(" ")
  puts '  <path stroke="' + color + '" stroke-width="1" d="' + d + '"/>'
end

# draw 112 lines from 0 to 999
0.upto 111 do |i|
  j = i * 9
  line 'blue', 0, j, j, 999-j
  line 'blue', 999-j, 999, j, 999-j
  line 'red', 999, j, 999-j, 999-j
  line 'red', j, 999, 999-j, 999-j
  line 'green', 0, 999-j, j, j
  line 'green', 999-j, 0, j, j
  line 'yellow', 999, 999-j, 999-j, j
  line 'yellow', j, 0, 999-j, j
end

puts "</svg>"
