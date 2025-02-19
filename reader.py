import serial
from os import environ
import time

try: from pn532 import PN532_SPI
except: raise Exception('Ubuntu in python env: apt-get install git, Windows cmd: winget install --id Git.Git -e --source winget , Both git bash: git clone https://github.com/soonuse/pn532-nfc-hat.git')

environ['PYGAME_HIDE_SUPPORT_PROMPT'] = '1'
try: import pygame
except: raise Exception('pip install pygame')

try: import RPi.GPIO as GPIO  
except: raise Exception('pip install RPi.GPIO')
GPIO.setwarnings(False)

def setup_rfid(rfid):
    cmdRead = 0x06 #_COMMAND_READREGISTER
    cmdWrite = 0x08 #_COMMAND_WRITEREGISTER
    CIU_RFCfg = [0x63,0x16]
    rfid.call_function(cmdWrite, response_length=1, params=CIU_RFCfg+[0xFF])
    # response = rfid.call_function(cmdRead, response_length=1, params=CIU_RFCfg)
    
    # ic, ver, rev, support = rfid.get_firmware_version()
    # print('Found PN532 with firmware version: {0}.{1}'.format(ver, rev))
    
    # Configure PN532 to communicate with MiFare cards
    rfid.SAM_configuration()
    # print('Waiting for RFID/NFC card to read from!')

def SoundAndLights(sound):
    port = serial.Serial("/dev/ttyS0", baudrate=9600, timeout=3.0)
    msg = "send" #"send" is the message the Arduino Nano
    port.write(msg.encode())
    
    time.sleep(3.6) # Delay for Arduino Nano to start lights
    sound.play()

    # Keep the thread alive while the sound plays. This is crucial!
    while pygame.mixer.get_busy():
        pygame.time.Clock().tick(10)

def main():
    rfid = PN532_SPI(debug=False, reset=22, cs=17)
    setup_rfid(rfid)
    
    pygame.mixer.init()  # Initialize the mixer
    sound = pygame.mixer.Sound('play.mp3')
    sound.set_volume(0.25)
    SoundAndLights(sound) # Initial startup test
    
    while True:
        # Check if a card is available to read
        uid = rfid.read_passive_target(timeout=0.1)
        if uid is not None:
            print('Found card')
            SoundAndLights(sound)
        # Try again if no card is available or at end of sound
        
if __name__ == "__main__":
    main()