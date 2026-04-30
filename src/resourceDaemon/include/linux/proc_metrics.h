/*
|--------------------------------------------------------------------------
|
|
|	FOR COLLECTING ESSENTIAL RUNTIME MATRICS FROM /PROC FILESYSTEM
|
|	OS: Linux
|
|	Includes:
|	- CPU and scheduler statistics
|	- memory information
|	- disk I/O statistics
|	- network statistics
|	- thermal sensors
|	- power supply information
|
|
|--------------------------------------------------------------------------
*/

/*
    |--------------------------------------------------------------------------
    | CPU TIME SPEND STATS
    |--------------------------------------------------------------------------
*/

#ifndef PROC_STAT_H
#define PROC_STAT_H
#include <stdint.h>

typedef struct
{
	uint64_t	user;
	uint64_t	nice;
	uint64_t	system;
	uint64_t	idle;

	uint64_t	iowait;
	uint64_t	irq;
	uint64_t	softirq;

	uint64_t	steal;
	uint64_t	guest;
	uint64_t	guest_nice;
}proc_stat;
#endif

/*
    |--------------------------------------------------------------------------
    | RAM STAT
    |--------------------------------------------------------------------------
*/

#ifndef MEM_INFO_H
#define MEM_INFO_H
#include <stdint.h>

typedef struct
{
	uint64_t	mem_total_kb;
	uint64_t	mem_free_kb;
	uint64_t	mem_available_kb;

	uint64_t	buffers_kb;
	uint64_t	cached_kb;
	uint64_t	swap_total_kb;
	uint64_t	swap_free_kb;
}mem_info;

#endif

/*
    |--------------------------------------------------------------------------
    | LOADS
    |--------------------------------------------------------------------------
*/

#ifndef LOADAVG_INFO_H
#define LOADAVG_INFO_H
#include <stdint.h>

typedef struct
{
	double		load_1m;
	double		load_5m;
	double		load_15m;

	uint32_t	running_tasks;
	uint32_t	total_tasks;
	uint32_t	last_pid;
}loadavg_info;

#endif

/*
    |--------------------------------------------------------------------------
    | UP TIME
    |--------------------------------------------------------------------------
*/

#ifndef UPTIME_INFO_H
#define UPTIME_INFO_H

typedef struct
{
	double	uptime_seconds;
	double	idle_seconds;
}uptime_info;

#endif

/*
    |--------------------------------------------------------------------------
    | CPU DETAILS
    |--------------------------------------------------------------------------
*/

#ifndef CPU_INFO_H
#define CPU_INFO_H
#include <stdint.h>

#define CPU_MODEL_NAME_LEN 128

typedef struct
{
	uint32_t	processor_id;
	char		model_name[CPU_MODEL_NAME_LEN];
	double		cpu_mhz;

	uint32_t	cache_size_kb;
	uint32_t	cpu_cores;
}cpu_info;

#endif
