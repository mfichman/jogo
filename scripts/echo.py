
import socket, sys 

host = '' 
port = 5000 
backlog = 5 
size = 1024 
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
s.setsockopt(socket.IPPROTO_TCP, socket.SO_REUSEADDR, True)
s.bind((host,port)) 
s.listen(backlog) 
while 1: 
    client, address = s.accept() 
    data = client.recv(size) 
    client.send(data)
    sys.stdout.write(data)
    client.close()
