#ifndef _instance_factory_h_
#define _instance_factory_h_

class PMInstance;

/*-----------------------------------------------------------
  | Given a filename, creates an instances of the appropriate
  | type (based on the extension), reads the instance, and
  | returns a pointer to the allocated instance; the instance
  | must be deallocated elsewhere.
  *----------------------------------------------------------*/

extern PMInstance *readInstance(const char *filename, int p);

#endif
