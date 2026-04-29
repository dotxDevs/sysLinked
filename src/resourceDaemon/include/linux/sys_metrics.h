 /*
    |--------------------------------------------------------------------------
    |		
    |
    |		FOR COLLECTING ESSENTIAL RUNTIME MATRICS FROM /SRC FILESYSTEM
    |		OS: LINUX
    |		Includes:
    | 		- disk I/O statistics
    | 		- network statistics
    | 		- thermal sensors
    | 		- power supply information
    |
    |
    |
    |--------------------------------------------------------------------------
 */


/*
    |--------------------------------------------------------------------------
    | NETWORK
    |--------------------------------------------------------------------------
 */

#ifndef NET_DEV_STATS_H
#define NET_DEV_STATS_H
#include <stdint.h>
#include <stdbool.h>

typedef struct
{ 
	uint64_t	rx_bytes;
	uint64_t	rx_packets;
	uint64_t	tx_packets;
	uint64_t	tx_bytes;
	
	uint64_t	rx_errors;
	uint64_t	rx_missed_errors;
	uint64_t	rx_dropped;

	uint64_t	tx_errors;
	uint64_t	tx_missed_errors;
	uint64_t	tx_dropped;
}net_dev_stats;

#endif
 
/*
    |--------------------------------------------------------------------------
    | THERMAL
    |--------------------------------------------------------------------------
*/

#ifndef THERMAL_ZONE_H
#define THERMAL_ZONE_H
#include <stdbool.h>
#include <stdint.h>

#define MAX_THERMAL_ZONES 32
#define TYPE_LEN          64

typedef struct
{ 
	uint64_t	temp;
	char		type[TYPE_LEN];
	bool 		valid;	
}thermal_zone;

typedef struct
{
        uint32_t	zone_count;
	thermal_zone	zones[MAX_THERMAL_ZONES];
}thermal_info;

#endif

 /*
    |--------------------------------------------------------------------------
    | POWER
    |--------------------------------------------------------------------------
 */

#ifndef POWER_SUPPLY_INFO_H
#define POWER_SUPPLY_INFO_H
#include <stdint.h>
#include <stdbool.h>

#define POWER_STATUS_LEN 32

typedef struct
{
	uint8_t		capacity;
	char		status[POWER_STATUS_LEN];
	bool		valid;
}power_supply_info;

#endif

/*
    |--------------------------------------------------------------------------
    | BLOCK
    |--------------------------------------------------------------------------
 */

#ifndef BLOCK_STAT_H
#define BLOCK_STAT_H
#include <stdint.h>

typedef struct
{
	uint64_t	reads_completed;
	uint64_t	sectors_read;
	uint64_t	writes_completed;

	uint64_t	sectors_written;
	uint64_t	inflight_ios;
	uint64_t	io_time_ms;
}block_stat;

#endif
