import cv2
import numpy as np
import serial
from time import sleep

port = '/dev/ttyACM1'

ser = serial.Serial(port, 9600, timeout=1)
ser.flush()
    
x = -1
w = -1
h = -1
y = -1
cap = cv2.VideoCapture(0)
line = 0

while(True):

        
    _, frame = cap.read()
    hsv_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    
    low_red = np.array([161, 155, 84])
    high_red = np.array([179, 255, 255])
    red_mask = cv2.inRange(hsv_frame, low_red, high_red)
    
    
    contours,_ = cv2.findContours(red_mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    contours = sorted(contours, key=lambda x:cv2.contourArea(x), reverse=True)
    if ser.in_waiting > 0:
        line = ser.readline().decode('ascii').rstrip()
        print(line)
        
    for cnt in contours:
        (x, y, w, h) = cv2.boundingRect(cnt)
        
        cv2.rectangle(frame, (x,y), (x+w, y+h), (0,255,0),2)
        cv2.line(frame, (int((w/2)+x), 0), (int((w/2)+x), 400), (0,255,0), 2)
        cv2.line(frame, (340, 0), (340, 400), (255,0,0), 2)
        cv2.line(frame, (260, 0), (260, 400), (255,0,0), 2)
        if line == '1':
            if int((w/2)+x) < 260:
                ser.write(b'd\n')
                print("right")
            elif int((w/2)+x) > 340:
                ser.write(b'a\n')
                print("left")
            elif (int(w)*int(h)) > 400:
                ser.write(b's\n')
                print("stop")
            elif (int(w)*int(h)) < 50:
                ser.write(b's\n')
                print("stop")
            elif (int(w)*int(h)) >= 50 and (int(w)*int(h)) < 400:
                ser.write(b'w\n')
                print("stop")
            sleep(0.1)
        break
    
    cv2.imshow("Frame",frame)
    #cv2.imshow("",red_mask)
    key = cv2.waitKey(1)
    
    if key == 27:
        break
    
    
    
        
    
        

    
cap.release()
cv2.destroyAllWindows()