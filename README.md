# BallBlaster
Three-dimensional puzzle game where the goal is to match groups of four spheres of the same color. Made with OpenGL.

## The game
### Setup
* Five different color spheres: red, green, blue, yellow and pink.
* Distributed on screen on a 5 by 5 matrix.
* Set of 4 or more neighboring spheres of the same color are destroyed and the score is increased.
* Game starts with a single sphere in the center with a random color.

### Gameplay
* Player moves the camera with the mouse and the space bar to place the sphere.
* Sphere must be placed near a visible sphere in 7 seconds.
* Color of the next sphere is chosen at random.
* If all the spheres are destroyed a new one is placed in the center.

### End Condition
* The game ends if a sphere isn’t placed in time or the number of spheres exceeds the maximum tenderable (215)
* The objective is to accumulate the maximum number of points.

