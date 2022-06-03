
# Removes alpha bytes
from PIL import Image
import sys
import os

CIPath = "./ConvertedImages/"
ITCPath = "./ImagesToConvert"
headerName = "graphics.h"

def processFrame(img, name):
  red_image_rgb = img.convert("RGB")

  result = "unsigned char " + name + "[32][32][3] = {\n"

  for i in range(1024):
    for item in red_image_rgb.getpixel((i%32,i/32)):
      result = result + "0x" + hex(item)[2:].zfill(2) + ", "
    if i%32 == 31:
      result += "\n"

  result = result + "};"

  f = open(CIPath + name + ".h", "w")
  f.write(result)
  f.close()

  print("file created!")

def processFile(filename):

  (name, filetype) = filename.split("\\")[-1].split(".")

  if(filetype == "png"):
    img = Image.open(filename)
    processFrame(img, name)
  elif(filetype == "gif"):
    if not os.path.exists(CIPath + name + "/"):
      os.makedirs(CIPath + name + "/")
    img = Image.open(filename)
    for i in range(img.n_frames):
      img.seek(i)
      processFrame(img,  name + "/" + name + str(i))
    print("hmmmm")


def updateGraphicsHeader():
  content = """//https://lvgl.io/tools/imageconverter
//Big endian
//Pixel format: Fix 0xFF: 8 bit, Red: 8 bit, Green: 8 bit, Blue: 8 bit
"""
  for file in os.listdir(CIPath):
    content = content + "#include \"" + CIPath + file + "\"\n"
  f = open(headerName, "w")
  f.write(content)
  f.close()



if(len(sys.argv) > 1):
  filename = sys.argv[1]
else:
  filename = input('Enter file or folder name:')

if filename=="":
  filename = ITCPath

# #checks if path is a file
if(os.path.isfile(filename)):
  processFile(filename)
  updateGraphicsHeader()

# #checks if path is a directory
if(os.path.isdir(filename)):
  for file in os.listdir(filename):
    processFile(filename + "\\" + file)
  updateGraphicsHeader()
  


