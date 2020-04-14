import sys
import glob
import serial
import time
import tkinter as tk
import threading




def serial_ports():
    """ Lists serial port names

        :raises EnvironmentError:
            On unsupported or unknown platforms
        :returns:
            A list of the serial ports available on the system
    """
    if sys.platform.startswith('win'):
        ports = ['COM%s' % (i + 1) for i in range(256)]
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this excludes your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')
    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')
    else:
        raise EnvironmentError('Unsupported platform')

    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result


    



selected_port = ''
for x in serial_ports():
    print(x)
for x in serial_ports():
    print(x)
    select = input("Use port? (Y/N)")
    if (select.upper() == "Y"):
        selected_port = x
if (selected_port == ''):
    print("No port selected")
    exit()
else:
    print("Port ",selected_port," selected")

#app.label.configure(text=selected_port)

ser = serial.Serial(
port=selected_port,\
baudrate=57600,\
parity=serial.PARITY_NONE,\
xonxoff= True,\
rtscts = True,\
dsrdtr = True,\
stopbits=serial.STOPBITS_ONE,\
bytesize=serial.EIGHTBITS,\
    timeout=1000)

print("connected to: " + ser.portstr)



count=1
while True:
    count += 1
    
    line = ser.readline()
    while (line == b''):
        line = ser.readline()
 
    line = line[:-2].decode()
    
    if "node" in line:
        line = line[4:]
        print("Node: ",' ' * 10,line)
    elif "sync" in line:
        line = line[4:]
        print("Syncing: ",' ' * 10,line)
    elif "data" in line:
        line = line[4:]
        print("Data: ",' ' * 10,line)
    else:
        print("Unknown: ",' ' * 10,line)

    if (count%30 == 0):
        ser.write("datar".encode())
    
    
ser.close()

