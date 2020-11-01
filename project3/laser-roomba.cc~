/**
 * laser-roomba.cc
 *  
 *  Group 12: Jennie Kang, Edmund Lam, Jamila Toaha
 *  
 *  Project 3 - Part 2 - Laser Navigation 
 *  
 *  This program uses laser sensor to help a robot avoid obstacles.
 *
 *
 * 
 *  Date: 10/27/2020
 *
 * Sample code for a robot that has two front bumpers and a laser scanner.
 * Suitable for use with the roomba.
 *
 * Written by: Simon Parsons
 * Date:       11th October 2011
 *  
 **/


#include <iostream>
#include <libplayerc++/playerc++.h>
using namespace PlayerCc;  

  double maxRange, minLeft, minRight, range, bearing, middleScanLine;
	

  double leftRange, rightRange;
  int points;


/**
 * Function headers
 *
 **/


void printLaserData();
void printRobotData(BumperProxy& bp);

/**
 * main()
 *
 **/
int main(int argc, char *argv[])
{  
	

  // Variables
  int counter = 0;
  double speed = 0;            // How fast do we want the robot to go forwards?
  double turnrate = 0;         // How fast do we want the robot to turn?
  player_laser_data laser; // For handling laser data



  // Set up proxies. These are the names we will use to connect to 
  // the interface to the robot.
  PlayerClient    robot("localhost");  
  BumperProxy     bp(&robot,0);  
  Position2dProxy pp(&robot,0);
  LaserProxy      sp (&robot, 0);

  // Allow the program to take charge of the motors (take care now)
  pp.SetMotorEnable(true);

  // Main control loop

	int counterForCentering1 = 0;
	int counterForCentering2 = 0;
	int degrees = 0;
  while(true) 
    {    
      // Update information from the robot.
      robot.Read();



      // Print information about the laser. Check the counter first to stop
      // problems on startup
      // Note: the values have been adjusted for 2 decimals. Otherwise, it seems to miss the matching values
      if(counter > 2){
  	  maxRange  = (double) ((int)(sp.GetMaxRange()*10/1))/10;
	  minLeft   = (double) ((int)(sp.MinLeft()*10/1))/10;
	  minRight  = (double) ((int)(sp.MinRight()*10/1))/10;
	  points    = (double) ((int)(sp.GetCount()*10/1))/10;
	  range     = (double) ((int)(sp.GetRange(5)*10/1))/10;
     middleScanLine = (double) ((int)(sp.GetRange(180)*10/1))/10;
	  bearing   = (double) ((int)(sp.GetBearing(5)*10/1))/10;
	  leftRange = (double) ((int)(sp.GetRange(360)*10/1))/10; //left most range
   	  rightRange =(double) ((int)(sp.GetRange(0)*10/1))/10;  //right most range
	printLaserData();
      }

      // Print data on the robot to the terminal
      printRobotData(bp);
      
           // If either bumper is pressed, stop. Otherwise just go forwards

      if(bp[0] || bp[1]){
	speed= 0;
	turnrate= 0;
      } 
  
	//turn when robot reaches first wall && middleScanLine < 1
		
	// Adjust robot to center of lane if not centered - part 1/2 - if robot is off center, it heads towards the center
	else if(leftRange + rightRange <= 2.8 && maxRange > 2 && (minLeft > minRight || minLeft < minRight) ) {
		//counterForCentering1 = 0;

			// Momentarily stops robot for 1 second, while it shifts it's rotation, then moves forward,
			if(counterForCentering1 == 0) degrees = minLeft > minRight ? 20 : -20;

			if(counterForCentering1++ < 10){ 
				speed= 0;
                       		turnrate = dtor(degrees);
			} 
			else {
				turnrate = 0;
				speed = .3;
			}

		
	} 

	// Adjust robot to center of lane if not centered - part 2/2 - when it reaches the center, it rotates in opposite direction and alternates as needed. For example, if part 1 it rotated 20 degrees, in part 2 it will rotate -20 degrees.
	else if( leftRange + rightRange <= 2.8 && minLeft == minRight) {

			if(counterForCentering2++ < 10) {
				speed= 0;
				turnrate = dtor(degrees * -1); // Straightens out robot
			}
			else {
				counterForCentering1 = 0; // This is reset so part 1 can adjust towards the center, when not even. 
							  // If it's even, it will continue going straight ahead. 
				turnrate = 0;
				speed = .3;
			}
	
	}


 else 
	

//2nd try based on Jennie's code
	if (leftRange + rightRange > 2.5 ) {
          counterForCentering1 = 0
	   //speed= 0;
           turnrate = 0;
		//if the left side is larger than the right side/ turn to the left
		if(leftRange > rightRange){
			speed=.3;
        		turnrate = 0;
        		if(minLeft > 2) {
				turnrate = dtor(10);
			} else if(minRight > 0.45) {
				turnrate = dtor(10);
			}
		} else { // if right side is larger than the left side, turn to the right
			if(minRight > 2) {
				turnrate = dtor(-20);
			}
			else if(minLeft > 1.5) {
				turnrate = dtor(-20);
			}
		}
	} else
/*
 //1st try based on counter	
if(counter < 412) {
		speed=.3;
        	turnrate = 0;
        	if(sp.MinLeft() > 2) {
			turnrate = dtor(10);
		}
		else if(sp.MinRight() > 0.45) {
			turnrate = dtor(10);
		}
	}
	//go forward
	if(counter > 412) {
		speed = 0.3;
		turnrate = 0;
	}
	//turn when robot reaches second wall
	if(counter > 570) {
		if(sp.MinRight() > 2) {
			turnrate = dtor(-20);
		}
		else if(sp.MinLeft() > 1.5) {
			turnrate = dtor(-20);
		}
	}
	//go forward
	if(counter > 670) {
		speed = .3;
		turnrate = 0;
	}
	//turn when robot reaches third wall
	if(counter > 840) {
		if(sp.MinLeft() > 1.8) {
			turnrate = dtor(23);
		}
		else if(sp.MinRight() > 1.4) {
			turnrate = dtor(22);
		}
	}
	//go forward
	if(counter > 950) {
		speed = 0.3;
		turnrate = 0;
	}

*/

	//stop as close as possible to last wall without hitting it
	// this basically checks the max values of the left-most and right-most points, adds up value, and makes sure value is between 2.3 and 2.5. Then it makes sure it's close to wall as possible before stopping. In this case .5 meters away.
	if(leftRange + rightRange <= 2.5 && leftRange + rightRange > 2.3 && middleScanLine <= .5) {
		speed = 0;
		turnrate = 0;
			break;
	} else {
		//keep going forward
		counterForCentering2 = 0;
		counterForCentering1 = 0;
		speed = 0.3;
		turnrate = 0;
	}
    

      // What are we doing?
      std::cout << "Speed: " << speed << std::endl;      
      std::cout << "Turn rate: " << turnrate << std::endl;

      // Send the commands to the robot
      pp.SetSpeed(speed, turnrate);  
	
      // Count how many times we do this
      counter++;
      std::cout << "Counter: " << counter << std::endl <<std::endl;
    }
  
} // end of main()


void printLaserData()
{

  // This illustrates the things the proxy provides. These should be self-explanatory
  // if you look at the print statements, except possible GetRange(5) and GetBearing(5).
  // These allow you to access one particular reading. You might want to use this
  // to look through all the values in a given arc to find the closest object.

  std::cout << "Laser says..." << std::endl;
  std::cout << "Maximum distance I can see: " << maxRange << std::endl;
  std::cout << "Number of readings I return: " << points << std::endl;
  std::cout << "Closest thing on left: " << minLeft << std::endl;
  std::cout << "Closest thing on right: " << minRight << std::endl;
  std::cout << "Range of a single point: " << range << std::endl;
  std::cout << "Bearing of a single point: " << bearing << std::endl;

  std::cout << "Range of a middle scan line: " << middleScanLine << std::endl;
  std::cout << "Range of Left Most Point: " << leftRange << std::endl;
  std::cout << "Range of Right Most Point: " << rightRange << std::endl;

  return;
} // End of printLaserData()

/**
 *  printRobotData
 *
 * Print out data on the state of the bumpers and the current location
 * of the robot.
 *
 **/

//turn for 3 seconds
void turnInPlace(int degrees) {


}

void printRobotData(BumperProxy& bp)
{

  // Print out what the bumpers tell us:
  std::cout << "Left  bumper: " << bp[0] << std::endl;
  std::cout << "Right bumper: " << bp[1] << std::endl;

  // Might want to add odometry data here also, but then you'd have
  // to pass the position proxy to the function.

} // End of printRobotData()
