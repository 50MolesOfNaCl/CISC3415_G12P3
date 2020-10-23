/*
 *  blobs.cc
 *
 *  Group 12: Jennie Kang, Edmund Lam, Jamila Toaha
 *
 *  Project 3 - Part 1 - Blobs
 * 
 *  Date: 10/22/2020
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
  // Allow the program to take charge of the motors (take care now)
  pp.SetMotorEnable(true);

	
  blobfinder.Read();
  //init myBlob as the first red blob we can find.
	for(int i = 0; i < bf.GetCount(); i++)
	{
		if(bf.getBlob(i).color == 0) //ERROR: has no member named GetBlob
		{
			myBlob = bf.GetBlob(i);
			break;//then we get the hell out of there.
		}
	}
  // Control loop
  while(true) 
    {    
      double turnrate, speed;

      // Read from the proxies
      robot.Read();
      blobfinder.Read();
      // We only want to drive if the bumpers are not pressed

      if ((bp[0] == 0)&&(bp[1] == 0))
	{
	  if (bf.GetCount() > 0){

	    // Here the blobfinder has found some color blobs

	    std::cout << std::endl;
	    std::cout << bf.GetCount() << " Blobs found!" << std::endl;

	    for(int i = 0; i < bf.GetCount(); i++){
	      std::cout << "Id: "    << bf.GetBlob(i).id    << std::endl;
	      std::cout << "Color: " << (short)bf.GetBlob(i).color << std::endl;
	      std::cout << "Area: "  << bf.GetBlob(i).area  << std::endl;
	      std::cout << "X: "     << bf.GetBlob(i).x     << std::endl;
	      std::cout << "Y: "     << bf.GetBlob(i).y     << std::endl;
	      std::cout << std::endl;
              }
	      

	      if(bf.GetBlob(0).x == 160) {
 	  	   if(bf.GetBlob(0).area > 10000) {
			speed = 0.3;
			turnrate = 0;
		    }
		   if(bf.GetBlob(0).area > 20000) {
			speed = 0.2;
	                turnrate = 0;
		    }
		   if(bf.GetBlob(0).area > 30000) {
	              speed = 0;
		      turnrate = 0;
		   }
	      }
		
	      else if(bf.GetBlob(0).x > 160 && bf.GetBlob(0).area > 1000) {
	             turnrate = dtor(-1);
		     if(bf.GetBlob(0).area == 160) {
			if(bf.GetBlob(0).area > 10000) {
			speed = 0.3;
			turnrate = 0;
		    }
		   if(bf.GetBlob(0).area > 20000) {
			speed = 0.2;
	                turnrate = 0;
		    }
		   if(bf.GetBlob(0).area > 30000) {
	              speed = 0;
		      turnrate = 0;
}
}
}
		else if(bf.GetBlob(0).x < 160 && bf.GetBlob(0).area > 1000) {
			turnrate = dtor(1);
		     if(bf.GetBlob(0).area == 160) {
			if(bf.GetBlob(0).area > 10000) {
			speed = 0.3;
			turnrate = 0;
		    }
		   if(bf.GetBlob(0).area > 20000) {
			speed = 0.2;
	                turnrate = 0;
		    }
		   if(bf.GetBlob(0).area > 30000) {
	              speed = 0;
		      turnrate = 0;
}
}
}



               else if(bf.GetCount() == 3) {
		   turnrate = dtor(1);
		   if(bf.GetBlob(1).x == 160) {
			if(bf.GetBlob(1).area > 600) {
			turnrate = 0;
			speed = 1.8;
		     }
			if(bf.GetBlob(1).area > 700) {
			turnrate = 0;
			speed = 1.5;
			}
			if(bf.GetBlob(1).area > 800) {
			turnrate = 0;
			speed = 1.3;
		        }
			if(bf.GetBlob(1).area > 900) {
			turnrate = 0;
			speed = 1.0;
			}
			if(bf.GetBlob(1).area > 1000) {
			turnrate = 0;
			speed = 0.8;
			}
			if(bf.GetBlob(1).area > 2000) {
			turnrate = 0;
			speed = 0.6;
			}
			if(bf.GetBlob(1).area > 2100) {
			turnrate = 0;
			speed = 0.5;
			}
			if(bf.GetBlob(1).area > 2200) {
			turnrate = 0;
			speed = 0.4;
			}

		   }  	
                 }  	
	
	      else {
	        turnrate = dtor(25);
		speed = 0;
               }

   	     

		if(bf.GetBlob(i).color == 0 && ((myBlob.x < bf.GetBlob(i).x) || (myBlob.y < bf.GetBlob(i).y) )) //Is it red? Is it bigger than myBlob
		{
			myBlob = bf.GetBlob(i); //Then we have a new target.
		}
	    }

	  }
	  else
	    {
	      // Here the blobfinder doesn;t see any blobs
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



