# P1-Boarding-An-Airplane
 
This is a university project, that simulates the boarding of passengers onto an airplane. The project is split into two programs; the simulation itself and a boarding method generator.
The boarding method generator creates several files, which represents the different boarding methods. A config file is used to decide how many rows, seats per row and doors there should be in the plane. The files are then used by the simulation to create the plane map.
The simulation itself takes the input of the boarding method, a config file for the different static values used in the simulation, e.g. walking speed, luggage count etc. In the program you can choose whether you want to generate an output file for the simulation, which saves the time used by every run as well as the average time used by all runs.

**Output file:** P1Draft/P1Draft/output.csv

**Boarding method config file:** BoardingMethodGeneration/BoardingMethodGeneration/config.ini

**Simulation config file:** P1Draft/P1Draft/config.ini

**Boarding method config file description:**

first_class_rows - Amount of rows that are first class (boarded first)

seats_per_row - Amount of seats placed in one row (this number takes both rows on each side of the aisle into consideration, e.g. if you   have four seats on each side of the aisle, write 8 here)

boarding_group_size - Amount of rows the generator considers as one boarding group for boarding methods front-to-back and back-to-front

upper_door - Boolean for whether the upper door should be there. 1: True  0: False

lower_door - Boolean for whether the lower door should be there. 1: True  0: False

**Simulation config file description:**

MapName - Name of the text file used for the simulation, e.g. steffenperfect.txt  This file contains the order passengers are boarded in, and is used to generate the map used by the simulation

MultipleMaps - Name of all boarding method filnes used for the simulation. These are used for running all boarding methods with one run of the program.

CrossDelay - Integer used to determine how long it should take for passengers to cross eachother on the aisle

ShuffleDelay - Integer used to determine how long it should take for passengers to shuffle past other seated passengers to get to their seat

LuggageGen - Integers used to determine how much luggage each passenger should get, as well as their chance of getting it. Written in form of "[<Luggage>,<Chance>]". So for a config with 30% chance of getting 2 pieces of luggage and 70% chance of getting 6 pieces of luggage, you write "LuggageGen = [2,30][6,70]"
 
WalkSpeedGen - Integers used to determine the walkspeed of each passenger, as well as their chance of getting it. Same method as LuggageGen, so for 50% chance of 1 walkspeed and 50% chance of 2 walkspeed you write "WalkSpeedGen = [1,50][2,50]"
 
AssignToNearestDoor - Whether passengers should be assigen to the door closest to their seat or not.

DoAllRuns - Whether it should run all boarding methods for the current configuration.
