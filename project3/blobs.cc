/*
 *  blobs.cc
 * 
 *  Group 12: Jennie Kang, Edmund Lam, Jamila Toaha
 *
 *  Project 3: Part 1 - Blobs
 *  
 * 
 *
 *  The beginnings of a controller that uses the blobfinder. The robot hunts for a beacon with a rectangular red target. It searches for red blobs, navigates towards the target with tracking. And while it moves toward the robot, it uses Proportional Control.
 *
 * Dates: 10/26/20 - 11/2/20
 *  Created:     Simon Parsons
 *  Date:        7th October 2011
 *  
 */


#include <iostream>
#include <cstdlib>
#include <libplayerc++/playerc++.h>
#include <stdint.h>

int main(int argc, char *argv[])
{  
	using namespace PlayerCc;  

	// Set up proxies
	PlayerClient    robot("localhost");  
	BumperProxy     bp(&robot,0);  
	Position2dProxy pp(&robot,0);
	PlayerClient    blobfinder("localhost");
	BlobfinderProxy bf(&blobfinder,0);

 	// Variables

  	player_blobfinder_data_t  blobList;   // Structure holding the blobs found
  	player_blobfinder_blob_t* theBlobs;   // A point to a list of blobs
  	player_blobfinder_blob_t  myBlob;     // A single blob
  	int threshold = 120; // 120 min blob size we care about
	bool isSet; //I don't feel like setting myBlob to null
	
  	// Allow the program to take charge of the motors (take care now)
  	pp.SetMotorEnable(true);

	bool flag = false;

  	// Control loop
	while(true)
	{
	
		
	
		double turnrate = 0,speed = 0;
	
		// Read from the proxies
		robot.Read();
		blobfinder.Read();
		
		// We only want to drive if the bumpers are not pressed
		if ((bp[0] == 0)&&(bp[1] == 0))
		{
		//Look for Blobs
			if (bf.GetCount() > 0){
				std::cout << std::endl;
				std::cout << bf.GetCount() << " Blobs found!" << std::endl;
			
				//We only see one blob. See if it's larger than threshold
				if(bf.GetCount() == 1 && bf.GetBlob(0).area > threshold)
				{
					std::cout << "Setting myBlob to it." << std::endl;
					std::cout << "Area: " << bf.GetBlob(0).area <<std::endl;
					std::cout << "X: " << bf.GetBlob(0).x <<std::endl;
					std::cout << "Y: " << bf.GetBlob(0).y <<std::endl;
					std::cout << std::endl;
					myBlob = bf.GetBlob(0); //Set our target

				}
				//Handle more than one blob. Find the right one
				else {
					isSet = false; //only true if myBlob points to a blob down below.
					//Look at each and every blob, look for largest area

					int indexAreaLargest= 0;
					int areaLargest = bf.GetBlob(0).area;

		    			for(int i = 0; i < bf.GetCount(); i++){
		     				//std::cout << "Id: "    << bf.GetBlob(i).id    << std::endl;//Does not work atm
		      				//std::cout << "Color: " << (short)bf.GetBlob(i).color << std::endl; //Not important atm
		      				std::cout << "Area: "  << bf.GetBlob(i).area  << std::endl;
		      				std::cout << "X: "     << bf.GetBlob(i).x     << std::endl;
		      				std::cout << "Y: "     << bf.GetBlob(i).y     << std::endl;
						std::cout << "Right, Left, Top, Bottom: " 
							<< bf.GetBlob(i).right << "," 
							<< bf.GetBlob(i).left << "," 
							<< bf.GetBlob(i).top << "," 
							<< bf.GetBlob(i).bottom << bf.GetBlob(i).y    
							 << std::endl;

		      				std::cout << std::endl;
						
						//compare and see who is bigger. myBlob will be set to that by the end of this loop
						//Compare blob to threshold first, speeds up loop
						if(bf.GetBlob(i).area > threshold){
							//if an area value already exists, compares the value to the current blobs area. If the current blob area is larger, area value gets replaced.
							//if it doesn't already exist, will replace it with default value
							if (bf.GetBlob(i).area > areaLargest ) {
								areaLargest = bf.GetBlob(i).area;
								indexAreaLargest = i;
							}
							std::cout << "area Largest, index" << areaLargest << "," << indexAreaLargest << std::endl;
							//std::cout << "Comparing blobs" << std::endl;
							if(bf.GetBlob(i).area > bf.GetBlob(i+1).area){ //First blob is bigger
								//std::cout << "myyBlob set1" << std::endl;//1st Blob is bigger.
								myBlob = bf.GetBlob(i);
								isSet = true;
							} else {
								//std::cout << "myBlob set2" << std::endl;
								myBlob = bf.GetBlob(i+1); //2nd Blob is bigger.
								isSet = true;
							}
						}
						//None of the blobs we see pass threshold!
						if(isSet == false)
						{
							std::cout << "No viable blobs in sight, scanning!" <<std::endl;
							//we need to scan for our blob!
							speed = 0;
							turnrate = 0.1;
						}
					}//End of for loop, biggest blob found hopefully
					myBlob = bf.GetBlob(indexAreaLargest);
		    			std::cout << "---------------------------------------------------" <<std::endl;
				}

				//Now that we have our target, let's do something about it
				if(true){
					if(myBlob.x == 160)
					{//Are we facing the Blob?
						//Yes!
						std::cout << "approaching blob" << std::endl;
						speed = 0.1 - (100 / (double)myBlob.area); //Janky proportional control
						turnrate = 0;
					} else if(myBlob.x > 160)
					{ //Blob is to our left!
						speed = 0;
						turnrate = dtor(-1) ;
					} else if (myBlob.x < 160)
					{ //Blob is to our right!
						speed = 0;
						turnrate = dtor(1);
					} else {
						//We don't see myBlob?!
						//I honestly don't know if you can even reach here
					}
				} else {
					//We're close enough
					//Do nothing
					std::cout << "we're done!" << std::endl;
					speed = 0;
					turnrate = 0;
				}
		  	}
			else //We see no blobs
			{
				//start scanning for blobs by spinning.
				//If there's no blobs then we just spin forever I guess.
				//speed = 0;
				//turnrate = 0.1;

	      			// Here the blobfinder doesn't see any blobs
				turnrate = dtor(30);
			}
		}
		// If bumpers are pressed, do nothing.
		else{
			speed = 0;
			turnrate = 0;
		}
		// What are we doing?
		//std::cout << pp << std::endl;
		std::cout << "Speed: " << speed << std::endl;
		std::cout << "Turn rate: " << turnrate << std::endl << std::endl;
		// For safety, in the absence of a controller, do nothing.
		//speed = 0;
		//turnrate = 0;
		pp.SetSpeed(speed, turnrate);
	
	} // end of while(true) loop
}
