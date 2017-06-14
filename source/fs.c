#include "fs.h"
#include "utils.h"

void openSdArchive()
{
	FSUSER_OpenArchive(&sdmcArchive, ARCHIVE_SDMC, fsMakePath(PATH_EMPTY, ""));
}

void closeSdArchive()
{
	FSUSER_CloseArchive(sdmcArchive);
}

const char *get_filename_ext(const char *filename) 
{
    const char *dot = strrchr(filename, '.');
    
	if (!dot || dot == filename) 
		return "";
    
	return dot + 1;
}

int makeDir(const char *path)
{
	if (!path) 
		return -1;
	
	return mkdir(path, 0777);
}

bool fileExists(char *path) 
{
    FILE * temp = fopen(path, "r");
    if(temp == NULL)
        return false;

    fclose(temp);

    return true;
}

bool dirExists(const char *path)
{
    struct stat info;

    if(stat( path, &info ) != 0)
        return false;
    else if(info.st_mode & S_IFDIR)
        return true;
    else
        return false;
}

char* getFileCreationTime(char *path) 
{
    struct stat attr;
    stat(path, &attr);
	
    return ctime(&attr.st_ctime);
}

char* getFileModifiedTime(char *path) 
{
	static char timeStr[30];
	u64 mtime;
	sdmc_getmtime(path, &mtime);
	time_t mt = mtime;
	struct tm *timeStruct = gmtime(&mt);
	
	int hours = timeStruct->tm_hour;
	int minutes = timeStruct->tm_min;
	int amOrPm = 0;
	
	if(hours < 12)
		amOrPm = 1;
	if(hours == 0)
		hours = 12;
	else if(hours > 12)
		hours = hours - 12;
	
	int day = timeStruct->tm_mday;
	int month = timeStruct->tm_mon + 1; // January being 0
	int year = timeStruct->tm_year + 1900;
	
	switch(getRegion())
	{
		case CFG_REGION_JPN:
			sprintf(timeStr, "%d/%d/%d %2i:%02i %s", year, month, day, hours, minutes, amOrPm ? "AM" : "PM");
			break;
		case CFG_REGION_USA: // This is the worst one
			sprintf(timeStr, "%d/%d/%d %2i:%02i %s", month, day, year, hours, minutes, amOrPm ? "AM" : "PM");
			break;
		case CFG_REGION_EUR:
			sprintf(timeStr, "%d/%d/%d %2i:%02i %s", day, month, year, hours, minutes, amOrPm ? "AM" : "PM");
			break;
		case CFG_REGION_AUS:
			sprintf(timeStr, "%d/%d/%d %2i:%02i %s", day, month, year, hours, minutes, amOrPm ? "AM" : "PM");
			break;
		case CFG_REGION_CHN:
			sprintf(timeStr, "%d/%d/%d %2i:%02i %s", year, month, day, hours, minutes, amOrPm ? "AM" : "PM");
			break;
		case CFG_REGION_KOR:
			sprintf(timeStr, "%d/%d/%d %2i:%02i %s", year, month, day, hours, minutes, amOrPm ? "AM" : "PM");
			break;
		case CFG_REGION_TWN:
			sprintf(timeStr, "%d/%d/%d %2i:%02i %s", year, month, day, hours, minutes, amOrPm ? "AM" : "PM");
			break;
		default:
			sprintf(timeStr, "%d/%d/%d %2i:%02i %s", day, month, year, hours, minutes, amOrPm ? "AM" : "PM");
			break;	
	}
	
	return timeStr;
}

char* getFileAccessedTime(char *path) 
{
    struct stat attr;
    stat(path, &attr);
	
    return ctime(&attr.st_atime);
}

u64 getFileSize(FS_Archive archive, const char *path)
{
	u64 st_size;
	Handle handle;

	FSUSER_OpenFile(&handle, archive, fsMakePath(PATH_ASCII, path), FS_OPEN_READ, 0);
	FSFILE_GetSize(handle, &st_size);
	FSFILE_Close(handle);
	
	return st_size;
}

int fsRemove(FS_Archive archive, const char *filename)
{
    Result ret = FSUSER_DeleteFile(archive, fsMakePath(PATH_ASCII, filename));

    return ret == 0 ? 0 : -1;
}

int fsRename(FS_Archive archive, const char *old_filename, const char *new_filename)
{
    Result ret = FSUSER_RenameFile(archive, fsMakePath(PATH_ASCII, old_filename), archive, fsMakePath(PATH_ASCII, new_filename));

    return ret == 0 ? 0 : -1;
}