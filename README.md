[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)

Kraken-cpp is a work in progress !

# Kraken-cpp

Kraken-cpp is a C++ port of [The Kraken Pathfinding](https://github.com/PFGimenez/The-Kraken-Pathfinding). It is a multithreaded tentacle-based pathfinding library for nonholonomic robotic vehicles. It finds a trajectory followable by a car-like vehicle in the form of a list of points.

### Features

The trajectory created by Kraken has several properties, making it suitable for robotic vehicles, namely:

- position continuity (G0 continuity), orientation continuity (G1 continuity) and curvature piecewise continuity (piecewise G2 continuity);
- handles forward and backward movement;
- takes into account the limited linear acceleration and deceleration;
- limits the radial acceleration.

Kraken has two default modes:

- find a path given a start position, a start orientation and an end position;
- find a path given a start position, a start orientation, an end position and an end orientation.

You can easily add new modes that suit your need.

![Trajectory example](https://raw.githubusercontent.com/PFGimenez/The-Kraken-Pathfinding/master/resources/example.png)

## Great, I have a trajectory. How do my robot follow it ?

Getting the trajectory is only half of the work, because you won't go far if your robot can't follow it. Different control algorithms exist; in this section the Samson control algorithm [1] is presented.

First, recall that the [curvature](https://en.wikipedia.org/wiki/Curvature#Curvature_of_plane_curves) of a curve _C_ at the point _P_ is the inverse of the radius of curvature at _P_, i.e. the inverse of the radius of the circle that "fits" the best _C_ at _P_.

<img align="right" src="https://raw.githubusercontent.com/PFGimenez/The-Kraken-Pathfinding/master/resources/asser-samson.png">

Let _R_ be the vehicle and _R'_ its orthogonal projection to the curve and denote _θ(R)_ the orientation of the robot, _θ(R')_ the orientation setpoint at _R'_, _κ(R')_ the curvature setpoint at _R'_ and _d_ the algebric distance between _R_ and _R'_ (_d > 0_ if the robot is on the left of the curve, _d < 0_ otherwise).

The curvature setpoint is _κ = κ(R') - k₁×d - k₂×(θ(R) - θ(R'))_, where _k₁_ and _k₂_ are two constants depending on the system.

In practice, if the robot has a small orientation error, one can approximate _d_ with _(R.y - R'.y) cos(θ(R')) - (R.x - R'.x) sin(θ(R'))_.

This control algorithm has been successfully used with Kraken in the [INTech Senpaï Moon-Rover project (french !)](https://intechsenpai.github.io/moon-rover/).

## License

This project is licensed under the [MIT license](https://raw.githubusercontent.com/PFGimenez/Kraken-cpp/master/LICENSE), which is very permissive. I'd love to know the projects that use Kraken; please keep me posted !

## Acknowledgements

I would like to thank the [INTech robotics club](https://github.com/Club-INTech) that taught me all I know about robotics and greatly influenced the developpement of Kraken.

## Bibliography

[1] Samson, C. (1995). Control of chained systems application to path following and time-varying point-stabilization of mobile robots. IEEE transactions on Automatic Control, 40(1), 64-77.
