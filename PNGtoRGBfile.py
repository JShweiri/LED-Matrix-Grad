
# Removes alpha bytes
from PIL import Image
import sys
import os


def processFile(filename):
  red_image = Image.open(filename)

  name = filename.split("\\")[-1].split(".")[0]

  red_image_rgb = red_image.convert("RGB")

  result = "unsigned char " + name + "[32][32][3] = {\n"

  for i in range(1024):
    for item in red_image_rgb.getpixel((i%32,i/32)):
      result = result + "0x" + hex(item)[2:].zfill(2) + ", "
    if i%32 == 31:
      result += "\n"

  result = result + "};"

  f = open("./ConvertedImages/" + name + ".h", "w")
  f.write(result)
  f.close()

  print("file created!")


def updateGraphicsHeader():
  content = """//https://lvgl.io/tools/imageconverter
//Big endian
//Pixel format: Fix 0xFF: 8 bit, Red: 8 bit, Green: 8 bit, Blue: 8 bit
"""
  for file in os.listdir("./ConvertedImages"):
    content = content + "#include\"" + file + "\"\n"
  f = open("graphics.h", "w")
  f.write(content)
  f.close()



if(len(sys.argv) > 1):
  filename = sys.argv[1]
else:
  filename = input('Enter file or folder name:')

if filename=="":
  filename = "./ImagesToConvert"

# #checks if path is a file
if(os.path.isfile(filename)):
  processFile(filename)
  updateGraphicsHeader()

# #checks if path is a directory
if(os.path.isdir(filename)):
  for file in os.listdir(filename):
    processFile(filename + "\\" + file)
  updateGraphicsHeader()
  


