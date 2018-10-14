import serial
import subprocess
import time
import sys

if(len(sys.argv) != 2):
  sys.exit(1)

try:
  ser = serial.Serial("COM52", 1200)
  ser.close()
except:
  pass
  
  
while(1):
  try:
    ser = serial.Serial("COM64", 1200)
    ser.close()
    break
  except:
    time.sleep(0.1)
  

subprocess.call(["C:\\Users\\Jena\\AppData\\Local\\Arduino15\\packages\\arduino\\tools\\bossac\\1.6.1-arduino\\bossac.exe", "--port=com64", "-e", "-w", "-v", sys.argv[1], "-R"])

