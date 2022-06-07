
# Removes alpha bytes
from PIL import Image
import sys
import os



CIPath = "./ConvertedImages/"
ITCPath = "./ImagesToConvert"
headerName = "graphics.h"

# This function takes in a PIL image object and returns the list of RGB hex values as a string
def processFrame(img):
    width, height = img.size
    rgb_img = img.convert("RGB")

    result = ""
    # for every pixel in the image
    for i in range(width*height):
        # for every color in each picture (r/g/b)       the last part just converts index i to (x, y) value
        for item in rgb_img.getpixel((i%width,i/width)):
            # Add the hex value to the string
            result = result + "0x" + hex(item)[2:].zfill(2) + ", "
        # Add newlines to format nicely
        if i%width == width-1:
            result += "\n"
    return result


# TODO: ADD include guards...

# include width and height in struct so we can have bigger/smaller images?

def processImageFile(filename):

    # get Name of file excluding filetype
    name = filename.split("\\")[-1].split(".")[0]

    img = Image.open(filename)
    width, height = img.size
    # start include guard 
    res = "#ifndef " + name.upper() + "_H\n#define " + name.upper() + "_H\n\n"
    
    # Here we are creating the byte array containing the hex in our c file
    res = res + "unsigned char " + name + "Data[" + str(img.n_frames) + "][" + str(height) + "][" + str(width) + "][3] = {"

    # If it is a gif we append the hex for every frame.. if a png this will only run once
    for i in range(img.n_frames):
        img.seek(i)
        res = res + "\n" + processFrame(img)
    
    # Finish the byte array formatting
    res = res + "};"

    # Here we make a struct using the data(byte array) from above and the number of frames it contains
    res = res + "\nImage " + name + " = {" + str(img.n_frames) + ", " + str(height) + ", "  + str(width) + ", " + name + "Data};"

    # end include guard
    res = res + "\n#endif"

    # We then write all of this to its own header file
    f = open(CIPath + name + ".h", "w")
    f.write(res)
    f.close()
    print("file created!")



# get char width and height from file name 
def processFontFile(filename):

    # get Name of file excluding filetype
    name = filename.split("\\")[-1].split(".")[0]

    img = Image.open(filename)
    width, height = img.size
    # start include guard 
    res = "#ifndef " + name.upper() + "_H\n#define " + name.upper() + "_H\n\n"
    
    # Here we are creating the byte array containing the hex in our c file
    res = res + "unsigned char " + name + "Data[" + str(img.n_frames) + "][" + str(height) + "][" + str(width) + "][3] = {"

    # If it is a gif we append the hex for every frame.. if a png this will only run once
    for i in range(img.n_frames):
        img.seek(i)
        res = res + "\n" + processFrame(img)
    
    # Finish the byte array formatting
    res = res + "};"

    # Here we make a struct using the data(byte array) from above and the number of frames it contains
    res = res + "\nFont " + name + " = {" + str(img.n_frames) + ", " + str(height) + ", "  + str(width) + ", " + charHeight + ", "  + charWidth  + ", " + name + "Data};"

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



# get path from command line argument if available
# if not available prompt the user for one
if(len(sys.argv) > 1):
    filename = sys.argv[1]
else:
    filename = input('Enter file or folder name:')

# if the user doesnt provide a path, just assume the default directory
if filename=="":
    filename = ITCPath

# #checks if path is a file
if(os.path.isfile(filename)):
    processImageFile(filename)
    updateGraphicsHeader()

# #checks if path is a directory
if(os.path.isdir(filename)):
    for file in os.listdir(filename):
        processImageFile(filename + "\\" + file)
    updateGraphicsHeader()


