"""
Python dependencies:
pyserial
pywin32

"""


import serial
import subprocess
import time
import sys
import win32com.client

ports = []
string = ""
port_erase = ""


"""
wmi = win32com.client.GetObject("winmgmts:")
for serial in wmi.InstancesOf("Win32_SerialPort"):
    ports.append((serial.Name, serial.Description))
print(ports)       
"""

# for x in ports:
  # if "Arduino Zero" in x:
      # string = str(x)

# def getComIndex(sparsed):
    # sfind = "COM"
    # startIndex = sparsed.find(sfind) + len(sfind)

    # sparsed = sparsed[startIndex:]
    # comIndex = int(sparsed.split(')')[0])
    # return comIndex


# #port_erase = "COM"+str(getComIndex(string))
# print( port_erase)

port = 9

COM0 = "COM"+repr(port-1)
COM1 = "COM"+repr(port)


if(len(sys.argv) != 2):
  sys.exit(1)

print("Looking for ports.")
for i in range(20):
    COM0 = "COM"+repr(i)
    print("Connecting to port {:s}...".format(COM0), end='')
    try:
      ser = serial.Serial(COM0, 1200)
      ser.close()
    except Exception as ex:
      print("Not this one.")
    else:
        print("Found open port: ", COM0)
        COM1 = "COM" + repr(i+1)
        break
print("Done.")
  
flag = False
print("Connecting to port {:s}...".format(COM1), end='')
for k in range(100):
  try:
    ser = serial.Serial(COM1, 1200)
    ser.close()
    flag = True
    print("Success.")
    break
  except:
    print(".", end='')
    time.sleep(0.1)
 
COM = COM0
if flag:
    COM = COM1
 
print('Loading file "{:s}": to port {:s}'.format(sys.argv[1], COM))
subprocess.call(["D:\\VariYO\\bossac.exe", "--port="+COM, "-e", "-w", "-v", sys.argv[1], "-R"])

