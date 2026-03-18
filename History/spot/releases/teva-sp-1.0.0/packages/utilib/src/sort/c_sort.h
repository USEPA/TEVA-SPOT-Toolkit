/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file c_sort.h
 *
 * Header for C-style sort routines.
 */

#ifndef utilib_c_sort_h
#define utilib_c_sort_h

#if defined(__cplusplus) && defined(ANSI_HDRS)
#include <cstdlib>
#else
#include <stdlib.h>
#endif

#include <utilib/_generic.h>


/**
 * \def SORT_VOID
 *
 * `VOID' type used to pass in the array for sorting.
 */
#if defined(DOXYGEN)
#define SORT_VOID
#endif
/**
 * \def COMP_VOID
 *
 * The pointer type that is passed into the comparison function.
 */
#if defined(DOXYGEN)
#define COMP_VOID
#endif
/**
 * \def SORT_RETVAL
 *
 * The return type for the sorting routine.
 */
#if defined(DOXYGEN)
#define SORT_RETVAL
#endif
/**
 * \def SORT_RETURN
 *
 * The return value used in the source files for the normal termination.
 */
#if defined(DOXYGEN)
#define SORT_RETURN
#endif
/**
 * \def SORT_ARGS
 *
 * The argument specification for a sorting routine.
 */
#if defined(DOXYGEN)
#define SORT_ARGS
#endif


#if defined(SUNOS)
#ifdef __GNUC__
#define SORT_VOID	void
#define COMP_VOID	const void
#define SORT_RETVAL	int
#define SORT_RETURN	0
#define SORT_ARGS (SORT_VOID* base, long unsigned int nel, long unsigned int width, \
                                compare_fn_type compare )
#else
#define SORT_VOID	void
#define COMP_VOID	const void
#define SORT_RETVAL	void
#define SORT_RETURN	
#define SORT_ARGS (SORT_VOID* base, int nel, int width, \
                                compare_fn_type compare )
#endif
#endif

#if defined(SGI)
#ifdef __GNUC__
#define SORT_VOID	void
#define COMP_VOID	const void
#define SORT_RETVAL	void
#define SORT_RETURN	0
#define SORT_ARGS (SORT_VOID* base, size_t nel, size_t width, \
                                compare_fn_type compare )
#else
#define SORT_VOID	void
#define COMP_VOID	const void
#define SORT_RETVAL	void
#define SORT_RETURN	
#define SORT_ARGS (SORT_VOID* base, size_t nel, size_t width, \
                                compare_fn_type compare )
#endif
#endif

#if defined(SOLARIS)
#if defined(__GNUC__)
#define SORT_VOID	void
#define COMP_VOID	const void
#define SORT_RETVAL	void
#define SORT_RETURN	
#define SORT_ARGS (SORT_VOID* base, size_t nel, size_t width, \
                                compare_fn_type compare )
#else
#define SORT_VOID	void
#define COMP_VOID	const void
#define SORT_RETVAL	void
#define SORT_RETURN	
#define SORT_ARGS (SORT_VOID* base, unsigned int nel, unsigned int width, \
                                compare_fn_type compare )
#endif
#endif

#ifdef PARAGON
#define SORT_VOID       void
#define COMP_VOID       const void
#define SORT_RETVAL     void
#define SORT_RETURN	
#define SORT_ARGS (SORT_VOID* base, size_t nel, size_t width, \
                                compare_fn_type compare )

#elif COUGAR
#define SORT_VOID       void
#define COMP_VOID       const void
#define SORT_RETVAL     void
#define SORT_RETURN	
#define SORT_ARGS (SORT_VOID* base, size_t nel, size_t width, \
                                compare_fn_type compare )

#elif TFLOPS_SERVICE
#define SORT_VOID       void
#define COMP_VOID       const void
#define SORT_RETVAL     void
#define SORT_RETURN	
#define SORT_ARGS (SORT_VOID* base, size_t nel, size_t width, \
                                compare_fn_type compare )

#elif OSF
#define SORT_VOID       void
#define COMP_VOID       const void
#define SORT_RETVAL     void
#define SORT_RETURN	
#define SORT_ARGS (SORT_VOID* base, size_t nel, size_t width, \
                                compare_fn_type compare )

#elif CPLANT
#define SORT_VOID       void
#define COMP_VOID       const void
#define SORT_RETVAL     void
#define SORT_RETURN	
#define SORT_ARGS (SORT_VOID* base, size_t nel, size_t width, \
                                compare_fn_type compare )

#elif HPUX
#define SORT_VOID       void
#define COMP_VOID       const void
#define SORT_RETVAL     void
#define SORT_RETURN	
#define SORT_ARGS (SORT_VOID* base, size_t nel, size_t width, \
                                compare_fn_type compare )

#elif UWIN
#ifdef __GNUC__
/* TODO */
#else
#define SORT_VOID       void
#define COMP_VOID       const void
#define SORT_RETVAL     void
#define SORT_RETURN	
#define SORT_ARGS (SORT_VOID* base, size_t nel, size_t width, \
                                compare_fn_type compare )
#endif

#elif _MSC_VER
#define SORT_VOID       void
#define COMP_VOID       const void
#define SORT_RETVAL     void
#define SORT_RETURN	
#define SORT_ARGS (SORT_VOID* base, size_t nel, size_t width, \
                                compare_fn_type compare )

#elif CYGWIN
#define SORT_VOID       void
#define COMP_VOID       __const void
#define SORT_RETVAL     void
#define SORT_RETURN	
#define SORT_ARGS (SORT_VOID* base, size_t nel, size_t width, \
                                compare_fn_type compare )

#elif LINUX
#define SORT_VOID       void
#define COMP_VOID       __const void
#define SORT_RETVAL     void
#define SORT_RETURN	
#define SORT_ARGS (SORT_VOID* base, size_t nel, size_t width, \
                                compare_fn_type compare )

#elif DARWIN
#define SORT_VOID       void
#define COMP_VOID       __const void
#define SORT_RETVAL     void
#define SORT_RETURN	
#define SORT_ARGS (SORT_VOID* base, size_t nel, size_t width, \
                                compare_fn_type compare )

#elif RS6K
#define SORT_VOID       void
#define COMP_VOID       const void
#define SORT_RETVAL     void
#define SORT_RETURN	
#define SORT_ARGS (SORT_VOID* base, size_t nel, size_t width, \
                                compare_fn_type compare )
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** A typedef for generic comparison functions used in C */
typedef int (*compare_fn_type)(COMP_VOID*,COMP_VOID*);
#ifdef __cplusplus

};
#endif

/** Functional form of a sorting routine. */
#define SORT_FORM	SORT_RETVAL (*) SORT_ARGS


#ifdef __cplusplus
extern "C" {
#endif

/** Insertion sort. */
#ifdef DOXYGEN
SORT_RETVAL   ins_sort( SORT_ARGS );
#else
SORT_RETVAL   ins_sort _( SORT_ARGS );
#endif

/** Heap sort. */
#ifdef DOXYGEN
SORT_RETVAL  heap_sort( SORT_ARGS );
#else
SORT_RETVAL  heap_sort _( SORT_ARGS );
#endif

/** quick sort. */
#ifdef DOXYGEN
SORT_RETVAL quick_sort( SORT_ARGS );
#else
SORT_RETVAL quick_sort _( SORT_ARGS );
#endif

/** Merge sort. */
#ifdef DOXYGEN
SORT_RETVAL merge_sort( SORT_ARGS );
#else
SORT_RETVAL merge_sort _( SORT_ARGS );
#endif

/** Shell sort. */
#ifdef DOXYGEN
SORT_RETVAL shell_sort( SORT_ARGS );
#else
SORT_RETVAL shell_sort _( SORT_ARGS );
#endif


/** Integer comparison used for sorting. */
#ifdef DOXYGEN
int compare_int(COMP_VOID*, COMP_VOID*);
#else
int compare_int _((COMP_VOID*, COMP_VOID*));
#endif

/** Float comparison used for sorting. */
#ifdef DOXYGEN
int compare_float(COMP_VOID*, COMP_VOID*);
#else
int compare_float _((COMP_VOID*, COMP_VOID*));
#endif

/** Double comparison used for sorting. */
#ifdef DOXYGEN
int compare_double(COMP_VOID*, COMP_VOID*);
#else
int compare_double _((COMP_VOID*, COMP_VOID*));
#endif

/** Ereal<double> comparison used for sorting. */
#ifdef DOXYGEN
int compare_ereal_double(COMP_VOID*, COMP_VOID*);
#else
int compare_ereal_double _((COMP_VOID*, COMP_VOID*));
#endif


/* 
 * Misc sorting utilities
 */

/**
 * Fills the array \a ndx with the vector of indeces in order.
 * ndx[i] is the index for the element of \a base with rank i.
 */
#ifdef DOXYGEN
void orderx(int *ndx, int n, SORT_VOID* base, int width, 
	compare_fn_type compar, SORT_FORM);
#else
void orderx _((int *ndx, int n, SORT_VOID* base, int width, 
	compare_fn_type compar, SORT_FORM));
#endif

/**
 * Fills the array \a rank_vec with the rank of the elements in the original 
 * array.  This function uses the index array generated by \c orderx.
 */
#ifdef DOXYGEN
void rank(int* ndx_vec, int* rank_vec, int n);
#else
void rank _((int* ndx_vec, int* rank_vec, int n));
#endif

/**
 * TODO.
 */
#ifdef DOXYGEN
void order_k(SORT_VOID* base, int nel, int width, compare_fn_type compar,
                                int k);
#else
void order_k _((SORT_VOID* base, int nel, int width, compare_fn_type compar,
                                int k));
#endif

/**
 * TODO.
 */
#ifdef DOXYGEN
void order_ki(SORT_VOID* base, int nel, int width,
                                compare_fn_type compar, int k, int* ndx);
#else
void order_ki(SORT_VOID* base, int nel, int width,
                                compare_fn_type compar, int k, int* ndx);
#endif

#ifdef __cplusplus
};
#endif

#endif
