# LookABook - Autonomous Book Retriever
LookABook is an autonomous book retriever which was built and designed to retrieve books off a bookshelf easily and in a timely manner. The aim of this project is to assist those who have difficulties locating specific books in a library. 

## Hardware Design
LookABook was assembled using a combination of Lego and TETRIX components. TETRIX was used for the main body, the wheels, and the servo motors controlling the wheels and rotation of the lifter. The remaining parts of the robot was mainly made up of Lego, including the color sensor, the lifter, and a motor to control linear movement of the arm. A rack and pinion mechanism was used to convert the torque generated by the motor into this linear motion. Additionally, the slider used for the lifter was designed with AutoCAD and laser-cut from a piece of wood.

## Software Design
The overall software design for LookABook is made up of two main parts - a C++ program and a RobotC program.
### C++
C++ was used to make a simple book search program, which simply prompts the user to input the books they desire. Based on these inputs, the program will look through a predefined catalogue to check if the books are available, and generate a new file containing the titles of all available books, if any, and their corresponding unique color-code, which would be found in the catalogue. This file is then downloaded by the robot and is used to retrieve the correct books from the shelf.
### RobotC
RobotC was used for the main controls of the robot - wheels of the main body, linear movement of the arm, and rotation of the lifter. The user simply has to press a button on the robot, and using the file that was generated by the C++ program, it will go through the shelf scanning for the right book based on its color-code. Once found, the arm and lifter grabs the book and goes back to the end of the shelf. The user takes the book and can repeat this whole process for as many books as they wish.


