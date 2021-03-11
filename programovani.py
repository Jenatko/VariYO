"""
Python dependencies:
pyserial
pywin32
"""


from __future__ import print_function
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

COM0 = ""
COM1 = ""

if(len(sys.argv) != 3):
  sys.exit(1)

flag_found = False
print("Looking for ports.")
for i in range(2, 20):
    COM0 = "COM"+repr(i)
    print("\tConnecting to port {:s}...".format(COM0), end='')
    try:
      ser = serial.Serial(COM0, 1200)
      ser.close()
    except Exception as ex:
      print("Not this one.")
    else:
        flag_found = True
        print("Found open port: ", COM0)
        COM1 = "COM" + repr(i+1)
        break
print("Done.")

if not flag_found:
    print('No open port found. Aborting.')
    exit(-1)

flag = False
print("Connecting to port {:s}...".format(COM1), end='')
for k in range(2, 100):
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
print(sys.argv[2]+'\\bossac.exe')
subprocess.call([sys.argv[2]+"\\bossac.exe", "--port="+COM, "-e", "-w", "-v", sys.argv[1], "-R"])