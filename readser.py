import serial
import time

ser = serial.Serial('/dev/ttyACM0', 115200)
ser2 = serial.Serial('/dev/ttyACM1', 115200)

# Initialize the counter for each serial
counter_ser = 0
counter_ser2 = 0

# Time when the script started
start_time = time.time()

while True:
    try:
        ser.readline()
        counter_ser += 1

        ser2.readline()
        counter_ser2 += 1

        # If one second has passed
        if time.time() - start_time >= 1:
            # Print packet per second for each serial
            print("ser packets per second: ", counter_ser)
            print("ser2 packets per second: ", counter_ser2)

            # Reset the counters and the start time
            counter_ser = 0
            counter_ser2 = 0
            start_time = time.time()
    except Exception as ex:
        print("Keyboard Interrupt " )
        print(ex)
        break