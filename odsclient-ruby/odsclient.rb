#!/usr/bin/ruby

require 'socket'
require 'timeout'
require 'yaml'

cfg_file = ENV['HOME'] + '/.odscfg'
cfg = YAML.load_file cfg_file

class ODSClient
  attr_accessor :sock
  def initialize(username, password, host='ods.org', port=7070)
    self.sock = Timeout::timeout(30) { TCPSocket.new(host, port) }
    expect [100]
    send_cmd 'LOGIN', username, password
    expect [225,226]
  end

  def expect(codes)
    ret = Timeout::timeout(30) { sock.gets }
    p ret if $DEBUG
    return if codes.include? ret.to_i
    raise "Expected #{codes.inspect} but got #{ret.inspect}"
  end

  def send_cmd(*args)
    p args if $DEBUG
    sock.puts(args.join(' '))
  end

  def update(hostname, ipaddr)
    send_cmd 'DELRR', hostname
    expect [300,901]
    send_cmd 'ADDRR', hostname, 'A', ipaddr || 'CONNIP'
    expect [795,796]
  end
end

client = ODSClient.new(cfg['username'], cfg['password'])
cfg['hosts'].each do |hostname,ipaddr|
  client.update hostname, ipaddr
end
