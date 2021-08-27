#ifndef SWABIAN_H
#define SWABIAN_H 

#include <timetagger/TimeTagger.h> /* for timestamp_t */
#include <timetagger/Iterators.h> /* For SynchronizedMeasurements */

class Swabian
{
public:
    Swabian(void);
    ~Swabian(void);
    std::vector<std::string> check_for_devices(void);
    int connect(std::string serial="");
    int initialize_measurements(int start_channel, int click_channel_mask, int bin_width, timestamp_t time);
    int get_histogram(int channel, std::vector<timestamp_t> *data);
    int set_delay(int channel, int delay);
    int set_trigger_level(int channel, float level);
    int get_count_rates(int *channels, double *out, size_t n);
    int set_test_signal(int channel, int value);

    TimeTagger *t;
    SynchronizedMeasurements *measurementGroup;
    std::vector<StartStop*> measurements;
    std::vector<int> channels;
};

#endif
