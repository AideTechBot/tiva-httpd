import serial
import socket
import time
import sys

def printUsage():
	print("Usage: python " + sys.argv[0] + " [IP]... [PORT]... ")
	print("Listens on a certain port and forwards it to serial /dev/ACM0")
	print("-d,\t\t\tuse a different serial device")

def recvall(sock):
	size = 8096
	req = ""
	while True:
		some = sock.recv(size).decode()
		req += some
		if len(some) < size:
			break
	return req.encode()


#main exection block
if __name__ == "__main__":
	
	BUFF_LEN = 1024

	#exit if theres no args
	if(len(sys.argv) < 2):
		printUsage()
		exit()

	if("-d" in sys.argv):
		DEVICE = sys.argv[sys.argv.index("-d") + 1]
	else:
		DEVICE = '/dev/ttyACM0'	
	
	try:
		#put a sock in it
		sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		sock.bind((sys.argv[1],int(sys.argv[2])))
		sock.listen(5)
	except socket.gaierror:
		print("Invalid arguments.")
		printUsage()
		exit()
	
	
	
	devSerial = serial.Serial(
		port=DEVICE,
		baudrate=9600,
		parity=serial.PARITY_NONE,
		stopbits=serial.STOPBITS_ONE,
		bytesize=serial.EIGHTBITS
	)

	devSerial.isOpen()

	try:
		while 1:
			##To clear everything in the buffer already
			while(devSerial.inWaiting()):
				devSerial.read(devSerial.inWaiting())
		
			print("Waiting for packet...")
			devSerial.flushInput()

			connection, address = sock.accept()
			cmd = recvall(connection).splitlines()
			try:
				if(cmd[0].decode().split()[0] == "POST"): 
					content = recvall(connection).splitlines()
			except IndexError:
				pass
			if(len(cmd) > 0):
				print("\n")
				print(repr(cmd))
				print("\n")
				for line in cmd:
					devSerial.write(line + b'\\r' + b'\\n')
				try:
					if(cmd[0].decode().split()[0] == "POST"):
						print(repr(content))
						for line in content:
							devSerial.write(line + b'\\r' + b'\\n')
				except IndexError:
					pass
			devSerial.write(b'\\r\\n')
			print("BOT")
			response = []
			response = [devSerial.readline().decode()]
			print("wat")
			time.sleep(0.5)
			while(devSerial.inWaiting() > 0):
				print("loop: " + str(devSerial.inWaiting()))
				response.append(devSerial.read(devSerial.inWaiting()).decode())
				time.sleep(0.2)
			print(response)
			response = ''.join(response)
			print(response)
			connection.sendall(response.encode())
			connection.close()
			
			##To clear everything in the buffer already
			time.sleep(0.1)
			while(devSerial.inWaiting()):
				devSerial.read(devSerial.inWaiting())
			print("EOT")
	except KeyboardInterrupt:
		print("\nKeyboardInterrupt")
		devSerial.close()
		sock.close()
		exit()

