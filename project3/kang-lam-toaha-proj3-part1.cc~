/*
 *  blobs.cc
 * 
 *  Group 12: Jennie Kang, Edmund Lam, Jamila Toaha
 *
 *  Project 3: Part 1 - Blobs
 *  
 * 
 *
 *  The robot hunts for a beacon with a rectangular 
 *  red target: It searches for red blobs (ignores noises), navigates towards the target with tracking. And while it moves toward 
 *  the robot, it uses Proportional Control, and stops 1 meter away from the target. 
 * 
 * Note: the area of the blob from 1 meter away was manually tested for and measured by placing robot 1 meter away from target
 * - The area from 1 meter away ranges between 24000 and 25000 pixels. 
 *   When the area approaches that amount, the robot is set to stop.
 * 
 * Logic: The robot sets target only if the blob meets the dimension/ ratio requirement ~1:1. 
 * Derived using top, bottom, left, right values of bounding box of blob provided by the Blobfinder Proxy
 *
 * Proportional Control: Once target set, robot makes its way over using speed adjusted with Proportional Control 
 * (maximum speed: .5, minimum speed: .1)
 * 
 * Deals with various edge cases: 
 * - Robot is considered stuck in 2 cases. 
 *   	1 - When it bumps into noise 
 * 	2 - Or the robot is spinning around for a long time without finding a target blob
 *   	In these cases, robot is set up to move backwards with rotation for 2 seconds, and then move forward for 4 seconds
 * - Sometimes noise interferes with robot from proceeding forward. Adjusted by allowing robot to move forward when the x  
 *   coordinate of blob is in the range between 152 and 164.
 * 
 * Various other things taken into account. Details in code below.
 ( 
 * Dates: 10/26/20 - 11/3/20
 *
 * 
 * 
 * Original documentation: The beginnings of a controller that uses the blobfinder.
 *  Created:     Simon Parsons
 *  Date:        7th October 2011
 *  
 */


#include <iostream>
#include <cstdlib>
#include <libplayerc++/playerc++.h>
#include <stdint.h>



//=== Creates values for x and y values for isStuck function
double xState [3] = {};			// xState: {xPreviousPrevious, xPrevious, xCurrent}
double yState [3] = {};			// yState: {yPreviousPrevious, yPrevious, yCurrent}
double yawState [3] = {}; 		// yawState: {yawPreviousPrevious, yawPrevious, yawCurrent}
int isStuckTimer = 0;
bool isStuckCount = 0 ; //1 corresonds to being stuck for 3 cycles, 2 corresponds to being stuck for 6 cycles

//=== Function determines if robot is stuck by comparing the last 3 states the robot is in
// Goal: to compare x, y, and yaw values from 3 consecutive cycles
bool isStuck(double xCurrent, double yCurrent, double yawCurrent){

  // The follow updates State arrays, to contain the 3 most recent x and y coordinates and yaw orientations

  xState[0] = xState[1];
  xState[1] = xState[2];
  xState[2] = xCurrent;

  yState[0] = yState[1];
  yState[1] = yState[2];
  yState[2] = yCurrent;

  yawState[0] = yawState[1];
  yawState[1] = yawState[2];
  yawState[2] = yawCurrent;

  // If the values from the last 3 cycles are the same across the x States, the y States, and the yaw States for more than one second
  if( xState[0] == xState [1] && xState[1] == xState[2] &&
      yState[0] == yState [1] && yState[1] == yState[2] 
     && yawState[0] == yawState[1] && yawState[1] == yawState[2]
)
	return true;
 
	return false;
}





int main(int argc, char *argv[])
{  
	using namespace PlayerCc;  

	//=== Set up proxies
	PlayerClient    robot("localhost");  
	BumperProxy     bp(&robot,0);  
	Position2dProxy pp(&robot,0);
	PlayerClient    blobfinder("localhost");
	BlobfinderProxy bf(&blobfinder,0);

 	//=== Variables

  	player_blobfinder_data_t  blobList;   // Structure holding the blobs found
  	player_blobfinder_blob_t* theBlobs;   // A point to a list of blobs
  	player_blobfinder_blob_t  myBlob;     // A single blob


	double xPosRobot, yPosRobot, yawRobot; // Keep track of robot odemetry to get an idea of physical distance
 
  	int threshold = 120; // 120 min blob size we care about 
	double threshholdUpper = 1.4; // **This threshold corresponds with the desired ratio (~1:1) of the dimensions of the
    				      // blob. Takes into account, viewing it at an angle
	double threshholdLower = .9;
	bool isSet; 	 // When program locks onto a blob
	
  	// Allow the program to take charge of the motors (take care now)
  	pp.SetMotorEnable(true);

	bool flag = false;

	int trackYawCycles = 0; // **Measured in degrees. Keeps track of how many cycles of rotation robot completes without moving. 
				//  (trackYawCycles/360) <- 1.25 cycles is enough to know that it didn't find anything of interest
	bool arrived = false;
	bool foundBlob = false; // once we found a matching blob, keeping track of this will help us avoid noise;
	bool foundBlobLastX;    // this will take compare last position of blob on screen and try to rotate	if
			        // blob were to be removed from screen
	bool stuckResult= false;

  	// === Control loop
	while(true)
	{

	// === Read from the proxies.
	robot.Read();

	xPosRobot = pp.GetXPos();
	yPosRobot = pp.GetYPos();
	yawRobot = pp.GetYaw();
	std::cout << "Robot X Position: " << xPosRobot << std::endl;
	std::cout << "Robot Y Position: " << yPosRobot << std::endl;
	std::cout << "Robot Yaw: " << yawRobot << std::endl;

	double turnrate, speed;

	// ** Stuck Result is only changed if bumper is pressed, otherwise, conflicts with code
	//if (stuckResult) isStuckCount++;

    			std::cout << "isStuckResult: " << stuckResult  << std::endl; 
	
		//=== Checks if robot is stuck (2 cases: 1- bumps into noise or 2- spinning around for 
		// too long without detecting viable blob. Note: it is not considered stuck if it has arrived). 
		// If it is stuck will backup, and turn 30 degrees

		if( (stuckResult && arrived == false) || isStuckTimer > 0 ) {
		
		 	//** Robot backs up for 2 seconds while rotating itself, and moves forwards for 4 seconds- 
			//	takes into account that there's no bumpers on the back, hence only 2 seconds spent backing up
			// when 4 seconds is up, it sets itself as unstuck, so it can restart scanning for blobs
			// and continue with rest of while loop
			if (isStuckTimer++ < 20 ){
				speed = -.3;
				turnrate = dtor(30); // make it rotate 60 degrees
			} else if (isStuckTimer >= 20 && isStuckTimer <60 ){
		 	 	speed = .3;	
				turnrate = 0;	
			} 
			else {
				isStuckTimer = 0;  //resets timer
				trackYawCycles = 0; // this takes into account when robot is spinning 
							// in place for too long (more than 1 cycle . When dones backing up,
							// trackYawCycle is set back to acceptable value 
							// of less than 1
				stuckResult = false;
			}
				

		
    			std::cout << "isStuck speed" << speed  << std::endl;
   			std::cout << "isStuck yaw" << yawRobot  << std::endl;
  		    	std::cout << "isStuckTimer" << isStuckTimer << ","  << std::endl;

			
			pp.SetSpeed(speed, turnrate); // need to set this, so robot can skip the rest of this cycle

			// ** do not proceed with the rest of this loop since robot is stuck
			continue;
		}


		isStuckTimer = 0;
		stuckResult= 0;
		turnrate = 0; 
		speed = 0;

	
		// === Read from the proxies
		robot.Read();
		blobfinder.Read();
		
		// === We only want to drive if the bumpers are not pressed
		if ((bp[0] == 0)&&(bp[1] == 0))
		{
		//Look for Blobs
			if (bf.GetCount() > 0){
				std::cout << std::endl;
				std::cout << bf.GetCount() << " Blobs found!" << std::endl;
			

					isSet = false; //only true if myBlob points to a blob down below.
					//Look at each and every blob, look for largest area

					int indexAreaLargest= 0;
					int areaLargest = bf.GetBlob(0).area;

					//=== for loop goes through all detected blobs on the camera at any given cycle
		    			for(int i = 0; i < bf.GetCount(); i++){
		     				//std::cout << "Id: "    << bf.GetBlob(i).id    << std::endl;//Does not work atm
		      				//std::cout << "Color: " << (short)bf.GetBlob(i).color << std::endl; //Not important atm
		      				std::cout << "Area: "  << bf.GetBlob(i).area  << std::endl;
		      				std::cout << "X: "     << bf.GetBlob(i).x     << std::endl;
		      				std::cout << "Y: "     << bf.GetBlob(i).y     << std::endl;
						std::cout << "Right, Left, Top, Bottom: " 
							<< bf.GetBlob(i).right << ", " 
							<< bf.GetBlob(i).left << ", " 
							<< bf.GetBlob(i).top << ", " 
							<< bf.GetBlob(i).bottom <<", " << bf.GetBlob(i).y    
							 << std::endl;

		      				std::cout << std::endl;
						
						// Compare and see which blob matches ratio
						// Compare blob to threshold first, speeds up loop

						// Compute dimensions and ratio of blob
						double rightLeftDimension = bf.GetBlob(i).right - bf.GetBlob(i).left;
						double bottomTopDimension = bf.GetBlob(i).bottom - bf.GetBlob(i).top;
						double blobRatio = rightLeftDimension/ bottomTopDimension;
						std::cout << "Blob ratio, Bottom-top, Right-top: " << blobRatio << "," << rightLeftDimension << "," << bottomTopDimension << ","  << std::endl;	
	
						//=== ** Only allow blob that fits certain dimensions to be attached
						//also the right, left, bottom, and top portion of blob can't touch edge of screen
						if(blobRatio >= threshholdLower &&
						  blobRatio <= threshholdUpper && bf.GetBlob(i).left > 10 && bf.GetBlob(i).right < 310 && bf.GetBlob(i).top > 10 && bf.GetBlob(i).bottom < 230)
						{
							//Note: We don't make use of this at the moment. It was our initial
							// code before we figured out, dimensions was a better measurment
							// if an area value already exists, compares the value to the 
							// current blobs area. If the current blob area is larger, 
							// area value gets replaced.
							//if it doesn't already exist, will replace it with default value
							if (bf.GetBlob(i).area > areaLargest ) 
							{
								areaLargest = bf.GetBlob(i).area;
								indexAreaLargest = i;
							}
							std::cout << "Area Largest, index: " << areaLargest << "," << indexAreaLargest << std::endl;


							myBlob = bf.GetBlob(i);
							arrived = true;
							foundBlob = true;
							foundBlobLastX = bf.GetBlob(i).x; // this is so blob rotates in right direction if moved;				
							isSet = true;
							trackYawCycles = 0; //reset this, so we can keep track if the robot is stuck in the future;
						}
						//=== None of the blobs we see pass threshold!
						if(isSet == false)
						{
							arrived = false;
							//std::cout << "No viable blobs in sight, scanning!" <<std::endl;
							//we need to scan for our blob!
							speed = 0;

							// Blob was found in the past, but not now. The purpose of this is to enable
							// further tracking when blob is off view. 
							// Already set up otherwise to track while on screen.
							if (foundBlob) {
								if(foundBlobLastX > 160) // to the right, turn to right
									turnrate = dtor(-30);
								else
									turnrate = dtor(30);
							}
							else 
								turnrate = dtor(30);

							trackYawCycles += 30;
							std::cout << "Track Yaw Cycles" << trackYawCycles <<std::endl;
							// === If robot rotates more than 1 cycle, it hasn't found anything viable within it's field of view, we know it's view is being blocked, so we'll have it move
							if((double)trackYawCycles / 360 > 1.25) {
								if (trackYawCycles > 5000) {
										trackYawCycles = 0;
								} else if(bf.GetBlob(i).x != 160) {
									stuckResult = true;
								}

							} 

						}
					}//=== End of for loop, suitable blob found hopefully
					
					if( isSet == true) {
						trackYawCycles = 0;
					}

					//this sets the blob to the blob with the largest area, not made use of as of now
					//myBlob = bf.GetBlob(indexAreaLargest);
		    			std::cout << "---------------------------------------------------" <<std::endl;
			

				//===== Now that we have our target, let's do something about it ====
				if(isSet){


					// Had issue where robot was overshooting / undershooting simultaneously.
					// So accounted for that by allowing blob to proceed forward if myBlob.x was within range. The robot does adjust itself when it becomes out of range, in which time speed is 0, but still has proportional control
					// This range also helps robot ignore noise and move forward
					std::cout << "myBlob.x: " << myBlob.x << std::endl;
					if(myBlob.x == 160) {//Are we facing the Blob?
						//Yes!
						speed = 0.1 +  0.4 * ((double) 1 - (double)myBlob.area/24000); //proportional control version 2 //1.1
					}
					else if(myBlob.x >= 152 && myBlob.x <= 166)
					{
						std::cout << "approaching blob" << std::endl;
											
						//speed = 0.1 + (100 / (double)myBlob.area); //proportional control
						speed = 0.1 +  0.4 * ((double) 1 - (double)myBlob.area/24000); //proportional control version 2 //1.1
								//maximum speed: .5, minimum speed: .1
						
						// If the blob reaches this amount of area which measured beforehand, 
						// we know that it is 1 meter awa meter away, so stop robot.
						if(myBlob.area >= 24000 && myBlob.area <= 25000) {
							speed = 0;
							std::cout << "FINISH LINE: we have reached the target blob" << std::endl;
						}
						turnrate = 0;
					} else if(myBlob.x > 165)
					{ //Blob is to our left!
						speed = 0;
						turnrate = dtor(-1) ;
					} else if (myBlob.x < 153)
					{ //Blob is to our right!
						speed = 0;
						turnrate = dtor(1);
					} else {
						//We don't see myBlob?!
						
					}
				} else {
					//We're close enough
					//Do nothing
					std::cout << "we're done!" << std::endl;
					//speed = 0;
					//turnrate = 0;
				}
		  	}
			else //=== We see no blobs
			{
				//start scanning for blobs by spinning.
				//If there's no blobs then we just spin forever I guess.
				speed = 0;

	      			// Here the blobfinder doesn't see any blobs
				turnrate = dtor(30);
			}
		}
		//==== If bumpers are pressed, do nothing, call the isStuck function to get out of any 
		else{
			speed = 0;
			turnrate = 0;
			stuckResult = isStuck(xPosRobot, yPosRobot, yawRobot); //this starts of the stuckResult conditional
			isStuckTimer++;

		}

		// ==== What are we doing?
		//std::cout << pp << std::endl;
		std::cout << "Speed: " << speed << std::endl;
		std::cout << "Turn rate: " << turnrate << std::endl << std::endl;
		// For safety, in the absence of a controller, do nothing.
		//speed = 0;
		//turnrate = 0;
		pp.SetSpeed(speed, turnrate);
	
	} // end of while(true) loop
}
