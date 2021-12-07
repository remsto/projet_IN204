#ifndef EXTRACTOR_H
#define EXTRACTOR_H

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

static int copy_data(struct archive *ar, struct archive *aw);
static int extract(const char *filename);
void browse(std::string filename);

#endif // EXTRACTOR_H
