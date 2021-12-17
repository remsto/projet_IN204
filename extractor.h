#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <archive.h>
#include <archive_entry.h>

#include <filesystem>
#include <fstream>

int copy_data(struct archive *ar, struct archive *aw);
int extract(const char *filename);
void browse(std::string path);

#endif // EXTRACTOR_H
