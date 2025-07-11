# server.py
import socket

# Server will listen on all interfaces (0.0.0.0) and port 8080
server_ip = '0.0.0.0'
server_port = 8080

# Create a TCP socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Bind the socket to the IP and port
s.bind((server_ip, server_port))

# Start listening for incoming connections (max 1 queued connection)
s.listen(1)
print(f"Server listening on port {server_port}...")

# Accept a connection from a client (e.g., ESP32)
conn, addr = s.accept()
print(f"Connected by {addr}")

# Receive data in a loop
while True:
    data = conn.recv(1024)  # Receive up to 1024 bytes
    if not data:
        break  # Exit loop if no data is received (client disconnected)
    print("Received from ESP32:", data.decode())  # Decode and print data

# Close the connection
conn.close()
