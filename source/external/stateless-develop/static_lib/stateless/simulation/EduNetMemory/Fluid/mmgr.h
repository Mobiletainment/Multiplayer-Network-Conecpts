// ---------------------------------------------------------------------------------------------------------------------------------
//                                     _
//                                    | |
//  _ __ ___  _ __ ___   __ _ _ __    | |__
// | '_ ` _ \| '_ ` _ \ / _` | '__|   | '_ `|
// | | | | | | | | | | | (_| | |    _ | | | |
// |_| |_| |_|_| |_| |_|\__, |_|   (_)|_| |_|
//                       __/ |
//                      |___/
//
// Memory manager & tracking software
//
// Best viewed with 8-character tabs and (at least) 132 columns
//
// ---------------------------------------------------------------------------------------------------------------------------------
//
// Restrictions & freedoms pertaining to usage and redistribution of this software:
//
//  * This software is 100% free
//  * If you use this software (in part or in whole) you must credit the author.
//  * This software may not be re-distributed (in part or in whole) in a modified
//    form without clear documentation on how to obtain a copy of the original work.
//  * You may not use this software to directly or indirectly cause harm to others.
//  * This software is provided as-is and without warrantee. Use at your own risk.
//
// For more information, visit HTTP://www.FluidStudios.com
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 12/22/2000 by Paul Nettle
//
// Copyright 2000, Fluid Studios, Inc., all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#ifndef	_H_MMGR
#define	_H_MMGR

#include <new>
using std::bad_alloc;

#if (defined(__GNUC__)  || defined(__GCCXML__) || defined(__SNC__) || defined(__APPLE__) )
#include <stdint.h>
#endif

// [JF] make it 64bit compatible
#if _LP64
typedef uint64_t FMM_HashIndexType;
#define __MMGR_WIPESHIFT 3
#else
typedef unsigned int FMM_HashIndexType;
#define __MMGR_WIPESHIFT 2
#endif
// ---------------------------------------------------------------------------------------------------------------------------------
// For systems that don't have the __FUNCTION__ variable, we can just define it here
// AK: No, we cannot. a Macro will overrride any existing variable of that name, so ANY
//     systems, regardless whether they have __FUNCTION__ or not will get "??"
// ---------------------------------------------------------------------------------------------------------------------------------

//#define	__FUNCTION__ "??"

// ---------------------------------------------------------------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------------------------------------------------------------

typedef	struct tag_au
{
	size_t		actualSize;
	size_t		reportedSize;
	void		*actualAddress;
	void		*reportedAddress;
	char		sourceFile[40];
	char		sourceFunc[40];
	unsigned int	sourceLine;
	unsigned int	allocationType;
	bool		breakOnDealloc;
	bool		breakOnRealloc;
	unsigned int	allocationNumber;
	struct tag_au	*next;
	struct tag_au	*prev;
} sAllocUnit;

typedef	struct
{
	unsigned int	totalReportedMemory;
	unsigned int	totalActualMemory;
	unsigned int	peakReportedMemory;
	unsigned int	peakActualMemory;
	unsigned int	accumulatedReportedMemory;
	unsigned int	accumulatedActualMemory;
	unsigned int	accumulatedAllocUnitCount;
	unsigned int	totalAllocUnitCount;
	unsigned int	peakAllocUnitCount;
} sMStats;

// ---------------------------------------------------------------------------------------------------------------------------------
// External constants
// ---------------------------------------------------------------------------------------------------------------------------------

extern	const	unsigned int	m_alloc_unknown;
extern	const	unsigned int	m_alloc_new;
extern	const	unsigned int	m_alloc_new_array;
extern	const	unsigned int	m_alloc_malloc;
extern	const	unsigned int	m_alloc_calloc;
extern	const	unsigned int	m_alloc_realloc;
extern	const	unsigned int	m_alloc_delete;
extern	const	unsigned int	m_alloc_delete_array;
extern	const	unsigned int	m_alloc_free;

// ---------------------------------------------------------------------------------------------------------------------------------
// Used by the macros
// ---------------------------------------------------------------------------------------------------------------------------------

void		m_setOwner(const char *file, const unsigned int line, const char *func);

// ---------------------------------------------------------------------------------------------------------------------------------
// Allocation breakpoints
// ---------------------------------------------------------------------------------------------------------------------------------

bool		&m_breakOnRealloc(void *reportedAddress);
bool		&m_breakOnDealloc(void *reportedAddress);

// ---------------------------------------------------------------------------------------------------------------------------------
// The meat of the memory tracking software
// ---------------------------------------------------------------------------------------------------------------------------------

void		*m_allocator(const char *sourceFile, const unsigned int sourceLine, const char *sourceFunc,
			     const unsigned int allocationType, const size_t reportedSize);
void		*m_reallocator(const char *sourceFile, const unsigned int sourceLine, const char *sourceFunc,
			       const unsigned int reallocationType, const size_t reportedSize, void *reportedAddress);
void		m_deallocator(const char *sourceFile, const unsigned int sourceLine, const char *sourceFunc,
			      const unsigned int deallocationType, const void *reportedAddress);

// ---------------------------------------------------------------------------------------------------------------------------------
// Utilitarian functions
// ---------------------------------------------------------------------------------------------------------------------------------

bool		m_validateAddress(const void *reportedAddress);
bool		m_validateAllocUnit(const sAllocUnit *allocUnit);
bool		m_validateAllAllocUnits();

// ---------------------------------------------------------------------------------------------------------------------------------
// Unused RAM calculations
// ---------------------------------------------------------------------------------------------------------------------------------

unsigned int	m_calcUnused(const sAllocUnit *allocUnit);
unsigned int	m_calcAllUnused();

// ---------------------------------------------------------------------------------------------------------------------------------
// Logging and reporting
// ---------------------------------------------------------------------------------------------------------------------------------

void		m_dumpAllocUnit(const sAllocUnit *allocUnit, const char *prefix = "");
void		m_dumpMemoryReport(const char *filename = "memreport.log", const bool overwrite = true);
sMStats		m_getMemoryStatistics();

// ---------------------------------------------------------------------------------------------------------------------------------
// Variations of global operators new & delete
// ---------------------------------------------------------------------------------------------------------------------------------

void	*operator new(size_t reportedSize) throw (std::bad_alloc);
void	*operator new[](size_t reportedSize) throw (std::bad_alloc);
void	*operator new(size_t reportedSize, const char *sourceFile, int sourceLine) throw (std::bad_alloc);
void	*operator new[](size_t reportedSize, const char *sourceFile, int sourceLine) throw (std::bad_alloc);
void	operator delete(void *reportedAddress) throw ();
void	operator delete[](void *reportedAddress) throw ();

#endif // _H_MMGR

// ---------------------------------------------------------------------------------------------------------------------------------
// Macros -- "Kids, please don't try this at home. We're trained professionals here." :)
// ---------------------------------------------------------------------------------------------------------------------------------
#ifdef __USEMMGR_MAKROS_
#include "nommgr.h"
#define	new		(m_setOwner  (__FILE__,__LINE__,__FUNCTION__),false) ? NULL : new
#define	delete		(m_setOwner  (__FILE__,__LINE__,__FUNCTION__),false) ? m_setOwner("",0,"") : delete
#define	malloc(sz)	m_allocator  (__FILE__,__LINE__,__FUNCTION__,m_alloc_malloc,sz)
#define	calloc(num,sz)	m_allocator  (__FILE__,__LINE__,__FUNCTION__,m_alloc_calloc,((num) * (sz)))
#define	realloc(ptr,sz)	m_reallocator(__FILE__,__LINE__,__FUNCTION__,m_alloc_realloc,sz,ptr)
#define	free(ptr)	m_deallocator(__FILE__,__LINE__,__FUNCTION__,m_alloc_free,ptr)
#else
#define	mmgr_new		(m_setOwner  (__FILE__,__LINE__,__FUNCTION__),false) ? NULL : new
#define	mmgr_delete		(m_setOwner  (__FILE__,__LINE__,__FUNCTION__),false) ? m_setOwner("",0,"") : delete
#define	mmgr_malloc(sz)	m_allocator  (__FILE__,__LINE__,__FUNCTION__,m_alloc_malloc,sz)
#define	mmgr_calloc(num,sz)	m_allocator  (__FILE__,__LINE__,__FUNCTION__,m_alloc_calloc,((num) * (sz)))
#define	mmgr_realloc(ptr,sz)	m_reallocator(__FILE__,__LINE__,__FUNCTION__,m_alloc_realloc,sz,ptr)
#define	mmgr_free(ptr)	m_deallocator(__FILE__,__LINE__,__FUNCTION__,m_alloc_free,ptr)
#endif



class	MemStaticTimeTracker
{
public:
	MemStaticTimeTracker() {}
	~MemStaticTimeTracker() {}

	static void sdmInit( void );
	static void sdmShutdown( void );
};
// ---------------------------------------------------------------------------------------------------------------------------------
// mmgr.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
