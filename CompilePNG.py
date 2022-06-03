
# Removes alpha bytes
from PIL import Image
import sys
import os

CIPath = "./ConvertedImages/"
ITCPath = "./ImagesToConvert"
headerName = "graphics.h"

def processFrame(img):
    red_image_rgb = img.convert("RGB")

    result = ""

    for i in range(1024):
        for item in red_image_rgb.getpixel((i%32,i/32)):
            result = result + "0x" + hex(item)[2:].zfill(2) + ", "
        if i%32 == 31:
            result += "\n"

    return result

def processFile(filename):

    name = filename.split("\\")[-1].split(".")[0]

    img = Image.open(filename)
    res = "unsigned char " + name + "Data[" + str(img.n_frames) + "][32][32][3] = {"
    for i in range(img.n_frames):
        img.seek(i)
        res = res + processFrame(img) + "\n"
    res = res + "};\nImage " + name + " = {" + str(img.n_frames) + ", " + name + "Data};"
    f = open(CIPath + name + ".h", "w")
    f.write(res)
    f.close()
    print("file created!")


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
    


