#!/usr/bin/ruby

class WaveWriter
  def initialize(name)
    @fd=File.new(name,'w+b')
    write_header
  end

  # only works for 8 bit stereo, unsigned char
  def write_header(filesize = 44)
    header = [
      "RIFF",
      filesize - 8,
      "WAVE",
      "fmt ",
      16, # chunksize
      1, # compression
      2, # channels
      8192, # sample rate
      16384, # bytes/sec
      2, # block align
      8, # bits / sample
      "data",
      filesize - 44,
    ].pack "A4VA4A4VvvVVvvA4V"
    @fd.write header
  end

  def encode(x,y)
    @fd.write [x.to_i+128,y.to_i+128].pack("CC")
  end

  def finalize
    filesize = @fd.tell
    @fd.rewind
    write_header filesize
    @fd.close
    @fd=nil
  end
end

