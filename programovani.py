import serial
import subprocess
import time
import sys
import win32com.client

ports = []
string = ""
port_erase = ""



##wmi = win32com.client.GetObject("winmgmts:")
###for serial in wmi.InstancesOf("Win32_SerialPort"):
## #      ports.append((serial.Name, serial.Description))
##       
##
##for x in ports:
##  if "Arduino Zero" in x:
##      string = str(x)
##
##def getComIndex(sparsed):
##    sfind = "COM"
##    startIndex = sparsed.find(sfind) + len(sfind)
##
##    sparsed = sparsed[startIndex:]
##    comIndex = int(sparsed.split(')')[0])
##    return comIndex
##
##
###port_erase = "COM"+str(getComIndex(string))
##print port_erase


if(len(sys.argv) != 2):
  sys.exit(1)

try:
  ser = serial.Serial("COM5", 1200)
  ser.close()
except:
  pass
  
  
while(1):
  try:
    ser = serial.Serial("COM6", 1200)
    ser.close()
    break
  except:
    time.sleep(0.1)
  

subprocess.call(["D:\\Data\\Bastelni\\Vario\\VarioSW_as\\bossac.exe", "--port=COM6", "-e", "-w", "-v", sys.argv[1], "-R"])

