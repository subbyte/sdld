#include <fstream>
#include <cstdio>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

#include "dao.h"

using namespace std;

DAO::DAO(char *dirname, uint32_t shingle_len)
{
    m_shingle_len = shingle_len;
    m_dirname = dirname;

    read_filelist();
}

DAO::~DAO()
{
    for(auto it = m_filenames.begin(); it != m_filenames.end(); ++it)
    {
        delete[] *it;
    }
    m_filenames.clear();
}

void DAO::read_filelist()
{
    char *basename;
    DIR *dir = opendir(m_dirname);
    if (dir == NULL)
    {
        throw "Directory Invalid.";
    }
    struct dirent *direntry;

    while ((direntry = readdir(dir)))
    {
        if (direntry->d_name[0] != '.')
        // avoid directories such as ".", "..", ".svn"
        {
            basename = new char[strlen(direntry->d_name) + 1];
            strcpy(basename, direntry->d_name);
            m_filenames.push_back(basename);
        }
    }

    closedir(dir);
 
    m_it = m_filenames.begin();
}

FPS * DAO::next()
{
    char *filename;
    char *basename;
    ifstream file;
    uint32_t file_size;
    char *content;

    if (m_it == m_filenames.end())
    {
        return NULL;
    }

    basename = *m_it;
    ++m_it;

    filename = new char[strlen(m_dirname) + strlen("/") + strlen(basename) + 1];
    strcpy(filename, m_dirname);
    strcat(filename, "/");
    strcat(filename, basename);

    file.open(filename, ios::in|ios::binary);
    file.seekg(0, ios::end);
    file_size = file.tellg();
    file.seekg(0, ios::beg);
    content = new char[file_size];
    file.read(content, file_size);
    file.close();

    return new FPS(basename, content, file_size, m_shingle_len);
}
