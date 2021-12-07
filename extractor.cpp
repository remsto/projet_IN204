#include "extractor.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <iostream>

#include <archive.h>
#include <archive_entry.h>

#include <filesystem>
#include <fstream>

// g++ -Wall -o test extractor.cpp -larchive -std=c++1z

static int
copy_data(struct archive *ar, struct archive *aw)
{
    int r;
    const void *buff;
    size_t size;
    off_t offset;

    for (;;)
    {
        r = archive_read_data_block(ar, &buff, &size, &offset);
        if (r == ARCHIVE_EOF)
            return (ARCHIVE_OK);
        if (r < ARCHIVE_OK)
            return (r);
        r = archive_write_data_block(aw, buff, size, offset);
        if (r < ARCHIVE_OK)
        {
            fprintf(stderr, "%s\n", archive_error_string(aw));
            return (r);
        }
    }
}

static int
extract(const char *filename)
{
    struct archive *a;
    struct archive *ext;
    struct archive_entry *entry;
    int flags;
    int r;

    /* Select which attributes we want to restore. */
    flags = ARCHIVE_EXTRACT_TIME;
    flags |= ARCHIVE_EXTRACT_PERM;
    flags |= ARCHIVE_EXTRACT_ACL;
    flags |= ARCHIVE_EXTRACT_FFLAGS;

    a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);
    ext = archive_write_disk_new();
    archive_write_disk_set_options(ext, flags);
    archive_write_disk_set_standard_lookup(ext);
    if ((r = archive_read_open_filename(a, filename, 10240)))
        return 1;
    for (;;)
    {
        r = archive_read_next_header(a, &entry);
        if (r == ARCHIVE_EOF)
            break;
        if (r < ARCHIVE_OK)
            fprintf(stderr, "%s\n", archive_error_string(a));
        if (r < ARCHIVE_WARN)
            return 1;
        // std::string currentFile = archive_entry_pathname(entry);
        // std::string fullOutputPath = std::string("/home/hypnos/IN/IN204/Projet/") + currentFile;
        // archive_entry_set_pathname(entry, fullOutputPath.c_str());
        r = archive_write_header(ext, entry);
        if (r < ARCHIVE_OK)
            fprintf(stderr, "%s\n", archive_error_string(ext));
        else if (archive_entry_size(entry) > 0)
        {
            r = copy_data(a, ext);
            if (r < ARCHIVE_OK)
                fprintf(stderr, "%s\n", archive_error_string(ext));
            if (r < ARCHIVE_WARN)
                return 1;
        }
        r = archive_write_finish_entry(ext);
        if (r < ARCHIVE_OK)
            fprintf(stderr, "%s\n", archive_error_string(ext));
        if (r < ARCHIVE_WARN)
            return 1;
    }
    archive_read_close(a);
    archive_read_free(a);
    archive_write_close(ext);
    archive_write_free(ext);
    return 0;
}

void browse(std::string filename)
{
    namespace fs = std::filesystem; // C++17
    std::string extracted_fname = filename.erase(filename.rfind('.')); //remove the extension
    std::string path = "/home/hypnos/IN/IN204/Projet/" + extracted_fname;
    std::cout << path << "\n";
    for (const auto &entry : fs::directory_iterator(path))
        std::cout << entry.path() << std::endl;
}

/*int main(int argc, const char **argv)
{
    extract(argv[1]);
    browse(argv[1]);
    return 0;
}*/
