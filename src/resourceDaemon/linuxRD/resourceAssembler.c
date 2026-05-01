#include "struct_path.h"

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

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

int res_asm_proc_stat(proc_stat *stat	,const char *path){
	if(stat == NULL || path == NULL)
		return -1;

	FILE *fp;
        char line[512];
	
	memset(stat, 0, sizeof(*stat));
	fp = open_file(path,"r");
	if(fp == NULL)
		return -1;
	
	if(fgets(line, sizeof(line), fp) == NULL){
		fclose(fp);
		return -1;
	}
	fclose(fp);

	//Parser
	if(strncmp(line, "cpu ", 4) != 0)
		return -1;
	if (sscanf(line + 4,
        	"%" SCNu64 " %" SCNu64 " %" SCNu64 " %" SCNu64
        	" %" SCNu64 " %" SCNu64 " %" SCNu64 " %" SCNu64
        	" %" SCNu64 " %" SCNu64,
		&stat->	user,
                &stat->	nice,
                &stat->	system,
                &stat->	idle,
                &stat->	iowait,
                &stat->	irq,
                &stat->	softirq,
                &stat->	steal,
                &stat->	guest,
                &stat->	guest_nice ) != 10)
    	{
		return -1;
    	}
	return 0;
}

int res_asm_mem_info(mem_info *stat, const char *path)
{	
    	if (stat == NULL || path == NULL)
    		return -1;

	FILE *fp;
    	char line[256];
	int found = 0;

    	memset(stat, 0, sizeof(*stat));
    	fp = open_file(path, "r");
    	if (fp == NULL)
        	return -1;
    	//Parser
    	while (fgets(line, sizeof(line), fp) != NULL)
    	{
    	    if (sscanf(line, 	"MemTotal: %" 		SCNu64 " kB", &stat-> mem_total_kb) == 1)
    	        found++;
    	    else if (sscanf(line, 	"MemFree: %" 		SCNu64 " kB", &stat-> mem_free_kb) == 1)
    	        found++;
    	    else if (sscanf(line, 	"MemAvailable: %" 	SCNu64 " kB", &stat-> mem_available_kb) == 1)
    	        found++;
    	    else if (sscanf(line, 	"Buffers: %" 		SCNu64 " kB", &stat-> buffers_kb) == 1)
        	    found++;
        	else if (sscanf(line, 	"Cached: %" 		SCNu64 " kB", &stat-> cached_kb) == 1)
        	    found++;
        	else if (sscanf(line, 	"SwapTotal: %" 		SCNu64 " kB", &stat-> swap_total_kb) == 1)
        	    found++;
       		else if (sscanf(line, 	"SwapFree: %" 		SCNu64 " kB", &stat-> swap_free_kb) == 1)
        	    found++;
    	}
	fclose(fp);
    	return (found == 7) ? 0 : -1;
}

int res_asm_loadavg(loadavg_info *stat, const char *path)
{
    	if (stat == NULL || path == NULL)
        	return -1;

	FILE *fp;
    	memset(stat, 0, sizeof(*stat));

	fp = open_file(path, "r");
    	if (fp == NULL)
    	    return -1;

    	if (fscanf(fp,
    	           "%lf %lf %lf %u/%u %u",
    	           &stat-> load_1m,
    	           &stat-> load_5m,
    	           &stat-> load_15m,
		   &stat-> running_tasks,
		   &stat-> total_tasks,
                   &stat-> last_pid) != 6)
    	{
   	     fclose(fp);
   	     return -1;
   	 }
   	fclose(fp);
   	return 0;
}

int res_asm_uptime(uptime_info *stat, const char *path)
{
    	if (stat == NULL || path == NULL)
        	return -1;
    	
	FILE *fp;
    	memset(stat, 0, sizeof(*stat));

	fp = open_file(path, "r");
    	if (fp == NULL)
    	    return -1;
        
	if (fscanf(fp, "%lf %lf",
	               &stat-> uptime_seconds,
	               &stat-> idle_seconds) != 2)
	    {
	        fclose(fp);
	        return -1;
	    }
	fclose(fp);
	return 0;
}

void res_asm_proc_cpu(cpu_info *stat	,const char *path){
	//TO DO
}
