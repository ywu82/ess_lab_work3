# server.py
import socket

server_ip = '0.0.0.0'
server_port = 8080

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((server_ip, server_port))
s.listen(1)
print(f"Server listening on port {server_port}...")

conn, addr = s.accept()
print(f"Connected by {addr}")
while True:
    data = conn.recv(1024)
    if not data:
        break
    print("Received from ESP32:", data.decode())
conn.close()
