#!/usr/bin/python3.4
'''
    Athena Raspberry Pi Node
    For vehicle control ONLY
    2019/03/22
    Copyright (c) Cen XIN
'''

import time
import serial

NODE_NAME = 'Raspberry Pi'
UART_PORT = '/dev/ttyS0'

def main():
    '''
        Usage:
            Input operating command for driving the robotic car
        Command format:
            1. Single command:
                'w'/'s' for moving forward/backward 0.2 meter
                'a'/'d' for turning left/right 10 degree
                'q'/'e' for turning left/right while moving forward
                'x' for immediately stopping
                'c' for quitting from the program
            2. Double command:
                'w [s]'/'s [s]' for moving forward/backward 0.2 meter
                with speed [s] ranging from 0 to 9
            3. Compound command:
                'w [s] [d]'/'s [s] [d]' for moving forward/backward
                [d] meter with speed [s]
                'a [d]'/'d [d]' for turning left/right [d] degree
    '''
    ser = serial.Serial(UART_PORT, 9600, timeout=1)
    print('[Athena] UART port successfully initialized.')
    time.sleep(1)
    print('[Athena] Node %s starts!' % NODE_NAME)

    try:
        while True:
            print('> ', end='')
            cmd = input()
            if cmd == 'c':
                print('[Athena] Quit from Node %s' % NODE_NAME)
                break;
            ser.write(cmd.encode('utf-8'))
            print('[Athena] Send to UART port %s' % UART_PORT)
            time.sleep(0.1)
    except KeyboardInterrupt:
        ser.close()

if __name__ == '__main__':
    main()

