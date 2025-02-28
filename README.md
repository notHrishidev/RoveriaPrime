# RoveriaPrime
Design and Simulation of an Asteroid Exploration System
This project was part of the induction process of Spider, the Research and Development club of my college. The problem statement was to model a rover navigating through an asteroid
# Context:
* Joystick controlled navigation, terrain is represented by a dot matrix display
* The rover goes to the base of a volcano to collect samples. There is a safe zone at a specific point beyond which the rover should't go, marked by two red dots.
* When the rover reaches the red dots, the sample collection sequence starts.
* Given the angle of projection and speed of the rock shard flying out from the volcano, a capturing mechanism should extend to the required height (using a stepper motor) and a capsule must angle itself in the correct orientation (controlled by a servo motor)
* Once the shard is collected, water is used to cool it down by 100 degrees and the amount of water used for this is noted. This gives the specific heat of the rock sample.
* The specific heat is to be transmitted to the base station as a laser pulse beam. For this, it should be converted to a bitstream and then an LED should be made to blink in the same sequence.
#Implementation and how to view:
* The entire system is modelled in Wokwi
* The left side shows the code used by the arduino while the right top part shows the circuit.
* The right bottom part will show the Serial monitor for giving inputs.
* When the simulation starts, the angle, velocity and the temperature of the rock is asked. This is used for the calculation of height and angle of the capturing module.
* An RGB LED represents the current temperature of the rock and it needs to be cooled to a temperature represented by the colour of the LED.
* The amount of water used is controlled by the servo. The water supply is kept untill the colour reaches a specific value.
* Using the amount of water used, the specific heat capacity is calculated and converted to BCD format before sending it through the LED.
* The Wokwi project can be accessed through [this link](https://wokwi.com/projects/400419151934386177)
* The arduino sketch and the .json file for the diagram can be found in the repository.
