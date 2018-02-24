# Graphics-3d
A graphics app that depicts the shape of 3d surfaces.

Plan for projecting 3d onto 2d:
1. First calculate all the points on the wire grid that will need to be graphed
2. Let's say the "camera" is at (x,y,z). We need to do a 3d rotation so that it lies on 1 of the axes, along with all the other points. Ex: move camera to (0,y,0). Makes projection onto a plane easy
3. For projecting, we first have a plane perpendicular to the line of sight of camera. If camera at (0,y,0), then plane will be in xz direction, behind all the points we want to graph. Projecting: Find distance from camera to plane (call this d) > Calculate the vector from the camera to each of the points > Extend each vector so it hits the plane (Do this by multiplying each vector by d/(y component of vector)) > use X and Z coords on the plane as projection
