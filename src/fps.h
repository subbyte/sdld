/************************************************
*           Fingerprints Module
*
* Author:   Xiaokui Shu
* Email:    subx@cs.vt.edu
* Update:   11/06/2013
*
************************************************/
#ifndef FPS_HEADER
#define FPS_HEADER

#include "util.h"

class FPS
{
    private:
        // input data
        char *m_raw_data;
        uint32_t m_raw_data_len;
        uint32_t m_shingle_len;

        // just a pointer, no need to handle allocated space
        char *m_name;

        // CRC64 table
        static const uint64_t sctp_crc_64[256];

        // main data structure
        std::unordered_map<uint64_t, uint32_t> *m_fp2freq;

        // sizes
        uint32_t m_size;
        uint32_t m_set_size;

        uint64_t CRC64(char *buffer);

        // generate fingerprints to fill m_fp2freq
        void gen_FPs();

        void calculate_sizes();

    public:
        // build FPS from raw byte string
        FPS(char *name, char *raw_data, uint32_t raw_data_len, uint32_t shingle_len);

        // build FPS from existing fp2freq map, useful to copy or generate new FPS
        FPS(std::unordered_map<uint64_t, uint32_t> *fp2freq, uint32_t shingle_len);

        ~FPS();

        std::unordered_map<uint64_t, uint32_t> * get_FPsMap();

        uint32_t get_shingle_len();

        // collection intersection between this and other
        FPS * intersection(FPS *other);

        // return the size of the collection
        uint32_t size();

        // return the size of the set
        uint32_t set_size();

        // return name of the file
        char *get_name();
};

#endif
