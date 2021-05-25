# ADC---Embedded-Systems-Activity-1

Introduction

For this activity, students were tasked with designing, implementing, and verifying
an angular distance measurement system using a microcontroller, an LCD, and a
potentiometer. The system is meant to use the analog to digital converter on the
microcontroller to determine the angle of the potentiometer and output that angle
along with its corresponding voltage value to the LCD.


Design

The physical system is set up such that the output lead of the potentiometer is
connected to the analog input pin (PA1) on the microcontroller, as seen Figure 3.
The potentiometer is fed +3.3V from the microcontroller; therefore, it can be
assumed that when the potentiometer is set to its minimum value the output voltage
is +3.3V, and when it is set to its maximum value the output voltage is 0V. In this
case it is said that the reference voltage, Vref, for the ADC is +3.3V. The ADC on
the microcontroller has 12 bits of resolution, so that means that there are 2^12
or 4096 steps (where a step is the smallest change discernible by the ADC). Because
Vref is known, the result of the ADC can be converted to the voltage value by
multiplying by 3.3/4096 volts/step. Similarly, after knowing the maximum angle
the potentiometer can be turned to, which in this case is 240°, the result can be
multiplied by 240/4096 degrees/step. The voltage and angle values can then be
output to the LCD.


Results

The images in the results section show the output from the LCD when the potentiometer is
turned to various angles.


Conclusion

The system works as anticipated, achieving the goals set. This activity allows
students to work with the analog to digital converter discussed in lecture.
There was a concern that the output voltage from the potentiometer may not be
totally linear, which is an assumption made when converting from the result to the
angle value. However, it appears that this is not the case, as turning the
potentiometer between 0° and 240° is accurately represented by the system.
