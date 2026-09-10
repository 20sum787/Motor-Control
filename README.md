# Motor-Control
A repository containing all code files (and some description/images) of an ongoing group project with Cambridge University Spaceflight's Feed Systems team to use PID control to maintain/set a pressure ratio in a propellant valve.

Currently, I'm at the stage where I've made (using some help from Gemini) a motor position control (that is verified to work), along with working keyboard-based
safety controls. As of September 2026, my next steps are to develop this further, eventually to test with a pressure controller and a PID loop instead of a position
controller. To do this, I may explore the use of Simulink (of which I learnt the basics during a MATLAB talk and live exercise). Additionally, I have some sample
code from a colleague that allows two ESP-32s to communicate on radio, that I will examine and adapt.

It's important to note that the code here hasn't been refactored - as such, it is a little rough around the edges,
and perhaps there are better methods. However, my main priority in 1st year was to get it working, so next year,
I can focus on optimising it and cleaning it up (when I have access to the hardware again).
