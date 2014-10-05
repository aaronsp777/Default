#!/usr/bin/ruby

def log(client, line)
  File.open(client, "a") do |f|
    f.puts(line)
  end
end

def parse_line(line)
  time, pkgtype, src, arrow, dest, num, q, subject, cnt = line.split
  dest.gsub!(/:$/, '')
  if /\+$/.match(num)
    # request
    client = src
  else
    client = dest
  end

  m = /(^[0-9a-zA-Z._-]+)\.(\d+)$/.match(client)
  if m
    log(m[1], line)
  else
    log('unknown', line)
  end
  
end

STDIN.each_line do |line|
  parse_line(line)
end
