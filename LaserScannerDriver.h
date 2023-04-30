/**
 * @file LaserScannerDriver.h
 * @author Ciro Silvano (ciro.silvano@icloud.com)
 * @brief 
 * @version 0.1
 * @date 2020-11-29
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <vector>
#include <iostream>

class LaserScannerDriver
{
public:

    /**
     * @brief Construct a new Laser Scanner Driver object
     * 
     * @param res - Angular resolution of the LIDAR equipment, a double value ranging from 0 (not included) to 1 (included).
     */
    explicit LaserScannerDriver(double res);

    /**
     * @brief Destroy the Laser Scanner Driver object
     * 
     */
    ~LaserScannerDriver();

    /**
     * @brief Copy constructor for a new Laser Scanner Driver object
     * 
     * @param lsd - Another LaserScannerDriver object to copy
     */
    LaserScannerDriver(const LaserScannerDriver &lsd);

    /**
     * @brief Pushes a new scan into the buffer, completing it with zeros if there's 
     * less data available than necessary, truncating it if there's more than can be stored
     * 
     * @param scan - A sequence of values representing distance measured at a certain angle
     */
    void new_scan(std::vector<double> scan);

    /**
     * @brief Get the oldest scan in the buffer, then remove it
     * 
     * @return std::vector<double> - A sequence of values representing distance measured 
     * at a certain angle
     */
    std::vector<double> get_scan();

    /**
     * @brief Clear the whole buffer, deleting any saved scan
     * 
     */
    void clear_buffer();

    /**
     * @brief Get the distance measured at a given angle from the latest scan available
     * 
     * @param angle - Value representing the angle associated to a certain measurement
     * @return double - Value representing distance measured at a certain angle, -1 if no scans have been previously stored
     */
    double get_distance(double angle);

    /**
     * @brief 
     * 
     * @param os 
     * @param lsd 
     * @return std::ostream& 
     */
    friend std::ostream &operator<<(std::ostream &os, const LaserScannerDriver &lsd);

private:

    // size of buffer (max number of scans it can store)
    const int BUFFER_DIM = 10;

    // ring buffer implemented as 2d array (array of scans)
    double **buffer;

    // indexes to control read and write operation on the buffer
    // $head always points to the oldest item,
    // $tail always points to the next position to write a new scan into
    int head = 0, tail = 0;

    // number of single measurements a scan can store
    const int data_qty;

    // size of the buffer (increased when scan is added, decreased when scan is removed)
    int size = 0;

    // helper functions to calculate next position (simple variable increase/decrease can't work)
    // because of circular nature of ring buffer
    int next_position(int p) const;
    int prev_position(int p) const;

    std::vector<double> latest_scan() const;
};
