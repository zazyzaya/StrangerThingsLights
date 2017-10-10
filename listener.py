import serial
import pyaudio
import wave
import struct

port = "/dev/cu.usbserial-A800eHV0"

chunk = 1024
f = wave.open(r"data/song.wav", "rb")
p = pyaudio.PyAudio()
stream = p.open(format = p.get_format_from_width(f.getsampwidth()),  
                channels = f.getnchannels(),  
                rate = f.getframerate(),  
                output = True)
                
data = f.readframes(chunk) 

ser = serial.Serial(port, 9600)

while True:
    input = ser.read()
    print(input)
    if input == b'1':
        while data: 
            stream.write(data)  
            data = f.readframes(chunk)  

    #stop stream  
        stream.stop_stream()  
        stream.close()  

    #close PyAudio  
        p.terminate()   

