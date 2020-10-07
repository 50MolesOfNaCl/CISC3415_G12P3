# Create’s blobfinder
define simcam ptz
(
	blobfinder
	(
		# number of colours to look for
		channels_count 3

		# which colours to look for
		channels ["red" "green" "blue"]

		# camera parameters
		image [320 240] #resolution
		range_max 10.0
		ptz [0 0 0]
	)
)

define red model
(
	polygons 1
	polygon[0].points 4
	polygon[0].point[0] [0 0]
	polygon[0].point[1] [0 1]
	polygon[0].point[2] [1 1]
	polygon[0].point[3] [1 0]
	
	size [0.4 0.4]
	color "red"
	
	gui_outline 0
	
)

define red_small model
(
	polygons 1
	polygon[0].points 4
	polygon[0].point[0] [0 0]
	polygon[0].point[1] [0 1]
	polygon[0].point[2] [1 1]
	polygon[0].point[3] [1 0]
	
	size [0.1 0.1]
	color "red"
	
	gui_outline 0
	
)


