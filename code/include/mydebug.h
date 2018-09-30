
#ifndef _MY_DEBUG_H
#define _MY_DEBUG_H

#define MY_DEBUG

#ifdef MY_DEBUG
  #define dprint(fmt, s...)	printk("%s,%d: " fmt, __FUNCTION__, __LINE__, ##s)
#else
  #define dprint(fmt, s...)
#endif	

#endif	
