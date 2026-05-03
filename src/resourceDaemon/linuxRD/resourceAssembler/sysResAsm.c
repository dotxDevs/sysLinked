#include "struct_path.h"

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>


FILE *open_file(const char *path,const char *mode)
{
    //TO DO Exception Handling
    if (path == NULL || mode == NULL)
    {
        return NULL;
    }
    FILE *fp = fopen(path, mode);
    
    if (fp == NULL)
    {
        return NULL;
    }
    return fp;
}

int res_asm_thermal_info(thermal_info *stat, const char *path)
{
    	if (stat == NULL || path == NULL)
    	    return -1;
	
	    FILE *fp;
	    char thermal_path[256];
	    uint32_t i;

	    memset(stat, 0, sizeof(*stat));
	    for (i = 0; i < MAX_THERMAL_ZONES; i++)
	    {
	        thermal_zone *zone = &stat->zones[i];

        	//	temp
        	snprintf(thermal_path, sizeof(thermal_path), "%s/thermal_zone%u/temp",
        	         	path, i);
        	fp = open_file(thermal_path, "r");
        	if (fp == NULL)
        	    continue;

        	if (fscanf(fp, "%" SCNu64, &zone->temp) != 1)
        	{
        	    fclose(fp);
        	    continue;
        	}	
        	fclose(fp);

        	//	type
        	snprintf(thermal_path, sizeof(thermal_path), "%s/thermal_zone%u/type",
       		          	path, i);
       		 fp = open_file(thermal_path, "r");
       		 if (fp == NULL)
        	    continue;

        	 if (fgets(zone->type, sizeof(zone->type), fp) == NULL)
        	 {
        	     fclose(fp);
        	     continue;
        	  }
        	 fclose(fp);

        	 zone->type[strcspn(zone->type, "\n")] = '\0';	//
        	 zone->valid = true;
        	 stat->zone_count++;
    	}

    	return 0;
}


int res_asm_power_info(power_info *stat, const char *path)
{
    	uint32_t i;
    	FILE *fp;
    	char file_path[256];

    	if (stat == NULL || path == NULL)
    	    return -1;
	
    	memset(stat, 0, sizeof(*stat));

    	for (i = 0; i < MAX_POWER_SUPPLIES; i++)
    	{
        	power_supply *supply = &stat->supplies[stat->supply_count];
	
        	snprintf(file_path, sizeof(file_path),
        	         "%s/BAT%u/capacity", path, i);

        	fp = open_file(file_path, "r");
        	if (fp == NULL)
        	    continue;

        	if (fscanf(fp, "%" SCNu8, &supply->capacity) != 1)
        	{
        	    fclose(fp);
        	    continue;
        	}
        	fclose(fp);

        	snprintf(file_path, sizeof(file_path),
        	         "%s/BAT%u/status", path, i);

        	fp = open_file(file_path, "r");
        	if (fp == NULL)
        	    continue;

        	if (fgets(supply->status, sizeof(supply->status), fp) == NULL)
        	{
        	    fclose(fp);
        	    continue;
        	}
        	fclose(fp);
	
        	supply->status[strcspn(supply->status, "\n")] = '\0';          //
        	supply->valid = true;
        	snprintf(supply->name, sizeof(supply->name), "BAT%u", i);
        	stat->supply_count++;
    	}
	
    return 0;
}

int res_asm_block_info(block_info *stat, const char *path)
{
    	if (stat == NULL || path == NULL)
    	    return -1;

    	DIR *dir;
    	struct dirent *entry;
    	FILE *fp;
    	char stat_path[256];

    	memset(stat, 0, sizeof(*stat));
    	dir = opendir(path);
    	if (dir == NULL)
    	    return -1;

    	while ((entry = readdir(dir)) != NULL)
    	{
    	    block_device_stat *dev;
	        //        skip ../.
	        if (entry->d_name[0] == '.')
	            continue;

        	if (stat->device_count >= MAX_BLOCK_DEVICES)
        	    break;

        	dev = &stat->devices[stat->device_count];

        	strncpy(dev->name, entry->d_name, sizeof(dev->name) - 1);
	        snprintf(stat_path, sizeof(stat_path), "%s/%s/stat", path, entry->d_name);

	        fp = open_file(stat_path, "r");
	        if (fp == NULL)
	            continue;
        	if (fscanf(fp,
        	        "%" SCNu64 " %*u %" SCNu64 " %*u "
               	   	"%" SCNu64 " %*u %" SCNu64 " %*u "
                  	"%" SCNu64 " %" SCNu64,

                   	&dev->reads_completed,
                   	&dev->sectors_read,

                   	&dev->writes_completed,
                   	&dev->sectors_written,

                   	&dev->inflight_ios,
                   	&dev->io_time_ms) != 6)
        	{
        	    fclose(fp);
        	    continue;
        	}

        	fclose(fp);
        	dev->valid = true;
        	stat->device_count++;
    	}

    	closedir(dir);
    return 0;
}

static int read_net_stat_uint64(const char *path, uint64_t *value)
{
    FILE *fp;
    if (path == NULL || value == NULL)
        return -1;
    fp = open_file(path, "r");
    if (fp == NULL)
        return -1;

    if (fscanf(fp, "%" SCNu64, value) != 1)
    {
        fclose(fp);
        return -1;
    }

    fclose(fp);

    return 0;
}

int res_asm_net_info(net_info *stat, const char *path)
{
    if (stat == NULL || path == NULL)
        return -1;

    DIR *dir;
    struct dirent *entry;
    char stat_path[256];
    memset(stat, 0, sizeof(*stat));

    dir = opendir(path);
    if (dir == NULL)
        return -1;

    while ((entry = readdir(dir)) != NULL)
    {
        net_device_stat *dev;

        if (entry->d_name[0] == '.')
            continue;
        if (stat->device_count >= MAX_NET_DEVICES)
            break;

        dev = &stat->devices[stat->device_count];
        strncpy(dev->name, entry->d_name, sizeof(dev->name) - 1);
        snprintf(stat_path, sizeof(stat_path), "%s/%s/statistics/rx_bytes", path, entry->d_name);

        if (read_net_stat_uint64(stat_path, &dev->rx_bytes) != 0)
            continue;

        snprintf(stat_path, sizeof(stat_path), "%s/%s/statistics/rx_packets", path, entry->d_name);

        read_net_stat_uint64(stat_path, &dev->rx_packets);

        snprintf(stat_path, sizeof(stat_path), "%s/%s/statistics/tx_bytes", path, entry->d_name);

        read_net_stat_uint64(stat_path, &dev->tx_bytes);

        snprintf(stat_path, sizeof(stat_path), "%s/%s/statistics/tx_packets", path, entry->d_name);

        read_net_stat_uint64(stat_path, &dev->tx_packets);

        snprintf(stat_path, sizeof(stat_path), "%s/%s/statistics/rx_errors", path, entry->d_name);

        read_net_stat_uint64(stat_path, &dev->rx_errors);

        snprintf(stat_path, sizeof(stat_path), "%s/%s/statistics/rx_dropped", path, entry->d_name);

        read_net_stat_uint64(stat_path, &dev->rx_dropped);

        snprintf(stat_path, sizeof(stat_path), "%s/%s/statistics/tx_errors", path, entry->d_name);

        read_net_stat_uint64(stat_path, &dev->tx_errors);

        snprintf(stat_path, sizeof(stat_path), "%s/%s/statistics/tx_dropped", path,entry->d_name);

        read_net_stat_uint64(stat_path,&dev->tx_dropped);

        dev->valid = true;
        stat->device_count++;
    }

    closedir(dir);

    return 0;
}
