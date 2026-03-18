#ifndef __vectrie_h
#define __vectrie_h

/* Stefan Chakerian
 * Sandia National Labs
 * Sep 2006
 *
 * VecTrie.h
 * template for vector-based trie (uses vectors as keys)
 * Stores arbitrary data (pointers) in nodes
 * called with VecTrie<Alphabet_type key, Data_type data>
 * Keys are vector<Alphabet_type>, Data_type is arbitrary reference
 *
 * requires vlink.h
 */



#include <iostream>
#include <vector>

#include "vlink.h"

using namespace std;

template <class A> class VecTrieKey;
template <class A, class T> class VecTrieNode;
template <class A, class T> class VecTrie;

//================
//  VecTrieKey <A>
//================

template <class A>
class VecTrieKey {
  public:
    VecTrieKey() { };
    ~VecTrieKey() { };
    VecTrieKey(vector<A> &vec) {
      k = vec;
    };
    VecTrieKey(VecTrieKey &src) {
      k = src.k;
    };
    A operator[](int index) {
	return k[index];
    }
    A operator=(vector<A> &vec) {
	k = vec;
    }
    bool operator==(VecTrieKey &key) {
	return (this.k == key.k);
    }
    vector<A> getkey() {
      return k;
    }
    int size() {
      return k.size();
    }
    void setkey(vector<A> &veckey) {
      k = veckey;
    }
    void setkey(VecTrieKey<A> &key) {
      k = key.k;
    }

  private:
    vector<A> k;
};


template <class A, class T>
class VecTrie;				// forward decl


//================
//  VecTrieNode <A,T>
//================


template <class A, class T>
class VecTrieNode {
  public:
    VecTrieNode();
    ~VecTrieNode();

    T		getdata();
    void	setdata(T);
    bool	isleaf() { return leaf; };	// data is valid
    int		numlinks();			// links.size()
    A		link(int);			// direct access links[]
    VecTrieNode<A,T>*	linknode(int);		// direct access links[]
  private:
    friend class VecTrie<A,T>;
    T		data;
    bool	leaf;
    vlink<A, VecTrieNode> links;
};

template <class A, class T>
VecTrieNode<A,T>::VecTrieNode() {
  leaf = false;
};

// getdata
template <class A, class T>
T VecTrieNode<A,T>::getdata() {
  if (!leaf)
    cout << "VecTrieNode::getdata on empty node?" << endl;
  return data;
}

// setdata
template <class A, class T>
void VecTrieNode<A,T>::setdata(T newdata) {
  data = newdata;
  leaf = true;
  return;
}

// link
template <class A, class T>
A VecTrieNode<A,T>::link(int i) {
  return links.getval(i);
}

// linknode
template <class A, class T>
VecTrieNode<A,T>* VecTrieNode<A,T>::linknode(int i) {
  return links[i];
}

// numlinks
template <class A, class T>
int VecTrieNode<A,T>::numlinks() {
  return links.size();
}

//================
//  VecTrie <A,T>
//================

template <class A, class T>
class VecTrie {
  public:
    VecTrie();
    virtual ~VecTrie() { };
    virtual VecTrieNode<A,T>*	insert(VecTrieKey<A>&, T);
    virtual T			getdata(VecTrieKey<A>&);
    virtual VecTrieNode<A,T>*	getnode(VecTrieKey<A>&);
    virtual bool		delnode(VecTrieKey<A>&);
    void			dump();
    void			dumpnode(VecTrieNode<A,T>*);
// headnode() allows operations on trie as VecTrieNode
    VecTrieNode<A,T>*		headnode();

// disable copy and assignment
    VecTrie			( const VecTrie<A,T>& );	  // fake copy
    VecTrie			operator=( const VecTrie<A,T>& ); // fake =
  private:
    void			dumpnode(VecTrieNode<A,T>*, vector<A>);
    VecTrieNode<A,T>* head;
};


// constructor
template <class A, class T>
VecTrie<A,T>::VecTrie() {
  head = new VecTrieNode<A,T>;
};

// insert
template <class A, class T>
VecTrieNode<A,T>* VecTrie<A,T>::insert(VecTrieKey<A>& key, T data) {
  VecTrieNode<A,T> *p, *t;
  int i, j;
  p = head;

  if (key.size() == 0) {
    cout << "insert: empty key?" << endl;
    return NULL;
  }
  for (i = 0; i < key.size(); ++i) {
//    cout << "inserting key["<<i<<"] " << key[i] << endl;  //debug
    j = p->links.insert(key[i]); // inserts link if not there, returns loc
    if ( (t = p->links.getlink(j)) == NULL) {
      t = new VecTrieNode<A,T>();
      p->links.setlink(j, t);
    }
    p = t;
  }
  p->setdata(data);
  return p;
}


// getnode
template <class A, class T>
VecTrieNode<A,T>* VecTrie<A,T>::getnode(VecTrieKey<A>& key) {
  VecTrieNode<A,T> *p, *t;
  int i, j;
  p = head;

  if (key.size() == 0) {
    cout << "getnode: empty key?" << endl;
    return NULL;
  }
  for (i = 0; i < key.size(); ++i) {
    if (! p->links.find(key[i]))
      return NULL;
    j = p->links.findloc(key[i]);
    t = p->links.getlink(j);
    if ( i + 1 == key.size() )
      return t;
    p = t;
  }
  return NULL; // not reached, I hope
}


// getdata
template <class A, class T>
T VecTrie<A,T>::getdata(VecTrieKey<A>& key) {
  VecTrieNode<A,T> *p = getnode(key);
  if (p != NULL && p->isleaf() ) {
    return p->getdata();
  }
}

template <class A, class T>
bool VecTrie<A,T>::delnode(VecTrieKey<A>& key) {
  // don't do much atm
  VecTrieNode<A,T> *p = getnode(key);
  if (p != NULL) {
    p->leaf = false;
    return true;
  } else {
    return false;
  }
}

template <class A, class T>
VecTrieNode<A,T>* VecTrie<A,T>::headnode() {
  return head;
}


template <class A, class T>
void VecTrie<A,T>::dump() {
  dump;
}



// dumpnode
// DFS on node
template <class A, class T>
void VecTrie<A,T>::dumpnode(VecTrieNode<A,T>* node) {
  vector<A> k;
  dumpnode(node, k);
}

template <class A, class T>
void VecTrie<A,T>::dumpnode(VecTrieNode<A,T>* node, vector<A> k) {
  VecTrieNode<A,T> *p, *t;
  int i, j;

  cout << "node (" << node << ")";
  for (i=0; i< k.size(); ++i) {
    cout << " " << k[i];
  }
  cout << endl;
  cout << "  data: ";
  if (node->isleaf())
    cout << node->getdata() << endl;
  else
    cout << "(none)" << endl;
  if (j = node->numlinks() ) {
    cout << "  " << j << " links: ";
    for (i=0; i < j; ++i) {
      cout << node->link(i) << " ";
    }
    cout << endl;
  } else {
    cout << "  no links" << endl;
  }
  for (i=0; i < j; ++i) {
    cout << endl << "Link " << node->link(i) << endl;
    k.push_back(node->link(i));
    dumpnode(node->linknode(i), k);
    k.pop_back();
  }
}


#endif
