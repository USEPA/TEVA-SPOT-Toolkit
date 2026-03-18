/* HASH.C
**
** Implementation of a simple Hash Table for string storage & retrieval
**
** Written by L. Rossman
**
** The hash table data structure (HTable) is defined in "hash.h".
** Interface Functions:
**    HTcreate() - creates a hash table
**    HTinsert() - inserts a string & its index value into a hash table
**    HTfind()   - retrieves the index value of a string from a table
**    HTfree()   - frees a hash table
**
*/

#include <malloc.h>
#include <string.h>
#include "hash.h"

int hash(char *key)
{
	int n = 0;
	char *ch = key;
	while (*ch)
	{
		n <<= 1;
		n += (*ch);
		++ch;
	}

	/***Modified by Feng 04/05/2005***/ 
	if (n < 0)
		n = -n;
	/**********************/

	return(n % HTMAXSIZE);

}

HTtable *HTcreate()
{
	int i;
	HTtable *ht =
		(HTtable *) calloc(HTMAXSIZE, sizeof(HTtable));
	if (ht != NULL)
		for (i=0; i<HTMAXSIZE; i++) ht[i] = NULL;
	return(ht);
}
 
int	HTinsert(HTtable *ht, char *key, int data)
{
	int i = hash(key);
	struct HTentry *entry;
	if ((i < 0) || (i >= HTMAXSIZE)) return(0);
	entry = (struct HTentry *) malloc(sizeof(struct HTentry));
	if (entry == NULL) return(0);
	entry->key = key;
	entry->data = data;
	entry->next = ht[i];
	ht[i] = entry;
	return(1);
}
	
int 	HTfind(HTtable *ht, char *key)
{
	int i = hash(key);
	struct 	HTentry	*entry;
	if ((i < 0) || (i >= HTMAXSIZE)) return(NOTFOUND);
	entry = ht[i];
	while (entry != NULL)
	{
		if (strcmp(entry->key,key) == 0) return(entry->data);
		entry = entry->next;
	}
	return(NOTFOUND);
}

void	HTfree(HTtable *ht)
{
	struct 	HTentry	*entry,
			*nextentry;
	int i;
	for (i=0; i<HTMAXSIZE; i++)
	{
		entry = ht[i];
		while (entry != NULL)
		{
			nextentry = entry->next;
			free(entry);
			entry = nextentry;
		}
	}
	free(ht);
}

