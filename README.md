# EV3DevCppFirst

This is the First Test with the Eclipse framework and the compilation Toolchain for arm Compiling for the Lego-EV3. I will be updateing with documentation and news while this project is ongoing.

##Development Updates:
### 20160501 - Control Class
Currently developing a wrapper class to use motors and sensors easier

### 20161016 - Threads
[Threads2 4b10e56] Update Threadsafe motors, sensors, buttons. event management class with mutex thread safty. Still missing: thread safe getters and setters for a few vars. Expose of underlying structure still possible for people who like to tinker (I did not closed doors here!) most basic functionality now easy to access. threads still needed for Event management or fallback to linear code structure (if case statements or similar)