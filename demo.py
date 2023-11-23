import cv2
import numpy as np
from PIL import ImageGrab
import serial
import time

def capture(left, top, right, bottom):
    img = ImageGrab.grab(bbox=(left, top, right, bottom))
    img = cv2.cvtColor(np.array(img), cv2.COLOR_RGB2BGR)
    return img

com = serial.Serial('COM15', 115200, timeout=1)
com.read()

while True:
    img = capture(0, 0, 1920, 1080)
    img = cv2.resize(img, (128, 64))
    img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    _, mask = cv2.threshold(img_gray, 128, 255, cv2.THRESH_BINARY)

    mask2 = mask.flatten()
    mask2[mask2 == 255] = 1
    mask_str_list = []
    mask_str = ""

    for i in range(len(mask2)):
        mask_str = mask_str + str(mask2[i])
        if (i + 1) % 8 == 0:
            mask_str_list.append(mask_str)
            mask_str = ""

    list_hex = []
    for i in mask_str_list:
        hex_value = hex(int(i, 2))[2:].upper()
        if len(hex_value) % 2 != 0:
            hex_value = '0' + hex_value
        list_hex.append(hex_value)

    hex_str = ''.join(list_hex)
    hex_bytes = bytes.fromhex(hex_str)

    com.write(bytes.fromhex("FF"))
    com.write(hex_bytes)
    com.write(bytes.fromhex("FE"))

    time.sleep(0.2)

    cv2.imshow("screen", mask)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

com.close()
cv2.destroyAllWindows()
