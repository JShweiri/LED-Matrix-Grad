
# Removes alpha bytes
from PIL import Image
import sys
import os

CIPath = "./ConvertedImages/"
ITCPath = "./ImagesToConvert"
headerName = "graphics.h"

# This function takes in a PIL image object and returns the list of RGB hex values as a string
def processFrame(img):
    rgb_img = img.convert("RGB")
    result = ""
    # for every pixel in the image
    for i in range(1024):
        # for every color in each picture (r/g/b)       the last part just converts index i to (x, y) value
        for item in rgb_img.getpixel((i%32,i/32)):
            # Add the hex value to the string
            result = result + "0x" + hex(item)[2:].zfill(2) + ", "
        # Add newlines to format nicely
        if i%32 == 31:
            result += "\n"
    return result


# TODO: ADD include guards...

# include width and height in struct so we can have bigger/smaller images?

def processFile(filename):

    # get Name of file excluding filetype
    name = filename.split("\\")[-1].split(".")[0]

    img = Image.open(filename)

    # start include guard 
    res = "#ifndef " + name.upper() + "_H\n#define " + name.upper() + "_H\n\n"
    
    # Here we are creating the byte array containing the hex in our c file
    res = res + "unsigned char " + name + "Data[" + str(img.n_frames) + "][32][32][3] = {"

    # If it is a gif we append the hex for every frame.. if a png this will only run once
    for i in range(img.n_frames):
        img.seek(i)
        res = res + "\n" + processFrame(img)
    
    # Finish the byte array formatting
    res = res + "};"

    # Here we make a struct using the data(byte array) from above and the number of frames it contains
    res = res + "\nImage " + name + " = {" + str(img.n_frames) + ", " + name + "Data};"

    # end include guard
    res = res + "\n#endif"

    # We then write all of this to its own header file
    f = open(CIPath + name + ".h", "w")
    f.write(res)
    f.close()
    print("file created!")


def updateGraphicsHeader():
    content = ""
    # For each file in the directory we include it in graphics.h
    for file in os.listdir(CIPath):
        content = content + "#include \"" + CIPath + file + "\"\n"
    f = open(headerName, "w")
    f.write(content)
    f.close()


#        THE ACTUAL PROGRAM:

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


