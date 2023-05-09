/*
MIT License

Copyright (c) 2020 Steffen S.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#ifndef MBED_SERVO_H
#define MBED_SERVO_H

#include <mbed.h>

/**
 * The following parameters are servo specific but should work with most servos. They can be changed without modifing this header using the following code
 *      #undef SERVO_REFRESH_RATE_US
 *      #define SERVO_REFRESH_RATE_US [servo refresh rate in us]
 *      #undef SERVO_MIN_PULSE
 *      #define SERVO_MIN_PULSE [pulse width for -90 degrees in us]
 *      #undef SERVO_MAX_PULSE
 *      #define SERVO_MAX_PULSE [pulse width for +90 degrees in us]
 */

#define SERVO_REFRESH_RATE_US 20000 // 50Hz
#define SERVO_MIN_PULSE 500
#define SERVO_MAX_PULSE  2400


#define SERVO_MIN_DEGREE  -90
#define SERVO_MAX_DEGREE  90

#define SERVO_POS_CENTER  0
#define SERVO_POS_MIN  SERVO_MIN_DEGREE
#define SERVO_POS_MAX  SERVO_MAX_DEGREE

/**
 * Driver for a servo motor
 */
class Servo {
    public:
        /**
         * Constructs new servo object for servo connected to pin
         * @param pin the pin the servo is connected to
         */
        Servo(PinName pin);

        /**
         * Enables the servo and moves it to the initial position
         * @param startPos the initial position of the servo in degrees
         */ 
        void attach(int startPos);

        /**
         * Disables the servo
         */ 
        void detach();

        /**
         * Sets the position of the servo
         * @param pos the position of the servo in degrees
         */
        void write(int pos);

        /**
         * Gets the current position of the servo
         * @return the current servo position in degrees
         */
        int read();

    private:
        DigitalOut pulsePin;
        Ticker refreshTicker;
        Timeout pulseTimer;
        uint16_t currentPulse;

        void startPulse();
        void endPulse();
};

#endif
