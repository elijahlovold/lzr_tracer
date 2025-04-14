
from machine import Pin, PWM, ADC
import time

# Pin setup
laser = Pin(16, Pin.OUT)
laser.value(0)

# Servo setup
servo_h = PWM(Pin(17), freq=50)
servo_v = PWM(Pin(18), freq=50)

def set_servo(pwm, angle):
    duty = int(1638 + (angle / 180) * 819)  # 0.5ms–2.5ms pulse -> 1638–8192 (duty_u16)
    pwm.duty_u16(duty)

# Joystick setup
joy_x = ADC(0)
joy_y = ADC(1)
joy_btn = Pin(15, Pin.IN, Pin.PULL_UP)

SENSE_X = 1.0
SENSE_Y = 0.5
THRESH = 0.1

angle_x = 0.0
angle_y = 0.0
lazer_state = False

def read_axis(adc):
    val = adc.read_u16()
    norm = (val - 32768) / 32768  # range: ~[-1, 1]
    return norm if abs(norm) > THRESH else 0

prev_btn_state = 1
while True:
    btn = joy_btn.value()
    # fix
    if btn == 0 and prev_btn_state == 1:
        lazer_state = not lazer_state
        laser.value(int(lazer_state))
    prev_btn_state = btn

    delta_x = read_axis(joy_x)
    delta_y = read_axis(joy_y)

    angle_x += delta_x * SENSE_X
    angle_x = min(max(angle_x, 0), 180)

    angle_y -= delta_y * SENSE_Y
    angle_y = min(max(angle_y, 0), 180)

    set_servo(servo_h, angle_x)
    set_servo(servo_v, angle_y)

    time.sleep_ms(30)
