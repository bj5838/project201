import socket
import time

host = "127.0.0.1"  # Vulnserver runs on localhost
port = 9999         # Vulnserver default port

try:
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((host, port))
    
    # Vulnserver sends welcome message immediately after connection
    banner = sock.recv(2048).decode()
    print("Welcome Message:", banner)
    
    # Send HELP command (case-sensitive, needs to be uppercase)
    sock.send(b"HELP\n")
    time.sleep(0.1)  # Small delay for server processing
    
    # Get HELP response
    response = sock.recv(2048).decode()
    print("HELP Response:", response)
    
    # Test other commands (optional)
    # sock.send(b"STATS\n")
    # response = sock.recv(2048).decode()
    # print("STATS Response:", response)
    
    sock.close()
    print("\nConnection successful! Server is running.")
    
except ConnectionRefusedError:
    print("ERROR: Vulnserver not running on port 9999")
    print("1. Start Vulnserver first:")
    print("   Run: vulnserver.exe")
    print("2. Check if server is running:")
    print("   Windows: netstat -an | findstr 9999")
    print("   Linux:   netstat -an | grep 9999")
    print("3. Make sure essfunc.dll is in the same directory as vulnserver.exe")
except socket.timeout:
    print("ERROR: Connection timeout")
except Exception as e:
    print(f"Error: {str(e)}")