#!/usr/bin/ruby
require 'wave_writer'

#constants
sample_rate=8192
hz = Math::PI * 2.0 / sample_rate

freq1 = 89.3 * hz
freq2 = 88.7 * hz
freq3 = 90.3 * hz
freq4 = 83.9 * hz

amp1 = 100.0


f=WaveWriter.new 'circle.wav' 
# 10 seconds
1.upto(sample_rate*10) do |n|
  x = Math.sin(n * freq1) * amp1 *  Math.sin(n * freq3)
  y = Math.cos(n * freq2) * amp1 *  Math.cos(n * freq4)
  f.encode x,y
end

f.finalize
