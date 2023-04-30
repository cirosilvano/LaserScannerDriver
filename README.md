# Description
The assignment required to develop a C++ module for managing data coming from a LIDAR device.

A LIDAR device is a sensor capable of measuring its distance from a surface using a laser beam. The laser beam rotates and the measuring happens in regular angular intervals. 

# Implementation
The module implements the class LaserScannerDriver, which is able to receive data from the sensor, and provide them to the consumer when requested. The vision angle of the LIDAR is assumed to be 180°.

The angular resolution of the LIDAR is set in degrees within the constructor of the LaserScannerDriver object, and dictates the interval in which the individual measurements are supposed to happen. A 0.5° resolution would result in 180/0.5 = 360 total measurements per scan.

The internal buffer saves `BUFFER_DIM` scans in a FIFO manner. In practice, it's a 2D array containing `total_angle/angular_resolution`  measurements per each scan.
