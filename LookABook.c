// RobotC Main
#include "EV3Servo-lib-UW.c"
#include "EV3_FileIO.c"

const int BODY = 0, ARM = 1, LIFT = 2;
const float ARM_LIMIT = 9.3 * 360 / (2 * PI * 2.0);

void set_motor(int motor0, int power) 
{
	if (motor0 == ARM)
		motor[motorA] = power;

	else if (motor0 == BODY)
		setServoSpeed(S1, 1, power); // Set offsets during callibration

	else if (motor0 == LIFT)
		setServoPosition(S1, 2, power);
}

bool move_to_colour(int colour, int & timeToBook) 
{
	bool found_colour = false;
	int current_colour = 0;

	set_motor(BODY, -100);
	const int MAXCOUNT = 5000; // To be adjusted as needed
	time1[T1] = 0;
	time1[T2] = 0;

	while (SensorValue[S2] != colour && time1[T2] < MAXCOUNT) 
	// Loop stops when colour of desired book found, or when the same colour is read for too long 
	{
		if (current_colour != SensorValue[S2])
		{
			current_colour = SensorValue[S2];
			time1[T2] = 0; //Timer resets for every new colour
		}
	}

	if (SensorValue[S2] == colour)
	{
		found_colour = true;
		time1[T2] = 0;

    	//Runs the body for a certain time to align the arm with the book
		while (time1[T2] < 105.05 / (300.0 / 2502.0)) 
		{}

		//Value of distance obtained from physically measuring the robot
		//Value of speed determined from physical testing of the robot
		//Time needed = distance / speed
	}

	else
	{
		displayString(3, "Book not found");
		timeToBook -= MAXCOUNT;
	}

	timeToBook = time1[T1];
	set_motor(BODY, -12);
	return found_colour;
}

// Reset function (move back to beginning of shelf)
void reset(int time) 
{
	set_motor(BODY, 100);
	wait1Msec(time);
	set_motor(BODY, -12);
}

void grabBook() 
{
	// Lower lift
	set_motor(LIFT, -47);

	// Move arm forward
	set_motor(ARM, -20);
	nMotorEncoder[motorA] = 0;

	while(nMotorEncoder[motorA] > -ARM_LIMIT) 
      //Negative value of ARM_LIMIT used due to negative power
	{}

	set_motor(ARM, 0);

	// Lift lift
	for (int position = -47; position <= 40; position += 2)
	{
		set_motor(LIFT, position);
	}

	// Move arm backward
	set_motor(ARM, 20);
	nMotorEncoder[motorA] = 0;

	while(nMotorEncoder[motorA] < ARM_LIMIT) 
       //Positive value of ARM_LIMIT used due to positive power
	{}
	set_motor(ARM, 0);
}

task main() 
{
	// Initialize sensors and motors
	SensorType[S1] = sensorI2CCustom9V;
	SensorType[S2] = sensorEV3_Color;
	wait1Msec(50);
	SensorMode[S2] = modeEV3Color_Color;
	wait1Msec(50);

	// Open file
	TFileHandle fin;
	bool fileOkay = openReadPC(fin, "colour_assignment.txt");
	string title = " ";
	int colour = 0;

	time1[T3] = 0;
	while (readTextPC(fin, title) && readIntPC(fin, colour))
	{
        // Waits for the enter button to be pressed and released before robot starts looking for the book 
		displayString(2, "Press enter to begin the search for the book");

		while (!getButtonPress(buttonEnter))
		{}

		while (getButtonPress(buttonEnter))
		{}

		eraseDisplay();

		int timeToBook = 0;

		if (move_to_colour(colour, timeToBook))
		{
			grabBook();
			displayString(6, "\"%s\"", title);
			displayString(7, " has been found.");
		}

		displayString(4, "It took %f seconds to find the book.", timeToBook/1000.0);
		reset(timeToBook);
	}

	// Waits for the enter button to be pressed and released before showing total time 
	while (!getButtonPress(buttonEnter))
	{}

	while (getButtonPress(buttonEnter))
	{}
	eraseDisplay();
	displayString(2, "Time taken: %.2f seconds", time1[T3] / 1000.0);
	displayString(3, "Press enter to end the program");
	
	// Waits for the enter button to be pressed and released before ending the program 
	while (!getButtonPress(buttonEnter))
	{}

	while (getButtonPress(buttonEnter))
	{}

	set_motor(BODY, -12);

	closeFilePC(fin);
}
