/************************************************
*               Data Access Object
*
* Author:   Xiaokui Shu
* Email:    subx@cs.vt.edu
* Update:   08/06/2012
*
* Usage:    DAO()
*           next()
************************************************/
#ifndef DAO_HEADER
#define DAO_HEADER

#include "util.h"
#include "fps.h"

class DAO
{
    private:

        std::vector<char *> m_filenames;
        std::vector<char *>::iterator m_it;

        char *m_dirname;

        uint32_t m_shingle_len;

        void read_filelist();

    public:

        DAO(char *dirname, uint32_t shingle_len);
        ~DAO();

        // output the next FPS from the next file
        FPS * next();
};

#endif
