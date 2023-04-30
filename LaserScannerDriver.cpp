/**
 * @file LaserScannerDriver.cpp
 * @author Ciro Silvano
 * @brief 
 * @version 0.1
 * @date 2020-11-29
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "LaserScannerDriver.h"

// data_qty is set to 180/res, for example, a resolution of 0.5 results in 180/0.5 = 360
// total measurements for a single scan
LaserScannerDriver::LaserScannerDriver(double res) : data_qty((180 / res) + 1)
{
    if (res <= 0 || res > 1)
        throw std::invalid_argument("Invalid resolution. Must belong to ]0, 1] interval.");

    // initializing buffer and allocating memory space for each scan slot
    // buffer is a 2d array sized BUFFER_DIM * data_qty
    buffer = new double *[BUFFER_DIM];
    for (size_t i = 0; i < BUFFER_DIM; i++)
    {
        // each buffer[i] represents a scan, which itself is a data_qty long array
        buffer[i] = new double[data_qty];
    }
};

// destructor for LaserScannerDriver object,
LaserScannerDriver::~LaserScannerDriver()
{
    // deallocates space taken up for each scan ...
    for (size_t i = 0; i < BUFFER_DIM; i++)
        delete[] buffer[i];

    // ... then the buffer itself
    delete[] buffer;
};

// copy constructor
LaserScannerDriver::LaserScannerDriver(const LaserScannerDriver &lsd) : data_qty(lsd.data_qty)
{
    // initializes buffer
    buffer = new double *[BUFFER_DIM];

    // copies member variables
    tail = lsd.tail;
    head = lsd.head;
    size = lsd.size;

    // copies each scan, item by item
    for (size_t i = 0; i < BUFFER_DIM; i++)
    {
        this->buffer[i] = new double[data_qty];
        for (int j = 0; j < data_qty; j++)
        {
            buffer[i][j] = lsd.buffer[i][j];
        }
    }
};

void LaserScannerDriver::new_scan(std::vector<double> scan)
{
    // checks if next tail position is head
    if (next_position(tail) == head)
        // if so, head has to go one position further and buffer size doesn't change
        // because we are effectively removing and adding a scan at the same time
        head = next_position(head);
    else
        // otherwise, just increase size, and leave poor head alone
        size++;

    // then add the scan at position pointed by tail ...
    for (size_t i = 0; i < data_qty; i++)
        buffer[tail][i] = (i >= scan.size()) ? 0 : scan[i];

    // ... and make tail point to the next slot to write in
    tail = next_position(tail);
};

std::vector<double> LaserScannerDriver::get_scan()
{
    // create an empty vector to (eventually) fill and return later
    std::vector<double> tmp;

    // fill the temporary vector only if the buffer isn't empty
    if (size > 0)
    {
        // copy the oldest scan into the $tmp vector
        for (size_t i = 0; i < data_qty; i++)
            tmp.push_back(buffer[head][i]);

        // if head == tail, it means the oldest scan was also the newest,
        // hence we had just one scan remaining in the buffer. head should not
        // move forward in this case, because there would be no other scans to point to
        if (head != tail)
            head = next_position(head);

        // decrease size as we popped out a scan
        size--;
    }

    return tmp;
};

// while not "physically" deleting every scan from the memory, clear_buffer() resets all pointing
// variables effectively rendering all scans overwritable.
// why reallocate the whole thing when you can simply write over it again and again?
void LaserScannerDriver::clear_buffer()
{
    head = 0;
    tail = 0;
    size = 0;
};

double LaserScannerDriver::get_distance(double angle)
{
    // if angle is less than 0 or 180, throw invalid argument
    if (angle < 0 || angle > 180)
        throw std::invalid_argument("Invalid angle. Must belong to [0, 180[ interval.");

    // if size is null, return negative distance to signal it to user
    if (size == 0)
        return -1;

    // return distance corresponding to $angle. By rounding and casting to int, we ensure the array index
    // is valid and corresponding to the closest angle to the one inputted, in case it is not valid.
    return buffer[prev_position(tail)][(int)round(angle * (data_qty-1) / 180)];
};

std::ostream &operator<<(std::ostream &os, const LaserScannerDriver &lsd)
{
    // gets latest scan (or empty vector if size is 0)
    std::vector<double> tmp = lsd.latest_scan();

    // if size is 0, return "null"
    if(tmp.size() == 0) return os << "null";

    // adds each measurement from latest scan adding it to output stream
    for(int i=0; i<tmp.size(); i++)
        os << tmp[i] << std::endl;

    return os;
};

// helper function to find next valid position in the buffer (so if we reach [BUFFER_DIM-1] we can go straight back to [0])
int LaserScannerDriver::next_position(int p) const
{
    return (p + 1) % BUFFER_DIM;
};

// literally the same as next_position, just the opposite
int LaserScannerDriver::prev_position(int p) const
{
    return (p - 1) < 0 ? BUFFER_DIM - 1 : p - 1;
};

// helper function returning last scan added to the buffer, or empty vector<double> if size == 0
std::vector<double> LaserScannerDriver::latest_scan() const {
    std::vector<double> tmp;
    if(size > 0)
        for(int i=0; i<data_qty; i++)
            tmp.push_back(buffer[prev_position(tail)][i]);
    return tmp;
};