# platooning
This is a repository that contains the implementation of a maneuver responsible for the reorganization of a cartain platoon whose leader desires to leave. 
It was built to run, preferably on Ubuntu 16.04, using [Omnet++ 5.5.1](https://omnetpp.org/download/old) , [Plexe-Sumo](https://github.com/michele-segata/plexe-sumo.git), [Veins](https://github.com/sommer/veins.git) and, finally, [Plexe-Veins](https://github.com/michele-segata/plexe-veins.git).

## How to setup all the files
After installing all the required software, you must copy the *__src__* and *__examples__* folders inside *__platooning__* to the Plexe-Veins directory and replace all.

## Run the maneuver
To run the maneuver, go to the Plexe-Veins directory and open it on a terminal. Then do:

>cd examples/leaveManeuver

>run -u Cmdenv -c LeaveManeuver -r0

Then SUMO will open and the simulation will be executed.

## Other features
If you want to create graph related with the maneuver, you must follow the tutorial instructions present [here](http://plexe.car2x.org/). Any other doubts may be clarified by guiding yourself by that same website.
