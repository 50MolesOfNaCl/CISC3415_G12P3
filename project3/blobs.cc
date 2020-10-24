/*
 *  blobs.cc
 * 
 *  The beginnings of a controller that uses the blobfinder.
 *
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
  	int threshold = 120; //min blob size we care about
	
  	// Allow the program to take charge of the motors (take care now)
  	pp.SetMotorEnable(true);


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
					myBlob = bf.GetBlob(0); //Set our target
				}
				//Handle more than one blob. Find the right one
				else {
					//Look at each and every blob, look for largest area
		    			for(int i = 0; i < bf.GetCount(); i++){
		     				std::cout << "Id: "    << bf.GetBlob(i).id    << std::endl;
		      				std::cout << "Color: " << (short)bf.GetBlob(i).color << std::endl;
		      				std::cout << "Area: "  << bf.GetBlob(i).area  << std::endl;
		      				std::cout << "X: "     << bf.GetBlob(i).x     << std::endl;
		      				std::cout << "Y: "     << bf.GetBlob(i).y     << std::endl;
		      				std::cout << std::endl;
						
						//compare and see who is bigger. myBlob will be set to that by the end of this loop
						//Compare blob to threshold first, speeds up loop
						if(bf.GetBlob(i).area > threshold){
							//std::cout << "Comparing blobs" << std::endl;
							if(bf.GetBlob(i).area > bf.GetBlob(i+1).area){ //First blob is bigger
								//std::cout << "myyBlob set1" << std::endl;//1st Blob is bigger.
								myBlob = bf.GetBlob(i);
							} else {
								//std::cout << "myBlob set2" << std::endl;
								myBlob = bf.GetBlob(i+1); //2nd Blob is bigger.
							}
						}
					}//End of for loop, biggest blob found
		    			std::cout << "---------------------------------------------------" <<std::endl;
				}
				//Now that we have our target, let's do something about it
				if(true){ //TODO: If not close to blob
					if(myBlob.x == 160)
					{//Are we facing the Blob?
						//Yes!
						std::cout << "approaching blob" << std::endl;
						speed = 1;
						turnrate = 0;
					} else if(myBlob.x > 160)
					{ //Blob is to our left!
						speed = 0;
						turnrate = -.1;
					} else
					{ //Blob is to our right!
						speed = 0;
						turnrate = 0.1;
					}
				} else {
					//We're close enough
					//Do nothing
					speed = 0;
					turnrate = 0;
				}
		  	}
			else //We see no blobs
			{
				//Do Something
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

//We need to slow down as we get closer. This will take distance to target and return the adjusted speed.
double proportionalControl(int distance)
{
	double speedP = 1; //base speed
	return speedP - (distance/100); //temp until something better comes along.
}
