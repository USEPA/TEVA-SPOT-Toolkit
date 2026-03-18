<?xml version='1.0' encoding='ISO-8859-1' standalone='yes' ?>
<tagfile>
  <compound kind="file">
    <name>_generic.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/sys/</path>
    <filename>__generic_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>PAUSE</name>
      <anchorfile>__generic_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OK</name>
      <anchorfile>__generic_8h.html</anchorfile>
      <anchor>a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TRUE</name>
      <anchorfile>__generic_8h.html</anchorfile>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>FALSE</name>
      <anchorfile>__generic_8h.html</anchorfile>
      <anchor>a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ON</name>
      <anchorfile>__generic_8h.html</anchorfile>
      <anchor>a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OFF</name>
      <anchorfile>__generic_8h.html</anchorfile>
      <anchor>a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>YES</name>
      <anchorfile>__generic_8h.html</anchorfile>
      <anchor>a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NO</name>
      <anchorfile>__generic_8h.html</anchorfile>
      <anchor>a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NULL</name>
      <anchorfile>__generic_8h.html</anchorfile>
      <anchor>a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>EOF</name>
      <anchorfile>__generic_8h.html</anchorfile>
      <anchor>a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_</name>
      <anchorfile>__generic_8h.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>(args)</arglist>
    </member>
    <member kind="typedef">
      <type>unsigned</type>
      <name>size_t</name>
      <anchorfile>__generic_8h.html</anchorfile>
      <anchor>a11</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>char</type>
      <name>VOID</name>
      <anchorfile>__generic_8h.html</anchorfile>
      <anchor>a12</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>size_t</type>
      <name>size_type</name>
      <anchorfile>__generic_8h.html</anchorfile>
      <anchor>a13</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>EnumDataOwned</name>
      <anchor>a24</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DataNotOwned</name>
      <anchor>a24a14</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DataOwned</name>
      <anchor>a24a15</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>AcquireOwnership</name>
      <anchor>a24a16</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>AssumeOwnership</name>
      <anchor>a24a17</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>OrderSense</name>
      <anchor>a25</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>increasing</name>
      <anchor>a25a18</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>decreasing</name>
      <anchor>a25a19</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>_math.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/math/</path>
    <filename>__math_8h</filename>
  </compound>
  <compound kind="file">
    <name>AbstractHashTable.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>AbstractHashTable_8h</filename>
    <class kind="class">utilib::AbstractHashTable</class>
    <member kind="function">
      <type>ostream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>AbstractHashTable_8h.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(ostream &amp;output, const utilib::AbstractHashTable&lt; T, KEY, InfoT &gt; &amp;table)</arglist>
    </member>
    <member kind="function">
      <type>istream &amp;</type>
      <name>operator&gt;&gt;</name>
      <anchorfile>AbstractHashTable_8h.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(istream &amp;input, utilib::AbstractHashTable&lt; T, KEY, InfoT &gt; &amp;table)</arglist>
    </member>
    <member kind="variable">
      <type>unsigned long</type>
      <name>utilib_prime_list</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>utilib_num_primes</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>AbstractHeap.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>AbstractHeap_8h</filename>
    <class kind="class">utilib::AbstractHeap</class>
    <member kind="function">
      <type>ostream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>AbstractHeap_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(ostream &amp;output, const utilib::AbstractHeap&lt; T, KEY, Compare &gt; &amp;heap)</arglist>
    </member>
    <member kind="function">
      <type>istream &amp;</type>
      <name>operator&gt;&gt;</name>
      <anchorfile>AbstractHeap_8h.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(istream &amp;input, utilib::AbstractHeap&lt; T, KEY, Compare &gt; &amp;heap)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>AbstractSplayTree.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>AbstractSplayTree_8h</filename>
    <class kind="class">utilib::AbstractSplayTree</class>
    <member kind="define">
      <type>#define</type>
      <name>node_size</name>
      <anchorfile>AbstractSplayTree_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="function">
      <type>ostream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>AbstractSplayTree_8h.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(ostream &amp;output, utilib::AbstractSplayTree&lt; T, KEY &gt; &amp;tree)</arglist>
    </member>
    <member kind="function">
      <type>istream &amp;</type>
      <name>operator&gt;&gt;</name>
      <anchorfile>AbstractSplayTree_8h.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(istream &amp;input, utilib::AbstractSplayTree&lt; T, KEY &gt; &amp;tree)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>Any.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>Any_8h</filename>
    <class kind="class">utilib::AnyValue</class>
    <class kind="class">utilib::AnyValue::placeholder</class>
    <class kind="class">utilib::AnyValue::holder</class>
    <class kind="class">utilib::AnyReference</class>
    <class kind="class">utilib::AnyReference::placeholder</class>
    <class kind="class">utilib::AnyReference::holder</class>
    <class kind="class">utilib::bad_any_cast</class>
    <member kind="function">
      <type>ValueType *</type>
      <name>any_cast</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a76</anchor>
      <arglist>(AnyValue *operand)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>any_cast&lt; bool &gt;</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a83</anchor>
      <arglist>(const AnyReference &amp;operand)</arglist>
    </member>
    <member kind="function">
      <type>ostream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>Any_8h.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(ostream &amp;ostr, const utilib::AnyValue &amp;item)</arglist>
    </member>
    <member kind="function">
      <type>istream &amp;</type>
      <name>operator&gt;&gt;</name>
      <anchorfile>Any_8h.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(istream &amp;istr, utilib::AnyReference &amp;item)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>AnyRNG.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/ranlib/include/</path>
    <filename>AnyRNG_8h</filename>
    <class kind="class">utilib::AnyRNG</class>
    <class kind="class">utilib::AnyRNG::placeholder</class>
    <class kind="class">utilib::AnyRNG::holder</class>
    <member kind="function">
      <type>ostream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>AnyRNG_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(ostream &amp;output, const utilib::AnyRNG &amp;rng)</arglist>
    </member>
    <member kind="function">
      <type>istream &amp;</type>
      <name>operator&gt;&gt;</name>
      <anchorfile>AnyRNG_8h.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(istream &amp;input, utilib::AnyRNG &amp;rng)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>ArrayBase.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>ArrayBase_8h</filename>
    <class kind="class">utilib::ArrayBase</class>
    <member kind="define">
      <type>#define</type>
      <name>ArraySanityChecking</name>
      <anchorfile>ArrayBase_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>Basic2DArray.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>Basic2DArray_8h</filename>
    <class kind="class">utilib::Basic2DArrayRep</class>
    <class kind="class">utilib::Basic2DArray</class>
    <member kind="define">
      <type>#define</type>
      <name>TwoDArraySanityChecking</name>
      <anchorfile>Basic2DArray_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>utilib::Basic2DArray&lt; T &gt; &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>Basic2DArray_8h.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(utilib::Basic2DArray&lt; T &gt; &amp;x, const utilib::Basic2DArray&lt; T &gt; &amp;array)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>Basic3DArray.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>Basic3DArray_8h</filename>
    <class kind="class">utilib::Basic3DArrayRep</class>
    <class kind="class">utilib::Basic3DArray</class>
    <member kind="define">
      <type>#define</type>
      <name>ThreeDArraySanityChecking</name>
      <anchorfile>Basic3DArray_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UTILIB_API</type>
      <name>Basic3DArray</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UTILIB_API</type>
      <name>Simple3DArray</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UTILIB_API</type>
      <name>Num3DArray</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a5</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>BasicArray.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>BasicArray_8h</filename>
    <class kind="class">utilib::BasicArray</class>
    <member kind="function">
      <type>int</type>
      <name>compare</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a85</anchor>
      <arglist>(const BasicArray&lt; T &gt; &amp;first, const BasicArray&lt; T &gt; &amp;second)</arglist>
    </member>
    <member kind="function">
      <type>utilib::BasicArray&lt; T &gt; &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>BasicArray_8h.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(utilib::BasicArray&lt; T &gt; &amp;x, const utilib::BasicArray&lt; V &gt; &amp;y)</arglist>
    </member>
    <member kind="function">
      <type>istream &amp;</type>
      <name>operator&gt;&gt;</name>
      <anchorfile>BasicArray_8h.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(istream &amp;is, utilib::BasicArray&lt; T &gt; &amp;array)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>Binomial.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/ranlib/include/</path>
    <filename>Binomial_8h</filename>
    <class kind="class">utilib::Binomial</class>
  </compound>
  <compound kind="file">
    <name>BitArray.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>BitArray_8h</filename>
    <class kind="class">utilib::BitArray</class>
    <member kind="function">
      <type>utilib::BitArray &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>BitArray_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(utilib::BitArray &amp;x, int val)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>BitArrayBase.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>BitArrayBase_8h</filename>
    <class kind="class">utilib::BitArrayBaseHandle</class>
    <class kind="class">utilib::BitArrayBase</class>
    <member kind="define">
      <type>#define</type>
      <name>BIT_ARRAYS_BY_WORD</name>
      <anchorfile>BitArrayBase_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LOG2BITWORDSIZE</name>
      <anchorfile>BitArrayBase_8h.html</anchorfile>
      <anchor>a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>unsigned int</type>
      <name>bitword</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>unsigned int</type>
      <name>workingBitData</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>ostream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>BitArrayBase_8h.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(ostream &amp;output, const utilib::BitArrayBaseHandle&lt; k, T, P &gt; &amp;array)</arglist>
    </member>
    <member kind="function">
      <type>istream &amp;</type>
      <name>operator&gt;&gt;</name>
      <anchorfile>BitArrayBase_8h.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(istream &amp;input, utilib::BitArrayBaseHandle&lt; k, T, P &gt; &amp;array)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>CachedAllocator.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>CachedAllocator_8h</filename>
    <class kind="struct">utilib::is_cache_allocated</class>
    <class kind="class">utilib::CachedAllocatorObject</class>
    <class kind="class">utilib::CachedAllocator</class>
    <class kind="class">utilib::CacheList</class>
  </compound>
  <compound kind="file">
    <name>Cauchy.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/ranlib/include/</path>
    <filename>Cauchy_8h</filename>
    <class kind="class">utilib::Cauchy</class>
  </compound>
  <compound kind="file">
    <name>CharString.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>CharString_8h</filename>
    <class kind="class">utilib::CharString</class>
    <member kind="function">
      <type>long int</type>
      <name>aslong</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a86</anchor>
      <arglist>(const CharString &amp;str, int &amp;status)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>asdouble</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a87</anchor>
      <arglist>(const CharString &amp;str, int &amp;status)</arglist>
    </member>
    <member kind="function">
      <type>ostream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>CharString_8h.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(ostream &amp;os, const utilib::CharString &amp;array)</arglist>
    </member>
    <member kind="function">
      <type>istream &amp;</type>
      <name>operator&gt;&gt;</name>
      <anchorfile>CharString_8h.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(istream &amp;is, utilib::CharString &amp;array)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>ClassRef.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>ClassRef_8h</filename>
    <class kind="class">utilib::ClassRef</class>
    <class kind="class">utilib::ArrayRef</class>
  </compound>
  <compound kind="file">
    <name>comments.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/io/</path>
    <filename>comments_8h</filename>
    <member kind="function">
      <type>istream &amp;</type>
      <name>comment_lines</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a25</anchor>
      <arglist>(istream &amp;ins, int &amp;line_counter)</arglist>
    </member>
    <member kind="function">
      <type>istream &amp;</type>
      <name>whitespace</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a27</anchor>
      <arglist>(istream &amp;ins)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>CommonIO.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/io/</path>
    <filename>CommonIO_8h</filename>
    <class kind="class">utilib::CommonIO</class>
    <member kind="define">
      <type>#define</type>
      <name>DEBUGPR</name>
      <anchorfile>CommonIO_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(dlevel, cmd)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEBUGPRP</name>
      <anchorfile>CommonIO_8h.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(dlevel, cmd)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEBUGPRX</name>
      <anchorfile>CommonIO_8h.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(dlevel, ptr, stuff)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEBUGPRXP</name>
      <anchorfile>CommonIO_8h.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(dlevel, ptr, stuff)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>REFER_DEBUG</name>
      <anchorfile>CommonIO_8h.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(pointer)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ucout</name>
      <anchorfile>CommonIO_8h.html</anchorfile>
      <anchor>a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ucerr</name>
      <anchorfile>CommonIO_8h.html</anchorfile>
      <anchor>a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ucin</name>
      <anchorfile>CommonIO_8h.html</anchorfile>
      <anchor>a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>stdcout</name>
      <anchorfile>CommonIO_8h.html</anchorfile>
      <anchor>a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>stdcerr</name>
      <anchorfile>CommonIO_8h.html</anchorfile>
      <anchor>a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>stdcin</name>
      <anchorfile>CommonIO_8h.html</anchorfile>
      <anchor>a10</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>cout_print</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a28</anchor>
      <arglist>(const char *str)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>cerr_print</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a29</anchor>
      <arglist>(const char *str)</arglist>
    </member>
    <member kind="function">
      <type>utilib::CommonIO &amp;</type>
      <name>global_CommonIO</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a30</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>ostream &amp;</type>
      <name>Flush</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a31</anchor>
      <arglist>(ostream &amp;outstr)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>compare.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/sort/</path>
    <filename>compare_8h</filename>
    <class kind="struct">utilib::Comparison</class>
    <class kind="struct">utilib::ComparisonBase</class>
    <class kind="struct">utilib::SimpleCompare</class>
    <class kind="class">utilib::Reverse</class>
    <member kind="function">
      <type>int</type>
      <name>compare</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a173</anchor>
      <arglist>(const T &amp;x, const T &amp;y)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>default_rng.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/ranlib/include/</path>
    <filename>default__rng_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>ranf</name>
      <anchorfile>default__rng_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>set_global_runif</name>
      <anchorfile>default__rng_8h.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(func)</arglist>
    </member>
    <member kind="function">
      <type>REAL</type>
      <name>LEC_runif</name>
      <anchorfile>default__rng_8h.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable">
      <type>PM_LCG</type>
      <name>default_rng</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>REAL(*</type>
      <name>global_runif</name>
      <anchorfile>default__rng_8h.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>)(void)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>DoubleMatrix.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>DoubleMatrix_8h</filename>
    <member kind="typedef">
      <type>Num2DArray&lt; double &gt;</type>
      <name>DoubleMatrix</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a8</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>DoubleVector.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>DoubleVector_8h</filename>
    <member kind="typedef">
      <type>NumArray&lt; double &gt;</type>
      <name>DoubleVector</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a9</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>DUniform.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/ranlib/include/</path>
    <filename>DUniform_8h</filename>
    <class kind="class">utilib::DUniform</class>
  </compound>
  <compound kind="file">
    <name>EnumBitArray.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>EnumBitArray_8h</filename>
    <class kind="class">utilib::EnumBitArray</class>
  </compound>
  <compound kind="file">
    <name>Ereal.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>Ereal_8h</filename>
    <class kind="class">utilib::Ereal</class>
    <member kind="function">
      <type>Ereal&lt; Type &gt;</type>
      <name>operator+</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a88</anchor>
      <arglist>(const Ereal&lt; Type &gt; &amp;x, const Ereal&lt; Type &gt; &amp;y)</arglist>
    </member>
    <member kind="function">
      <type>Ereal&lt; Type &gt;</type>
      <name>operator-</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a91</anchor>
      <arglist>(const Ereal&lt; Type &gt; &amp;x, const Ereal&lt; Type &gt; &amp;y)</arglist>
    </member>
    <member kind="function">
      <type>Ereal&lt; Type &gt;</type>
      <name>operator *</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a94</anchor>
      <arglist>(const Ereal&lt; Type &gt; &amp;x, const Ereal&lt; Type &gt; &amp;y)</arglist>
    </member>
    <member kind="function">
      <type>Ereal&lt; Type &gt;</type>
      <name>operator/</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a97</anchor>
      <arglist>(const Ereal&lt; Type &gt; &amp;x, const Ereal&lt; Type &gt; &amp;y)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a102</anchor>
      <arglist>(const Ereal&lt; Type &gt; &amp;x, const Ereal&lt; Type &gt; &amp;y)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator!=</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a105</anchor>
      <arglist>(const Ereal&lt; Type &gt; &amp;x, const Ereal&lt; Type &gt; &amp;y)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&gt;</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a108</anchor>
      <arglist>(const Ereal&lt; Type &gt; &amp;x, const Ereal&lt; Type &gt; &amp;y)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&gt;=</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a111</anchor>
      <arglist>(const Ereal&lt; Type &gt; &amp;x, const Ereal&lt; Type &gt; &amp;y)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a114</anchor>
      <arglist>(const Ereal&lt; Type &gt; &amp;x, const Ereal&lt; Type &gt; &amp;y)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;=</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a117</anchor>
      <arglist>(const Ereal&lt; Type &gt; &amp;x, const Ereal&lt; Type &gt; &amp;y)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isinf</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a120</anchor>
      <arglist>(const Ereal&lt; Type &gt; &amp;num)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isnan</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a121</anchor>
      <arglist>(const Ereal&lt; Type &gt; &amp;num)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isind</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a122</anchor>
      <arglist>(const Ereal&lt; Type &gt; &amp;num)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>finite</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a123</anchor>
      <arglist>(const Ereal&lt; Type &gt; &amp;num)</arglist>
    </member>
    <member kind="function">
      <type>long int</type>
      <name>lround</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a124</anchor>
      <arglist>(Ereal&lt; Type &gt; &amp;val)</arglist>
    </member>
    <member kind="function">
      <type>istream &amp;</type>
      <name>operator&gt;&gt;</name>
      <anchorfile>Ereal_8h.html</anchorfile>
      <anchor>a15</anchor>
      <arglist>(istream &amp;is, utilib::Ereal&lt; Type &gt; &amp;num)</arglist>
    </member>
    <member kind="function">
      <type>ostream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>Ereal_8h.html</anchorfile>
      <anchor>a16</anchor>
      <arglist>(ostream &amp;os, const utilib::Ereal&lt; Type &gt; &amp;num)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>errmsg.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/sys/</path>
    <filename>errmsg_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>Warning</name>
      <anchorfile>errmsg_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(str)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ErrReturn</name>
      <anchorfile>errmsg_8h.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(str)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ErrAbort</name>
      <anchorfile>errmsg_8h.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(str)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ErrExit</name>
      <anchorfile>errmsg_8h.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(str)</arglist>
    </member>
    <member kind="typedef">
      <type>void(*</type>
      <name>null_fn_type</name>
      <anchorfile>errmsg_8h.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>)(void)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>errmsg</name>
      <anchorfile>errmsg_8h.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(const char *string,...)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>errmsg_abort</name>
      <anchorfile>errmsg_8h.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(const int flag)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>errmsg_exit_fn</name>
      <anchorfile>errmsg_8h.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>(null_fn_type fn)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DoWarning</name>
      <anchorfile>errmsg_8h.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>(const char *file, const int line, const char *str)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DoErrAbort</name>
      <anchorfile>errmsg_8h.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>(const char *file, const int line, const char *str)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>DoErrExit</name>
      <anchorfile>errmsg_8h.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>(const char *file, const int line, const char *str)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>exception_mngr.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/sys/</path>
    <filename>exception__mngr_8h</filename>
    <namespace>utilib::exception_mngr</namespace>
    <member kind="define">
      <type>#define</type>
      <name>EXCEPTION_TEST</name>
      <anchorfile>exception__mngr_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(exception_test, Exception, msg)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>EXCEPTION_MNGR</name>
      <anchorfile>exception__mngr_8h.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(Exception, msg)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>STD_CATCH</name>
      <anchorfile>exception__mngr_8h.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(cmd)</arglist>
    </member>
    <member kind="typedef">
      <type>void(*</type>
      <name>null_fn_type</name>
      <anchorfile>namespaceutilib_1_1exception__mngr.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>)(void)</arglist>
    </member>
    <member kind="enumeration">
      <name>handle_t</name>
      <anchor>a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Standard</name>
      <anchor>a9a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Abort</name>
      <anchor>a9a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Exit</name>
      <anchor>a9a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>exit_fn</name>
      <anchorfile>exception__mngr_8h.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_mode</name>
      <anchorfile>namespaceutilib_1_1exception__mngr.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(handle_t)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_exit_function</name>
      <anchorfile>namespaceutilib_1_1exception__mngr.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>(null_fn_type fn)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>handle_exception</name>
      <anchorfile>namespaceutilib_1_1exception__mngr.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>(ostringstream &amp;msg)</arglist>
    </member>
    <member kind="variable">
      <type>handle_t</type>
      <name>mode</name>
      <anchorfile>namespaceutilib_1_1exception__mngr.html</anchorfile>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>null_fn_type</type>
      <name>exit_function</name>
      <anchorfile>namespaceutilib_1_1exception__mngr.html</anchorfile>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>Exponential.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/ranlib/include/</path>
    <filename>Exponential_8h</filename>
    <class kind="class">utilib::Exponential</class>
  </compound>
  <compound kind="file">
    <name>general.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/sys/</path>
    <filename>general_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>FALSE</name>
      <anchorfile>general_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TRUE</name>
      <anchorfile>general_8h.html</anchorfile>
      <anchor>a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DO_COMMAND_TAB</name>
      <anchorfile>general_8h.html</anchorfile>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DO_COMMAND_EOLN</name>
      <anchorfile>general_8h.html</anchorfile>
      <anchor>a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SIMPLE_FLAG_OPTN</name>
      <anchorfile>general_8h.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(string, var)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>FLAG_OPTN</name>
      <anchorfile>general_8h.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(string, stmt)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CFLAG_OPTN</name>
      <anchorfile>general_8h.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(char, stmt)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SIMPLE_CFLAG_OPTN</name>
      <anchorfile>general_8h.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>(char, var)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DATA_OPTN</name>
      <anchorfile>general_8h.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>(string, desc, stmt)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DATA_OPTN2</name>
      <anchorfile>general_8h.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>(string, desc, stmt1, stmt2)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NON_SWITCH</name>
      <anchorfile>general_8h.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>(desc, stmt)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>FREE_ARGS_CONT</name>
      <anchorfile>general_8h.html</anchorfile>
      <anchor>a11</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>FREE_ARGS</name>
      <anchorfile>general_8h.html</anchorfile>
      <anchor>a12</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>REQ_ARG</name>
      <anchorfile>general_8h.html</anchorfile>
      <anchor>a13</anchor>
      <arglist>(desc, stmt)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OPT_ARG</name>
      <anchorfile>general_8h.html</anchorfile>
      <anchor>a14</anchor>
      <arglist>(desc, stmt)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ANY_OPTION</name>
      <anchorfile>general_8h.html</anchorfile>
      <anchor>a15</anchor>
      <arglist>(desc, stmt)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>COMMAND_LINE_ERROR</name>
      <anchorfile>general_8h.html</anchorfile>
      <anchor>a16</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DO_STANDARD_COMMAND_LINE</name>
      <anchorfile>general_8h.html</anchorfile>
      <anchor>a17</anchor>
      <arglist>(stmts)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>GenericHashTable.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>GenericHashTable_8h</filename>
    <class kind="class">utilib::GenericHashTableItem</class>
    <class kind="class">utilib::GenericHashTable</class>
  </compound>
  <compound kind="file">
    <name>GenericHeap.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>GenericHeap_8h</filename>
    <class kind="struct">utilib::GenericHeapCompare</class>
    <class kind="class">utilib::GenericHeapItem</class>
    <class kind="class">utilib::GenericHeapBase</class>
    <class kind="class">utilib::GenericHeap</class>
    <class kind="class">utilib::LocatedGenericHeap</class>
  </compound>
  <compound kind="file">
    <name>GenericSplayTree.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>GenericSplayTree_8h</filename>
    <class kind="class">utilib::GenericSplayTreeItem</class>
    <class kind="class">utilib::GenericSplayTree</class>
    <member kind="variable">
      <type>UTILIB_API</type>
      <name>GenericSplayTree</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a10</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>Geometric.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/ranlib/include/</path>
    <filename>Geometric_8h</filename>
    <class kind="class">utilib::Geometric</class>
  </compound>
  <compound kind="file">
    <name>hash_fn.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>hash__fn_8h</filename>
    <member kind="typedef">
      <type>unsigned long int</type>
      <name>ub4</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a11</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>unsigned char</type>
      <name>ub1</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a12</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>ub4</type>
      <name>bjlookup</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a125</anchor>
      <arglist>(register ub1 *k, register ub4 length, register ub4 level)</arglist>
    </member>
    <member kind="function">
      <type>unsigned long int</type>
      <name>hash_bj</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a126</anchor>
      <arglist>(const double &amp;val, unsigned long int level=0)</arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>hash_fn</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a134</anchor>
      <arglist>(const T &amp;key, size_type table_size)</arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>hash_fn1</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a137</anchor>
      <arglist>(const CharString &amp;key, size_type table_size)</arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>hash_fn2</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a138</anchor>
      <arglist>(const CharString &amp;key, size_type table_size)</arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>hash_fn3</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a139</anchor>
      <arglist>(const CharString &amp;key, size_type table_size)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>HashedSet.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>HashedSet_8h</filename>
    <class kind="class">utilib::__LinkedList_HashedSet_OpClass</class>
    <class kind="class">utilib::HashedSetBase</class>
    <class kind="class">utilib::HashedSetBase&lt; _Key, false &gt;</class>
    <class kind="class">utilib::HashedSet</class>
    <member kind="function">
      <type>bool</type>
      <name>is_disjoint</name>
      <anchorfile>HashedSet_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(const utilib::HashedSet&lt; _Key, simple_flag &gt; &amp;set1, const utilib::HashedSet&lt; _Key, simple_flag &gt; &amp;set2)</arglist>
    </member>
    <member kind="function">
      <type>ostream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>HashedSet_8h.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(ostream &amp;output, const utilib::HashedSet&lt; _Key, simple_flag &gt; &amp;item)</arglist>
    </member>
    <member kind="function">
      <type>istream &amp;</type>
      <name>operator&gt;&gt;</name>
      <anchorfile>HashedSet_8h.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(istream &amp;input, utilib::HashedSet&lt; _Key, simple_flag &gt; &amp;table)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>IntMatrix.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>IntMatrix_8h</filename>
    <member kind="typedef">
      <type>Num2DArray&lt; int &gt;</type>
      <name>IntMatrix</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a13</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>IntVector.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>IntVector_8h</filename>
    <member kind="typedef">
      <type>NumArray&lt; int &gt;</type>
      <name>IntVector</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a14</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>LCG.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/ranlib/include/</path>
    <filename>LCG_8h</filename>
    <class kind="class">utilib::LCG</class>
  </compound>
  <compound kind="file">
    <name>LEC_rng.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/ranlib/include/</path>
    <filename>LEC__rng_8h</filename>
    <member kind="function">
      <type>long</type>
      <name>mltmod</name>
      <anchorfile>LEC__rng_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(long a, long s, long m)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>advnst</name>
      <anchorfile>LEC__rng_8h.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(long k)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>phrtsd</name>
      <anchorfile>LEC__rng_8h.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(char *phrase, long *seed1, long *seed2)</arglist>
    </member>
    <member kind="function">
      <type>REAL</type>
      <name>LEC_runif</name>
      <anchorfile>LEC__rng_8h.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setall</name>
      <anchorfile>LEC__rng_8h.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(long iseed1, long iseed2)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setant</name>
      <anchorfile>LEC__rng_8h.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(long qvalue)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setgmn</name>
      <anchorfile>LEC__rng_8h.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(REAL *meanv, REAL *covm, long p, REAL *parm)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setsd</name>
      <anchorfile>LEC__rng_8h.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>(long iseed1, long iseed2)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>getsd</name>
      <anchorfile>LEC__rng_8h.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>(long *iseed1, long *iseed2)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>initgn</name>
      <anchorfile>LEC__rng_8h.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>(long isdtyp)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>gscgn</name>
      <anchorfile>LEC__rng_8h.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>(long getset, long *g)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>LinkedList.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>LinkedList_8h</filename>
    <class kind="class">utilib::ListItem</class>
    <class kind="class">utilib::__LinkedList_iterator_base</class>
    <class kind="class">utilib::__LinkedList_Standard_OpClass</class>
    <class kind="class">utilib::__LinkedList_Pointer_OpClass</class>
    <class kind="class">utilib::__LinkedList_iterator</class>
    <class kind="class">utilib::__LinkedList_ptr_iterator</class>
    <class kind="class">utilib::LinkedList</class>
    <member kind="function">
      <type>ostream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>LinkedList_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(ostream &amp;os, const utilib::LinkedList&lt; _Tp, _Alloc &gt; &amp;obj)</arglist>
    </member>
    <member kind="function">
      <type>istream &amp;</type>
      <name>operator&gt;&gt;</name>
      <anchorfile>LinkedList_8h.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(istream &amp;is, utilib::LinkedList&lt; _Tp, _Alloc &gt; &amp;obj)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>linpack.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/math/</path>
    <filename>linpack_8h</filename>
    <member kind="function">
      <type>void dpofa_</type>
      <name>_</name>
      <anchorfile>linpack_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>((double *A, int *lda, int *n, int *info))</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>logEvent.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/io/</path>
    <filename>logEvent_8h</filename>
    <class kind="class">utilib::logEvent</class>
    <member kind="define">
      <type>#define</type>
      <name>IF_LOGGING_COMPILED</name>
      <anchorfile>logEvent_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(arbitraryCode)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>IF_LOGGING_EVENTS</name>
      <anchorfile>logEvent_8h.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(level, action)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LOG_EVENT</name>
      <anchorfile>logEvent_8h.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(level, how, state)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LOG_EVENT_CONDITIONAL</name>
      <anchorfile>logEvent_8h.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(level, cond, how, state)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>EVENT_LOGGING_PRESENT</name>
      <anchorfile>logEvent_8h.html</anchorfile>
      <anchor>a4</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>LogNormal.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/ranlib/include/</path>
    <filename>LogNormal_8h</filename>
    <class kind="class">utilib::LogNormal</class>
  </compound>
  <compound kind="file">
    <name>LPHashTable.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>LPHashTable_8h</filename>
    <class kind="class">utilib::LPHashTableItem</class>
    <class kind="class">utilib::LPHashTable</class>
  </compound>
  <compound kind="file">
    <name>math_array.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/math/</path>
    <filename>math__array_8h</filename>
    <member kind="function">
      <type>T</type>
      <name>sum</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a38</anchor>
      <arglist>(const BasicArray&lt; T &gt; &amp;vec)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>shuffle</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a39</anchor>
      <arglist>(BasicArray&lt; T &gt; &amp;vec, RNGT *rng, size_type num)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>subshuffle</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a44</anchor>
      <arglist>(BasicArray&lt; T &gt; &amp;vec, RNGT *rng, Index1T start, Index2T end)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>length</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a45</anchor>
      <arglist>(const BasicArray&lt; T &gt; &amp;a)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>dist</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a46</anchor>
      <arglist>(const BasicArray&lt; T &gt; &amp;a, const BasicArray&lt; T &gt; &amp;b)</arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>argmin</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a47</anchor>
      <arglist>(const BasicArray&lt; T &gt; &amp;vector)</arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>argmax</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a48</anchor>
      <arglist>(const BasicArray&lt; T &gt; &amp;vector)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>rand_select</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a49</anchor>
      <arglist>(double *x, int p, int r, int i, int *ws, DUniform &amp;drnd)</arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>argmedian</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a50</anchor>
      <arglist>(double *x, size_type n, int *ws, RNGT *rng)</arglist>
    </member>
    <member kind="function">
      <type>T</type>
      <name>min</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a56</anchor>
      <arglist>(const BasicArray&lt; T &gt; &amp;vec)</arglist>
    </member>
    <member kind="function">
      <type>T</type>
      <name>max</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a57</anchor>
      <arglist>(const BasicArray&lt; T &gt; &amp;vec)</arglist>
    </member>
    <member kind="function">
      <type>T</type>
      <name>median</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a58</anchor>
      <arglist>(const BasicArray&lt; T &gt; &amp;vec)</arglist>
    </member>
    <member kind="function">
      <type>BasicArray&lt; T &gt;</type>
      <name>concat</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a59</anchor>
      <arglist>(const BasicArray&lt; T &gt; &amp;a, const BasicArray&lt; T &gt; &amp;b)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>mean</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a60</anchor>
      <arglist>(const BasicArray&lt; T &gt; &amp;array)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>trimmed_mean</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a61</anchor>
      <arglist>(BasicArray&lt; T &gt; &amp;array, const double percent)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>var</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a62</anchor>
      <arglist>(const BasicArray&lt; T &gt; &amp;array, const int sampleflag=TRUE)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>mad</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a63</anchor>
      <arglist>(BasicArray&lt; T &gt; &amp;array, BasicArray&lt; double &gt; &amp;work)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>lapply</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a65</anchor>
      <arglist>(BasicArray&lt; T &gt; &amp;vec, double(*func)(T val))</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>inner_product</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a66</anchor>
      <arglist>(const BasicArray&lt; T &gt; &amp;v1, const BasicArray&lt; T &gt; &amp;v2)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>d_round</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a67</anchor>
      <arglist>(const VectorT &amp;source, unsigned int num_digits, VectorT &amp;dest)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>math_basic.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/math/</path>
    <filename>math__basic_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>MAXINT</name>
      <anchorfile>math__basic_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Exp</name>
      <anchorfile>math__basic_8h.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>calc_filesize</name>
      <anchorfile>math__basic_8h.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(char *filename)</arglist>
    </member>
    <member kind="function">
      <type>long int</type>
      <name>lround</name>
      <anchorfile>math__basic_8h.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(double x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>d_round</name>
      <anchorfile>math__basic_8h.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(double to_round, unsigned int num_digits)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setup_bufexp</name>
      <anchorfile>math__basic_8h.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(int tabsz, double xmin, double xmax)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>bufexp</name>
      <anchorfile>math__basic_8h.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>(double x)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>isinf</name>
      <anchorfile>math__basic_8h.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>(double x)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>isnan</name>
      <anchorfile>math__basic_8h.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>(double x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>exp2</name>
      <anchorfile>math__basic_8h.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>(double x)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>bzero</name>
      <anchorfile>math__basic_8h.html</anchorfile>
      <anchor>a11</anchor>
      <arglist>(char *b, int length)</arglist>
    </member>
    <member kind="variable">
      <type>const long double</type>
      <name>PI</name>
      <anchorfile>math__basic_8h.html</anchorfile>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>math_matrix.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/math/</path>
    <filename>math__matrix_8h</filename>
    <member kind="function">
      <type>NumArray&lt; double &gt;</type>
      <name>mean</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a68</anchor>
      <arglist>(const Basic2DArray&lt; T &gt; &amp;matrix, const int stats_flag)</arglist>
    </member>
    <member kind="function">
      <type>DoubleVector</type>
      <name>var</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a69</anchor>
      <arglist>(const Basic2DArray&lt; T &gt; &amp;mat, const int sampleflag=TRUE)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>min</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a71</anchor>
      <arglist>(const Basic2DArray&lt; T &gt; &amp;mat)</arglist>
    </member>
    <member kind="function">
      <type>T</type>
      <name>max</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a72</anchor>
      <arglist>(const Basic2DArray&lt; T &gt; &amp;mat)</arglist>
    </member>
    <member kind="function">
      <type>T</type>
      <name>sum</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a73</anchor>
      <arglist>(const Basic2DArray&lt; T &gt; &amp;mat)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>rowscale</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a74</anchor>
      <arglist>(Basic2DArray&lt; T &gt; &amp;a)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>cholesky</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a75</anchor>
      <arglist>(DoubleMatrix &amp;A, DoubleMatrix &amp;G, int n)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>memdebug.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/sys/</path>
    <filename>memdebug_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>MEMDEBUG_START_NEW</name>
      <anchorfile>memdebug_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(this)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MEMDEBUG_END_NEW</name>
      <anchorfile>memdebug_8h.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(this)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MEMDEBUG_START_RESIZE</name>
      <anchorfile>memdebug_8h.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(this)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MEMDEBUG_END_RESIZE</name>
      <anchorfile>memdebug_8h.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(this)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MEMDEBUG_START_DEL</name>
      <anchorfile>memdebug_8h.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(this)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MEMDEBUG_END_DEL</name>
      <anchorfile>memdebug_8h.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(this)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MEMDEBUG_DUMP</name>
      <anchorfile>memdebug_8h.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(os)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>MixedIntVars.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>MixedIntVars_8h</filename>
    <class kind="class">utilib::MixedIntVarsRep</class>
    <class kind="class">utilib::MixedIntVars</class>
    <member kind="function">
      <type>utilib::PackBuffer &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>MixedIntVars_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(utilib::PackBuffer &amp;buff, const utilib::MixedIntVars &amp;vars)</arglist>
    </member>
    <member kind="function">
      <type>utilib::UnPackBuffer &amp;</type>
      <name>operator&gt;&gt;</name>
      <anchorfile>MixedIntVars_8h.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(utilib::UnPackBuffer &amp;buff, utilib::MixedIntVars &amp;vars)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>MNormal.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/ranlib/include/</path>
    <filename>MNormal_8h</filename>
    <class kind="class">utilib::MNormal</class>
  </compound>
  <compound kind="file">
    <name>mpi_utilib.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/sys/</path>
    <filename>mpi__utilib_8h</filename>
    <member kind="typedef">
      <type>void *</type>
      <name>MPI_Request</name>
      <anchorfile>mpi__utilib_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>void *</type>
      <name>MPI_Status</name>
      <anchorfile>mpi__utilib_8h.html</anchorfile>
      <anchor>a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>MPI_Datatype</type>
      <name>mpi_datatype</name>
      <anchorfile>mpi__utilib_8h.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>MPI_Datatype</type>
      <name>mpi_datatype&lt; char &gt;</name>
      <anchorfile>mpi__utilib_8h.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>MPI_Datatype</type>
      <name>mpi_datatype&lt; char const  &gt;</name>
      <anchorfile>mpi__utilib_8h.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>MPI_Datatype</type>
      <name>mpi_datatype&lt; unsigned char &gt;</name>
      <anchorfile>mpi__utilib_8h.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>MPI_Datatype</type>
      <name>mpi_datatype&lt; unsigned char const  &gt;</name>
      <anchorfile>mpi__utilib_8h.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>MPI_Datatype</type>
      <name>mpi_datatype&lt; short &gt;</name>
      <anchorfile>mpi__utilib_8h.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>MPI_Datatype</type>
      <name>mpi_datatype&lt; unsigned short &gt;</name>
      <anchorfile>mpi__utilib_8h.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>MPI_Datatype</type>
      <name>mpi_datatype&lt; int &gt;</name>
      <anchorfile>mpi__utilib_8h.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>MPI_Datatype</type>
      <name>mpi_datatype&lt; unsigned int &gt;</name>
      <anchorfile>mpi__utilib_8h.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>MPI_Datatype</type>
      <name>mpi_datatype&lt; long &gt;</name>
      <anchorfile>mpi__utilib_8h.html</anchorfile>
      <anchor>a11</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>MPI_Datatype</type>
      <name>mpi_datatype&lt; unsigned long &gt;</name>
      <anchorfile>mpi__utilib_8h.html</anchorfile>
      <anchor>a12</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>MPI_Datatype</type>
      <name>mpi_datatype&lt; float &gt;</name>
      <anchorfile>mpi__utilib_8h.html</anchorfile>
      <anchor>a13</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>MPI_Datatype</type>
      <name>mpi_datatype&lt; double &gt;</name>
      <anchorfile>mpi__utilib_8h.html</anchorfile>
      <anchor>a14</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>MPI_Datatype</type>
      <name>mpi_datatype&lt; long double &gt;</name>
      <anchorfile>mpi__utilib_8h.html</anchorfile>
      <anchor>a15</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>MPI_Datatype</type>
      <name>mpi_datatype&lt; long long int &gt;</name>
      <anchorfile>mpi__utilib_8h.html</anchorfile>
      <anchor>a16</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>PackSize</name>
      <anchorfile>mpi__utilib_8h.html</anchorfile>
      <anchor>a17</anchor>
      <arglist>(const TYPE &amp;data, const int num=1)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>mpiUtil.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/io/</path>
    <filename>mpiUtil_8h</filename>
    <class kind="class">utilib::uMPI</class>
    <member kind="define">
      <type>#define</type>
      <name>LOG_SEND</name>
      <anchorfile>mpiUtil_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(a, b, c, d)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LOG_RECV</name>
      <anchorfile>mpiUtil_8h.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(s)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>MUniform.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/ranlib/include/</path>
    <filename>MUniform_8h</filename>
    <class kind="class">utilib::MUniform</class>
  </compound>
  <compound kind="file">
    <name>nicePrint.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/io/</path>
    <filename>nicePrint_8h</filename>
    <member kind="function">
      <type>int</type>
      <name>digitsNeededFor</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a32</anchor>
      <arglist>(double value)</arglist>
    </member>
    <member kind="function">
      <type>std::ostream &amp;</type>
      <name>hyphens</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a33</anchor>
      <arglist>(std::ostream &amp;stream, int n)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>plural</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a34</anchor>
      <arglist>(int count, const char *suffix=&quot;s&quot;)</arglist>
    </member>
    <member kind="function">
      <type>std::ostream &amp;</type>
      <name>printPercent</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a35</anchor>
      <arglist>(std::ostream &amp;stream, double numerator, double denominator=1)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>Normal.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/ranlib/include/</path>
    <filename>Normal_8h</filename>
    <class kind="class">utilib::Normal</class>
  </compound>
  <compound kind="file">
    <name>Num2DArray.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>Num2DArray_8h</filename>
    <class kind="class">utilib::Num2DArray</class>
    <member kind="define">
      <type>#define</type>
      <name>BINARYOP</name>
      <anchorfile>Num2DArray_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(opname, pseudonym)</arglist>
    </member>
    <member kind="function">
      <type>Num2DArray&lt; T &gt;</type>
      <name>operator%</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a163</anchor>
      <arglist>(const Num2DArray&lt; T &gt; &amp;m1, const Num2DArray&lt; T &gt; &amp;m2)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>matmult</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a166</anchor>
      <arglist>(Num2DArray&lt; T &gt; &amp;res, const Num2DArray&lt; T &gt; &amp;m1, const Num2DArray&lt; T &gt; &amp;m2)</arglist>
    </member>
    <member kind="variable">
      <type>UTILIB_API</type>
      <name>Num2DArray</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a15</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>NumArray.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>NumArray_8h</filename>
    <class kind="class">utilib::NumArray</class>
    <member kind="define">
      <type>#define</type>
      <name>BINARYOP</name>
      <anchorfile>NumArray_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(opname, pseudonym)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>OrderedList.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>OrderedList_8h</filename>
    <class kind="class">utilib::OrderedListItem</class>
    <class kind="class">utilib::OrderedList</class>
    <member kind="variable">
      <type>UTILIB_API</type>
      <name>OrderedListItem</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a16</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>UTILIB_API</type>
      <name>OrderedList</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a17</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>OrderedSet.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>OrderedSet_8h</filename>
    <class kind="class">utilib::OrderedSet</class>
    <member kind="define">
      <type>#define</type>
      <name>ForAllElements</name>
      <anchorfile>OrderedSet_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(index, set, elt_type)</arglist>
    </member>
    <member kind="typedef">
      <type>size_type</type>
      <name>OrderedSetIndex</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a18</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>utilib::OrderedSet&lt; T &gt; &amp;</type>
      <name>operator&gt;&gt;</name>
      <anchorfile>OrderedSet_8h.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(utilib::OrderedSet&lt; T &gt; &amp;list, T &amp;val)</arglist>
    </member>
    <member kind="function">
      <type>utilib::OrderedSet&lt; T &gt; &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>OrderedSet_8h.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(utilib::OrderedSet&lt; T &gt; &amp;list, const T &amp;val)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>PackBuf.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/io/</path>
    <filename>PackBuf_8h</filename>
    <class kind="class">utilib::PackBuffer</class>
    <class kind="class">utilib::UnPackBuffer</class>
    <member kind="function">
      <type>utilib::PackBuffer &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>PackBuf_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(utilib::PackBuffer &amp;buff, const void *data)</arglist>
    </member>
    <member kind="function">
      <type>utilib::UnPackBuffer &amp;</type>
      <name>operator&gt;&gt;</name>
      <anchorfile>PackBuf_8h.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(utilib::UnPackBuffer &amp;buff, void *data)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>Parameter.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/io/</path>
    <filename>Parameter_8h</filename>
    <class kind="class">utilib::Parameter</class>
    <member kind="function">
      <type>std::ostream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>Parameter_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(std::ostream &amp;ostr, const utilib::Parameter &amp;item)</arglist>
    </member>
    <member kind="function">
      <type>std::istream &amp;</type>
      <name>operator&gt;&gt;</name>
      <anchorfile>Parameter_8h.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(std::istream &amp;istr, utilib::Parameter &amp;item)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>ParameterList.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/io/</path>
    <filename>ParameterList_8h</filename>
    <class kind="class">utilib::ParameterList</class>
    <class kind="class">utilib::ParameterList::ParameterListItem</class>
  </compound>
  <compound kind="file">
    <name>ParameterSet.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/io/</path>
    <filename>ParameterSet_8h</filename>
    <class kind="class">utilib::ParamFileCallback</class>
    <class kind="class">utilib::ParameterSet</class>
    <member kind="function">
      <type>std::ostream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>ParameterSet_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(std::ostream &amp;ostr, const utilib::ParameterSet &amp;item)</arglist>
    </member>
    <member kind="function">
      <type>std::istream &amp;</type>
      <name>operator&gt;&gt;</name>
      <anchorfile>ParameterSet_8h.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(std::istream &amp;istr, utilib::ParameterSet &amp;item)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>paramTable.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/io/</path>
    <filename>paramTable_8h</filename>
    <class kind="class">utilib::parameter</class>
    <member kind="define">
      <type>#define</type>
      <name>CLASS_PARAMETER</name>
      <anchorfile>paramTable_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(name)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GLOBAL_PARAMETER</name>
      <anchorfile>paramTable_8h.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(name)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CLASS_PARAMETER_DEF</name>
      <anchorfile>paramTable_8h.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(class, name, min, default, max)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GLOBAL_PARAMETER_DEF</name>
      <anchorfile>paramTable_8h.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(name, min, default, max)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>ParkMiller.c</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/ranlib/misc/</path>
    <filename>ParkMiller_8c</filename>
    <member kind="define">
      <type>#define</type>
      <name>MPLIER</name>
      <anchorfile>ParkMiller_8c.html</anchorfile>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MODLUS</name>
      <anchorfile>ParkMiller_8c.html</anchorfile>
      <anchor>a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MOBYMP</name>
      <anchorfile>ParkMiller_8c.html</anchorfile>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MOMDMP</name>
      <anchorfile>ParkMiller_8c.html</anchorfile>
      <anchor>a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>PMrand</name>
      <anchorfile>ParkMiller_8c.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(int *state)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>PersistentPointer.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>PersistentPointer_8h</filename>
    <class kind="class">utilib::PersistentPointer</class>
  </compound>
  <compound kind="file">
    <name>PM_LCG.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/ranlib/include/</path>
    <filename>PM__LCG_8h</filename>
    <class kind="class">utilib::PM_LCG</class>
  </compound>
  <compound kind="file">
    <name>pvector.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>pvector_8h</filename>
    <class kind="class">utilib::pvector</class>
    <member kind="define">
      <type>#define</type>
      <name>BINARYOP</name>
      <anchorfile>pvector_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(opname, pseudonym)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>QueueArray.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>QueueArray_8h</filename>
    <class kind="class">utilib::QueueArray</class>
    <member kind="function">
      <type>utilib::QueueArray&lt; T &gt; &amp;</type>
      <name>operator&gt;&gt;</name>
      <anchorfile>QueueArray_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(utilib::QueueArray&lt; T &gt; &amp;list, T &amp;val)</arglist>
    </member>
    <member kind="function">
      <type>utilib::QueueArray&lt; T &gt; &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>QueueArray_8h.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(utilib::QueueArray&lt; T &gt; &amp;list, const T &amp;val)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>QueueList.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>QueueList_8h</filename>
    <class kind="class">utilib::QueueList</class>
  </compound>
  <compound kind="file">
    <name>Random.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/ranlib/include/</path>
    <filename>Random_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>_</name>
      <anchorfile>Random_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(args)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>snorm</name>
      <anchorfile>Random_8h.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ABS</name>
      <anchorfile>Random_8h.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MIN</name>
      <anchorfile>Random_8h.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(a, b)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX</name>
      <anchorfile>Random_8h.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(a, b)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ranf</name>
      <anchorfile>Random_8h.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>set_global_runif</name>
      <anchorfile>Random_8h.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(func)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>RandomVariable.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/ranlib/include/</path>
    <filename>RandomVariable_8h</filename>
    <class kind="class">utilib::RandomVariableBase</class>
    <class kind="class">utilib::SimpleRandomVariable</class>
    <class kind="class">utilib::GeneralRandomVariable</class>
    <class kind="class">utilib::ExternalRandomVariable</class>
    <member kind="function">
      <type>void</type>
      <name>set_global_RNG</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a36</anchor>
      <arglist>(AnyRNG *rng)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>real.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/sys/</path>
    <filename>real_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>REALType</name>
      <anchorfile>real_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>double</type>
      <name>REAL</name>
      <anchorfile>real_8h.html</anchorfile>
      <anchor>a1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>RealMatrix.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>RealMatrix_8h</filename>
    <member kind="typedef">
      <type>DoubleMatrix</type>
      <name>RealMatrix</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a19</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>RealVector.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>RealVector_8h</filename>
    <member kind="typedef">
      <type>DoubleVector</type>
      <name>RealVector</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a20</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>RefCount.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>RefCount_8h</filename>
    <class kind="class">utilib::RefCount</class>
    <member kind="enumvalue">
      <name>DataNotOwned</name>
      <anchor>a175a21</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DataOwned</name>
      <anchor>a175a22</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>AcquireOwnership</name>
      <anchor>a175a23</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>AssumeOwnership</name>
      <anchor>a175a24</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>RNG.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/ranlib/include/</path>
    <filename>RNG_8h</filename>
    <class kind="class">utilib::RNG</class>
    <member kind="function">
      <type>ostream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>RNG_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(ostream &amp;output, const utilib::RNG &amp;rng)</arglist>
    </member>
    <member kind="function">
      <type>istream &amp;</type>
      <name>operator&gt;&gt;</name>
      <anchorfile>RNG_8h.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(istream &amp;input, utilib::RNG &amp;rng)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>SampleGenerators.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/ranlib/include/</path>
    <filename>SampleGenerators_8h</filename>
    <class kind="class">utilib::SampleGenerator</class>
    <class kind="class">utilib::UniformSampleGenerator</class>
    <class kind="class">utilib::UniformSampleGenerator&lt; BasicArray&lt; double &gt; &gt;</class>
  </compound>
  <compound kind="file">
    <name>seconds.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/sys/</path>
    <filename>seconds_8h</filename>
    <member kind="function">
      <type>double</type>
      <name>CPUSeconds</name>
      <anchorfile>seconds_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>ElapsedCPUSeconds</name>
      <anchorfile>seconds_8h.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>WallClockSeconds</name>
      <anchorfile>seconds_8h.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>ElapsedWallClockSeconds</name>
      <anchorfile>seconds_8h.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>InitializeTiming</name>
      <anchorfile>seconds_8h.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>CurrentTime</name>
      <anchorfile>seconds_8h.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>timerGranularitySeconds</name>
      <anchorfile>seconds_8h.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>signalError.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/sys/</path>
    <filename>signalError_8h</filename>
    <member kind="function">
      <type>void</type>
      <name>signalError</name>
      <anchorfile>signalError_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(const char *string,...)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>Simple2DArray.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>Simple2DArray_8h</filename>
    <class kind="class">utilib::Simple2DArray</class>
    <member kind="function">
      <type>UTILIB_API ostream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>Simple2DArray_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(ostream &amp;output, const utilib::Simple2DArray&lt; T &gt; &amp;array)</arglist>
    </member>
    <member kind="function">
      <type>UTILIB_API istream &amp;</type>
      <name>operator&gt;&gt;</name>
      <anchorfile>Simple2DArray_8h.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(istream &amp;input, utilib::Simple2DArray&lt; T &gt; &amp;array)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>SimpleHashTable.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>SimpleHashTable_8h</filename>
    <class kind="class">utilib::SimpleHashTableItem</class>
    <class kind="class">utilib::SimpleHashTable</class>
  </compound>
  <compound kind="file">
    <name>SimpleHeap.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>SimpleHeap_8h</filename>
    <class kind="class">utilib::SimpleHeapItem</class>
    <class kind="class">utilib::SimpleHeapBase</class>
    <class kind="class">utilib::SimpleHeap</class>
  </compound>
  <compound kind="file">
    <name>SimpleSplayTree.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>SimpleSplayTree_8h</filename>
    <class kind="class">utilib::SimpleSplayTreeItem</class>
    <class kind="class">utilib::SimpleSplayTree</class>
  </compound>
  <compound kind="file">
    <name>SmartPtr.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>SmartPtr_8h</filename>
    <class kind="class">utilib::SmartPtrInfoBase</class>
    <class kind="class">utilib::SmartPtrInfoBase&lt; Type, true &gt;</class>
    <class kind="class">utilib::SmartPtrInfo</class>
    <class kind="class">utilib::SmartPtr</class>
    <member kind="enumvalue">
      <name>DataNotOwned</name>
      <anchor>a175a21</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DataOwned</name>
      <anchor>a175a22</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>AcquireOwnership</name>
      <anchor>a175a23</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>AssumeOwnership</name>
      <anchor>a175a24</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>sort.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/sort/</path>
    <filename>sort_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>SORT_VOID</name>
      <anchorfile>sort_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>COMP_VOID</name>
      <anchorfile>sort_8h.html</anchorfile>
      <anchor>a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SORT_RETVAL</name>
      <anchorfile>sort_8h.html</anchorfile>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SORT_RETURN</name>
      <anchorfile>sort_8h.html</anchorfile>
      <anchor>a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SORT_ARGS</name>
      <anchorfile>sort_8h.html</anchorfile>
      <anchor>a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SORT_FORM</name>
      <anchorfile>sort_8h.html</anchorfile>
      <anchor>a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>compare_fn_type</name>
      <anchorfile>sort_8h.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>)(COMP_VOID *, COMP_VOID *)</arglist>
    </member>
    <member kind="function">
      <type>SORT_RETVAL</type>
      <name>ins_sort</name>
      <anchorfile>sort_8h.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>(SORT_ARGS)</arglist>
    </member>
    <member kind="function">
      <type>SORT_RETVAL</type>
      <name>heap_sort</name>
      <anchorfile>sort_8h.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>(SORT_ARGS)</arglist>
    </member>
    <member kind="function">
      <type>SORT_RETVAL</type>
      <name>quick_sort</name>
      <anchorfile>sort_8h.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>(SORT_ARGS)</arglist>
    </member>
    <member kind="function">
      <type>SORT_RETVAL</type>
      <name>merge_sort</name>
      <anchorfile>sort_8h.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>(SORT_ARGS)</arglist>
    </member>
    <member kind="function">
      <type>SORT_RETVAL</type>
      <name>shell_sort</name>
      <anchorfile>sort_8h.html</anchorfile>
      <anchor>a11</anchor>
      <arglist>(SORT_ARGS)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>compare_int</name>
      <anchorfile>sort_8h.html</anchorfile>
      <anchor>a12</anchor>
      <arglist>(COMP_VOID *, COMP_VOID *)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>compare_float</name>
      <anchorfile>sort_8h.html</anchorfile>
      <anchor>a13</anchor>
      <arglist>(COMP_VOID *, COMP_VOID *)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>compare_double</name>
      <anchorfile>sort_8h.html</anchorfile>
      <anchor>a14</anchor>
      <arglist>(COMP_VOID *, COMP_VOID *)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>compare_ereal_double</name>
      <anchorfile>sort_8h.html</anchorfile>
      <anchor>a15</anchor>
      <arglist>(COMP_VOID *, COMP_VOID *)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>orderx</name>
      <anchorfile>sort_8h.html</anchorfile>
      <anchor>a16</anchor>
      <arglist>(int *ndx, int n, SORT_VOID *base, int width, compare_fn_type compar, SORT_FORM)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>rank</name>
      <anchorfile>sort_8h.html</anchorfile>
      <anchor>a17</anchor>
      <arglist>(int *ndx_vec, int *rank_vec, int n)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>order_k</name>
      <anchorfile>sort_8h.html</anchorfile>
      <anchor>a18</anchor>
      <arglist>(SORT_VOID *base, int nel, int width, compare_fn_type compar, int k)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>order_ki</name>
      <anchorfile>sort_8h.html</anchorfile>
      <anchor>a19</anchor>
      <arglist>(SORT_VOID *base, int nel, int width, compare_fn_type compar, int k, int *ndx)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>SparseMatrix.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>SparseMatrix_8h</filename>
    <class kind="class">utilib::SparseMatrix</class>
    <class kind="class">utilib::CMSparseMatrix</class>
    <class kind="class">utilib::RMSparseMatrix</class>
    <member kind="function">
      <type>void</type>
      <name>product</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a169</anchor>
      <arglist>(NumArray&lt; T &gt; &amp;res, const RMSparseMatrix&lt; T &gt; &amp;mat, const NumArray&lt; T &gt; &amp;vec)</arglist>
    </member>
    <member kind="function">
      <type>ostream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>SparseMatrix_8h.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(ostream &amp;output, const utilib::SparseMatrix&lt; T &gt; &amp;array)</arglist>
    </member>
    <member kind="function">
      <type>istream &amp;</type>
      <name>operator&gt;&gt;</name>
      <anchorfile>SparseMatrix_8h.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(istream &amp;input, utilib::SparseMatrix&lt; T &gt; &amp;array)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>StackArray.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>StackArray_8h</filename>
    <class kind="class">utilib::StackArray</class>
    <member kind="function">
      <type>utilib::StackArray&lt; T &gt; &amp;</type>
      <name>operator&gt;&gt;</name>
      <anchorfile>StackArray_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(utilib::StackArray&lt; T &gt; &amp;list, T &amp;val)</arglist>
    </member>
    <member kind="function">
      <type>utilib::StackArray&lt; T &gt; &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>StackArray_8h.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(utilib::StackArray&lt; T &gt; &amp;list, const T &amp;val)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>StackList.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>StackList_8h</filename>
    <class kind="class">utilib::StackList</class>
  </compound>
  <compound kind="file">
    <name>stl_auxillary.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>stl__auxillary_8h</filename>
    <member kind="function">
      <type>TYPE</type>
      <name>max</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a170</anchor>
      <arglist>(const std::vector&lt; TYPE &gt; &amp;vec)</arglist>
    </member>
    <member kind="function">
      <type>ostream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>stl__auxillary_8h.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(ostream &amp;os, bool &amp;flag)</arglist>
    </member>
    <member kind="function">
      <type>istream &amp;</type>
      <name>operator&gt;&gt;</name>
      <anchorfile>stl__auxillary_8h.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(istream &amp;is, bool &amp;flag)</arglist>
    </member>
    <member kind="function">
      <type>vector&lt; TYPE &gt; &amp;</type>
      <name>operator+=</name>
      <anchorfile>stl__auxillary_8h.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(vector&lt; TYPE &gt; &amp;vec, const vector&lt; TYPE &gt; &amp;val)</arglist>
    </member>
    <member kind="function">
      <type>vector&lt; TYPE &gt; &amp;</type>
      <name>operator &amp;=</name>
      <anchorfile>stl__auxillary_8h.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(vector&lt; TYPE &gt; &amp;vec, const vector&lt; TYPE &gt; &amp;old)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>string_ops.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>string__ops_8h</filename>
    <member kind="function">
      <type>BasicArray&lt; CharString &gt;</type>
      <name>split</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a171</anchor>
      <arglist>(const CharString &amp;str, char sep)</arglist>
    </member>
    <member kind="function">
      <type>CharString</type>
      <name>join</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a172</anchor>
      <arglist>(const BasicArray&lt; CharString &gt; &amp;strs, char sep)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>Triang_bal.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/ranlib/include/</path>
    <filename>Triang__bal_8h</filename>
  </compound>
  <compound kind="file">
    <name>Triang_trunc.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/ranlib/include/</path>
    <filename>Triang__trunc_8h</filename>
  </compound>
  <compound kind="file">
    <name>Triang_trunc_50.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/ranlib/include/</path>
    <filename>Triang__trunc__50_8h</filename>
  </compound>
  <compound kind="file">
    <name>Triangular.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/ranlib/include/</path>
    <filename>Triangular_8h</filename>
    <class kind="class">utilib::Triangular</class>
    <member kind="define">
      <type>#define</type>
      <name>PROBABILITY_BALANCED_TRIANGULAR</name>
      <anchorfile>Triangular_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>EXPECTATION_BALANCED_TRIANGULAR</name>
      <anchorfile>Triangular_8h.html</anchorfile>
      <anchor>a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TRUNCATED_TRIANGULAR</name>
      <anchorfile>Triangular_8h.html</anchorfile>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Area</name>
      <anchorfile>Triangular_8h.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(c)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>Tuple.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>Tuple_8h</filename>
    <class kind="class">utilib::Tuple1</class>
    <class kind="class">utilib::Tuple2</class>
    <class kind="class">utilib::Tuple3</class>
    <class kind="class">utilib::Tuple4</class>
    <class kind="class">utilib::Tuple5</class>
    <class kind="class">utilib::Tuple6</class>
    <class kind="class">utilib::Tuple7</class>
    <member kind="function">
      <type>ostream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>Tuple_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(ostream &amp;os, const utilib::Tuple1&lt; T1 &gt; &amp;obj)</arglist>
    </member>
    <member kind="function">
      <type>istream &amp;</type>
      <name>operator&gt;&gt;</name>
      <anchorfile>Tuple_8h.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(istream &amp;is, utilib::Tuple1&lt; T1 &gt; &amp;obj)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>TwoBitArray.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>TwoBitArray_8h</filename>
    <class kind="class">utilib::TwoBitArray</class>
  </compound>
  <compound kind="file">
    <name>Uniform.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/ranlib/include/</path>
    <filename>Uniform_8h</filename>
    <class kind="class">utilib::Uniform</class>
    <member kind="function">
      <type>int</type>
      <name>Discretize</name>
      <anchorfile>namespaceutilib.html</anchorfile>
      <anchor>a37</anchor>
      <arglist>(double urand, int low, int high)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>utilib.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/</path>
    <filename>utilib_8h</filename>
  </compound>
  <compound kind="file">
    <name>utilib_dll.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/sys/</path>
    <filename>utilib__dll_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>UTILIB_API</name>
      <anchorfile>utilib__dll_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>UtilibExceptions.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/sys/</path>
    <filename>UtilibExceptions_8h</filename>
    <class kind="class">utilib::general_runtime_error</class>
  </compound>
  <compound kind="file">
    <name>ValidateParameter.h</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/io/</path>
    <filename>ValidateParameter_8h</filename>
    <class kind="class">utilib::ParameterValidatorBase</class>
    <class kind="class">utilib::ParameterValidatorFunction</class>
    <class kind="class">utilib::ParameterValidator</class>
    <class kind="class">utilib::ParameterLowerBound</class>
    <class kind="class">utilib::ParameterUpperBound</class>
    <class kind="class">utilib::ParameterBounds</class>
    <class kind="class">utilib::ParameterNonnegative</class>
    <class kind="class">utilib::ParameterPositive</class>
    <member kind="function">
      <type>std::ostream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>ValidateParameter_8h.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(std::ostream &amp;ostr, const utilib::ParameterValidatorBase &amp;item)</arglist>
    </member>
  </compound>
  <compound kind="dir">
    <name>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</path>
    <filename>dir_000003.html</filename>
    <file>AbstractHashTable.h</file>
    <file>AbstractHeap.h</file>
    <file>AbstractSplayTree.h</file>
    <file>Any.h</file>
    <file>ArrayBase.h</file>
    <file>Basic2DArray.h</file>
    <file>Basic3DArray.h</file>
    <file>BasicArray.h</file>
    <file>BitArray.h</file>
    <file>BitArrayBase.h</file>
    <file>CachedAllocator.h</file>
    <file>CharString.h</file>
    <file>ClassRef.h</file>
    <file>DoubleMatrix.h</file>
    <file>DoubleVector.h</file>
    <file>EnumBitArray.h</file>
    <file>Ereal.h</file>
    <file>GenericHashTable.h</file>
    <file>GenericHeap.h</file>
    <file>GenericSplayTree.h</file>
    <file>hash_fn.h</file>
    <file>HashedSet.h</file>
    <file>IntMatrix.h</file>
    <file>IntVector.h</file>
    <file>LinkedList.h</file>
    <file>LPHashTable.h</file>
    <file>MixedIntVars.h</file>
    <file>Num2DArray.h</file>
    <file>NumArray.h</file>
    <file>OrderedList.h</file>
    <file>OrderedSet.h</file>
    <file>PersistentPointer.h</file>
    <file>pvector.h</file>
    <file>QueueArray.h</file>
    <file>QueueList.h</file>
    <file>RealMatrix.h</file>
    <file>RealVector.h</file>
    <file>RefCount.h</file>
    <file>Simple2DArray.h</file>
    <file>SimpleHashTable.h</file>
    <file>SimpleHeap.h</file>
    <file>SimpleSplayTree.h</file>
    <file>SmartPtr.h</file>
    <file>SparseMatrix.h</file>
    <file>StackArray.h</file>
    <file>StackList.h</file>
    <file>stl_auxillary.h</file>
    <file>string_ops.h</file>
    <file>traits.h</file>
    <file>Tuple.h</file>
    <file>TwoBitArray.h</file>
    <file>ValuedContainer.h</file>
  </compound>
  <compound kind="dir">
    <name>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/ranlib/include/</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/ranlib/include/</path>
    <filename>dir_000005.html</filename>
    <file>AnyRNG.h</file>
    <file>Binomial.h</file>
    <file>Cauchy.h</file>
    <file>default_rng.h</file>
    <file>DUniform.h</file>
    <file>Exponential.h</file>
    <file>Geometric.h</file>
    <file>LCG.h</file>
    <file>LEC_rng.h</file>
    <file>LogNormal.h</file>
    <file>MNormal.h</file>
    <file>MUniform.h</file>
    <file>Normal.h</file>
    <file>PM_LCG.h</file>
    <file>Random.h</file>
    <file>RandomVariable.h</file>
    <file>RNG.h</file>
    <file>SampleGenerators.h</file>
    <file>Triang_bal.h</file>
    <file>Triang_trunc.h</file>
    <file>Triang_trunc_50.h</file>
    <file>Triangular.h</file>
    <file>Uniform.h</file>
  </compound>
  <compound kind="dir">
    <name>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/io/</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/io/</path>
    <filename>dir_000006.html</filename>
    <file>comments.h</file>
    <file>CommonIO.h</file>
    <file>logEvent.h</file>
    <file>mpiUtil.h</file>
    <file>nicePrint.h</file>
    <file>PackBuf.h</file>
    <file>Parameter.h</file>
    <file>ParameterList.h</file>
    <file>ParameterSet.h</file>
    <file>paramTable.h</file>
    <file>ValidateParameter.h</file>
  </compound>
  <compound kind="dir">
    <name>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/math/</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/math/</path>
    <filename>dir_000002.html</filename>
    <file>_math.h</file>
    <file>linpack.h</file>
    <file>math_array.h</file>
    <file>math_basic.h</file>
    <file>math_matrix.h</file>
  </compound>
  <compound kind="dir">
    <name>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/ranlib/misc/</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/ranlib/misc/</path>
    <filename>dir_000008.html</filename>
    <file>ParkMiller.c</file>
  </compound>
  <compound kind="dir">
    <name>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/ranlib/</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/ranlib/</path>
    <filename>dir_000004.html</filename>
    <dir>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/ranlib/include/</dir>
    <dir>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/ranlib/misc/</dir>
  </compound>
  <compound kind="dir">
    <name>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/sort/</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/sort/</path>
    <filename>dir_000007.html</filename>
    <file>compare.h</file>
    <file>sort.h</file>
  </compound>
  <compound kind="dir">
    <name>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/</path>
    <filename>dir_000000.html</filename>
    <dir>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/adt/</dir>
    <dir>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/io/</dir>
    <dir>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/math/</dir>
    <dir>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/ranlib/</dir>
    <dir>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/sort/</dir>
    <dir>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/sys/</dir>
    <file>utilib.h</file>
  </compound>
  <compound kind="dir">
    <name>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/sys/</name>
    <path>/cygdrive/c/home/wehart/src/gnlp/acro/packages/utilib/src/sys/</path>
    <filename>dir_000001.html</filename>
    <file>_generic.h</file>
    <file>errmsg.h</file>
    <file>exception_mngr.h</file>
    <file>general.h</file>
    <file>memdebug.h</file>
    <file>mpi_utilib.h</file>
    <file>real.h</file>
    <file>seconds.h</file>
    <file>signalError.h</file>
    <file>utilib_dll.h</file>
    <file>utilib_malloc.h</file>
    <file>UtilibExceptions.h</file>
  </compound>
  <compound kind="class">
    <name>utilib::general_runtime_error</name>
    <filename>classutilib_1_1general__runtime__error.html</filename>
    <member kind="function">
      <type></type>
      <name>general_runtime_error</name>
      <anchorfile>classutilib_1_1general__runtime__error.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(const char *fmt,...)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual const char *</type>
      <name>what</name>
      <anchorfile>classutilib_1_1general__runtime__error.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>char</type>
      <name>str</name>
      <anchorfile>classutilib_1_1general__runtime__error.html</anchorfile>
      <anchor>p0</anchor>
      <arglist>[512]</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::CommonIO</name>
    <filename>classutilib_1_1CommonIO.html</filename>
    <member kind="function">
      <type></type>
      <name>CommonIO</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~CommonIO</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual int</type>
      <name>verbosity</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const int level) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setDebug</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(int level)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>begin</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>e0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>begin_buffered</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>e1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>end</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>e2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>map_off</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>e3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>map_on</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>e4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>reset</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>e5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>setIORank</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>e6</anchor>
      <arglist>(int Rank_)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>setIOFlush</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>e7</anchor>
      <arglist>(int flush_)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>rank</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>e8</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>io_rank</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>e9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>numProcs</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>e10</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>set_streams</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>e11</anchor>
      <arglist>(ostream *cout_, ostream *cerr_, istream *cin_)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>set_cout</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>e12</anchor>
      <arglist>(ostream *cout_)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>set_cerr</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>e13</anchor>
      <arglist>(ostream *cerr_)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>set_cin</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>e14</anchor>
      <arglist>(istream *cin_)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>set_ofile</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>e15</anchor>
      <arglist>(const char *ofile)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>begin_tagging</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>e16</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>begin_tagging</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>e17</anchor>
      <arglist>(const int numDigits_)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>end_tagging</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>e18</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>tagging_active</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>e19</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>flush</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>e20</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>sync_stdio</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>e21</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>end_all</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>e22</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>debug</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static int</type>
      <name>numDigits</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>s0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static ostream *</type>
      <name>MapCout</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>s1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static ostream *</type>
      <name>MapCerr</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>s2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static istream *</type>
      <name>MapCin</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>s3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static ostream *</type>
      <name>common_cout</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>s4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static ostream *</type>
      <name>common_cerr</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>s5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static istream *</type>
      <name>common_cin</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>s6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static bool</type>
      <name>io_mapping</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>s7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static int</type>
      <name>begin_end_counter</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>s8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static bool</type>
      <name>atexit_flag</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>s9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static stringstream *</type>
      <name>pStrCout</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>s10</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static stringstream *</type>
      <name>pStrCerr</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>s11</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static ostream &amp;</type>
      <name>o_stream</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>h0</anchor>
      <arglist>(const int flag)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>IOflush</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>h1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>flush_stream</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>h2</anchor>
      <arglist>(stringstream &amp;stream, int val)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>reset_map</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>h3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static int</type>
      <name>io_buffering</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>v0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static int</type>
      <name>IO_Rank</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>v1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static bool</type>
      <name>tagging</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>v2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static int</type>
      <name>nref</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>v3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static int</type>
      <name>flush_flag</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>v4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static int</type>
      <name>seqNum</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>v5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static bool</type>
      <name>mpi_initialized</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>v6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static int</type>
      <name>Rank</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>v7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static int</type>
      <name>Size</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>v8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static int</type>
      <name>stream_flag</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>v9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static bool</type>
      <name>header_flag</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>v10</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static ofstream *</type>
      <name>common_ofstr</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>v11</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend" protection="private">
      <type>friend void</type>
      <name>cppMessage_abort</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>n0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="friend" protection="private">
      <type>friend ostream &amp;</type>
      <name>Flush</name>
      <anchorfile>classutilib_1_1CommonIO.html</anchorfile>
      <anchor>n1</anchor>
      <arglist>(ostream &amp;outstr)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::logEvent</name>
    <filename>classutilib_1_1logEvent.html</filename>
    <base>utilib::ParameterSet</base>
    <member kind="function">
      <type></type>
      <name>logEvent</name>
      <anchorfile>classutilib_1_1logEvent.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>init</name>
      <anchorfile>classutilib_1_1logEvent.html</anchorfile>
      <anchor>e0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>open</name>
      <anchorfile>classutilib_1_1logEvent.html</anchorfile>
      <anchor>e1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>close</name>
      <anchorfile>classutilib_1_1logEvent.html</anchorfile>
      <anchor>e2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>defineState</name>
      <anchorfile>classutilib_1_1logEvent.html</anchorfile>
      <anchor>e3</anchor>
      <arglist>(const char *description, const char *color)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>log_start</name>
      <anchorfile>classutilib_1_1logEvent.html</anchorfile>
      <anchor>e4</anchor>
      <arglist>(int stateNumber)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>log_end</name>
      <anchorfile>classutilib_1_1logEvent.html</anchorfile>
      <anchor>e5</anchor>
      <arglist>(int stateNumber)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>log_point</name>
      <anchorfile>classutilib_1_1logEvent.html</anchorfile>
      <anchor>e6</anchor>
      <arglist>(int stateNumber)</arglist>
    </member>
    <member kind="function" protection="protected" static="yes">
      <type>static void</type>
      <name>event</name>
      <anchorfile>classutilib_1_1logEvent.html</anchorfile>
      <anchor>f0</anchor>
      <arglist>(int eventNumber)</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>int</type>
      <name>eventLog</name>
      <anchorfile>classutilib_1_1logEvent.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected" static="yes">
      <type>static int</type>
      <name>numberOfStates</name>
      <anchorfile>classutilib_1_1logEvent.html</anchorfile>
      <anchor>t0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::uMPI</name>
    <filename>classutilib_1_1uMPI.html</filename>
    <member kind="function">
      <type></type>
      <name>CLASS_PARAMETER</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(messageLog)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>logSend</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>e0</anchor>
      <arglist>(int dest, int tag, int count, MPI_Datatype t)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>logRecv</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>e1</anchor>
      <arglist>(MPI_Status *status)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>running</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>e2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>packSlop</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>e3</anchor>
      <arglist>(int m)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>sizeOf</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>e4</anchor>
      <arglist>(MPI_Datatype t)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>init</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>e5</anchor>
      <arglist>(int *argcP, char ***argv, MPI_Comm comm_=MPI_COMM_WORLD)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>init</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>e6</anchor>
      <arglist>(MPI_Comm comm_=MPI_COMM_WORLD)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>done</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>e7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>barrier</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>e8</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>reduce</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>e9</anchor>
      <arglist>(void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, int root)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>sum</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>e10</anchor>
      <arglist>(int value, int root=ioProc)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static double</type>
      <name>sum</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>e11</anchor>
      <arglist>(double value, int root=ioProc)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>max</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>e12</anchor>
      <arglist>(int value, int root=ioProc)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static double</type>
      <name>max</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>e13</anchor>
      <arglist>(double value, int root=ioProc)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>broadcast</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>e14</anchor>
      <arglist>(void *buffer, int count, MPI_Datatype datatype, int root)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>reduceCast</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>e15</anchor>
      <arglist>(void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>isend</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>e16</anchor>
      <arglist>(void *buffer, int count, MPI_Datatype datatype, int dest, int tag, MPI_Request *request)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>isend</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>e17</anchor>
      <arglist>(void *buffer, int count, MPI_Datatype datatype, int dest, int tag)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>send</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>e18</anchor>
      <arglist>(void *buffer, int count, MPI_Datatype datatype, int dest, int tag)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>ssend</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>e19</anchor>
      <arglist>(void *buffer, int count, MPI_Datatype datatype, int dest, int tag)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>requestFree</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>e20</anchor>
      <arglist>(MPI_Request *request)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>issend</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>e21</anchor>
      <arglist>(void *buffer, int count, MPI_Datatype datatype, int dest, int tag, MPI_Request *request)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>irecv</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>e22</anchor>
      <arglist>(void *buffer, int count, MPI_Datatype datatype, int source, int tag, MPI_Request *request)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>recv</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>e23</anchor>
      <arglist>(void *buffer, int count, MPI_Datatype datatype, int source, int tag, MPI_Status *status)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>getCount</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>e24</anchor>
      <arglist>(MPI_Status *status, MPI_Datatype datatype)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>test</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>e25</anchor>
      <arglist>(MPI_Request *request, int *flag, MPI_Status *status)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>test</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>e26</anchor>
      <arglist>(MPI_Request *request, MPI_Status *status)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>test</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>e27</anchor>
      <arglist>(MPI_Request *request)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>testsome</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>e28</anchor>
      <arglist>(int incount, MPI_Request *request_array, int &amp;outcount, int *array_of_indeces, MPI_Status *status_array)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>cancel</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>e29</anchor>
      <arglist>(MPI_Request *request)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>wait</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>e30</anchor>
      <arglist>(MPI_Request *request, MPI_Status *status)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>killSendRequest</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>e31</anchor>
      <arglist>(MPI_Request *request)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>killRecvRequest</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>e32</anchor>
      <arglist>(MPI_Request *request)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static MPI_Comm</type>
      <name>comm</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>s0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static int</type>
      <name>rank</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>s1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static int</type>
      <name>size</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>s2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static int</type>
      <name>ioProc</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>s3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static int</type>
      <name>iDoIO</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>s4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static int</type>
      <name>errorCode</name>
      <anchorfile>classutilib_1_1uMPI.html</anchorfile>
      <anchor>s5</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::PackBuffer</name>
    <filename>classutilib_1_1PackBuffer.html</filename>
    <member kind="function">
      <type></type>
      <name>PackBuffer</name>
      <anchorfile>classutilib_1_1PackBuffer.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(int size_=1024)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~PackBuffer</name>
      <anchorfile>classutilib_1_1PackBuffer.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>buf</name>
      <anchorfile>classutilib_1_1PackBuffer.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>size</name>
      <anchorfile>classutilib_1_1PackBuffer.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>curr</name>
      <anchorfile>classutilib_1_1PackBuffer.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>reset</name>
      <anchorfile>classutilib_1_1PackBuffer.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>pack</name>
      <anchorfile>classutilib_1_1PackBuffer.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(const TYPE *data, const int num)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>pack</name>
      <anchorfile>classutilib_1_1PackBuffer.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>(const TYPE &amp;data)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>dump</name>
      <anchorfile>classutilib_1_1PackBuffer.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>(ostream &amp;ostr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>pack</name>
      <anchorfile>classutilib_1_1PackBuffer.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>(char *data)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>pack</name>
      <anchorfile>classutilib_1_1PackBuffer.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>(const char *data)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>writeBinary</name>
      <anchorfile>classutilib_1_1PackBuffer.html</anchorfile>
      <anchor>a11</anchor>
      <arglist>(ostream &amp;ostr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>pack</name>
      <anchorfile>classutilib_1_1PackBuffer.html</anchorfile>
      <anchor>a12</anchor>
      <arglist>(const bool *data, const int num)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>resize</name>
      <anchorfile>classutilib_1_1PackBuffer.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>(const size_type newsize)</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>char *</type>
      <name>buffer</name>
      <anchorfile>classutilib_1_1PackBuffer.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>size_type</type>
      <name>Index</name>
      <anchorfile>classutilib_1_1PackBuffer.html</anchorfile>
      <anchor>p1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>size_type</type>
      <name>Size</name>
      <anchorfile>classutilib_1_1PackBuffer.html</anchorfile>
      <anchor>p2</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::UnPackBuffer</name>
    <filename>classutilib_1_1UnPackBuffer.html</filename>
    <member kind="function">
      <type>void</type>
      <name>setup</name>
      <anchorfile>classutilib_1_1UnPackBuffer.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(char *buf_, size_type size_, bool flag=false)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setup</name>
      <anchorfile>classutilib_1_1UnPackBuffer.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(const char *buf_, size_type size_)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>UnPackBuffer</name>
      <anchorfile>classutilib_1_1UnPackBuffer.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>UnPackBuffer</name>
      <anchorfile>classutilib_1_1UnPackBuffer.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(size_type size_)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>UnPackBuffer</name>
      <anchorfile>classutilib_1_1UnPackBuffer.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(char *buf_, size_type size_, bool flag=false)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>UnPackBuffer</name>
      <anchorfile>classutilib_1_1UnPackBuffer.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(const char *buf_, size_type size_)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>UnPackBuffer</name>
      <anchorfile>classutilib_1_1UnPackBuffer.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(PackBuffer &amp;pack)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~UnPackBuffer</name>
      <anchorfile>classutilib_1_1UnPackBuffer.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>resize</name>
      <anchorfile>classutilib_1_1UnPackBuffer.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>(const size_type newsize)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>buf</name>
      <anchorfile>classutilib_1_1UnPackBuffer.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>size</name>
      <anchorfile>classutilib_1_1UnPackBuffer.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>message_length</name>
      <anchorfile>classutilib_1_1UnPackBuffer.html</anchorfile>
      <anchor>a11</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>curr</name>
      <anchorfile>classutilib_1_1UnPackBuffer.html</anchorfile>
      <anchor>a12</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>reset</name>
      <anchorfile>classutilib_1_1UnPackBuffer.html</anchorfile>
      <anchor>a13</anchor>
      <arglist>(size_type message_size)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>reset</name>
      <anchorfile>classutilib_1_1UnPackBuffer.html</anchorfile>
      <anchor>a14</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>reset</name>
      <anchorfile>classutilib_1_1UnPackBuffer.html</anchorfile>
      <anchor>a15</anchor>
      <arglist>(MPI_Status *statusP)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>operator bool</name>
      <anchorfile>classutilib_1_1UnPackBuffer.html</anchorfile>
      <anchor>a16</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>UnPackBuffer &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1UnPackBuffer.html</anchorfile>
      <anchor>a17</anchor>
      <arglist>(const PackBuffer &amp;pbuff)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>unpack</name>
      <anchorfile>classutilib_1_1UnPackBuffer.html</anchorfile>
      <anchor>a18</anchor>
      <arglist>(TYPE *data, const int num)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>unpack</name>
      <anchorfile>classutilib_1_1UnPackBuffer.html</anchorfile>
      <anchor>a19</anchor>
      <arglist>(TYPE &amp;data)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>dump</name>
      <anchorfile>classutilib_1_1UnPackBuffer.html</anchorfile>
      <anchor>a20</anchor>
      <arglist>(ostream &amp;ostr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>unpack</name>
      <anchorfile>classutilib_1_1UnPackBuffer.html</anchorfile>
      <anchor>a21</anchor>
      <arglist>(char *data)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>readBinary</name>
      <anchorfile>classutilib_1_1UnPackBuffer.html</anchorfile>
      <anchor>a22</anchor>
      <arglist>(istream &amp;istr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>unpack</name>
      <anchorfile>classutilib_1_1UnPackBuffer.html</anchorfile>
      <anchor>a23</anchor>
      <arglist>(bool *data, const int num)</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>char *</type>
      <name>buffer</name>
      <anchorfile>classutilib_1_1UnPackBuffer.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>size_type</type>
      <name>Index</name>
      <anchorfile>classutilib_1_1UnPackBuffer.html</anchorfile>
      <anchor>p1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>size_type</type>
      <name>Size</name>
      <anchorfile>classutilib_1_1UnPackBuffer.html</anchorfile>
      <anchor>p2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>size_type</type>
      <name>MessageLength</name>
      <anchorfile>classutilib_1_1UnPackBuffer.html</anchorfile>
      <anchor>p3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>own_flag</name>
      <anchorfile>classutilib_1_1UnPackBuffer.html</anchorfile>
      <anchor>p4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>status_flag</name>
      <anchorfile>classutilib_1_1UnPackBuffer.html</anchorfile>
      <anchor>p5</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::Parameter</name>
    <filename>classutilib_1_1Parameter.html</filename>
    <member kind="function">
      <type></type>
      <name>Parameter</name>
      <anchorfile>classutilib_1_1Parameter.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Parameter</name>
      <anchorfile>classutilib_1_1Parameter.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(const Parameter &amp;param)</arglist>
    </member>
    <member kind="function">
      <type>Parameter &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1Parameter.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const Parameter &amp;param)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Parameter</name>
      <anchorfile>classutilib_1_1Parameter.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1Parameter.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(std::ostream &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1Parameter.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(std::istream &amp;is)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1Parameter.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(utilib::PackBuffer &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1Parameter.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>(utilib::UnPackBuffer &amp;is)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>valid</name>
      <anchorfile>classutilib_1_1Parameter.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>AnyReference</type>
      <name>info</name>
      <anchorfile>classutilib_1_1Parameter.html</anchorfile>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>string</type>
      <name>name</name>
      <anchorfile>classutilib_1_1Parameter.html</anchorfile>
      <anchor>o1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>string</type>
      <name>syntax</name>
      <anchorfile>classutilib_1_1Parameter.html</anchorfile>
      <anchor>o2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>string</type>
      <name>default_value</name>
      <anchorfile>classutilib_1_1Parameter.html</anchorfile>
      <anchor>o3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>string</type>
      <name>description</name>
      <anchorfile>classutilib_1_1Parameter.html</anchorfile>
      <anchor>o4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>initialized</name>
      <anchorfile>classutilib_1_1Parameter.html</anchorfile>
      <anchor>o5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>disabled</name>
      <anchorfile>classutilib_1_1Parameter.html</anchorfile>
      <anchor>o6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>referenced</name>
      <anchorfile>classutilib_1_1Parameter.html</anchorfile>
      <anchor>o7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::set&lt; string &gt;</type>
      <name>aliases</name>
      <anchorfile>classutilib_1_1Parameter.html</anchorfile>
      <anchor>o8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>SmartPtr&lt; ParameterValidatorBase &gt;</type>
      <name>validator</name>
      <anchorfile>classutilib_1_1Parameter.html</anchorfile>
      <anchor>o9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::unary_function&lt; AnyReference, void &gt; *</type>
      <name>callback</name>
      <anchorfile>classutilib_1_1Parameter.html</anchorfile>
      <anchor>o10</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>copy</name>
      <anchorfile>classutilib_1_1Parameter.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>(const Parameter &amp;param)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::ParameterList</name>
    <filename>classutilib_1_1ParameterList.html</filename>
    <member kind="function">
      <type></type>
      <name>ParameterList</name>
      <anchorfile>classutilib_1_1ParameterList.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~ParameterList</name>
      <anchorfile>classutilib_1_1ParameterList.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>synchronize</name>
      <anchorfile>classutilib_1_1ParameterList.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(int root=0, MPI_Comm comm=MPI_COMM_WORLD)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read_parameters</name>
      <anchorfile>classutilib_1_1ParameterList.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(istream &amp;is, const string &amp;terminator)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read_parameters</name>
      <anchorfile>classutilib_1_1ParameterList.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(istream &amp;is)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write_parameters</name>
      <anchorfile>classutilib_1_1ParameterList.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(ostream &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write_parameters</name>
      <anchorfile>classutilib_1_1ParameterList.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(PackBuffer &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read_parameters</name>
      <anchorfile>classutilib_1_1ParameterList.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>(UnPackBuffer &amp;is)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>process_parameters</name>
      <anchorfile>classutilib_1_1ParameterList.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>(int &amp;argc, char **&amp;argv, unsigned int min_num_required_args)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>process_parameters</name>
      <anchorfile>classutilib_1_1ParameterList.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>(const std::string &amp;filename)</arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>size</name>
      <anchorfile>classutilib_1_1ParameterList.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>add_parameter</name>
      <anchorfile>classutilib_1_1ParameterList.html</anchorfile>
      <anchor>a11</anchor>
      <arglist>(const std::string &amp;label, const std::string &amp;value)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>register_parameters</name>
      <anchorfile>classutilib_1_1ParameterList.html</anchorfile>
      <anchor>a12</anchor>
      <arglist>(ParameterSet &amp;pset)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>register_parameters</name>
      <anchorfile>classutilib_1_1ParameterList.html</anchorfile>
      <anchor>a13</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write_registered_parameters</name>
      <anchorfile>classutilib_1_1ParameterList.html</anchorfile>
      <anchor>a14</anchor>
      <arglist>(ostream &amp;os, bool verbose=true, const char *seperator=&quot;\n&quot;) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write_unused_parameters</name>
      <anchorfile>classutilib_1_1ParameterList.html</anchorfile>
      <anchor>a15</anchor>
      <arglist>(ostream &amp;os)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>unused</name>
      <anchorfile>classutilib_1_1ParameterList.html</anchorfile>
      <anchor>a16</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::list&lt; ParameterListItem &gt;</type>
      <name>parameters</name>
      <anchorfile>classutilib_1_1ParameterList.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>ParameterListItem</type>
      <name>tmp_param</name>
      <anchorfile>classutilib_1_1ParameterList.html</anchorfile>
      <anchor>p1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>ParameterSet</type>
      <name>registered_params</name>
      <anchorfile>classutilib_1_1ParameterList.html</anchorfile>
      <anchor>p2</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend" protection="private">
      <type>friend class</type>
      <name>ParameterSet</name>
      <anchorfile>classutilib_1_1ParameterList.html</anchorfile>
      <anchor>n0</anchor>
      <arglist></arglist>
    </member>
    <class kind="class">utilib::ParameterList::ParameterListItem</class>
  </compound>
  <compound kind="class">
    <name>utilib::ParamFileCallback</name>
    <filename>classutilib_1_1ParamFileCallback.html</filename>
    <member kind="function">
      <type>void</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1ParamFileCallback.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(const AnyReference &amp;value)</arglist>
    </member>
    <member kind="variable">
      <type>ParameterSet *</type>
      <name>params</name>
      <anchorfile>classutilib_1_1ParamFileCallback.html</anchorfile>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::ParameterSet</name>
    <filename>classutilib_1_1ParameterSet.html</filename>
    <member kind="function">
      <type></type>
      <name>ParameterSet</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>ParameterSet</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(const ParameterSet &amp;params)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~ParameterSet</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>ParameterSet &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(const ParameterSet &amp;params)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>set_parameters</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(ParameterList &amp;params, bool delete_used=true)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_parameter</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(const string &amp;name, const T &amp;value)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_parameter_with_string</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(const string &amp;name, const string &amp;value)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>alias_parameter</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>(const string &amp;name, const string &amp;alias)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>get_parameter</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>(const string &amp;name, T &amp;value) const </arglist>
    </member>
    <member kind="function">
      <type>T &amp;</type>
      <name>get_parameter</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>(const string &amp;name)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>parameter_initialized</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>(const string &amp;name)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>write_parameters</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>a11</anchor>
      <arglist>(ostream &amp;os, bool verbose=true, const char *seperator=&quot;\n&quot;) const </arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>write_parameters_html</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>a12</anchor>
      <arglist>(ostream &amp;os) const </arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>write_parameter_values</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>a13</anchor>
      <arglist>(ostream &amp;os, const char *opt_label=&quot;&quot;) const </arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>read_parameter_values</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>a14</anchor>
      <arglist>(istream &amp;is, const string &amp;terminator)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>read_parameter_values</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>a15</anchor>
      <arglist>(istream &amp;is)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>write_parameter_values</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>a16</anchor>
      <arglist>(PackBuffer &amp;os) const </arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>read_parameter_values</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>a17</anchor>
      <arglist>(UnPackBuffer &amp;is)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>process_parameters</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>a18</anchor>
      <arglist>(int &amp;argc, char **&amp;argv, unsigned int min_num_required_parameters=0)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>reset_parameters</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>a19</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>disable_parameter</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>a20</anchor>
      <arglist>(const string &amp;name)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>enable_parameter</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>a21</anchor>
      <arglist>(const string &amp;name)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>parameter_exists</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>a22</anchor>
      <arglist>(const string &amp;name)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>create_parameter</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>a23</anchor>
      <arglist>(const string &amp;name, T &amp;data, const string &amp;syntax, const string &amp;default_value, const string &amp;description)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>create_categorized_parameter</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>a24</anchor>
      <arglist>(const string &amp;name, T &amp;data, const string &amp;syntax, const string &amp;default_value, const string &amp;description, const string &amp;category)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>create_parameter</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>a25</anchor>
      <arglist>(const string &amp;name, T &amp;data, const string &amp;syntax, const string &amp;default_value, const string &amp;description, FuncT validation_func)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>create_categorized_parameter</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>a26</anchor>
      <arglist>(const string &amp;name, T &amp;data, const string &amp;syntax, const string &amp;default_value, const string &amp;description, const string &amp;category, FuncT validation_func)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_parameter_callback</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>a27</anchor>
      <arglist>(const string &amp;name, std::unary_function&lt; AnyReference, void &gt; &amp;func)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_parameter_default</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>a28</anchor>
      <arglist>(const string &amp;name, const string &amp;default_value)</arglist>
    </member>
    <member kind="function">
      <type>Parameter &amp;</type>
      <name>get_parameter_object</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>a29</anchor>
      <arglist>(const string &amp;name)</arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>size</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>a30</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>augment_parameters</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>a31</anchor>
      <arglist>(ParameterSet &amp;pset, bool overwrite=false)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>create_categorized_parameter</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>a32</anchor>
      <arglist>(const string &amp;name, char *&amp;, const string &amp;, const string &amp;, const string &amp;, const string &amp;)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static size_type</type>
      <name>num_default_parameters</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>s0</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>add_parameter</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>(const Parameter &amp;param)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>add_parameter</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>b1</anchor>
      <arglist>(const Parameter &amp;param, const string &amp;category)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>add_parameter</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>b2</anchor>
      <arglist>(Parameter *param, const string &amp;category, bool ownership=true)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>int</type>
      <name>parameter_index</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>b3</anchor>
      <arglist>(const string &amp;name) const </arglist>
    </member>
    <member kind="function" protection="protected">
      <type>const Parameter &amp;</type>
      <name>find_parameter</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>b4</anchor>
      <arglist>(const string &amp;str) const </arglist>
    </member>
    <member kind="function" protection="protected">
      <type>Parameter &amp;</type>
      <name>find_parameter</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>b5</anchor>
      <arglist>(const string &amp;str)</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>vector&lt; Parameter * &gt;</type>
      <name>param_info</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>BitArray</type>
      <name>param_ownership</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>p1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>map&lt; string, int &gt;</type>
      <name>parameters</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>p2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>map&lt; string, map&lt; string, int &gt; * &gt;</type>
      <name>categorized_parameters</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>p3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>help_parameter</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>p4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>string</type>
      <name>param_file</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>p5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>string</type>
      <name>tmp_string</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>p6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>ParamFileCallback</type>
      <name>param_file_callback</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>p7</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend" protection="private">
      <type>friend class</type>
      <name>ParameterList</name>
      <anchorfile>classutilib_1_1ParameterSet.html</anchorfile>
      <anchor>n0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::parameter</name>
    <filename>classutilib_1_1parameter.html</filename>
    <member kind="function">
      <type></type>
      <name>parameter</name>
      <anchorfile>classutilib_1_1parameter.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(const char *name_, double minValue_, double value_, double maxValue_)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~parameter</name>
      <anchorfile>classutilib_1_1parameter.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1parameter.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>changeValue</name>
      <anchorfile>classutilib_1_1parameter.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(double newValue)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>hasBeenChanged</name>
      <anchorfile>classutilib_1_1parameter.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>readAllSerial</name>
      <anchorfile>classutilib_1_1parameter.html</anchorfile>
      <anchor>e0</anchor>
      <arglist>(int argc, char **argv, int minArgs=0, int maxArgs=1)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>readAllParallel</name>
      <anchorfile>classutilib_1_1parameter.html</anchorfile>
      <anchor>e1</anchor>
      <arglist>(int argc, char **argv, int minArgs=0, int maxArgs=1)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>howMany</name>
      <anchorfile>classutilib_1_1parameter.html</anchorfile>
      <anchor>e2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static unsigned int</type>
      <name>hash</name>
      <anchorfile>classutilib_1_1parameter.html</anchorfile>
      <anchor>h0</anchor>
      <arglist>(const char *name_)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static parameter *</type>
      <name>lookup</name>
      <anchorfile>classutilib_1_1parameter.html</anchorfile>
      <anchor>h1</anchor>
      <arglist>(const char *name_, int hashValue_)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static parameter *</type>
      <name>lookup</name>
      <anchorfile>classutilib_1_1parameter.html</anchorfile>
      <anchor>h2</anchor>
      <arglist>(const char *name_)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>parseLine</name>
      <anchorfile>classutilib_1_1parameter.html</anchorfile>
      <anchor>h3</anchor>
      <arglist>(int argc, char **arv, int minArgs=0, int maxArgs=1)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static void</type>
      <name>readFromFile</name>
      <anchorfile>classutilib_1_1parameter.html</anchorfile>
      <anchor>h4</anchor>
      <arglist>(const char *fileName)</arglist>
    </member>
    <member kind="function" protection="private" static="yes">
      <type>static double</type>
      <name>convert</name>
      <anchorfile>classutilib_1_1parameter.html</anchorfile>
      <anchor>h5</anchor>
      <arglist>(const char *buffer, const char *errorKey)</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>double</type>
      <name>value</name>
      <anchorfile>classutilib_1_1parameter.html</anchorfile>
      <anchor>r0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>double</type>
      <name>maxValue</name>
      <anchorfile>classutilib_1_1parameter.html</anchorfile>
      <anchor>r1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>double</type>
      <name>minValue</name>
      <anchorfile>classutilib_1_1parameter.html</anchorfile>
      <anchor>r2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>const char *</type>
      <name>name</name>
      <anchorfile>classutilib_1_1parameter.html</anchorfile>
      <anchor>r3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>changeCounter</name>
      <anchorfile>classutilib_1_1parameter.html</anchorfile>
      <anchor>r4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>parameter *</type>
      <name>next</name>
      <anchorfile>classutilib_1_1parameter.html</anchorfile>
      <anchor>r5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>parameter *</type>
      <name>prev</name>
      <anchorfile>classutilib_1_1parameter.html</anchorfile>
      <anchor>r6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static int</type>
      <name>numParameters</name>
      <anchorfile>classutilib_1_1parameter.html</anchorfile>
      <anchor>v0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static parameter *</type>
      <name>hashHeader</name>
      <anchorfile>classutilib_1_1parameter.html</anchorfile>
      <anchor>v1</anchor>
      <arglist>[tableSize]</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::ParameterValidatorBase</name>
    <filename>classutilib_1_1ParameterValidatorBase.html</filename>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~ParameterValidatorBase</name>
      <anchorfile>classutilib_1_1ParameterValidatorBase.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual bool</type>
      <name>validate</name>
      <anchorfile>classutilib_1_1ParameterValidatorBase.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(AnyReference &amp;info)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1ParameterValidatorBase.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(ostream &amp;os) const =0</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::ParameterValidatorFunction</name>
    <filename>classutilib_1_1ParameterValidatorFunction.html</filename>
    <templarg>Type</templarg>
  </compound>
  <compound kind="class">
    <name>utilib::ParameterValidator</name>
    <filename>classutilib_1_1ParameterValidator.html</filename>
    <templarg>Type</templarg>
    <templarg>ValidatorFuncT</templarg>
    <base>utilib::ParameterValidatorBase</base>
    <member kind="function">
      <type></type>
      <name>ParameterValidator</name>
      <anchorfile>classutilib_1_1ParameterValidator.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(ValidatorFuncT func)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>validate</name>
      <anchorfile>classutilib_1_1ParameterValidator.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(AnyReference &amp;info)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1ParameterValidator.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(ostream &amp;os) const </arglist>
    </member>
    <member kind="variable">
      <type>ValidatorFuncT</type>
      <name>test_func</name>
      <anchorfile>classutilib_1_1ParameterValidator.html</anchorfile>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::ParameterLowerBound</name>
    <filename>classutilib_1_1ParameterLowerBound.html</filename>
    <templarg>Type</templarg>
    <member kind="function">
      <type></type>
      <name>ParameterLowerBound</name>
      <anchorfile>classutilib_1_1ParameterLowerBound.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(Type lower_)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1ParameterLowerBound.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(ostream &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1ParameterLowerBound.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const Type &amp;x)</arglist>
    </member>
    <member kind="variable">
      <type>Type</type>
      <name>lower</name>
      <anchorfile>classutilib_1_1ParameterLowerBound.html</anchorfile>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::ParameterUpperBound</name>
    <filename>classutilib_1_1ParameterUpperBound.html</filename>
    <templarg>Type</templarg>
    <member kind="function">
      <type></type>
      <name>ParameterUpperBound</name>
      <anchorfile>classutilib_1_1ParameterUpperBound.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(Type upper_)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1ParameterUpperBound.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(ostream &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1ParameterUpperBound.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const Type &amp;x)</arglist>
    </member>
    <member kind="variable">
      <type>Type</type>
      <name>upper</name>
      <anchorfile>classutilib_1_1ParameterUpperBound.html</anchorfile>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::ParameterBounds</name>
    <filename>classutilib_1_1ParameterBounds.html</filename>
    <templarg>Type</templarg>
    <member kind="function">
      <type></type>
      <name>ParameterBounds</name>
      <anchorfile>classutilib_1_1ParameterBounds.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(Type lower_, Type upper_)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1ParameterBounds.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(ostream &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1ParameterBounds.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const Type &amp;x)</arglist>
    </member>
    <member kind="variable">
      <type>Type</type>
      <name>lower</name>
      <anchorfile>classutilib_1_1ParameterBounds.html</anchorfile>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Type</type>
      <name>upper</name>
      <anchorfile>classutilib_1_1ParameterBounds.html</anchorfile>
      <anchor>o1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::ParameterNonnegative</name>
    <filename>classutilib_1_1ParameterNonnegative.html</filename>
    <templarg>Type</templarg>
    <member kind="function">
      <type></type>
      <name>ParameterNonnegative</name>
      <anchorfile>classutilib_1_1ParameterNonnegative.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1ParameterNonnegative.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(ostream &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1ParameterNonnegative.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const Type &amp;x)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::ParameterPositive</name>
    <filename>classutilib_1_1ParameterPositive.html</filename>
    <templarg>Type</templarg>
    <member kind="function">
      <type></type>
      <name>ParameterPositive</name>
      <anchorfile>classutilib_1_1ParameterPositive.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1ParameterPositive.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(ostream &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1ParameterPositive.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const Type &amp;x)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::AnyRNG</name>
    <filename>classutilib_1_1AnyRNG.html</filename>
    <member kind="function">
      <type></type>
      <name>AnyRNG</name>
      <anchorfile>classutilib_1_1AnyRNG.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>AnyRNG</name>
      <anchorfile>classutilib_1_1AnyRNG.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(RNGT *rng)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>AnyRNG</name>
      <anchorfile>classutilib_1_1AnyRNG.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(AnyRNG *rng)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>AnyRNG</name>
      <anchorfile>classutilib_1_1AnyRNG.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(const AnyRNG &amp;other)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~AnyRNG</name>
      <anchorfile>classutilib_1_1AnyRNG.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>AnyRNG &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1AnyRNG.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(RNGT *rhs)</arglist>
    </member>
    <member kind="function">
      <type>AnyRNG &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1AnyRNG.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(const AnyRNG &amp;rhs)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>classutilib_1_1AnyRNG.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>(RNGT *rhs) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>classutilib_1_1AnyRNG.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>(const AnyRNG &amp;rhs) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator!=</name>
      <anchorfile>classutilib_1_1AnyRNG.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>(RNGT *rhs) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator!=</name>
      <anchorfile>classutilib_1_1AnyRNG.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>(const AnyRNG &amp;rhs) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>empty</name>
      <anchorfile>classutilib_1_1AnyRNG.html</anchorfile>
      <anchor>a11</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>operator bool</name>
      <anchorfile>classutilib_1_1AnyRNG.html</anchorfile>
      <anchor>a12</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>asFloat</name>
      <anchorfile>classutilib_1_1AnyRNG.html</anchorfile>
      <anchor>a13</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>asDouble</name>
      <anchorfile>classutilib_1_1AnyRNG.html</anchorfile>
      <anchor>a14</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>unsigned long</type>
      <name>asLong</name>
      <anchorfile>classutilib_1_1AnyRNG.html</anchorfile>
      <anchor>a15</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>reset</name>
      <anchorfile>classutilib_1_1AnyRNG.html</anchorfile>
      <anchor>a16</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1AnyRNG.html</anchorfile>
      <anchor>a17</anchor>
      <arglist>(ostream &amp;output) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1AnyRNG.html</anchorfile>
      <anchor>a18</anchor>
      <arglist>(istream &amp;input)</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>placeholder *</type>
      <name>content</name>
      <anchorfile>classutilib_1_1AnyRNG.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
    <class kind="class">utilib::AnyRNG::holder</class>
    <class kind="class">utilib::AnyRNG::placeholder</class>
  </compound>
  <compound kind="class">
    <name>utilib::AnyRNG::holder</name>
    <filename>classutilib_1_1AnyRNG_1_1holder.html</filename>
    <templarg>ValueType</templarg>
    <base>utilib::AnyRNG::placeholder</base>
    <member kind="function">
      <type></type>
      <name>holder</name>
      <anchorfile>classutilib_1_1AnyRNG_1_1holder.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(ValueType *value)</arglist>
    </member>
    <member kind="function">
      <type>placeholder *</type>
      <name>clone</name>
      <anchorfile>classutilib_1_1AnyRNG_1_1holder.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>asFloat</name>
      <anchorfile>classutilib_1_1AnyRNG_1_1holder.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>asDouble</name>
      <anchorfile>classutilib_1_1AnyRNG_1_1holder.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>long</type>
      <name>asLong</name>
      <anchorfile>classutilib_1_1AnyRNG_1_1holder.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>reset</name>
      <anchorfile>classutilib_1_1AnyRNG_1_1holder.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1AnyRNG_1_1holder.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(ostream &amp;ostr) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1AnyRNG_1_1holder.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>(istream &amp;istr)</arglist>
    </member>
    <member kind="function">
      <type>const void *</type>
      <name>voidptr</name>
      <anchorfile>classutilib_1_1AnyRNG_1_1holder.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="variable">
      <type>ValueType *</type>
      <name>held</name>
      <anchorfile>classutilib_1_1AnyRNG_1_1holder.html</anchorfile>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::AnyRNG::placeholder</name>
    <filename>classutilib_1_1AnyRNG_1_1placeholder.html</filename>
    <member kind="function" virtualness="pure">
      <type>virtual placeholder *</type>
      <name>clone</name>
      <anchorfile>classutilib_1_1AnyRNG_1_1placeholder.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>() const =0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual float</type>
      <name>asFloat</name>
      <anchorfile>classutilib_1_1AnyRNG_1_1placeholder.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual double</type>
      <name>asDouble</name>
      <anchorfile>classutilib_1_1AnyRNG_1_1placeholder.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual long</type>
      <name>asLong</name>
      <anchorfile>classutilib_1_1AnyRNG_1_1placeholder.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>reset</name>
      <anchorfile>classutilib_1_1AnyRNG_1_1placeholder.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1AnyRNG_1_1placeholder.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(ostream &amp;) const =0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1AnyRNG_1_1placeholder.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>(istream &amp;)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual const void *</type>
      <name>voidptr</name>
      <anchorfile>classutilib_1_1AnyRNG_1_1placeholder.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>() const =0</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::Binomial</name>
    <filename>classutilib_1_1Binomial.html</filename>
    <base>utilib::SimpleRandomVariable&lt; int &gt;</base>
    <member kind="function">
      <type></type>
      <name>Binomial</name>
      <anchorfile>classutilib_1_1Binomial.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Binomial</name>
      <anchorfile>classutilib_1_1Binomial.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(RNGT *gen, double _p=0.0, int _num=0)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>probability</name>
      <anchorfile>classutilib_1_1Binomial.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>probability</name>
      <anchorfile>classutilib_1_1Binomial.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(double _p)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>num_trials</name>
      <anchorfile>classutilib_1_1Binomial.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>num_trials</name>
      <anchorfile>classutilib_1_1Binomial.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(int _num)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1Binomial.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(double p_, int num_)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1Binomial.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>reset_generator</name>
      <anchorfile>classutilib_1_1Binomial.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>double</type>
      <name>p</name>
      <anchorfile>classutilib_1_1Binomial.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>int</type>
      <name>num</name>
      <anchorfile>classutilib_1_1Binomial.html</anchorfile>
      <anchor>p1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>Uniform</type>
      <name>uvar</name>
      <anchorfile>classutilib_1_1Binomial.html</anchorfile>
      <anchor>p2</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::Cauchy</name>
    <filename>classutilib_1_1Cauchy.html</filename>
    <base>utilib::ExternalRandomVariable&lt; double &gt;</base>
    <member kind="function">
      <type></type>
      <name>Cauchy</name>
      <anchorfile>classutilib_1_1Cauchy.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Cauchy</name>
      <anchorfile>classutilib_1_1Cauchy.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(RNGT *gen, double alpha_=0.0, double beta_=1.0)</arglist>
    </member>
    <member kind="variable">
      <type>double</type>
      <name>alpha</name>
      <anchorfile>classutilib_1_1Cauchy.html</anchorfile>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>double</type>
      <name>beta</name>
      <anchorfile>classutilib_1_1Cauchy.html</anchorfile>
      <anchor>o1</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" protection="protected">
      <type>double</type>
      <name>eval_c_func</name>
      <anchorfile>classutilib_1_1Cauchy.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::DUniform</name>
    <filename>classutilib_1_1DUniform.html</filename>
    <base>utilib::SimpleRandomVariable&lt; int &gt;</base>
    <member kind="function">
      <type></type>
      <name>DUniform</name>
      <anchorfile>classutilib_1_1DUniform.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>DUniform</name>
      <anchorfile>classutilib_1_1DUniform.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(RNGT *gen, int low=0, int high=1)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>low</name>
      <anchorfile>classutilib_1_1DUniform.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>low</name>
      <anchorfile>classutilib_1_1DUniform.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(int x)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>high</name>
      <anchorfile>classutilib_1_1DUniform.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>high</name>
      <anchorfile>classutilib_1_1DUniform.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(int x)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1DUniform.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1DUniform.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>(int low_, int high_)</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>int</type>
      <name>pLow</name>
      <anchorfile>classutilib_1_1DUniform.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>int</type>
      <name>pHigh</name>
      <anchorfile>classutilib_1_1DUniform.html</anchorfile>
      <anchor>p1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>int</type>
      <name>delta</name>
      <anchorfile>classutilib_1_1DUniform.html</anchorfile>
      <anchor>p2</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::Exponential</name>
    <filename>classutilib_1_1Exponential.html</filename>
    <base>utilib::SimpleRandomVariable&lt; double &gt;</base>
    <member kind="function">
      <type></type>
      <name>Exponential</name>
      <anchorfile>classutilib_1_1Exponential.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Exponential</name>
      <anchorfile>classutilib_1_1Exponential.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(RNGT *gen, double _meanval=1.0)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>meanval</name>
      <anchorfile>classutilib_1_1Exponential.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>meanval</name>
      <anchorfile>classutilib_1_1Exponential.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(double _b)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1Exponential.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>reset_generator</name>
      <anchorfile>classutilib_1_1Exponential.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>double</type>
      <name>b</name>
      <anchorfile>classutilib_1_1Exponential.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>Uniform</type>
      <name>uvar</name>
      <anchorfile>classutilib_1_1Exponential.html</anchorfile>
      <anchor>p1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::Geometric</name>
    <filename>classutilib_1_1Geometric.html</filename>
    <base>utilib::SimpleRandomVariable&lt; int &gt;</base>
    <member kind="function">
      <type></type>
      <name>Geometric</name>
      <anchorfile>classutilib_1_1Geometric.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Geometric</name>
      <anchorfile>classutilib_1_1Geometric.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(RNGT *gen, double _p=0.0)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>probability</name>
      <anchorfile>classutilib_1_1Geometric.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>probability</name>
      <anchorfile>classutilib_1_1Geometric.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(double _p)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1Geometric.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(double _p)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1Geometric.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>reset_generator</name>
      <anchorfile>classutilib_1_1Geometric.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>double</type>
      <name>p</name>
      <anchorfile>classutilib_1_1Geometric.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>Uniform</type>
      <name>uvar</name>
      <anchorfile>classutilib_1_1Geometric.html</anchorfile>
      <anchor>p1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::LCG</name>
    <filename>classutilib_1_1LCG.html</filename>
    <base>utilib::RNG</base>
    <member kind="function">
      <type></type>
      <name>LCG</name>
      <anchorfile>classutilib_1_1LCG.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(unsigned short seed=0)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>reseed</name>
      <anchorfile>classutilib_1_1LCG.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(unsigned seed)</arglist>
    </member>
    <member kind="function">
      <type>unsigned short</type>
      <name>getSeed</name>
      <anchorfile>classutilib_1_1LCG.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>reset</name>
      <anchorfile>classutilib_1_1LCG.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>unsigned long</type>
      <name>asLong</name>
      <anchorfile>classutilib_1_1LCG.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1LCG.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(ostream &amp;output) const </arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1LCG.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(istream &amp;input)</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>unsigned short</type>
      <name>State</name>
      <anchorfile>classutilib_1_1LCG.html</anchorfile>
      <anchor>r0</anchor>
      <arglist>[3]</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>unsigned short</type>
      <name>Seed</name>
      <anchorfile>classutilib_1_1LCG.html</anchorfile>
      <anchor>r1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::LogNormal</name>
    <filename>classutilib_1_1LogNormal.html</filename>
    <base>utilib::ExternalRandomVariable&lt; double &gt;</base>
    <member kind="function">
      <type></type>
      <name>LogNormal</name>
      <anchorfile>classutilib_1_1LogNormal.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>LogNormal</name>
      <anchorfile>classutilib_1_1LogNormal.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(RNGT *gen, double _m=0.0, double _s=1.0)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1LogNormal.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(double _scale, double _shape)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1LogNormal.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>double</type>
      <name>scale</name>
      <anchorfile>classutilib_1_1LogNormal.html</anchorfile>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>double</type>
      <name>shape</name>
      <anchorfile>classutilib_1_1LogNormal.html</anchorfile>
      <anchor>o1</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" protection="protected">
      <type>double</type>
      <name>eval_c_func</name>
      <anchorfile>classutilib_1_1LogNormal.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::MNormal</name>
    <filename>classutilib_1_1MNormal.html</filename>
    <base>utilib::GeneralRandomVariable&lt; DoubleVector &gt;</base>
    <member kind="function">
      <type></type>
      <name>MNormal</name>
      <anchorfile>classutilib_1_1MNormal.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>MNormal</name>
      <anchorfile>classutilib_1_1MNormal.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(RNGT *gen, int ndim=0, double meanval=0.0, double covval=1.0)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>MNormal</name>
      <anchorfile>classutilib_1_1MNormal.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(RNGT *gen, DoubleVector &amp;mean_vec, DoubleMatrix &amp;covariance)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>MNormal</name>
      <anchorfile>classutilib_1_1MNormal.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(RNGT *gen, DoubleVector &amp;mean_vec, DoubleVector &amp;covariance)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>MNormal</name>
      <anchorfile>classutilib_1_1MNormal.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(RNGT *gen, DoubleVector &amp;mean_vec, double covariance)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set</name>
      <anchorfile>classutilib_1_1MNormal.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(DoubleVector &amp;mean_vec, DoubleMatrix &amp;covariance)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set</name>
      <anchorfile>classutilib_1_1MNormal.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(DoubleVector &amp;mean_vec, DoubleVector &amp;covariance)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set</name>
      <anchorfile>classutilib_1_1MNormal.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>(DoubleVector &amp;mean_vec, double covariance)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set</name>
      <anchorfile>classutilib_1_1MNormal.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>(int dimension, double mean_val=0.0, double cov_val=1.0)</arglist>
    </member>
    <member kind="function">
      <type>DoubleVector &amp;</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1MNormal.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1MNormal.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>(DoubleVector &amp;new_vec)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>reset_generator</name>
      <anchorfile>classutilib_1_1MNormal.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>Normal</type>
      <name>norm_rng</name>
      <anchorfile>classutilib_1_1MNormal.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>DoubleMatrix</type>
      <name>CF_matrix</name>
      <anchorfile>classutilib_1_1MNormal.html</anchorfile>
      <anchor>p1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>DoubleVector</type>
      <name>mean_vec</name>
      <anchorfile>classutilib_1_1MNormal.html</anchorfile>
      <anchor>p2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>DoubleVector</type>
      <name>diag_covariance</name>
      <anchorfile>classutilib_1_1MNormal.html</anchorfile>
      <anchor>p3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>double</type>
      <name>const_covariance</name>
      <anchorfile>classutilib_1_1MNormal.html</anchorfile>
      <anchor>p4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>int</type>
      <name>pd_flag</name>
      <anchorfile>classutilib_1_1MNormal.html</anchorfile>
      <anchor>p5</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::MUniform</name>
    <filename>classutilib_1_1MUniform.html</filename>
    <base>utilib::GeneralRandomVariable&lt; DoubleVector &gt;</base>
    <member kind="function">
      <type></type>
      <name>MUniform</name>
      <anchorfile>classutilib_1_1MUniform.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>MUniform</name>
      <anchorfile>classutilib_1_1MUniform.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(RNGT *gen, DoubleVector &amp;mean_, DoubleVector &amp;range_vec_)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>MUniform</name>
      <anchorfile>classutilib_1_1MUniform.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(RNGT *gen, DoubleVector &amp;mean_, double range_const_)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~MUniform</name>
      <anchorfile>classutilib_1_1MUniform.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set</name>
      <anchorfile>classutilib_1_1MUniform.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(DoubleVector &amp;mean, DoubleVector &amp;range_vec_)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set</name>
      <anchorfile>classutilib_1_1MUniform.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(DoubleVector &amp;mean, double range_const_)</arglist>
    </member>
    <member kind="function">
      <type>DoubleVector &amp;</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1MUniform.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1MUniform.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>(DoubleVector &amp;new_vec)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>reset_generator</name>
      <anchorfile>classutilib_1_1MUniform.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>Uniform</type>
      <name>unif_dev</name>
      <anchorfile>classutilib_1_1MUniform.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>DoubleVector</type>
      <name>mean</name>
      <anchorfile>classutilib_1_1MUniform.html</anchorfile>
      <anchor>p1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>DoubleVector</type>
      <name>range_vec</name>
      <anchorfile>classutilib_1_1MUniform.html</anchorfile>
      <anchor>p2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>double</type>
      <name>range_const</name>
      <anchorfile>classutilib_1_1MUniform.html</anchorfile>
      <anchor>p3</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::Normal</name>
    <filename>classutilib_1_1Normal.html</filename>
    <base>utilib::ExternalRandomVariable&lt; double &gt;</base>
    <member kind="function">
      <type></type>
      <name>Normal</name>
      <anchorfile>classutilib_1_1Normal.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Normal</name>
      <anchorfile>classutilib_1_1Normal.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(RNGT *gen, double _mean=0.0, double _variance=1.0)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1Normal.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(double _mean, double _variance)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1Normal.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>double</type>
      <name>mean</name>
      <anchorfile>classutilib_1_1Normal.html</anchorfile>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>double</type>
      <name>stddev</name>
      <anchorfile>classutilib_1_1Normal.html</anchorfile>
      <anchor>o1</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" protection="protected">
      <type>double</type>
      <name>eval_c_func</name>
      <anchorfile>classutilib_1_1Normal.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::PM_LCG</name>
    <filename>classutilib_1_1PM__LCG.html</filename>
    <base>utilib::RNG</base>
    <member kind="function">
      <type></type>
      <name>PM_LCG</name>
      <anchorfile>classutilib_1_1PM__LCG.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(int seed=0)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>reseed</name>
      <anchorfile>classutilib_1_1PM__LCG.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(int seed=0)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getSeed</name>
      <anchorfile>classutilib_1_1PM__LCG.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getState</name>
      <anchorfile>classutilib_1_1PM__LCG.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>reset</name>
      <anchorfile>classutilib_1_1PM__LCG.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>unsigned long</type>
      <name>asLong</name>
      <anchorfile>classutilib_1_1PM__LCG.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>asDouble</name>
      <anchorfile>classutilib_1_1PM__LCG.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1PM__LCG.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>(ostream &amp;output) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1PM__LCG.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>(istream &amp;input)</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>state</name>
      <anchorfile>classutilib_1_1PM__LCG.html</anchorfile>
      <anchor>r0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>jseed</name>
      <anchorfile>classutilib_1_1PM__LCG.html</anchorfile>
      <anchor>r1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::RandomVariableBase</name>
    <filename>classutilib_1_1RandomVariableBase.html</filename>
    <member kind="function">
      <type></type>
      <name>RandomVariableBase</name>
      <anchorfile>classutilib_1_1RandomVariableBase.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~RandomVariableBase</name>
      <anchorfile>classutilib_1_1RandomVariableBase.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const AnyRNG &amp;</type>
      <name>generator</name>
      <anchorfile>classutilib_1_1RandomVariableBase.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>AnyRNG</type>
      <name>generator</name>
      <anchorfile>classutilib_1_1RandomVariableBase.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>generator</name>
      <anchorfile>classutilib_1_1RandomVariableBase.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(RNGT *p)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>generator</name>
      <anchorfile>classutilib_1_1RandomVariableBase.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(AnyRNG *p)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>reset_generator</name>
      <anchorfile>classutilib_1_1RandomVariableBase.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>AnyRNG</type>
      <name>pGenerator</name>
      <anchorfile>classutilib_1_1RandomVariableBase.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::SimpleRandomVariable</name>
    <filename>classutilib_1_1SimpleRandomVariable.html</filename>
    <templarg>DomainT</templarg>
    <base>utilib::RandomVariableBase</base>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~SimpleRandomVariable</name>
      <anchorfile>classutilib_1_1SimpleRandomVariable.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual DomainT</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1SimpleRandomVariable.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>()=0</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::GeneralRandomVariable</name>
    <filename>classutilib_1_1GeneralRandomVariable.html</filename>
    <templarg>DomainT</templarg>
    <base>utilib::RandomVariableBase</base>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~GeneralRandomVariable</name>
      <anchorfile>classutilib_1_1GeneralRandomVariable.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual DomainT &amp;</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1GeneralRandomVariable.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>DomainT</type>
      <name>results</name>
      <anchorfile>classutilib_1_1GeneralRandomVariable.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::ExternalRandomVariable</name>
    <filename>classutilib_1_1ExternalRandomVariable.html</filename>
    <templarg>DomainT</templarg>
    <base>utilib::SimpleRandomVariable</base>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~ExternalRandomVariable</name>
      <anchorfile>classutilib_1_1ExternalRandomVariable.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>DomainT</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1ExternalRandomVariable.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private" virtualness="pure">
      <type>virtual DomainT</type>
      <name>eval_c_func</name>
      <anchorfile>classutilib_1_1ExternalRandomVariable.html</anchorfile>
      <anchor>d0</anchor>
      <arglist>()=0</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::RNG</name>
    <filename>classutilib_1_1RNG.html</filename>
    <member kind="function" virtualness="pure">
      <type>virtual unsigned long</type>
      <name>asLong</name>
      <anchorfile>classutilib_1_1RNG.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>reset</name>
      <anchorfile>classutilib_1_1RNG.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual float</type>
      <name>asFloat</name>
      <anchorfile>classutilib_1_1RNG.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual double</type>
      <name>asDouble</name>
      <anchorfile>classutilib_1_1RNG.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1RNG.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(ostream &amp;output) const =0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1RNG.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(istream &amp;input)=0</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::SampleGenerator</name>
    <filename>classutilib_1_1SampleGenerator.html</filename>
    <templarg>DomainT</templarg>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>reset</name>
      <anchorfile>classutilib_1_1SampleGenerator.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>new_point</name>
      <anchorfile>classutilib_1_1SampleGenerator.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(DomainT &amp;)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>set_rng</name>
      <anchorfile>classutilib_1_1SampleGenerator.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(AnyRNG &amp;)=0</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::UniformSampleGenerator</name>
    <filename>classutilib_1_1UniformSampleGenerator.html</filename>
    <templarg>DomainT</templarg>
    <member kind="function">
      <type>void</type>
      <name>reset</name>
      <anchorfile>classutilib_1_1UniformSampleGenerator.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>new_point</name>
      <anchorfile>classutilib_1_1UniformSampleGenerator.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(DomainT &amp;)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_rng</name>
      <anchorfile>classutilib_1_1UniformSampleGenerator.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(AnyRNG &amp;)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::UniformSampleGenerator&lt; BasicArray&lt; double &gt; &gt;</name>
    <filename>classutilib_1_1UniformSampleGenerator_3_01BasicArray_3_01double_01_4_01_4.html</filename>
    <member kind="function">
      <type>void</type>
      <name>reset</name>
      <anchorfile>classutilib_1_1UniformSampleGenerator_3_01BasicArray_3_01double_01_4_01_4.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>new_point</name>
      <anchorfile>classutilib_1_1UniformSampleGenerator_3_01BasicArray_3_01double_01_4_01_4.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(BasicArray&lt; double &gt; &amp;pt)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_rng</name>
      <anchorfile>classutilib_1_1UniformSampleGenerator_3_01BasicArray_3_01double_01_4_01_4.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(AnyRNG &amp;rng)</arglist>
    </member>
    <member kind="variable">
      <type>BasicArray&lt; double &gt;</type>
      <name>l_bound</name>
      <anchorfile>classutilib_1_1UniformSampleGenerator_3_01BasicArray_3_01double_01_4_01_4.html</anchorfile>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>BasicArray&lt; double &gt;</type>
      <name>u_bound</name>
      <anchorfile>classutilib_1_1UniformSampleGenerator_3_01BasicArray_3_01double_01_4_01_4.html</anchorfile>
      <anchor>o1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Uniform</type>
      <name>urnd</name>
      <anchorfile>classutilib_1_1UniformSampleGenerator_3_01BasicArray_3_01double_01_4_01_4.html</anchorfile>
      <anchor>o2</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::Triangular</name>
    <filename>classutilib_1_1Triangular.html</filename>
    <base>utilib::SimpleRandomVariable&lt; double &gt;</base>
    <member kind="function">
      <type></type>
      <name>Triangular</name>
      <anchorfile>classutilib_1_1Triangular.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Triangular</name>
      <anchorfile>classutilib_1_1Triangular.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(RNGT *gen, double _scale=1.0, int _trunc_flag=TRUNCATED_TRIANGULAR)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1Triangular.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const double lbound, const double rbound)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1Triangular.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>truncation</name>
      <anchorfile>classutilib_1_1Triangular.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(const int flag)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>reset_generator</name>
      <anchorfile>classutilib_1_1Triangular.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>Uniform</type>
      <name>urnd</name>
      <anchorfile>classutilib_1_1Triangular.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>double</type>
      <name>scale</name>
      <anchorfile>classutilib_1_1Triangular.html</anchorfile>
      <anchor>p1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>int</type>
      <name>trunc_flag</name>
      <anchorfile>classutilib_1_1Triangular.html</anchorfile>
      <anchor>p2</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::Uniform</name>
    <filename>classutilib_1_1Uniform.html</filename>
    <base>utilib::SimpleRandomVariable&lt; double &gt;</base>
    <member kind="function">
      <type></type>
      <name>Uniform</name>
      <anchorfile>classutilib_1_1Uniform.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Uniform</name>
      <anchorfile>classutilib_1_1Uniform.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(RNGT *gen, double low=0.0, double high=1.0)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>low</name>
      <anchorfile>classutilib_1_1Uniform.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>low</name>
      <anchorfile>classutilib_1_1Uniform.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(double x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>high</name>
      <anchorfile>classutilib_1_1Uniform.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>high</name>
      <anchorfile>classutilib_1_1Uniform.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(double x)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual double</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1Uniform.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>double</type>
      <name>pLow</name>
      <anchorfile>classutilib_1_1Uniform.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>double</type>
      <name>pHigh</name>
      <anchorfile>classutilib_1_1Uniform.html</anchorfile>
      <anchor>p1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>double</type>
      <name>delta</name>
      <anchorfile>classutilib_1_1Uniform.html</anchorfile>
      <anchor>p2</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::AbstractHashTable</name>
    <filename>classutilib_1_1AbstractHashTable.html</filename>
    <templarg>T</templarg>
    <templarg>KEY</templarg>
    <templarg>InfoT</templarg>
    <member kind="typedef">
      <type>size_t</type>
      <name>size_type</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>w0</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>ListItem&lt; T * &gt; *</type>
      <name>hash_element</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>w1</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>ListItem&lt; T * &gt;</type>
      <name>_Node</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>w2</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>__LinkedList_iterator&lt; T, T &amp;, T *, _Node, __LinkedList_Pointer_OpClass&lt; T &amp;, _Node &gt; &gt;</type>
      <name>iterator</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>w3</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>__LinkedList_iterator&lt; T, const T &amp;, const T *, _Node, __LinkedList_Pointer_OpClass&lt; const T &amp;, _Node &gt; &gt;</type>
      <name>const_iterator</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>w4</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>AbstractHashTable</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(const char *nameBuff=&quot;Unnamed&quot;)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>AbstractHashTable</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(size_type initial_size, const char *nameBuff=&quot;Unnamed&quot;)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~AbstractHashTable</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>empty</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>operator bool</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>size</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>iterator</type>
      <name>begin</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const_iterator</type>
      <name>begin</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>iterator</type>
      <name>end</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const_iterator</type>
      <name>end</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual iterator</type>
      <name>find</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>(const KEY &amp;key)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual const_iterator</type>
      <name>find</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>a11</anchor>
      <arglist>(const KEY &amp;key) const </arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>find</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>a12</anchor>
      <arglist>(const KEY &amp;key, const_iterator &amp;begin, const_iterator &amp;end)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>exists</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>a13</anchor>
      <arglist>(const KEY &amp;key)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual iterator</type>
      <name>add</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>a14</anchor>
      <arglist>(const KEY &amp;key, InfoT &amp;info, bool use_cached_hash=false)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>remove</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>a15</anchor>
      <arglist>(const KEY &amp;key, bool &amp;status)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>remove</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>a16</anchor>
      <arglist>(iterator &amp;item, bool &amp;status)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>remove</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>a17</anchor>
      <arglist>(iterator &amp;item, KEY &amp;key, bool &amp;status)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>a18</anchor>
      <arglist>(ostream &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>a19</anchor>
      <arglist>(istream &amp;is)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>a20</anchor>
      <arglist>(PackBuffer &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>a21</anchor>
      <arglist>(UnPackBuffer &amp;is)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>clear</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>a22</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>statistics</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>a23</anchor>
      <arglist>(ostream &amp;os)</arglist>
    </member>
    <member kind="function">
      <type>bool &amp;</type>
      <name>ignore_duplicates</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>a24</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_hash_fn</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>a25</anchor>
      <arglist>(size_type(*hashfn)(const KEY &amp;, size_type))</arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>using_prime_ht</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_type(*</type>
      <name>curr_hashfn</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>o1</anchor>
      <arglist>)(const KEY &amp;, size_type)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual T *</type>
      <name>create_item</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>(const KEY *key, const InfoT *info)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual size_type</type>
      <name>hash</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>b1</anchor>
      <arglist>(const KEY &amp;key, size_type table_size) const </arglist>
    </member>
    <member kind="function" protection="protected">
      <type>iterator</type>
      <name>insert</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>b2</anchor>
      <arglist>(T *item, const KEY *key, InfoT *info, bool use_cached_hash=false, const bool resize_OK=true)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>extract</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>b3</anchor>
      <arglist>(T *item, bool &amp;status)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>resize</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>b4</anchor>
      <arglist>(const int newsize=-1)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual bool</type>
      <name>exists_item</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>b5</anchor>
      <arglist>(const T *item)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual bool</type>
      <name>exists_key</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>b6</anchor>
      <arglist>(const KEY &amp;key)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual bool</type>
      <name>exists_core</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>b7</anchor>
      <arglist>(const KEY &amp;key)</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>LinkedList&lt; T * &gt;</type>
      <name>data</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>BasicArray&lt; ListItem&lt; T * &gt; * &gt;</type>
      <name>table</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>p1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>BasicArray&lt; int &gt;</type>
      <name>bucket_size</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>p2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>double</type>
      <name>max_load</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>p3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>ignore_duplicates_flag</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>p4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>size_type</type>
      <name>curr_index</name>
      <anchorfile>classutilib_1_1AbstractHashTable.html</anchorfile>
      <anchor>p5</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::AbstractHeap</name>
    <filename>classutilib_1_1AbstractHeap.html</filename>
    <templarg>T</templarg>
    <templarg>KEY</templarg>
    <templarg>Compare</templarg>
    <member kind="typedef">
      <type>T</type>
      <name>item_t</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>w0</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>AbstractHeap</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(const char *nameBuff=&quot;Unnamed&quot;, int initSize=-1, int quantumChoice=-1)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~AbstractHeap</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>empty</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>operator bool</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>size</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>T *</type>
      <name>top</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>T *</type>
      <name>member</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(int elem) const </arglist>
    </member>
    <member kind="function">
      <type>T *</type>
      <name>find</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>(KEY &amp;key)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>add</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>(KEY &amp;key)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>add</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>(KEY &amp;key, T *&amp;item)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>remove</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>(KEY &amp;key, bool &amp;status)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>remove</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>a11</anchor>
      <arglist>(T *&amp;item, bool &amp;status)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>remove</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>a12</anchor>
      <arglist>(T *&amp;item, KEY &amp;key, bool &amp;status)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>refloat</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>a13</anchor>
      <arglist>(KEY &amp;key, bool &amp;status)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>floatUp</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>a14</anchor>
      <arglist>(KEY &amp;key, bool &amp;status)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>sinkDown</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>a15</anchor>
      <arglist>(KEY &amp;key, bool &amp;status)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>write</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>a16</anchor>
      <arglist>(ostream &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>read</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>a17</anchor>
      <arglist>(istream &amp;is)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>prune</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>a18</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>clear</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>a19</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>validate</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>a20</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>refloatElement</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>a21</anchor>
      <arglist>(int elem)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>reheapify</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>a22</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const int</type>
      <name>defaultSize</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>s0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const int</type>
      <name>defaultQuantum</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>s1</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual T *</type>
      <name>insert</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>(KEY *item)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>extract</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>b1</anchor>
      <arglist>(T *item, bool &amp;status)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="pure">
      <type>virtual int &amp;</type>
      <name>element</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>b2</anchor>
      <arglist>(T *item)=0</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual int</type>
      <name>prunable</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>b3</anchor>
      <arglist>(T *)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>moveEffect</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>b4</anchor>
      <arglist>(T *)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>addEffect</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>b5</anchor>
      <arglist>(T *)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>removeEffect</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>b6</anchor>
      <arglist>(T *item)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>pruneEffect</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>b7</anchor>
      <arglist>(T *)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>swap</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>b8</anchor>
      <arglist>(int a, int b)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>int</type>
      <name>floatUp</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>b9</anchor>
      <arglist>(int elem)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>int</type>
      <name>sinkDown</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>b10</anchor>
      <arglist>(int elem)</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>int</type>
      <name>heap_size</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>int</type>
      <name>used</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>p1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>int</type>
      <name>allocated</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>p2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>int</type>
      <name>quantum</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>p3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>T **</type>
      <name>tree</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>p4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>const char *</type>
      <name>name</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>p5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>Compare</type>
      <name>compare</name>
      <anchorfile>classutilib_1_1AbstractHeap.html</anchorfile>
      <anchor>p6</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::AbstractSplayTree</name>
    <filename>classutilib_1_1AbstractSplayTree.html</filename>
    <templarg>T</templarg>
    <templarg>KEY</templarg>
    <member kind="function">
      <type></type>
      <name>AbstractSplayTree</name>
      <anchorfile>classutilib_1_1AbstractSplayTree.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(const char *nameBuff=&quot;Unnamed&quot;)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~AbstractSplayTree</name>
      <anchorfile>classutilib_1_1AbstractSplayTree.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>AbstractSplayTree&lt; T, KEY &gt; &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1AbstractSplayTree.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const AbstractSplayTree&lt; T, KEY &gt; &amp;tree)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>empty</name>
      <anchorfile>classutilib_1_1AbstractSplayTree.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>operator bool</name>
      <anchorfile>classutilib_1_1AbstractSplayTree.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>size</name>
      <anchorfile>classutilib_1_1AbstractSplayTree.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>OrderSense</type>
      <name>sense</name>
      <anchorfile>classutilib_1_1AbstractSplayTree.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setSense</name>
      <anchorfile>classutilib_1_1AbstractSplayTree.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>(OrderSense sense_)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>splay</name>
      <anchorfile>classutilib_1_1AbstractSplayTree.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>(const KEY &amp;key)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual T *</type>
      <name>find_rank</name>
      <anchorfile>classutilib_1_1AbstractSplayTree.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>(int r)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual const T *</type>
      <name>find_rank</name>
      <anchorfile>classutilib_1_1AbstractSplayTree.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>(int r) const </arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual T *</type>
      <name>find</name>
      <anchorfile>classutilib_1_1AbstractSplayTree.html</anchorfile>
      <anchor>a11</anchor>
      <arglist>(const KEY &amp;key)</arglist>
    </member>
    <member kind="function">
      <type>T *</type>
      <name>top</name>
      <anchorfile>classutilib_1_1AbstractSplayTree.html</anchorfile>
      <anchor>a12</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const T *</type>
      <name>top</name>
      <anchorfile>classutilib_1_1AbstractSplayTree.html</anchorfile>
      <anchor>a13</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>add</name>
      <anchorfile>classutilib_1_1AbstractSplayTree.html</anchorfile>
      <anchor>a14</anchor>
      <arglist>(KEY &amp;key)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>add</name>
      <anchorfile>classutilib_1_1AbstractSplayTree.html</anchorfile>
      <anchor>a15</anchor>
      <arglist>(KEY &amp;key, T *&amp;item)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>remove</name>
      <anchorfile>classutilib_1_1AbstractSplayTree.html</anchorfile>
      <anchor>a16</anchor>
      <arglist>(KEY &amp;key, bool &amp;status)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>remove</name>
      <anchorfile>classutilib_1_1AbstractSplayTree.html</anchorfile>
      <anchor>a17</anchor>
      <arglist>(T *&amp;item, bool &amp;status)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>remove</name>
      <anchorfile>classutilib_1_1AbstractSplayTree.html</anchorfile>
      <anchor>a18</anchor>
      <arglist>(T *&amp;item, KEY &amp;key, bool &amp;status)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>write</name>
      <anchorfile>classutilib_1_1AbstractSplayTree.html</anchorfile>
      <anchor>a19</anchor>
      <arglist>(ostream &amp;os)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>read</name>
      <anchorfile>classutilib_1_1AbstractSplayTree.html</anchorfile>
      <anchor>a20</anchor>
      <arglist>(istream &amp;is)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>rank</name>
      <anchorfile>classutilib_1_1AbstractSplayTree.html</anchorfile>
      <anchor>a21</anchor>
      <arglist>(const KEY &amp;key)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>clear</name>
      <anchorfile>classutilib_1_1AbstractSplayTree.html</anchorfile>
      <anchor>a22</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>T *</type>
      <name>exec_splay</name>
      <anchorfile>classutilib_1_1AbstractSplayTree.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>(const KEY *key, T *item)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual T *</type>
      <name>insert</name>
      <anchorfile>classutilib_1_1AbstractSplayTree.html</anchorfile>
      <anchor>b1</anchor>
      <arglist>(KEY *key)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>extract</name>
      <anchorfile>classutilib_1_1AbstractSplayTree.html</anchorfile>
      <anchor>b2</anchor>
      <arglist>(T *item, bool &amp;status)</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>T *</type>
      <name>tree</name>
      <anchorfile>classutilib_1_1AbstractSplayTree.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>OrderSense</type>
      <name>Sense</name>
      <anchorfile>classutilib_1_1AbstractSplayTree.html</anchorfile>
      <anchor>p1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>const char *</type>
      <name>name</name>
      <anchorfile>classutilib_1_1AbstractSplayTree.html</anchorfile>
      <anchor>p2</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::AnyValue</name>
    <filename>classutilib_1_1AnyValue.html</filename>
    <member kind="function">
      <type></type>
      <name>AnyValue</name>
      <anchorfile>classutilib_1_1AnyValue.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>AnyValue</name>
      <anchorfile>classutilib_1_1AnyValue.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(const ValueType &amp;value)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>AnyValue</name>
      <anchorfile>classutilib_1_1AnyValue.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const AnyValue &amp;other)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~AnyValue</name>
      <anchorfile>classutilib_1_1AnyValue.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>AnyValue &amp;</type>
      <name>swap</name>
      <anchorfile>classutilib_1_1AnyValue.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(AnyValue &amp;rhs)</arglist>
    </member>
    <member kind="function">
      <type>AnyValue &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1AnyValue.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(const ValueType &amp;rhs)</arglist>
    </member>
    <member kind="function">
      <type>AnyValue &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1AnyValue.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(const AnyValue &amp;rhs)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>empty</name>
      <anchorfile>classutilib_1_1AnyValue.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>const std::type_info &amp;</type>
      <name>type</name>
      <anchorfile>classutilib_1_1AnyValue.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1AnyValue.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>(ostream &amp;ostr) const </arglist>
    </member>
    <member kind="variable" protection="private">
      <type>placeholder *</type>
      <name>content</name>
      <anchorfile>classutilib_1_1AnyValue.html</anchorfile>
      <anchor>r0</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend" protection="private">
      <type>friend ValueType *</type>
      <name>any_cast</name>
      <anchorfile>classutilib_1_1AnyValue.html</anchorfile>
      <anchor>n0</anchor>
      <arglist>(AnyValue *)</arglist>
    </member>
    <class kind="class">utilib::AnyValue::holder</class>
    <class kind="class">utilib::AnyValue::placeholder</class>
  </compound>
  <compound kind="class">
    <name>utilib::AnyReference</name>
    <filename>classutilib_1_1AnyReference.html</filename>
    <member kind="function">
      <type></type>
      <name>AnyReference</name>
      <anchorfile>classutilib_1_1AnyReference.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>AnyReference</name>
      <anchorfile>classutilib_1_1AnyReference.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(RefType *value)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>AnyReference</name>
      <anchorfile>classutilib_1_1AnyReference.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const AnyReference &amp;other)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~AnyReference</name>
      <anchorfile>classutilib_1_1AnyReference.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>AnyReference &amp;</type>
      <name>swap</name>
      <anchorfile>classutilib_1_1AnyReference.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(AnyReference &amp;rhs)</arglist>
    </member>
    <member kind="function">
      <type>AnyReference &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1AnyReference.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(const ValueType &amp;rhs)</arglist>
    </member>
    <member kind="function">
      <type>AnyReference &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1AnyReference.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(const char *rhs)</arglist>
    </member>
    <member kind="function">
      <type>AnyReference &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1AnyReference.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>(const AnyReference &amp;rhs)</arglist>
    </member>
    <member kind="function">
      <type>AnyReference &amp;</type>
      <name>set_ref</name>
      <anchorfile>classutilib_1_1AnyReference.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>(RefType &amp;rhs)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>empty</name>
      <anchorfile>classutilib_1_1AnyReference.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>const std::type_info &amp;</type>
      <name>type</name>
      <anchorfile>classutilib_1_1AnyReference.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1AnyReference.html</anchorfile>
      <anchor>a11</anchor>
      <arglist>(ostream &amp;ostr) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1AnyReference.html</anchorfile>
      <anchor>a12</anchor>
      <arglist>(istream &amp;istr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1AnyReference.html</anchorfile>
      <anchor>a13</anchor>
      <arglist>(PackBuffer &amp;ostr) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1AnyReference.html</anchorfile>
      <anchor>a14</anchor>
      <arglist>(UnPackBuffer &amp;istr)</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>placeholder *</type>
      <name>content</name>
      <anchorfile>classutilib_1_1AnyReference.html</anchorfile>
      <anchor>r0</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend" protection="private">
      <type>friend ValueType *</type>
      <name>any_cast</name>
      <anchorfile>classutilib_1_1AnyReference.html</anchorfile>
      <anchor>n0</anchor>
      <arglist>(AnyReference *)</arglist>
    </member>
    <class kind="class">utilib::AnyReference::holder</class>
    <class kind="class">utilib::AnyReference::placeholder</class>
  </compound>
  <compound kind="class">
    <name>utilib::bad_any_cast</name>
    <filename>classutilib_1_1bad__any__cast.html</filename>
    <member kind="function">
      <type></type>
      <name>bad_any_cast</name>
      <anchorfile>classutilib_1_1bad__any__cast.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(const char *msg_)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual const char *</type>
      <name>what</name>
      <anchorfile>classutilib_1_1bad__any__cast.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>const char *</type>
      <name>msg</name>
      <anchorfile>classutilib_1_1bad__any__cast.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::ArrayBase</name>
    <filename>classutilib_1_1ArrayBase.html</filename>
    <templarg>A</templarg>
    <templarg>P</templarg>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~ArrayBase</name>
      <anchorfile>classutilib_1_1ArrayBase.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>shared_mem</name>
      <anchorfile>classutilib_1_1ArrayBase.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>resize</name>
      <anchorfile>classutilib_1_1ArrayBase.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const size_type newl, int set_new_contents=1)</arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>size</name>
      <anchorfile>classutilib_1_1ArrayBase.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual size_type</type>
      <name>alloc_size</name>
      <anchorfile>classutilib_1_1ArrayBase.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(size_type l) const </arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>allocation</name>
      <anchorfile>classutilib_1_1ArrayBase.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>bytes_used</name>
      <anchorfile>classutilib_1_1ArrayBase.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>A *</type>
      <name>data</name>
      <anchorfile>classutilib_1_1ArrayBase.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>nrefs</name>
      <anchorfile>classutilib_1_1ArrayBase.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>ArrayBase&lt; A, P &gt; &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1ArrayBase.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>(const ArrayBase&lt; A, P &gt; &amp;array)</arglist>
    </member>
    <member kind="function">
      <type>ArrayBase&lt; A, P &gt; &amp;</type>
      <name>operator &amp;=</name>
      <anchorfile>classutilib_1_1ArrayBase.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>(ArrayBase&lt; A, P &gt; &amp;array)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_data</name>
      <anchorfile>classutilib_1_1ArrayBase.html</anchorfile>
      <anchor>a11</anchor>
      <arglist>(const size_type len, A *data, const EnumDataOwned o=DataNotOwned)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>debug_print</name>
      <anchorfile>classutilib_1_1ArrayBase.html</anchorfile>
      <anchor>a12</anchor>
      <arglist>(ostream &amp;str, int max_elements=0)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>copy_data</name>
      <anchorfile>classutilib_1_1ArrayBase.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>(A *target, size_type target_size, A *source, size_type source_size)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>construct</name>
      <anchorfile>classutilib_1_1ArrayBase.html</anchorfile>
      <anchor>b1</anchor>
      <arglist>(const size_type mylen, A *d=0, const EnumDataOwned o=DataNotOwned)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>initialize</name>
      <anchorfile>classutilib_1_1ArrayBase.html</anchorfile>
      <anchor>b2</anchor>
      <arglist>(A *, const size_type, const size_type)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>free</name>
      <anchorfile>classutilib_1_1ArrayBase.html</anchorfile>
      <anchor>b3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>dump_data</name>
      <anchorfile>classutilib_1_1ArrayBase.html</anchorfile>
      <anchor>b4</anchor>
      <arglist>(ostream &amp;, unsigned int=0)</arglist>
    </member>
    <member kind="function" protection="protected" static="yes">
      <type>static P *</type>
      <name>special_not_owned_pointer</name>
      <anchorfile>classutilib_1_1ArrayBase.html</anchorfile>
      <anchor>f0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected" static="yes">
      <type>static int</type>
      <name>true_pointer</name>
      <anchorfile>classutilib_1_1ArrayBase.html</anchorfile>
      <anchor>f1</anchor>
      <arglist>(P *p)</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>A *</type>
      <name>Data</name>
      <anchorfile>classutilib_1_1ArrayBase.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>P *</type>
      <name>prev_share</name>
      <anchorfile>classutilib_1_1ArrayBase.html</anchorfile>
      <anchor>p1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>P *</type>
      <name>next_share</name>
      <anchorfile>classutilib_1_1ArrayBase.html</anchorfile>
      <anchor>p2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>size_type</type>
      <name>Len</name>
      <anchorfile>classutilib_1_1ArrayBase.html</anchorfile>
      <anchor>p3</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::Basic2DArrayRep</name>
    <filename>classutilib_1_1Basic2DArrayRep.html</filename>
    <templarg>T</templarg>
    <member kind="function" protection="protected">
      <type></type>
      <name>Basic2DArrayRep</name>
      <anchorfile>classutilib_1_1Basic2DArrayRep.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>T **</type>
      <name>Data</name>
      <anchorfile>classutilib_1_1Basic2DArrayRep.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>int</type>
      <name>ref</name>
      <anchorfile>classutilib_1_1Basic2DArrayRep.html</anchorfile>
      <anchor>p1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>size_type</type>
      <name>Nrows</name>
      <anchorfile>classutilib_1_1Basic2DArrayRep.html</anchorfile>
      <anchor>p2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>size_type</type>
      <name>Ncols</name>
      <anchorfile>classutilib_1_1Basic2DArrayRep.html</anchorfile>
      <anchor>p3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>EnumDataOwned</type>
      <name>own_data</name>
      <anchorfile>classutilib_1_1Basic2DArrayRep.html</anchorfile>
      <anchor>p4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>EnumDataOwned</type>
      <name>own_ptrs</name>
      <anchorfile>classutilib_1_1Basic2DArrayRep.html</anchorfile>
      <anchor>p5</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend" protection="private">
      <type>friend class</type>
      <name>Basic2DArray&lt; T &gt;</name>
      <anchorfile>classutilib_1_1Basic2DArrayRep.html</anchorfile>
      <anchor>n0</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend" protection="private">
      <type>friend class</type>
      <name>Simple2DArray&lt; T &gt;</name>
      <anchorfile>classutilib_1_1Basic2DArrayRep.html</anchorfile>
      <anchor>n1</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend" protection="private">
      <type>friend class</type>
      <name>Num2DArray&lt; T &gt;</name>
      <anchorfile>classutilib_1_1Basic2DArrayRep.html</anchorfile>
      <anchor>n2</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::Basic2DArray</name>
    <filename>classutilib_1_1Basic2DArray.html</filename>
    <templarg>T</templarg>
    <member kind="function">
      <type></type>
      <name>Basic2DArray</name>
      <anchorfile>classutilib_1_1Basic2DArray.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Basic2DArray</name>
      <anchorfile>classutilib_1_1Basic2DArray.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(const BasicArray&lt; T &gt; &amp;array, const size_type nrows=1, const EnumDataOwned own=DataNotOwned)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Basic2DArray</name>
      <anchorfile>classutilib_1_1Basic2DArray.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const size_type nrows, const size_type ncols, T *d=((T *) 0), const EnumDataOwned own=DataNotOwned)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Basic2DArray</name>
      <anchorfile>classutilib_1_1Basic2DArray.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(const size_type nrows, const size_type ncols, const BasicArray&lt; T &gt; &amp;array, const EnumDataOwned own=DataNotOwned)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Basic2DArray</name>
      <anchorfile>classutilib_1_1Basic2DArray.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(const Basic2DArray &amp;array)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~Basic2DArray</name>
      <anchorfile>classutilib_1_1Basic2DArray.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>resize</name>
      <anchorfile>classutilib_1_1Basic2DArray.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(const size_type nrows, const size_type ncols)</arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>nrows</name>
      <anchorfile>classutilib_1_1Basic2DArray.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>ncols</name>
      <anchorfile>classutilib_1_1Basic2DArray.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>operator T **</name>
      <anchorfile>classutilib_1_1Basic2DArray.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>T **</type>
      <name>data</name>
      <anchorfile>classutilib_1_1Basic2DArray.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>nrefs</name>
      <anchorfile>classutilib_1_1Basic2DArray.html</anchorfile>
      <anchor>a11</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>T *</type>
      <name>operator[]</name>
      <anchorfile>classutilib_1_1Basic2DArray.html</anchorfile>
      <anchor>a12</anchor>
      <arglist>(const int i)</arglist>
    </member>
    <member kind="function">
      <type>T *</type>
      <name>operator[]</name>
      <anchorfile>classutilib_1_1Basic2DArray.html</anchorfile>
      <anchor>a13</anchor>
      <arglist>(const int i) const </arglist>
    </member>
    <member kind="function">
      <type>T *</type>
      <name>operator[]</name>
      <anchorfile>classutilib_1_1Basic2DArray.html</anchorfile>
      <anchor>a14</anchor>
      <arglist>(const unsigned int i)</arglist>
    </member>
    <member kind="function">
      <type>T *</type>
      <name>operator[]</name>
      <anchorfile>classutilib_1_1Basic2DArray.html</anchorfile>
      <anchor>a15</anchor>
      <arglist>(const unsigned long int i) const </arglist>
    </member>
    <member kind="function">
      <type>T *</type>
      <name>operator[]</name>
      <anchorfile>classutilib_1_1Basic2DArray.html</anchorfile>
      <anchor>a16</anchor>
      <arglist>(const unsigned long int i)</arglist>
    </member>
    <member kind="function">
      <type>T *</type>
      <name>operator[]</name>
      <anchorfile>classutilib_1_1Basic2DArray.html</anchorfile>
      <anchor>a17</anchor>
      <arglist>(const unsigned int i) const </arglist>
    </member>
    <member kind="function">
      <type>T &amp;</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1Basic2DArray.html</anchorfile>
      <anchor>a18</anchor>
      <arglist>(const size_type row, const size_type col)</arglist>
    </member>
    <member kind="function">
      <type>const T &amp;</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1Basic2DArray.html</anchorfile>
      <anchor>a19</anchor>
      <arglist>(const size_type row, const size_type col) const </arglist>
    </member>
    <member kind="function">
      <type>Basic2DArray&lt; T &gt; &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1Basic2DArray.html</anchorfile>
      <anchor>a20</anchor>
      <arglist>(const Basic2DArray&lt; T &gt; &amp;array)</arglist>
    </member>
    <member kind="function">
      <type>Basic2DArray&lt; T &gt; &amp;</type>
      <name>operator &amp;=</name>
      <anchorfile>classutilib_1_1Basic2DArray.html</anchorfile>
      <anchor>a21</anchor>
      <arglist>(const Basic2DArray&lt; T &gt; &amp;array)</arglist>
    </member>
    <member kind="function">
      <type>Basic2DArray&lt; T &gt; &amp;</type>
      <name>set_data</name>
      <anchorfile>classutilib_1_1Basic2DArray.html</anchorfile>
      <anchor>a22</anchor>
      <arglist>(const size_type len, T *data, const EnumDataOwned o=DataNotOwned)</arglist>
    </member>
    <member kind="function">
      <type>Basic2DArray&lt; T &gt; &amp;</type>
      <name>set_data</name>
      <anchorfile>classutilib_1_1Basic2DArray.html</anchorfile>
      <anchor>a23</anchor>
      <arglist>(const BasicArray&lt; T &gt; &amp;array, const EnumDataOwned o=DataNotOwned)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>construct</name>
      <anchorfile>classutilib_1_1Basic2DArray.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>(const size_type nrows, const size_type ncols, T *d, const EnumDataOwned o=DataNotOwned)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>construct</name>
      <anchorfile>classutilib_1_1Basic2DArray.html</anchorfile>
      <anchor>b1</anchor>
      <arglist>(const size_type nrows, const size_type ncols, T **d, const EnumDataOwned o=DataNotOwned)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>free</name>
      <anchorfile>classutilib_1_1Basic2DArray.html</anchorfile>
      <anchor>b2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>Basic2DArrayRep&lt; T &gt; *</type>
      <name>a</name>
      <anchorfile>classutilib_1_1Basic2DArray.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::Basic3DArrayRep</name>
    <filename>classutilib_1_1Basic3DArrayRep.html</filename>
    <templarg>T</templarg>
    <member kind="variable" protection="protected">
      <type>T **</type>
      <name>Data</name>
      <anchorfile>classutilib_1_1Basic3DArrayRep.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>int</type>
      <name>ref</name>
      <anchorfile>classutilib_1_1Basic3DArrayRep.html</anchorfile>
      <anchor>p1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>size_type</type>
      <name>Nrows</name>
      <anchorfile>classutilib_1_1Basic3DArrayRep.html</anchorfile>
      <anchor>p2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>size_type</type>
      <name>Ncols</name>
      <anchorfile>classutilib_1_1Basic3DArrayRep.html</anchorfile>
      <anchor>p3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>EnumDataOwned</type>
      <name>own_data</name>
      <anchorfile>classutilib_1_1Basic3DArrayRep.html</anchorfile>
      <anchor>p4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>EnumDataOwned</type>
      <name>own_ptrs</name>
      <anchorfile>classutilib_1_1Basic3DArrayRep.html</anchorfile>
      <anchor>p5</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend" protection="private">
      <type>friend class</type>
      <name>Basic3DArray&lt; T &gt;</name>
      <anchorfile>classutilib_1_1Basic3DArrayRep.html</anchorfile>
      <anchor>n0</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend" protection="private">
      <type>friend class</type>
      <name>Simple3DArray&lt; T &gt;</name>
      <anchorfile>classutilib_1_1Basic3DArrayRep.html</anchorfile>
      <anchor>n1</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend" protection="private">
      <type>friend class</type>
      <name>Num3DArray&lt; T &gt;</name>
      <anchorfile>classutilib_1_1Basic3DArrayRep.html</anchorfile>
      <anchor>n2</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::Basic3DArray</name>
    <filename>classutilib_1_1Basic3DArray.html</filename>
    <templarg>T</templarg>
    <member kind="function">
      <type></type>
      <name>Basic3DArray</name>
      <anchorfile>classutilib_1_1Basic3DArray.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(const BasicArray&lt; T &gt; &amp;array, const size_type nrows=1, const EnumDataOwned own=DataNotOwned)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Basic3DArray</name>
      <anchorfile>classutilib_1_1Basic3DArray.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const size_type nrows, const size_type ncols, const size_type ndeep, T *d=((T *) 0), const EnumDataOwned own=DataNotOwned)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Basic3DArray</name>
      <anchorfile>classutilib_1_1Basic3DArray.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(const size_type nrows, const size_type ncols, const BasicArray&lt; T &gt; &amp;array, const EnumDataOwned own=DataNotOwned)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Basic3DArray</name>
      <anchorfile>classutilib_1_1Basic3DArray.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(const Basic3DArray &amp;array)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>resize</name>
      <anchorfile>classutilib_1_1Basic3DArray.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(const size_type nrows, const size_type ncols, const size_type ndeep)</arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>nrows</name>
      <anchorfile>classutilib_1_1Basic3DArray.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>ncols</name>
      <anchorfile>classutilib_1_1Basic3DArray.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>operator T **</name>
      <anchorfile>classutilib_1_1Basic3DArray.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>T **</type>
      <name>data</name>
      <anchorfile>classutilib_1_1Basic3DArray.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>nrefs</name>
      <anchorfile>classutilib_1_1Basic3DArray.html</anchorfile>
      <anchor>a11</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>T *</type>
      <name>operator[]</name>
      <anchorfile>classutilib_1_1Basic3DArray.html</anchorfile>
      <anchor>a12</anchor>
      <arglist>(const size_type)</arglist>
    </member>
    <member kind="function">
      <type>const T *</type>
      <name>operator[]</name>
      <anchorfile>classutilib_1_1Basic3DArray.html</anchorfile>
      <anchor>a13</anchor>
      <arglist>(const size_type) const </arglist>
    </member>
    <member kind="function">
      <type>T &amp;</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1Basic3DArray.html</anchorfile>
      <anchor>a14</anchor>
      <arglist>(const size_type row, const size_type col)</arglist>
    </member>
    <member kind="function">
      <type>const T &amp;</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1Basic3DArray.html</anchorfile>
      <anchor>a15</anchor>
      <arglist>(const size_type row, const size_type col) const </arglist>
    </member>
    <member kind="function">
      <type>Basic3DArray&lt; T &gt; &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1Basic3DArray.html</anchorfile>
      <anchor>a16</anchor>
      <arglist>(const Basic3DArray&lt; T &gt; &amp;array)</arglist>
    </member>
    <member kind="function">
      <type>Basic3DArray&lt; T &gt; &amp;</type>
      <name>operator &amp;=</name>
      <anchorfile>classutilib_1_1Basic3DArray.html</anchorfile>
      <anchor>a17</anchor>
      <arglist>(const Basic3DArray&lt; T &gt; &amp;array)</arglist>
    </member>
    <member kind="function">
      <type>Basic3DArray&lt; T &gt; &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>classutilib_1_1Basic3DArray.html</anchorfile>
      <anchor>a18</anchor>
      <arglist>(const Basic3DArray&lt; T &gt; &amp;array)</arglist>
    </member>
    <member kind="function">
      <type>Basic3DArray&lt; T &gt; &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1Basic3DArray.html</anchorfile>
      <anchor>a19</anchor>
      <arglist>(const T &amp;val)</arglist>
    </member>
    <member kind="function">
      <type>Basic3DArray&lt; T &gt; &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1Basic3DArray.html</anchorfile>
      <anchor>a20</anchor>
      <arglist>(const BasicArray&lt; T &gt; &amp;val)</arglist>
    </member>
    <member kind="function">
      <type>Basic3DArray&lt; T &gt; &amp;</type>
      <name>set_data</name>
      <anchorfile>classutilib_1_1Basic3DArray.html</anchorfile>
      <anchor>a21</anchor>
      <arglist>(const size_type len, T *data, const EnumDataOwned o=DataNotOwned)</arglist>
    </member>
    <member kind="function">
      <type>Basic3DArray&lt; T &gt; &amp;</type>
      <name>set_data</name>
      <anchorfile>classutilib_1_1Basic3DArray.html</anchorfile>
      <anchor>a22</anchor>
      <arglist>(const BasicArray&lt; T &gt; &amp;array, const EnumDataOwned o=DataNotOwned)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>construct</name>
      <anchorfile>classutilib_1_1Basic3DArray.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>(const size_type nrows, const size_type ncols, const size_type ndeep, T *d, const EnumDataOwned o=DataNotOwned)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>construct</name>
      <anchorfile>classutilib_1_1Basic3DArray.html</anchorfile>
      <anchor>b1</anchor>
      <arglist>(const size_type nrows, const size_type ncols, const size_type ndeep, T **d, const EnumDataOwned o=DataNotOwned)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>free</name>
      <anchorfile>classutilib_1_1Basic3DArray.html</anchorfile>
      <anchor>b2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>Basic3DArrayRep&lt; T &gt; *</type>
      <name>a</name>
      <anchorfile>classutilib_1_1Basic3DArray.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::BasicArray</name>
    <filename>classutilib_1_1BasicArray.html</filename>
    <templarg>T</templarg>
    <base>utilib::ArrayBase&lt; T, BasicArray&lt; T &gt; &gt;</base>
    <member kind="typedef">
      <type>ArrayBase&lt; T, BasicArray&lt; T &gt; &gt;</type>
      <name>base_t</name>
      <anchorfile>classutilib_1_1BasicArray.html</anchorfile>
      <anchor>w0</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>BasicArray&lt; T &gt;</type>
      <name>vector_type</name>
      <anchorfile>classutilib_1_1BasicArray.html</anchorfile>
      <anchor>w1</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>T</type>
      <name>value_type</name>
      <anchorfile>classutilib_1_1BasicArray.html</anchorfile>
      <anchor>w2</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>value_type *</type>
      <name>pointer</name>
      <anchorfile>classutilib_1_1BasicArray.html</anchorfile>
      <anchor>w3</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>const value_type *</type>
      <name>const_pointer</name>
      <anchorfile>classutilib_1_1BasicArray.html</anchorfile>
      <anchor>w4</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>BasicArray</name>
      <anchorfile>classutilib_1_1BasicArray.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>BasicArray</name>
      <anchorfile>classutilib_1_1BasicArray.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(const size_type mylen, T *d=((T *) 0), const EnumDataOwned o=DataNotOwned)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>BasicArray</name>
      <anchorfile>classutilib_1_1BasicArray.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const BasicArray&lt; T &gt; &amp;array)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~BasicArray</name>
      <anchorfile>classutilib_1_1BasicArray.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>clear</name>
      <anchorfile>classutilib_1_1BasicArray.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>T &amp;</type>
      <name>operator[]</name>
      <anchorfile>classutilib_1_1BasicArray.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(const size_type i)</arglist>
    </member>
    <member kind="function">
      <type>const T &amp;</type>
      <name>operator[]</name>
      <anchorfile>classutilib_1_1BasicArray.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(const size_type i) const </arglist>
    </member>
    <member kind="function">
      <type>BasicArray&lt; T &gt; &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1BasicArray.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>(const BasicArray&lt; T &gt; &amp;val)</arglist>
    </member>
    <member kind="function">
      <type>BasicArray&lt; T &gt; &amp;</type>
      <name>set_data</name>
      <anchorfile>classutilib_1_1BasicArray.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>(const size_type len, T *data, const EnumDataOwned o=DataNotOwned)</arglist>
    </member>
    <member kind="function">
      <type>BasicArray&lt; T &gt; &amp;</type>
      <name>set_data</name>
      <anchorfile>classutilib_1_1BasicArray.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>(const BasicArray&lt; T &gt; &amp;vec, const size_type start, const size_type newlen)</arglist>
    </member>
    <member kind="function">
      <type>BasicArray&lt; T &gt; &amp;</type>
      <name>set_data</name>
      <anchorfile>classutilib_1_1BasicArray.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>(const BasicArray&lt; T &gt; &amp;vec, const size_type start=0)</arglist>
    </member>
    <member kind="function">
      <type>BasicArray&lt; T &gt; &amp;</type>
      <name>set_subvec</name>
      <anchorfile>classutilib_1_1BasicArray.html</anchorfile>
      <anchor>a11</anchor>
      <arglist>(const size_type start, const size_type len, const BasicArray&lt; T &gt; &amp;datavec, const size_type offset=0)</arglist>
    </member>
    <member kind="function">
      <type>BasicArray&lt; T &gt; &amp;</type>
      <name>at</name>
      <anchorfile>classutilib_1_1BasicArray.html</anchorfile>
      <anchor>a12</anchor>
      <arglist>(const size_type start, const size_type newlen, BasicArray&lt; T &gt; *array=(BasicArray&lt; T &gt; *) 0) const </arglist>
    </member>
    <member kind="function">
      <type>BasicArray&lt; T &gt; &amp;</type>
      <name>at</name>
      <anchorfile>classutilib_1_1BasicArray.html</anchorfile>
      <anchor>a13</anchor>
      <arglist>(const size_type start=0) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>append</name>
      <anchorfile>classutilib_1_1BasicArray.html</anchorfile>
      <anchor>a14</anchor>
      <arglist>(const T &amp;element)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>push_back</name>
      <anchorfile>classutilib_1_1BasicArray.html</anchorfile>
      <anchor>a15</anchor>
      <arglist>(const T &amp;element)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>append</name>
      <anchorfile>classutilib_1_1BasicArray.html</anchorfile>
      <anchor>a16</anchor>
      <arglist>(BasicArray&lt; T &gt; &amp;vec)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>classutilib_1_1BasicArray.html</anchorfile>
      <anchor>a17</anchor>
      <arglist>(const T &amp;val) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator!=</name>
      <anchorfile>classutilib_1_1BasicArray.html</anchorfile>
      <anchor>a18</anchor>
      <arglist>(const T &amp;val) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>classutilib_1_1BasicArray.html</anchorfile>
      <anchor>a19</anchor>
      <arglist>(const BasicArray&lt; T &gt; &amp;array) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator!=</name>
      <anchorfile>classutilib_1_1BasicArray.html</anchorfile>
      <anchor>a20</anchor>
      <arglist>(const BasicArray&lt; T &gt; &amp;array) const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>compare</name>
      <anchorfile>classutilib_1_1BasicArray.html</anchorfile>
      <anchor>a21</anchor>
      <arglist>(const BasicArray&lt; T &gt; &amp;array) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;</name>
      <anchorfile>classutilib_1_1BasicArray.html</anchorfile>
      <anchor>a22</anchor>
      <arglist>(const BasicArray&lt; T &gt; &amp;array) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&gt;</name>
      <anchorfile>classutilib_1_1BasicArray.html</anchorfile>
      <anchor>a23</anchor>
      <arglist>(const BasicArray&lt; T &gt; &amp;array) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;=</name>
      <anchorfile>classutilib_1_1BasicArray.html</anchorfile>
      <anchor>a24</anchor>
      <arglist>(const BasicArray&lt; T &gt; &amp;array) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&gt;=</name>
      <anchorfile>classutilib_1_1BasicArray.html</anchorfile>
      <anchor>a25</anchor>
      <arglist>(const BasicArray&lt; T &gt; &amp;array) const </arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::BitArray</name>
    <filename>classutilib_1_1BitArray.html</filename>
    <base>utilib::BitArrayBase&lt; 0, int, BitArray &gt;</base>
    <member kind="typedef">
      <type>BitArrayBase&lt; 0, int, BitArray &gt;</type>
      <name>base_t</name>
      <anchorfile>classutilib_1_1BitArray.html</anchorfile>
      <anchor>w0</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>BitArray</name>
      <anchorfile>classutilib_1_1BitArray.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>BitArray</name>
      <anchorfile>classutilib_1_1BitArray.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(const size_type len, bitword *d=0, const EnumDataOwned o=DataNotOwned)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>BitArray</name>
      <anchorfile>classutilib_1_1BitArray.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const BitArray &amp;array)</arglist>
    </member>
    <member kind="function">
      <type>BitArray &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1BitArray.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(const BitArray &amp;array)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set</name>
      <anchorfile>classutilib_1_1BitArray.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(const size_type ndx)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set</name>
      <anchorfile>classutilib_1_1BitArray.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>reset</name>
      <anchorfile>classutilib_1_1BitArray.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(const size_type ndx)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>reset</name>
      <anchorfile>classutilib_1_1BitArray.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>flip</name>
      <anchorfile>classutilib_1_1BitArray.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>(const size_type ndx)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>flip</name>
      <anchorfile>classutilib_1_1BitArray.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>nbits</name>
      <anchorfile>classutilib_1_1BitArray.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>shared_one</name>
      <anchorfile>classutilib_1_1BitArray.html</anchorfile>
      <anchor>a11</anchor>
      <arglist>(BitArray &amp;other) const </arglist>
    </member>
    <member kind="function" protection="protected" static="yes">
      <type>static void</type>
      <name>count_bits</name>
      <anchorfile>classutilib_1_1BitArray.html</anchorfile>
      <anchor>f0</anchor>
      <arglist>(size_type &amp;counter, workingBitData stuff)</arglist>
    </member>
    <member kind="friend">
      <type>friend void</type>
      <name>bitwise_or</name>
      <anchorfile>classutilib_1_1BitArray.html</anchorfile>
      <anchor>n0</anchor>
      <arglist>(BitArray &amp;a1, BitArray &amp;a2, BitArray &amp;result)</arglist>
    </member>
    <member kind="friend">
      <type>friend void</type>
      <name>bitwise_xor</name>
      <anchorfile>classutilib_1_1BitArray.html</anchorfile>
      <anchor>n1</anchor>
      <arglist>(BitArray &amp;a1, BitArray &amp;a2, BitArray &amp;result)</arglist>
    </member>
    <member kind="friend">
      <type>friend void</type>
      <name>bitwise_and</name>
      <anchorfile>classutilib_1_1BitArray.html</anchorfile>
      <anchor>n2</anchor>
      <arglist>(BitArray &amp;a1, BitArray &amp;a2, BitArray &amp;result)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::BitArrayBase</name>
    <filename>classutilib_1_1BitArrayBase.html</filename>
    <templarg>k</templarg>
    <templarg>T</templarg>
    <templarg>P</templarg>
    <base>utilib::ArrayBase&lt; bitword, P &gt;</base>
    <member kind="typedef">
      <type>ArrayBase&lt; bitword, P &gt;</type>
      <name>base_t</name>
      <anchorfile>classutilib_1_1BitArrayBase.html</anchorfile>
      <anchor>w0</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>BitArrayBase</name>
      <anchorfile>classutilib_1_1BitArrayBase.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>BitArrayBase</name>
      <anchorfile>classutilib_1_1BitArrayBase.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(const size_type len, bitword *d=0, const EnumDataOwned o=DataNotOwned)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>BitArrayBase</name>
      <anchorfile>classutilib_1_1BitArrayBase.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(BitArrayBase&lt; k, T, P &gt; &amp;array)</arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>alloc_size</name>
      <anchorfile>classutilib_1_1BitArrayBase.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(size_type l) const </arglist>
    </member>
    <member kind="function">
      <type>T</type>
      <name>operator[]</name>
      <anchorfile>classutilib_1_1BitArrayBase.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(const size_type ndx) const </arglist>
    </member>
    <member kind="function">
      <type>BitArrayBaseHandle&lt; k, T, P &gt; &amp;</type>
      <name>operator[]</name>
      <anchorfile>classutilib_1_1BitArrayBase.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(const size_type ndx)</arglist>
    </member>
    <member kind="function">
      <type>T</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1BitArrayBase.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(const size_type ndx) const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>element_size</name>
      <anchorfile>classutilib_1_1BitArrayBase.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>put</name>
      <anchorfile>classutilib_1_1BitArrayBase.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>(const size_type ndx, const T val)</arglist>
    </member>
    <member kind="function">
      <type>BitArrayBase&lt; k, T, P &gt; &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1BitArrayBase.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>(const BitArrayBase&lt; k, T, P &gt; &amp;array)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>operator==</name>
      <anchorfile>classutilib_1_1BitArrayBase.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>(const BitArrayBase&lt; k, T, P &gt; &amp;array) const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>operator!=</name>
      <anchorfile>classutilib_1_1BitArrayBase.html</anchorfile>
      <anchor>a11</anchor>
      <arglist>(const BitArrayBase&lt; k, T, P &gt; &amp;array) const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>compare</name>
      <anchorfile>classutilib_1_1BitArrayBase.html</anchorfile>
      <anchor>a12</anchor>
      <arglist>(const BitArrayBase&lt; k, T, P &gt; &amp;array) const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>operator&gt;</name>
      <anchorfile>classutilib_1_1BitArrayBase.html</anchorfile>
      <anchor>a13</anchor>
      <arglist>(const BitArrayBase&lt; k, T, P &gt; &amp;vec) const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>operator&gt;=</name>
      <anchorfile>classutilib_1_1BitArrayBase.html</anchorfile>
      <anchor>a14</anchor>
      <arglist>(const BitArrayBase&lt; k, T, P &gt; &amp;vec) const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>operator&lt;</name>
      <anchorfile>classutilib_1_1BitArrayBase.html</anchorfile>
      <anchor>a15</anchor>
      <arglist>(const BitArrayBase&lt; k, T, P &gt; &amp;vec) const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>operator&lt;=</name>
      <anchorfile>classutilib_1_1BitArrayBase.html</anchorfile>
      <anchor>a16</anchor>
      <arglist>(const BitArrayBase&lt; k, T, P &gt; &amp;vec) const </arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual int</type>
      <name>write</name>
      <anchorfile>classutilib_1_1BitArrayBase.html</anchorfile>
      <anchor>a17</anchor>
      <arglist>(ostream &amp;output) const </arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual int</type>
      <name>read</name>
      <anchorfile>classutilib_1_1BitArrayBase.html</anchorfile>
      <anchor>a18</anchor>
      <arglist>(istream &amp;input)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>write</name>
      <anchorfile>classutilib_1_1BitArrayBase.html</anchorfile>
      <anchor>a19</anchor>
      <arglist>(PackBuffer &amp;output) const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>read</name>
      <anchorfile>classutilib_1_1BitArrayBase.html</anchorfile>
      <anchor>a20</anchor>
      <arglist>(UnPackBuffer &amp;input)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual char</type>
      <name>translate_to_char</name>
      <anchorfile>classutilib_1_1BitArrayBase.html</anchorfile>
      <anchor>a21</anchor>
      <arglist>(const int digit) const </arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual int</type>
      <name>translate_from_char</name>
      <anchorfile>classutilib_1_1BitArrayBase.html</anchorfile>
      <anchor>a22</anchor>
      <arglist>(const char c) const </arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>construct</name>
      <anchorfile>classutilib_1_1BitArrayBase.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>(const size_type mylen, bitword *d=0, const EnumDataOwned o=DataNotOwned)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>initialize</name>
      <anchorfile>classutilib_1_1BitArrayBase.html</anchorfile>
      <anchor>b1</anchor>
      <arglist>(bitword *data, const size_type old_len, const size_type new_len)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>dump_data</name>
      <anchorfile>classutilib_1_1BitArrayBase.html</anchorfile>
      <anchor>b2</anchor>
      <arglist>(ostream &amp;str, unsigned int max_elements=0)</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>BitArrayBaseHandle&lt; k, T, P &gt;</type>
      <name>handle</name>
      <anchorfile>classutilib_1_1BitArrayBase.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>utilib::is_cache_allocated</name>
    <filename>structutilib_1_1is__cache__allocated.html</filename>
    <templarg>DomainT</templarg>
    <member kind="variable" static="yes">
      <type>static const bool</type>
      <name>value</name>
      <anchorfile>structutilib_1_1is__cache__allocated.html</anchorfile>
      <anchor>s0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::CachedAllocatorObject</name>
    <filename>classutilib_1_1CachedAllocatorObject.html</filename>
    <templarg>TYPE</templarg>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>allocate_exec</name>
      <anchorfile>classutilib_1_1CachedAllocatorObject.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>deallocate_exec</name>
      <anchorfile>classutilib_1_1CachedAllocatorObject.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>deallocate_derived</name>
      <anchorfile>classutilib_1_1CachedAllocatorObject.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>()=0</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::CachedAllocator</name>
    <filename>classutilib_1_1CachedAllocator.html</filename>
    <templarg>TYPE</templarg>
    <member kind="function">
      <type></type>
      <name>CachedAllocator</name>
      <anchorfile>classutilib_1_1CachedAllocator.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>delete_unused</name>
      <anchorfile>classutilib_1_1CachedAllocator.html</anchorfile>
      <anchor>e0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static TYPE *</type>
      <name>allocate</name>
      <anchorfile>classutilib_1_1CachedAllocator.html</anchorfile>
      <anchor>e1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>deallocate</name>
      <anchorfile>classutilib_1_1CachedAllocator.html</anchorfile>
      <anchor>e2</anchor>
      <arglist>(TYPE *)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>deallocate_derived</name>
      <anchorfile>classutilib_1_1CachedAllocator.html</anchorfile>
      <anchor>e3</anchor>
      <arglist>(TYPE *)</arglist>
    </member>
    <member kind="variable" protection="protected" static="yes">
      <type>static CacheList&lt; TYPE &gt; *</type>
      <name>unused_list</name>
      <anchorfile>classutilib_1_1CachedAllocator.html</anchorfile>
      <anchor>t0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected" static="yes">
      <type>static int</type>
      <name>unused_len</name>
      <anchorfile>classutilib_1_1CachedAllocator.html</anchorfile>
      <anchor>t1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected" static="yes">
      <type>static CacheList&lt; TYPE &gt; *</type>
      <name>tmp_list</name>
      <anchorfile>classutilib_1_1CachedAllocator.html</anchorfile>
      <anchor>t2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected" static="yes">
      <type>static int</type>
      <name>tmp_len</name>
      <anchorfile>classutilib_1_1CachedAllocator.html</anchorfile>
      <anchor>t3</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::CacheList</name>
    <filename>classutilib_1_1CacheList.html</filename>
    <templarg>ListType</templarg>
    <member kind="variable">
      <type>CacheList&lt; ListType &gt; *</type>
      <name>next</name>
      <anchorfile>classutilib_1_1CacheList.html</anchorfile>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ListType *</type>
      <name>data</name>
      <anchorfile>classutilib_1_1CacheList.html</anchorfile>
      <anchor>o1</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" protection="private">
      <type></type>
      <name>CacheList</name>
      <anchorfile>classutilib_1_1CacheList.html</anchorfile>
      <anchor>d0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private">
      <type></type>
      <name>~CacheList</name>
      <anchorfile>classutilib_1_1CacheList.html</anchorfile>
      <anchor>d1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="friend" protection="private">
      <type>friend class</type>
      <name>CachedAllocator&lt; ListType &gt;</name>
      <anchorfile>classutilib_1_1CacheList.html</anchorfile>
      <anchor>n0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::CharString</name>
    <filename>classutilib_1_1CharString.html</filename>
    <base>utilib::BasicArray&lt; char &gt;</base>
    <member kind="function">
      <type></type>
      <name>CharString</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>CharString</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(const size_type len, char *str=(char *) 0, const EnumDataOwned own=DataNotOwned)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>CharString</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(char *str, const EnumDataOwned own=DataNotOwned)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>CharString</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(const char *str, const EnumDataOwned own=DataNotOwned)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>CharString</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(const CharString &amp;array)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>c_str</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>c_str</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>alloc_size</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>(size_type l) const </arglist>
    </member>
    <member kind="function">
      <type>CharString &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>(const CharString &amp;str)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>compare</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>(const char *s) const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>compare</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>(const CharString &amp;otherString) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a11</anchor>
      <arglist>(const char *array) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a12</anchor>
      <arglist>(const CharString &amp;otherString) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator!=</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a13</anchor>
      <arglist>(const char *array) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator!=</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a14</anchor>
      <arglist>(const CharString &amp;otherString) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a15</anchor>
      <arglist>(char *array) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator!=</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a16</anchor>
      <arglist>(char *array) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a17</anchor>
      <arglist>(const char *array) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a18</anchor>
      <arglist>(const CharString &amp;otherString) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&gt;</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a19</anchor>
      <arglist>(const char *array) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&gt;</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a20</anchor>
      <arglist>(const CharString &amp;otherString) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;=</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a21</anchor>
      <arglist>(const char *array) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;=</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a22</anchor>
      <arglist>(const CharString &amp;otherString) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&gt;=</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a23</anchor>
      <arglist>(const char *array) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&gt;=</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a24</anchor>
      <arglist>(const CharString &amp;otherString) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>equivalent</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a25</anchor>
      <arglist>(CharString &amp;array) const </arglist>
    </member>
    <member kind="function">
      <type>CharString &amp;</type>
      <name>operator+=</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a26</anchor>
      <arglist>(const char *array)</arglist>
    </member>
    <member kind="function">
      <type>CharString &amp;</type>
      <name>operator+=</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a27</anchor>
      <arglist>(char *array)</arglist>
    </member>
    <member kind="function">
      <type>CharString &amp;</type>
      <name>operator+=</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a28</anchor>
      <arglist>(const CharString &amp;array)</arglist>
    </member>
    <member kind="function">
      <type>CharString &amp;</type>
      <name>operator+=</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a29</anchor>
      <arglist>(CharString &amp;array)</arglist>
    </member>
    <member kind="function">
      <type>CharString &amp;</type>
      <name>operator+=</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a30</anchor>
      <arglist>(char val)</arglist>
    </member>
    <member kind="function">
      <type>CharString &amp;</type>
      <name>operator+=</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a31</anchor>
      <arglist>(const int val)</arglist>
    </member>
    <member kind="function">
      <type>CharString &amp;</type>
      <name>operator+=</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a32</anchor>
      <arglist>(const unsigned int val)</arglist>
    </member>
    <member kind="function">
      <type>CharString &amp;</type>
      <name>operator+=</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a33</anchor>
      <arglist>(const double val)</arglist>
    </member>
    <member kind="function">
      <type>CharString &amp;</type>
      <name>set_subvec</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a34</anchor>
      <arglist>(const size_type start, const size_type len, const CharString &amp;datavec, const size_type offset=0)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a35</anchor>
      <arglist>(ostream &amp;output) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a36</anchor>
      <arglist>(PackBuffer &amp;output) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a37</anchor>
      <arglist>(istream &amp;input)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>a38</anchor>
      <arglist>(UnPackBuffer &amp;input)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>copy_data</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>(char *target, size_type target_size, char *source, size_type source_size)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>initialize</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>b1</anchor>
      <arglist>(char *data, const size_type start, const size_type stop)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>dump_data</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>b2</anchor>
      <arglist>(ostream &amp;str, unsigned int max_elements=0)</arglist>
    </member>
    <member kind="friend">
      <type>friend int</type>
      <name>compare</name>
      <anchorfile>classutilib_1_1CharString.html</anchorfile>
      <anchor>n0</anchor>
      <arglist>(const CharString &amp;str1, const CharString &amp;str2)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::ClassRef</name>
    <filename>classutilib_1_1ClassRef.html</filename>
    <templarg>V</templarg>
    <member kind="function">
      <type></type>
      <name>ClassRef</name>
      <anchorfile>classutilib_1_1ClassRef.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~ClassRef</name>
      <anchorfile>classutilib_1_1ClassRef.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>update_refs</name>
      <anchorfile>classutilib_1_1ClassRef.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>acquire</name>
      <anchorfile>classutilib_1_1ClassRef.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(V *array)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>release</name>
      <anchorfile>classutilib_1_1ClassRef.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(V *array)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>resize_ref_ptrs</name>
      <anchorfile>classutilib_1_1ClassRef.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(const int)</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>copy_data</name>
      <anchorfile>classutilib_1_1ClassRef.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(V *array)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>reset_data</name>
      <anchorfile>classutilib_1_1ClassRef.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>(V *array)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>delete_data</name>
      <anchorfile>classutilib_1_1ClassRef.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>freeze</name>
      <anchorfile>classutilib_1_1ClassRef.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>EnumDataOwned</type>
      <name>own</name>
      <anchorfile>classutilib_1_1ClassRef.html</anchorfile>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>ref</name>
      <anchorfile>classutilib_1_1ClassRef.html</anchorfile>
      <anchor>o1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>const_ref</name>
      <anchorfile>classutilib_1_1ClassRef.html</anchorfile>
      <anchor>o2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static int</type>
      <name>num_refs_limit</name>
      <anchorfile>classutilib_1_1ClassRef.html</anchorfile>
      <anchor>s0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>int</type>
      <name>ptrlen</name>
      <anchorfile>classutilib_1_1ClassRef.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>V **</type>
      <name>ref_ptrs</name>
      <anchorfile>classutilib_1_1ClassRef.html</anchorfile>
      <anchor>p1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::ArrayRef</name>
    <filename>classutilib_1_1ArrayRef.html</filename>
    <templarg>T</templarg>
    <templarg>V</templarg>
    <base>utilib::ClassRef</base>
    <member kind="function">
      <type></type>
      <name>ArrayRef</name>
      <anchorfile>classutilib_1_1ArrayRef.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~ArrayRef</name>
      <anchorfile>classutilib_1_1ArrayRef.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>copy_data</name>
      <anchorfile>classutilib_1_1ArrayRef.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(V *array)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>reset_data</name>
      <anchorfile>classutilib_1_1ArrayRef.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(V *array)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>delete_data</name>
      <anchorfile>classutilib_1_1ArrayRef.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_data</name>
      <anchorfile>classutilib_1_1ArrayRef.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(T *Data_, size_type Len_, EnumDataOwned own_)</arglist>
    </member>
    <member kind="variable">
      <type>T *</type>
      <name>Data</name>
      <anchorfile>classutilib_1_1ArrayRef.html</anchorfile>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_type</type>
      <name>Len</name>
      <anchorfile>classutilib_1_1ArrayRef.html</anchorfile>
      <anchor>o1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::EnumBitArray</name>
    <filename>classutilib_1_1EnumBitArray.html</filename>
    <templarg>k</templarg>
    <templarg>T</templarg>
    <base>utilib::BitArrayBase&lt; k, T, EnumBitArray&lt; k, T &gt; &gt;</base>
    <member kind="function">
      <type></type>
      <name>EnumBitArray</name>
      <anchorfile>classutilib_1_1EnumBitArray.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>EnumBitArray</name>
      <anchorfile>classutilib_1_1EnumBitArray.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(const size_type len, bitword *d=0, const EnumDataOwned o=DataNotOwned)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>EnumBitArray</name>
      <anchorfile>classutilib_1_1EnumBitArray.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const EnumBitArray&lt; k, T &gt; &amp;array)</arglist>
    </member>
    <member kind="function">
      <type>EnumBitArray&lt; k, T &gt; &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1EnumBitArray.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(const EnumBitArray&lt; k, T &gt; &amp;array)</arglist>
    </member>
    <member kind="function">
      <type>char</type>
      <name>translate_to_char</name>
      <anchorfile>classutilib_1_1EnumBitArray.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(const int digit) const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>translate_from_char</name>
      <anchorfile>classutilib_1_1EnumBitArray.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(const char c) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setEnumInformation</name>
      <anchorfile>classutilib_1_1EnumBitArray.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(int new_count, char *new_labels, T *new_values)</arglist>
    </member>
    <member kind="variable" protection="protected" static="yes">
      <type>static int</type>
      <name>enum_count</name>
      <anchorfile>classutilib_1_1EnumBitArray.html</anchorfile>
      <anchor>t0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected" static="yes">
      <type>static char *</type>
      <name>enum_labels</name>
      <anchorfile>classutilib_1_1EnumBitArray.html</anchorfile>
      <anchor>t1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected" static="yes">
      <type>static T *</type>
      <name>enum_vals</name>
      <anchorfile>classutilib_1_1EnumBitArray.html</anchorfile>
      <anchor>t2</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::Ereal</name>
    <filename>classutilib_1_1Ereal.html</filename>
    <templarg>Type</templarg>
    <member kind="function">
      <type></type>
      <name>Ereal</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(const Type num=0.0)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Ereal</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(const Ereal &amp;num)</arglist>
    </member>
    <member kind="function">
      <type>const Ereal &amp;</type>
      <name>operator+=</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const Ereal&lt; Type &gt; &amp;num)</arglist>
    </member>
    <member kind="function">
      <type>const Ereal &amp;</type>
      <name>operator-=</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(const Ereal&lt; Type &gt; &amp;num)</arglist>
    </member>
    <member kind="function">
      <type>const Ereal &amp;</type>
      <name>operator *=</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(const Ereal&lt; Type &gt; &amp;num)</arglist>
    </member>
    <member kind="function">
      <type>const Ereal &amp;</type>
      <name>operator/=</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(const Ereal&lt; Type &gt; &amp;num)</arglist>
    </member>
    <member kind="function">
      <type>const Ereal &amp;</type>
      <name>operator+=</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(const NType &amp;num)</arglist>
    </member>
    <member kind="function">
      <type>const Ereal &amp;</type>
      <name>operator-=</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>(const NType &amp;num)</arglist>
    </member>
    <member kind="function">
      <type>const Ereal &amp;</type>
      <name>operator *=</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>(const NType &amp;num)</arglist>
    </member>
    <member kind="function">
      <type>const Ereal &amp;</type>
      <name>operator/=</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>(const NType &amp;num)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>operator Type</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>ostream &amp;</type>
      <name>write</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>a11</anchor>
      <arglist>(ostream &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>istream &amp;</type>
      <name>read</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>a12</anchor>
      <arglist>(istream &amp;is)</arglist>
    </member>
    <member kind="function">
      <type>PackBuffer &amp;</type>
      <name>write</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>a13</anchor>
      <arglist>(PackBuffer &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>UnPackBuffer &amp;</type>
      <name>read</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>a14</anchor>
      <arglist>(UnPackBuffer &amp;is)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static Ereal&lt; Type &gt;</type>
      <name>positive_infinity</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>s0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static Ereal&lt; Type &gt;</type>
      <name>negative_infinity</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>s1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static Ereal&lt; Type &gt;</type>
      <name>NaN</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>s2</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" protection="protected">
      <type></type>
      <name>Ereal</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>(const Type num, const bool f_flag)</arglist>
    </member>
    <member kind="function" protection="protected" static="yes">
      <type>static void</type>
      <name>plus</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>f0</anchor>
      <arglist>(const Type xval, const bool xFinite, const Type yval, const bool yFinite, Type &amp;rval, bool &amp;rFinite)</arglist>
    </member>
    <member kind="function" protection="protected" static="yes">
      <type>static void</type>
      <name>minus</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>f1</anchor>
      <arglist>(const Type xval, const bool xFinite, const Type yval, const bool yFinite, Type &amp;rval, bool &amp;rFinite)</arglist>
    </member>
    <member kind="function" protection="protected" static="yes">
      <type>static void</type>
      <name>mult</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>f2</anchor>
      <arglist>(const Type xval, const bool xFinite, const Type yval, const bool yFinite, Type &amp;rval, bool &amp;rFinite)</arglist>
    </member>
    <member kind="function" protection="protected" static="yes">
      <type>static void</type>
      <name>div</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>f3</anchor>
      <arglist>(const Type xval, const bool xFinite, const Type yval, const bool yFinite, Type &amp;rval, bool &amp;rFinite)</arglist>
    </member>
    <member kind="function" protection="protected" static="yes">
      <type>static void</type>
      <name>check_if_infinite</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>f4</anchor>
      <arglist>(Type &amp;val, bool &amp;Finite)</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>Type</type>
      <name>val</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>Finite</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>p1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected" static="yes">
      <type>static Type</type>
      <name>positive_infinity_val</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>t0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected" static="yes">
      <type>static Type</type>
      <name>negative_infinity_val</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>t1</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend">
      <type>friend Ereal&lt; LType &gt;</type>
      <name>operator+</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>n0</anchor>
      <arglist>(const Ereal&lt; LType &gt; &amp;x, const Ereal&lt; LType &gt; &amp;y)</arglist>
    </member>
    <member kind="friend">
      <type>friend Ereal&lt; LType &gt;</type>
      <name>operator+</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>n1</anchor>
      <arglist>(const Ereal&lt; LType &gt; &amp;x, const NType &amp;y)</arglist>
    </member>
    <member kind="friend">
      <type>friend Ereal&lt; LType &gt;</type>
      <name>operator+</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>n2</anchor>
      <arglist>(const NType &amp;x, const Ereal&lt; LType &gt; &amp;y)</arglist>
    </member>
    <member kind="friend">
      <type>friend Ereal&lt; LType &gt;</type>
      <name>operator-</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>n3</anchor>
      <arglist>(const Ereal&lt; LType &gt; &amp;x, const Ereal&lt; LType &gt; &amp;y)</arglist>
    </member>
    <member kind="friend">
      <type>friend Ereal&lt; LType &gt;</type>
      <name>operator-</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>n4</anchor>
      <arglist>(const Ereal&lt; LType &gt; &amp;x, const NType &amp;y)</arglist>
    </member>
    <member kind="friend">
      <type>friend Ereal&lt; LType &gt;</type>
      <name>operator-</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>n5</anchor>
      <arglist>(const NType &amp;x, const Ereal&lt; LType &gt; &amp;y)</arglist>
    </member>
    <member kind="friend">
      <type>friend Ereal&lt; LType &gt;</type>
      <name>operator *</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>n6</anchor>
      <arglist>(const Ereal&lt; LType &gt; &amp;x, const Ereal&lt; LType &gt; &amp;y)</arglist>
    </member>
    <member kind="friend">
      <type>friend Ereal&lt; LType &gt;</type>
      <name>operator *</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>n7</anchor>
      <arglist>(const Ereal&lt; LType &gt; &amp;x, const NType &amp;y)</arglist>
    </member>
    <member kind="friend">
      <type>friend Ereal&lt; LType &gt;</type>
      <name>operator *</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>n8</anchor>
      <arglist>(const NType &amp;x, const Ereal&lt; LType &gt; &amp;y)</arglist>
    </member>
    <member kind="friend">
      <type>friend Ereal&lt; LType &gt;</type>
      <name>operator/</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>n9</anchor>
      <arglist>(const Ereal&lt; LType &gt; &amp;x, const Ereal&lt; LType &gt; &amp;y)</arglist>
    </member>
    <member kind="friend">
      <type>friend Ereal&lt; LType &gt;</type>
      <name>operator/</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>n10</anchor>
      <arglist>(const Ereal&lt; LType &gt; &amp;x, const NType &amp;y)</arglist>
    </member>
    <member kind="friend">
      <type>friend Ereal&lt; LType &gt;</type>
      <name>operator/</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>n11</anchor>
      <arglist>(const NType &amp;x, const Ereal&lt; LType &gt; &amp;y)</arglist>
    </member>
    <member kind="friend">
      <type>friend Ereal&lt; Type &gt;</type>
      <name>operator+</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>n12</anchor>
      <arglist>(const Ereal&lt; Type &gt; &amp;x)</arglist>
    </member>
    <member kind="friend">
      <type>friend Ereal&lt; Type &gt;</type>
      <name>operator-</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>n13</anchor>
      <arglist>(const Ereal&lt; Type &gt; &amp;num)</arglist>
    </member>
    <member kind="friend">
      <type>friend bool</type>
      <name>operator==</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>n14</anchor>
      <arglist>(const NType &amp;x, const Ereal&lt; LType &gt; &amp;y)</arglist>
    </member>
    <member kind="friend">
      <type>friend bool</type>
      <name>operator==</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>n15</anchor>
      <arglist>(const Ereal&lt; Type &gt; &amp;, const Ereal&lt; Type &gt; &amp;)</arglist>
    </member>
    <member kind="friend">
      <type>friend bool</type>
      <name>operator==</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>n16</anchor>
      <arglist>(const Ereal&lt; LType &gt; &amp;, const NType &amp;)</arglist>
    </member>
    <member kind="friend">
      <type>friend bool</type>
      <name>operator&lt;</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>n17</anchor>
      <arglist>(const Ereal&lt; LType &gt; &amp;, const Ereal&lt; LType &gt; &amp;)</arglist>
    </member>
    <member kind="friend">
      <type>friend bool</type>
      <name>operator&lt;</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>n18</anchor>
      <arglist>(const Ereal&lt; LType &gt; &amp;, const NType &amp;)</arglist>
    </member>
    <member kind="friend">
      <type>friend bool</type>
      <name>operator&lt;</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>n19</anchor>
      <arglist>(const NType &amp;x, const Ereal&lt; LType &gt; &amp;y)</arglist>
    </member>
    <member kind="friend">
      <type>friend bool</type>
      <name>operator&lt;=</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>n20</anchor>
      <arglist>(const Ereal&lt; LType &gt; &amp;, const Ereal&lt; LType &gt; &amp;)</arglist>
    </member>
    <member kind="friend">
      <type>friend bool</type>
      <name>operator&lt;=</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>n21</anchor>
      <arglist>(const Ereal&lt; LType &gt; &amp;, const NType &amp;)</arglist>
    </member>
    <member kind="friend">
      <type>friend bool</type>
      <name>operator&lt;=</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>n22</anchor>
      <arglist>(const NType &amp;x, const Ereal&lt; LType &gt; &amp;y)</arglist>
    </member>
    <member kind="friend">
      <type>friend bool</type>
      <name>isinf</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>n23</anchor>
      <arglist>(const Ereal&lt; Type &gt; &amp;num)</arglist>
    </member>
    <member kind="friend">
      <type>friend bool</type>
      <name>isnan</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>n24</anchor>
      <arglist>(const Ereal&lt; Type &gt; &amp;num)</arglist>
    </member>
    <member kind="friend">
      <type>friend bool</type>
      <name>isind</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>n25</anchor>
      <arglist>(const Ereal&lt; Type &gt; &amp;num)</arglist>
    </member>
    <member kind="friend">
      <type>friend bool</type>
      <name>finite</name>
      <anchorfile>classutilib_1_1Ereal.html</anchorfile>
      <anchor>n26</anchor>
      <arglist>(const Ereal&lt; Type &gt; &amp;num)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::GenericHashTableItem</name>
    <filename>classutilib_1_1GenericHashTableItem.html</filename>
    <templarg>T</templarg>
    <templarg>InfoT</templarg>
    <member kind="typedef">
      <type>size_t</type>
      <name>size_type</name>
      <anchorfile>classutilib_1_1GenericHashTableItem.html</anchorfile>
      <anchor>w0</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericHashTableItem&lt; T, InfoT &gt;</type>
      <name>hash_item</name>
      <anchorfile>classutilib_1_1GenericHashTableItem.html</anchorfile>
      <anchor>w1</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>ListItem&lt; hash_item * &gt; *</type>
      <name>hash_element</name>
      <anchorfile>classutilib_1_1GenericHashTableItem.html</anchorfile>
      <anchor>w2</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1GenericHashTableItem.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(ostream &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1GenericHashTableItem.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(PackBuffer &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1GenericHashTableItem.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(istream &amp;is)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1GenericHashTableItem.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(UnPackBuffer &amp;is)</arglist>
    </member>
    <member kind="function">
      <type>T &amp;</type>
      <name>key</name>
      <anchorfile>classutilib_1_1GenericHashTableItem.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>InfoT &amp;</type>
      <name>info</name>
      <anchorfile>classutilib_1_1GenericHashTableItem.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const T &amp;</type>
      <name>key</name>
      <anchorfile>classutilib_1_1GenericHashTableItem.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>const InfoT &amp;</type>
      <name>info</name>
      <anchorfile>classutilib_1_1GenericHashTableItem.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>compare</name>
      <anchorfile>classutilib_1_1GenericHashTableItem.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>(const T &amp;key) const </arglist>
    </member>
    <member kind="function" protection="private">
      <type></type>
      <name>GenericHashTableItem</name>
      <anchorfile>classutilib_1_1GenericHashTableItem.html</anchorfile>
      <anchor>d0</anchor>
      <arglist>(const T *Key_, const InfoT *info_)</arglist>
    </member>
    <member kind="function" protection="private">
      <type></type>
      <name>GenericHashTableItem</name>
      <anchorfile>classutilib_1_1GenericHashTableItem.html</anchorfile>
      <anchor>d1</anchor>
      <arglist>(T *Key_, InfoT *info_)</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>T *</type>
      <name>Keyptr</name>
      <anchorfile>classutilib_1_1GenericHashTableItem.html</anchorfile>
      <anchor>r0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>InfoT *</type>
      <name>Infoptr</name>
      <anchorfile>classutilib_1_1GenericHashTableItem.html</anchorfile>
      <anchor>r1</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend" protection="private">
      <type>friend class</type>
      <name>AbstractHashTable&lt; GenericHashTableItem&lt; T, InfoT &gt;, T, InfoT &gt;</name>
      <anchorfile>classutilib_1_1GenericHashTableItem.html</anchorfile>
      <anchor>n0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::GenericHashTable</name>
    <filename>classutilib_1_1GenericHashTable.html</filename>
    <templarg>T</templarg>
    <templarg>InfoT</templarg>
    <base>utilib::AbstractHashTable</base>
    <member kind="typedef">
      <type>GenericHashTableItem&lt; T, InfoT &gt;</type>
      <name>item_type</name>
      <anchorfile>classutilib_1_1GenericHashTable.html</anchorfile>
      <anchor>w0</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>AbstractHashTable&lt; item_type, T, InfoT &gt;::iterator</type>
      <name>iterator</name>
      <anchorfile>classutilib_1_1GenericHashTable.html</anchorfile>
      <anchor>w1</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>AbstractHashTable&lt; item_type, T, InfoT &gt;::const_iterator</type>
      <name>const_iterator</name>
      <anchorfile>classutilib_1_1GenericHashTable.html</anchorfile>
      <anchor>w2</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>GenericHashTable</name>
      <anchorfile>classutilib_1_1GenericHashTable.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(const char *nameBuff=&quot;Unnamed&quot;)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>GenericHashTable</name>
      <anchorfile>classutilib_1_1GenericHashTable.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(size_type init_size, const char *nameBuff=&quot;Unnamed&quot;)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~GenericHashTable</name>
      <anchorfile>classutilib_1_1GenericHashTable.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>utilib::GenericHeapCompare</name>
    <filename>structutilib_1_1GenericHeapCompare.html</filename>
    <templarg>Type</templarg>
    <base>utilib::ComparisonBase</base>
    <base>utilib::Comparison</base>
    <member kind="function">
      <type>compare_type</type>
      <name>operator()</name>
      <anchorfile>structutilib_1_1GenericHeapCompare.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(const Type &amp;_arg1, const Type &amp;_arg2) const </arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::GenericHeapItem</name>
    <filename>classutilib_1_1GenericHeapItem.html</filename>
    <templarg>T</templarg>
    <member kind="function">
      <type></type>
      <name>GenericHeapItem</name>
      <anchorfile>classutilib_1_1GenericHeapItem.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(T *Key_)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1GenericHeapItem.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(ostream &amp;os)</arglist>
    </member>
    <member kind="function">
      <type>T &amp;</type>
      <name>key</name>
      <anchorfile>classutilib_1_1GenericHeapItem.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>T *</type>
      <name>Keyptr</name>
      <anchorfile>classutilib_1_1GenericHeapItem.html</anchorfile>
      <anchor>r0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>element</name>
      <anchorfile>classutilib_1_1GenericHeapItem.html</anchorfile>
      <anchor>r1</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend" protection="private">
      <type>friend class</type>
      <name>GenericHeapBase&lt; GenericHeapItem&lt; T &gt; &gt;</name>
      <anchorfile>classutilib_1_1GenericHeapItem.html</anchorfile>
      <anchor>n0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::GenericHeapBase</name>
    <filename>classutilib_1_1GenericHeapBase.html</filename>
    <templarg>ItemType</templarg>
    <member kind="function" protection="protected">
      <type>int &amp;</type>
      <name>element</name>
      <anchorfile>classutilib_1_1GenericHeapBase.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>(ItemType *item)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::GenericHeap</name>
    <filename>classutilib_1_1GenericHeap.html</filename>
    <templarg>T</templarg>
    <templarg>Compare</templarg>
    <base>utilib::AbstractHeap&lt; GenericHeapItem&lt; T &gt;, T, Compare &gt;</base>
    <base>utilib::GenericHeapBase</base>
    <member kind="typedef">
      <type>AbstractHeap&lt; GenericHeapItem&lt; T &gt;, T, Compare &gt;::item_t</type>
      <name>item_t</name>
      <anchorfile>classutilib_1_1GenericHeap.html</anchorfile>
      <anchor>w0</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>GenericHeap</name>
      <anchorfile>classutilib_1_1GenericHeap.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(const char *nameBuff=&quot;Unnamed&quot;, int initSize=-1, int quantumChoice=-1)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>int &amp;</type>
      <name>element</name>
      <anchorfile>classutilib_1_1GenericHeap.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>(GenericHeapItem&lt; T &gt; *item)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::LocatedGenericHeap</name>
    <filename>classutilib_1_1LocatedGenericHeap.html</filename>
    <templarg>T</templarg>
    <templarg>Compare</templarg>
    <base>utilib::GenericHeap</base>
    <member kind="function">
      <type></type>
      <name>LocatedGenericHeap</name>
      <anchorfile>classutilib_1_1LocatedGenericHeap.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(const char *nameBuff=&quot;Unnamed&quot;, int initSize=-1, int quantumChoice=-1)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>moveEffect</name>
      <anchorfile>classutilib_1_1LocatedGenericHeap.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>(GenericHeapItem&lt; T &gt; *item)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::GenericSplayTreeItem</name>
    <filename>classutilib_1_1GenericSplayTreeItem.html</filename>
    <templarg>T</templarg>
    <member kind="function">
      <type>int</type>
      <name>Size</name>
      <anchorfile>classutilib_1_1GenericSplayTreeItem.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1GenericSplayTreeItem.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(ostream &amp;os)</arglist>
    </member>
    <member kind="function">
      <type>T &amp;</type>
      <name>key</name>
      <anchorfile>classutilib_1_1GenericSplayTreeItem.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const T &amp;</type>
      <name>key</name>
      <anchorfile>classutilib_1_1GenericSplayTreeItem.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>compare</name>
      <anchorfile>classutilib_1_1GenericSplayTreeItem.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(const T &amp;key) const </arglist>
    </member>
    <member kind="function">
      <type>GenericSplayTreeItem&lt; T &gt; *</type>
      <name>clone</name>
      <anchorfile>classutilib_1_1GenericSplayTreeItem.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private">
      <type></type>
      <name>GenericSplayTreeItem</name>
      <anchorfile>classutilib_1_1GenericSplayTreeItem.html</anchorfile>
      <anchor>d0</anchor>
      <arglist>(T *key_ptr_)</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>T *</type>
      <name>Keyptr</name>
      <anchorfile>classutilib_1_1GenericSplayTreeItem.html</anchorfile>
      <anchor>r0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>ctr</name>
      <anchorfile>classutilib_1_1GenericSplayTreeItem.html</anchorfile>
      <anchor>r1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>GenericSplayTreeItem&lt; T &gt; *</type>
      <name>left</name>
      <anchorfile>classutilib_1_1GenericSplayTreeItem.html</anchorfile>
      <anchor>r2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>GenericSplayTreeItem&lt; T &gt; *</type>
      <name>right</name>
      <anchorfile>classutilib_1_1GenericSplayTreeItem.html</anchorfile>
      <anchor>r3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>size</name>
      <anchorfile>classutilib_1_1GenericSplayTreeItem.html</anchorfile>
      <anchor>r4</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend" protection="private">
      <type>friend class</type>
      <name>AbstractSplayTree&lt; GenericSplayTreeItem&lt; T &gt;, T &gt;</name>
      <anchorfile>classutilib_1_1GenericSplayTreeItem.html</anchorfile>
      <anchor>n0</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend" protection="private">
      <type>friend class</type>
      <name>GenericSplayTree&lt; T &gt;</name>
      <anchorfile>classutilib_1_1GenericSplayTreeItem.html</anchorfile>
      <anchor>n1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::GenericSplayTree</name>
    <filename>classutilib_1_1GenericSplayTree.html</filename>
    <templarg>T</templarg>
    <base>utilib::AbstractSplayTree</base>
    <member kind="typedef">
      <type>AbstractSplayTree&lt; GenericSplayTreeItem&lt; T &gt;, T &gt;</type>
      <name>base_t</name>
      <anchorfile>classutilib_1_1GenericSplayTree.html</anchorfile>
      <anchor>w0</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>GenericSplayTree</name>
      <anchorfile>classutilib_1_1GenericSplayTree.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(const char *nameBuff=&quot;Unnamed&quot;)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>GenericSplayTree</name>
      <anchorfile>classutilib_1_1GenericSplayTree.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(const GenericSplayTree&lt; T &gt; &amp;tree)</arglist>
    </member>
    <member kind="function">
      <type>GenericSplayTree&lt; T &gt; &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1GenericSplayTree.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const GenericSplayTree&lt; T &gt; &amp;tree)</arglist>
    </member>
    <member kind="function">
      <type>bool &amp;</type>
      <name>duplicate</name>
      <anchorfile>classutilib_1_1GenericSplayTree.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool &amp;</type>
      <name>check_duplicates</name>
      <anchorfile>classutilib_1_1GenericSplayTree.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>GenericSplayTreeItem&lt; T &gt; *</type>
      <name>insert</name>
      <anchorfile>classutilib_1_1GenericSplayTree.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>(T *key)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>extract</name>
      <anchorfile>classutilib_1_1GenericSplayTree.html</anchorfile>
      <anchor>b1</anchor>
      <arglist>(GenericSplayTreeItem&lt; T &gt; *item, bool &amp;status)</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>duplicate_flag</name>
      <anchorfile>classutilib_1_1GenericSplayTree.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>check_duplicates_flag</name>
      <anchorfile>classutilib_1_1GenericSplayTree.html</anchorfile>
      <anchor>p1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::__LinkedList_HashedSet_OpClass</name>
    <filename>classutilib_1_1____LinkedList__HashedSet__OpClass.html</filename>
    <templarg>_Ref</templarg>
    <templarg>_Node</templarg>
    <member kind="function">
      <type>_Ref</type>
      <name>value</name>
      <anchorfile>classutilib_1_1____LinkedList__HashedSet__OpClass.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(_Node *node) const </arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::ListItem</name>
    <filename>classutilib_1_1ListItem.html</filename>
    <templarg>T</templarg>
    <base>utilib::CachedAllocatorObject&lt; ListItem&lt; T &gt; &gt;</base>
    <member kind="function">
      <type></type>
      <name>ListItem</name>
      <anchorfile>classutilib_1_1ListItem.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(const T &amp;Data_)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>ListItem</name>
      <anchorfile>classutilib_1_1ListItem.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>T &amp;</type>
      <name>data</name>
      <anchorfile>classutilib_1_1ListItem.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>classutilib_1_1ListItem.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(const ListItem&lt; T &gt; &amp;item)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>deallocate_derived</name>
      <anchorfile>classutilib_1_1ListItem.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>T</type>
      <name>Data</name>
      <anchorfile>classutilib_1_1ListItem.html</anchorfile>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ListItem&lt; T &gt; *</type>
      <name>next</name>
      <anchorfile>classutilib_1_1ListItem.html</anchorfile>
      <anchor>o1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ListItem&lt; T &gt; *</type>
      <name>prev</name>
      <anchorfile>classutilib_1_1ListItem.html</anchorfile>
      <anchor>o2</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend" protection="private">
      <type>friend class</type>
      <name>LinkedList</name>
      <anchorfile>classutilib_1_1ListItem.html</anchorfile>
      <anchor>n0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::__LinkedList_iterator_base</name>
    <filename>classutilib_1_1____LinkedList__iterator__base.html</filename>
    <templarg>_Tp</templarg>
    <templarg>_Ref</templarg>
    <templarg>_Ptr</templarg>
    <templarg>_Node</templarg>
    <templarg>_NodeOpClass</templarg>
    <member kind="function">
      <type></type>
      <name>__LinkedList_iterator_base</name>
      <anchorfile>classutilib_1_1____LinkedList__iterator__base.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(_Node *__x)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>_M_incr</name>
      <anchorfile>classutilib_1_1____LinkedList__iterator__base.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>_M_decr</name>
      <anchorfile>classutilib_1_1____LinkedList__iterator__base.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>_Ref</type>
      <name>operator *</name>
      <anchorfile>classutilib_1_1____LinkedList__iterator__base.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>_Ptr</type>
      <name>operator-&gt;</name>
      <anchorfile>classutilib_1_1____LinkedList__iterator__base.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="variable">
      <type>_Node *</type>
      <name>_M_node</name>
      <anchorfile>classutilib_1_1____LinkedList__iterator__base.html</anchorfile>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::__LinkedList_Standard_OpClass</name>
    <filename>classutilib_1_1____LinkedList__Standard__OpClass.html</filename>
    <templarg>_Ref</templarg>
    <templarg>_Node</templarg>
    <member kind="function">
      <type>_Ref</type>
      <name>value</name>
      <anchorfile>classutilib_1_1____LinkedList__Standard__OpClass.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(_Node *node) const </arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::__LinkedList_Pointer_OpClass</name>
    <filename>classutilib_1_1____LinkedList__Pointer__OpClass.html</filename>
    <templarg>_Ref</templarg>
    <templarg>_Node</templarg>
    <member kind="function">
      <type>_Ref</type>
      <name>value</name>
      <anchorfile>classutilib_1_1____LinkedList__Pointer__OpClass.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(_Node *node) const </arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::__LinkedList_iterator</name>
    <filename>classutilib_1_1____LinkedList__iterator.html</filename>
    <templarg>_Tp</templarg>
    <templarg>_Ref</templarg>
    <templarg>_Ptr</templarg>
    <templarg>_Node</templarg>
    <templarg>_NodeOpClass</templarg>
    <base>utilib::__LinkedList_iterator_base</base>
    <member kind="typedef">
      <type>size_t</type>
      <name>size_type</name>
      <anchorfile>classutilib_1_1____LinkedList__iterator.html</anchorfile>
      <anchor>w0</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>ptrdiff_t</type>
      <name>difference_type</name>
      <anchorfile>classutilib_1_1____LinkedList__iterator.html</anchorfile>
      <anchor>w1</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>bidirectional_iterator_tag</type>
      <name>iterator_category</name>
      <anchorfile>classutilib_1_1____LinkedList__iterator.html</anchorfile>
      <anchor>w2</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>_Tp</type>
      <name>value_type</name>
      <anchorfile>classutilib_1_1____LinkedList__iterator.html</anchorfile>
      <anchor>w3</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>_Ptr</type>
      <name>pointer</name>
      <anchorfile>classutilib_1_1____LinkedList__iterator.html</anchorfile>
      <anchor>w4</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>_Ref</type>
      <name>reference</name>
      <anchorfile>classutilib_1_1____LinkedList__iterator.html</anchorfile>
      <anchor>w5</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>__LinkedList_iterator&lt; _Tp, _Ref, _Ptr, _Node, _NodeOpClass &gt;</type>
      <name>iterator</name>
      <anchorfile>classutilib_1_1____LinkedList__iterator.html</anchorfile>
      <anchor>w6</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>__LinkedList_iterator</name>
      <anchorfile>classutilib_1_1____LinkedList__iterator.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(_Node *__x)</arglist>
    </member>
    <member kind="function">
      <type>iterator &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1____LinkedList__iterator.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const iterator &amp;__x)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>classutilib_1_1____LinkedList__iterator.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(const iterator &amp;__x) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator!=</name>
      <anchorfile>classutilib_1_1____LinkedList__iterator.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(const iterator &amp;__x) const </arglist>
    </member>
    <member kind="function">
      <type>iterator &amp;</type>
      <name>operator++</name>
      <anchorfile>classutilib_1_1____LinkedList__iterator.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>iterator</type>
      <name>operator++</name>
      <anchorfile>classutilib_1_1____LinkedList__iterator.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(int)</arglist>
    </member>
    <member kind="function">
      <type>iterator &amp;</type>
      <name>operator--</name>
      <anchorfile>classutilib_1_1____LinkedList__iterator.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>iterator</type>
      <name>operator--</name>
      <anchorfile>classutilib_1_1____LinkedList__iterator.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>(int)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::__LinkedList_ptr_iterator</name>
    <filename>classutilib_1_1____LinkedList__ptr__iterator.html</filename>
    <templarg>_Tp</templarg>
    <templarg>_Ref</templarg>
    <templarg>_Ptr</templarg>
    <templarg>_Node</templarg>
    <templarg>_NodeOpClass</templarg>
    <base>utilib::__LinkedList_iterator_base</base>
    <member kind="typedef">
      <type>size_t</type>
      <name>size_type</name>
      <anchorfile>classutilib_1_1____LinkedList__ptr__iterator.html</anchorfile>
      <anchor>w0</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>ptrdiff_t</type>
      <name>difference_type</name>
      <anchorfile>classutilib_1_1____LinkedList__ptr__iterator.html</anchorfile>
      <anchor>w1</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>bidirectional_iterator_tag</type>
      <name>iterator_category</name>
      <anchorfile>classutilib_1_1____LinkedList__ptr__iterator.html</anchorfile>
      <anchor>w2</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>_Tp</type>
      <name>value_type</name>
      <anchorfile>classutilib_1_1____LinkedList__ptr__iterator.html</anchorfile>
      <anchor>w3</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>_Ptr</type>
      <name>pointer</name>
      <anchorfile>classutilib_1_1____LinkedList__ptr__iterator.html</anchorfile>
      <anchor>w4</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>_Ref</type>
      <name>reference</name>
      <anchorfile>classutilib_1_1____LinkedList__ptr__iterator.html</anchorfile>
      <anchor>w5</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>__LinkedList_ptr_iterator&lt; _Tp, _Ref, _Ptr, _Node, _NodeOpClass &gt;</type>
      <name>iterator</name>
      <anchorfile>classutilib_1_1____LinkedList__ptr__iterator.html</anchorfile>
      <anchor>w6</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>__LinkedList_ptr_iterator</name>
      <anchorfile>classutilib_1_1____LinkedList__ptr__iterator.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(_Node *__x)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>__LinkedList_ptr_iterator</name>
      <anchorfile>classutilib_1_1____LinkedList__ptr__iterator.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const iterator &amp;__x)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>__LinkedList_ptr_iterator</name>
      <anchorfile>classutilib_1_1____LinkedList__ptr__iterator.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(const typename __LinkedList_iterator&lt; _Tp *, _Ref, _Ptr, _Node, _NodeOpClass &gt;::iterator &amp;__x)</arglist>
    </member>
    <member kind="function">
      <type>iterator &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1____LinkedList__ptr__iterator.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(const iterator &amp;__x)</arglist>
    </member>
    <member kind="function">
      <type>iterator &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1____LinkedList__ptr__iterator.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(const typename __LinkedList_iterator&lt; _Tp *, _Tp *&amp;, _Tp **, _Node, _NodeOpClass &gt;::iterator &amp;__x)</arglist>
    </member>
    <member kind="function">
      <type>iterator &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1____LinkedList__ptr__iterator.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(const typename __LinkedList_iterator&lt; _Tp *, _Tp *const &amp;, _Tp *const *, _Node, _NodeOpClass &gt;::iterator &amp;__x)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator!=</name>
      <anchorfile>classutilib_1_1____LinkedList__ptr__iterator.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>(const iterator &amp;__x) const </arglist>
    </member>
    <member kind="function">
      <type>iterator &amp;</type>
      <name>operator++</name>
      <anchorfile>classutilib_1_1____LinkedList__ptr__iterator.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>iterator</type>
      <name>operator++</name>
      <anchorfile>classutilib_1_1____LinkedList__ptr__iterator.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>(int)</arglist>
    </member>
    <member kind="function">
      <type>iterator &amp;</type>
      <name>operator--</name>
      <anchorfile>classutilib_1_1____LinkedList__ptr__iterator.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>iterator</type>
      <name>operator--</name>
      <anchorfile>classutilib_1_1____LinkedList__ptr__iterator.html</anchorfile>
      <anchor>a11</anchor>
      <arglist>(int)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::LinkedList</name>
    <filename>classutilib_1_1LinkedList.html</filename>
    <templarg>_Tp</templarg>
    <templarg>_Alloc</templarg>
    <member kind="typedef">
      <type>_Tp</type>
      <name>value_type</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>w0</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>value_type *</type>
      <name>pointer</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>w1</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>const value_type *</type>
      <name>const_pointer</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>w2</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>value_type &amp;</type>
      <name>reference</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>w3</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>const value_type &amp;</type>
      <name>const_reference</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>w4</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>ListItem&lt; _Tp &gt;</type>
      <name>_Node</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>w5</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>size_t</type>
      <name>size_type</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>w6</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>ptrdiff_t</type>
      <name>difference_type</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>w7</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>__LinkedList_iterator&lt; _Tp, _Tp &amp;, _Tp *, _Node, __LinkedList_Standard_OpClass&lt; _Tp &amp;, _Node &gt; &gt;</type>
      <name>iterator</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>w8</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>__LinkedList_iterator&lt; _Tp, const _Tp &amp;, const _Tp *, _Node, __LinkedList_Standard_OpClass&lt; _Tp &amp;, _Node &gt; &gt;</type>
      <name>const_iterator</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>w9</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>std::reverse_iterator&lt; const_iterator &gt;</type>
      <name>const_reverse_iterator</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>w10</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>std::reverse_iterator&lt; iterator &gt;</type>
      <name>reverse_iterator</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>w11</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>LinkedList</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~LinkedList</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>LinkedList&lt; _Tp, _Alloc &gt; &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const LinkedList&lt; _Tp, _Alloc &gt; &amp;list_)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>LinkedList</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(const LinkedList&lt; _Tp, _Alloc &gt; &amp;list_)</arglist>
    </member>
    <member kind="function">
      <type>ListItem&lt; _Tp &gt; *</type>
      <name>find</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(_Tp &amp;data)</arglist>
    </member>
    <member kind="function">
      <type>iterator</type>
      <name>begin</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>iterator</type>
      <name>end</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const_iterator</type>
      <name>begin</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>const_iterator</type>
      <name>end</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>reverse_iterator</type>
      <name>rbegin</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const_reverse_iterator</type>
      <name>rbegin</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>reverse_iterator</type>
      <name>rend</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a11</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const_reverse_iterator</type>
      <name>rend</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a12</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>reference</type>
      <name>front</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a13</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const_reference</type>
      <name>front</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a14</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>reference</type>
      <name>back</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a15</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const_reference</type>
      <name>back</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a16</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>iterator</type>
      <name>insert</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a17</anchor>
      <arglist>(iterator __position, const _Tp &amp;__x)</arglist>
    </member>
    <member kind="function">
      <type>iterator</type>
      <name>insert</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a18</anchor>
      <arglist>(iterator __position)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>push_front</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a19</anchor>
      <arglist>(const _Tp &amp;__x)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>push_front</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a20</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>push_back</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a21</anchor>
      <arglist>(const _Tp &amp;__x)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>push_back</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a22</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>iterator</type>
      <name>erase</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a23</anchor>
      <arglist>(iterator __position)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>clear</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a24</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>pop_front</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a25</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>pop_back</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a26</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>ListItem&lt; _Tp &gt; *</type>
      <name>head</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a27</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>ListItem&lt; _Tp &gt; *</type>
      <name>tail</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a28</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>ListItem&lt; _Tp &gt; *</type>
      <name>next</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a29</anchor>
      <arglist>(ListItem&lt; _Tp &gt; *item) const </arglist>
    </member>
    <member kind="function">
      <type>_Tp &amp;</type>
      <name>top</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a30</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>add</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a31</anchor>
      <arglist>(_Tp &amp;data)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>add</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a32</anchor>
      <arglist>(_Tp &amp;data, ListItem&lt; _Tp &gt; *&amp;item)</arglist>
    </member>
    <member kind="function">
      <type>ListItem&lt; _Tp &gt; *</type>
      <name>insert</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a33</anchor>
      <arglist>(const _Tp *data=(_Tp *) 0, ListItem&lt; _Tp &gt; *item=(ListItem&lt; _Tp &gt; *) 0)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>remove</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a34</anchor>
      <arglist>(_Tp &amp;data)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>remove</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a35</anchor>
      <arglist>(ListItem&lt; _Tp &gt; *item)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>remove</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a36</anchor>
      <arglist>(ListItem&lt; _Tp &gt; *item, _Tp &amp;data)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>empty</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a37</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>operator int</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a38</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>size</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a39</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>stack_mode</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a40</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>queue_mode</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a41</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a42</anchor>
      <arglist>(ostream &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>a43</anchor>
      <arglist>(istream &amp;is)</arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>validate_flag</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>stackLL</name>
      <anchor>x2x0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>queueLL</name>
      <anchor>x2x1</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>extract</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>(ListItem&lt; _Tp &gt; *item)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>extract</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>b1</anchor>
      <arglist>(ListItem&lt; _Tp &gt; *item, _Tp &amp;data)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>validate</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>b2</anchor>
      <arglist>(ListItem&lt; _Tp &gt; *item=0)</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>int</type>
      <name>mode</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>ListItem&lt; _Tp &gt; *</type>
      <name>first</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>p1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>ListItem&lt; _Tp &gt; *</type>
      <name>last</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>p2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>size_type</type>
      <name>Len</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>p3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected" static="yes">
      <type>static int</type>
      <name>counter</name>
      <anchorfile>classutilib_1_1LinkedList.html</anchorfile>
      <anchor>t0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::LPHashTableItem</name>
    <filename>classutilib_1_1LPHashTableItem.html</filename>
    <templarg>ValueT</templarg>
    <templarg>InfoT</templarg>
    <base>utilib::SimpleHashTableItem&lt; ValueT, InfoT &gt;</base>
    <member kind="function">
      <type></type>
      <name>LPHashTableItem</name>
      <anchorfile>classutilib_1_1LPHashTableItem.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(const ValueT *array_, const InfoT *info_)</arglist>
    </member>
    <member kind="variable">
      <type>ValueT</type>
      <name>array</name>
      <anchorfile>classutilib_1_1LPHashTableItem.html</anchorfile>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend" protection="private">
      <type>friend class</type>
      <name>AbstractHashTable&lt; LPHashTableItem&lt; ValueT, InfoT &gt;, ValueT, InfoT &gt;</name>
      <anchorfile>classutilib_1_1LPHashTableItem.html</anchorfile>
      <anchor>n0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::LPHashTable</name>
    <filename>classutilib_1_1LPHashTable.html</filename>
    <templarg>ValueT</templarg>
    <templarg>InfoT</templarg>
    <base>utilib::AbstractHashTable</base>
    <member kind="typedef">
      <type>LPHashTableItem&lt; ValueT, InfoT &gt;</type>
      <name>htitem</name>
      <anchorfile>classutilib_1_1LPHashTable.html</anchorfile>
      <anchor>w0</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>AbstractHashTable&lt; htitem, ValueT, InfoT &gt;</type>
      <name>baseclass</name>
      <anchorfile>classutilib_1_1LPHashTable.html</anchorfile>
      <anchor>w1</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>baseclass::iterator</type>
      <name>iterator</name>
      <anchorfile>classutilib_1_1LPHashTable.html</anchorfile>
      <anchor>w2</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>baseclass::const_iterator</type>
      <name>const_iterator</name>
      <anchorfile>classutilib_1_1LPHashTable.html</anchorfile>
      <anchor>w3</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>LPHashTable</name>
      <anchorfile>classutilib_1_1LPHashTable.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(const char *nameBuff=&quot;Unnamed&quot;)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>LPHashTable</name>
      <anchorfile>classutilib_1_1LPHashTable.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(size_type init_size, const char *nameBuff=&quot;Unnamed&quot;)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~LPHashTable</name>
      <anchorfile>classutilib_1_1LPHashTable.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual iterator</type>
      <name>find</name>
      <anchorfile>classutilib_1_1LPHashTable.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(const ValueT &amp;key)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual const_iterator</type>
      <name>find</name>
      <anchorfile>classutilib_1_1LPHashTable.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(const ValueT &amp;key) const </arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>find</name>
      <anchorfile>classutilib_1_1LPHashTable.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(const ValueT &amp;key, const_iterator &amp;begin, const_iterator &amp;end)</arglist>
    </member>
    <member kind="variable">
      <type>unsigned int</type>
      <name>hash_precision</name>
      <anchorfile>classutilib_1_1LPHashTable.html</anchorfile>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual bool</type>
      <name>exists_key</name>
      <anchorfile>classutilib_1_1LPHashTable.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>(const ValueT &amp;key)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>htitem *</type>
      <name>create_item</name>
      <anchorfile>classutilib_1_1LPHashTable.html</anchorfile>
      <anchor>b1</anchor>
      <arglist>(const ValueT *key, const InfoT *info)</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>ValueT</type>
      <name>tmp_array</name>
      <anchorfile>classutilib_1_1LPHashTable.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::MixedIntVarsRep</name>
    <filename>classutilib_1_1MixedIntVarsRep.html</filename>
    <member kind="function" protection="protected">
      <type></type>
      <name>MixedIntVarsRep</name>
      <anchorfile>classutilib_1_1MixedIntVarsRep.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>(int nBinVars=0, int nIntVars=0, int nDblVars=0)</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>int</type>
      <name>ref</name>
      <anchorfile>classutilib_1_1MixedIntVarsRep.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>EnumDataOwned</type>
      <name>own</name>
      <anchorfile>classutilib_1_1MixedIntVarsRep.html</anchorfile>
      <anchor>p1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>BitArray</type>
      <name>bitVars</name>
      <anchorfile>classutilib_1_1MixedIntVarsRep.html</anchorfile>
      <anchor>p2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>NumArray&lt; int &gt;</type>
      <name>gintVars</name>
      <anchorfile>classutilib_1_1MixedIntVarsRep.html</anchorfile>
      <anchor>p3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>NumArray&lt; double &gt;</type>
      <name>doubleVars</name>
      <anchorfile>classutilib_1_1MixedIntVarsRep.html</anchorfile>
      <anchor>p4</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend" protection="private">
      <type>friend class</type>
      <name>MixedIntVars</name>
      <anchorfile>classutilib_1_1MixedIntVarsRep.html</anchorfile>
      <anchor>n0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::MixedIntVars</name>
    <filename>classutilib_1_1MixedIntVars.html</filename>
    <member kind="function">
      <type></type>
      <name>MixedIntVars</name>
      <anchorfile>classutilib_1_1MixedIntVars.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(const int numBits=0, const int numGenInts=0, const int numDoubles=0)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~MixedIntVars</name>
      <anchorfile>classutilib_1_1MixedIntVars.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>resize</name>
      <anchorfile>classutilib_1_1MixedIntVars.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const int numBits, const int numGenInts, const int numDoubles)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>nrefs</name>
      <anchorfile>classutilib_1_1MixedIntVars.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>numIntegerVars</name>
      <anchorfile>classutilib_1_1MixedIntVars.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>numBinaryVars</name>
      <anchorfile>classutilib_1_1MixedIntVars.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>numGeneralIntVars</name>
      <anchorfile>classutilib_1_1MixedIntVars.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>numDoubleVars</name>
      <anchorfile>classutilib_1_1MixedIntVars.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>numAllVars</name>
      <anchorfile>classutilib_1_1MixedIntVars.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>BitArray &amp;</type>
      <name>Binary</name>
      <anchorfile>classutilib_1_1MixedIntVars.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const BitArray &amp;</type>
      <name>Binary</name>
      <anchorfile>classutilib_1_1MixedIntVars.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>NumArray&lt; int &gt; &amp;</type>
      <name>Integer</name>
      <anchorfile>classutilib_1_1MixedIntVars.html</anchorfile>
      <anchor>a11</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const NumArray&lt; int &gt; &amp;</type>
      <name>Integer</name>
      <anchorfile>classutilib_1_1MixedIntVars.html</anchorfile>
      <anchor>a12</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>NumArray&lt; double &gt; &amp;</type>
      <name>Real</name>
      <anchorfile>classutilib_1_1MixedIntVars.html</anchorfile>
      <anchor>a13</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const NumArray&lt; double &gt; &amp;</type>
      <name>Real</name>
      <anchorfile>classutilib_1_1MixedIntVars.html</anchorfile>
      <anchor>a14</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>double &amp;</type>
      <name>get</name>
      <anchorfile>classutilib_1_1MixedIntVars.html</anchorfile>
      <anchor>a15</anchor>
      <arglist>(const int ndx)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>get</name>
      <anchorfile>classutilib_1_1MixedIntVars.html</anchorfile>
      <anchor>a16</anchor>
      <arglist>(const int ndx) const </arglist>
    </member>
    <member kind="function">
      <type>MixedIntVars &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1MixedIntVars.html</anchorfile>
      <anchor>a17</anchor>
      <arglist>(const MixedIntVars &amp;array)</arglist>
    </member>
    <member kind="function">
      <type>MixedIntVars &amp;</type>
      <name>operator &amp;=</name>
      <anchorfile>classutilib_1_1MixedIntVars.html</anchorfile>
      <anchor>a18</anchor>
      <arglist>(const MixedIntVars &amp;array)</arglist>
    </member>
    <member kind="function">
      <type>MixedIntVars &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1MixedIntVars.html</anchorfile>
      <anchor>a19</anchor>
      <arglist>(const char val)</arglist>
    </member>
    <member kind="function">
      <type>MixedIntVars &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1MixedIntVars.html</anchorfile>
      <anchor>a20</anchor>
      <arglist>(const int val)</arglist>
    </member>
    <member kind="function">
      <type>MixedIntVars &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1MixedIntVars.html</anchorfile>
      <anchor>a21</anchor>
      <arglist>(const double val)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>classutilib_1_1MixedIntVars.html</anchorfile>
      <anchor>a22</anchor>
      <arglist>(const MixedIntVars &amp;array) const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>compare</name>
      <anchorfile>classutilib_1_1MixedIntVars.html</anchorfile>
      <anchor>a23</anchor>
      <arglist>(const MixedIntVars &amp;array) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1MixedIntVars.html</anchorfile>
      <anchor>a24</anchor>
      <arglist>(ostream &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1MixedIntVars.html</anchorfile>
      <anchor>a25</anchor>
      <arglist>(istream &amp;is)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1MixedIntVars.html</anchorfile>
      <anchor>a26</anchor>
      <arglist>(PackBuffer &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1MixedIntVars.html</anchorfile>
      <anchor>a27</anchor>
      <arglist>(UnPackBuffer &amp;is)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>construct</name>
      <anchorfile>classutilib_1_1MixedIntVars.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>(const int nbits, const int nints, const int ndlbs)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>free</name>
      <anchorfile>classutilib_1_1MixedIntVars.html</anchorfile>
      <anchor>b1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>MixedIntVarsRep *</type>
      <name>a</name>
      <anchorfile>classutilib_1_1MixedIntVars.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::Num2DArray</name>
    <filename>classutilib_1_1Num2DArray.html</filename>
    <templarg>T</templarg>
    <base>utilib::Simple2DArray</base>
    <member kind="typedef">
      <type>Simple2DArray&lt; T &gt;</type>
      <name>base_t</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>w0</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Num2DArray</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Num2DArray</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(const BasicArray&lt; T &gt; &amp;array, const int nrows=1, const EnumDataOwned own=DataNotOwned)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Num2DArray</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const int nrows, const int ncols, T *d=((T *) 0), const EnumDataOwned own=DataNotOwned)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Num2DArray</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(const int nrows, const int ncols, const BasicArray&lt; T &gt; &amp;array, const EnumDataOwned own=DataNotOwned)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Num2DArray</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(const Num2DArray &amp;array)</arglist>
    </member>
    <member kind="function">
      <type>Num2DArray&lt; T &gt; &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(const Num2DArray&lt; T &gt; &amp;array)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>plus</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(const Num2DArray&lt; T &gt; &amp;x, const Num2DArray&lt; T &gt; &amp;y)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>minus</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>(const Num2DArray&lt; T &gt; &amp;x, const Num2DArray&lt; T &gt; &amp;y)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>times</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>(const Num2DArray&lt; T &gt; &amp;x, const Num2DArray&lt; T &gt; &amp;y)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>divide</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>(const Num2DArray&lt; T &gt; &amp;x, const Num2DArray&lt; T &gt; &amp;y)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>plus</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>(const Num2DArray&lt; T &gt; &amp;x, const NumArray&lt; T &gt; &amp;y)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>minus</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>a11</anchor>
      <arglist>(const Num2DArray&lt; T &gt; &amp;x, const NumArray&lt; T &gt; &amp;y)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>times</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>a12</anchor>
      <arglist>(const Num2DArray&lt; T &gt; &amp;x, const NumArray&lt; T &gt; &amp;y)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>divide</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>a13</anchor>
      <arglist>(const Num2DArray&lt; T &gt; &amp;x, const NumArray&lt; T &gt; &amp;y)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>plus</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>a14</anchor>
      <arglist>(const Num2DArray&lt; T &gt; &amp;x, const T &amp;z)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>minus</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>a15</anchor>
      <arglist>(const Num2DArray&lt; T &gt; &amp;x, const T &amp;z)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>times</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>a16</anchor>
      <arglist>(const Num2DArray&lt; T &gt; &amp;x, const T &amp;z)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>divide</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>a17</anchor>
      <arglist>(const Num2DArray&lt; T &gt; &amp;x, const T &amp;z)</arglist>
    </member>
    <member kind="function">
      <type>Num2DArray&lt; T &gt;</type>
      <name>operator-</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>a18</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Num2DArray&lt; T &gt; &amp;</type>
      <name>operator+=</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>a19</anchor>
      <arglist>(const Num2DArray&lt; T &gt; &amp;x)</arglist>
    </member>
    <member kind="function">
      <type>Num2DArray&lt; T &gt; &amp;</type>
      <name>operator-=</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>a20</anchor>
      <arglist>(const Num2DArray&lt; T &gt; &amp;x)</arglist>
    </member>
    <member kind="function">
      <type>Num2DArray&lt; T &gt; &amp;</type>
      <name>operator *=</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>a21</anchor>
      <arglist>(const Num2DArray&lt; T &gt; &amp;x)</arglist>
    </member>
    <member kind="function">
      <type>Num2DArray&lt; T &gt; &amp;</type>
      <name>operator/=</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>a22</anchor>
      <arglist>(const Num2DArray&lt; T &gt; &amp;x)</arglist>
    </member>
    <member kind="function">
      <type>Num2DArray&lt; T &gt; &amp;</type>
      <name>operator+=</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>a23</anchor>
      <arglist>(const NumArray&lt; T &gt; &amp;y)</arglist>
    </member>
    <member kind="function">
      <type>Num2DArray&lt; T &gt; &amp;</type>
      <name>operator-=</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>a24</anchor>
      <arglist>(const NumArray&lt; T &gt; &amp;y)</arglist>
    </member>
    <member kind="function">
      <type>Num2DArray&lt; T &gt; &amp;</type>
      <name>operator *=</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>a25</anchor>
      <arglist>(const NumArray&lt; T &gt; &amp;y)</arglist>
    </member>
    <member kind="function">
      <type>Num2DArray&lt; T &gt; &amp;</type>
      <name>operator/=</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>a26</anchor>
      <arglist>(const NumArray&lt; T &gt; &amp;y)</arglist>
    </member>
    <member kind="function">
      <type>Num2DArray&lt; T &gt; &amp;</type>
      <name>operator+=</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>a27</anchor>
      <arglist>(const T &amp;z)</arglist>
    </member>
    <member kind="function">
      <type>Num2DArray&lt; T &gt; &amp;</type>
      <name>operator-=</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>a28</anchor>
      <arglist>(const T &amp;z)</arglist>
    </member>
    <member kind="function">
      <type>Num2DArray&lt; T &gt; &amp;</type>
      <name>operator *=</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>a29</anchor>
      <arglist>(const T &amp;z)</arglist>
    </member>
    <member kind="function">
      <type>Num2DArray&lt; T &gt; &amp;</type>
      <name>operator/=</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>a30</anchor>
      <arglist>(const T &amp;z)</arglist>
    </member>
    <member kind="friend">
      <type>friend void</type>
      <name>matmult</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>n0</anchor>
      <arglist>(Num2DArray&lt; T &gt; &amp;res, const Num2DArray&lt; T &gt; &amp;, const Num2DArray&lt; T &gt; &amp;)</arglist>
    </member>
    <member kind="friend">
      <type>friend void</type>
      <name>matmult</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>n1</anchor>
      <arglist>(NumArray&lt; T &gt; &amp;res, const Num2DArray&lt; T &gt; &amp;, const NumArray&lt; T &gt; &amp;)</arglist>
    </member>
    <member kind="friend">
      <type>friend void</type>
      <name>matmult</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>n2</anchor>
      <arglist>(NumArray&lt; T &gt; &amp;res, const NumArray&lt; T &gt; &amp;, const Num2DArray&lt; T &gt; &amp;)</arglist>
    </member>
    <member kind="friend">
      <type>friend Num2DArray&lt; T &gt;</type>
      <name>operator%</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>n3</anchor>
      <arglist>(const Num2DArray&lt; T &gt; &amp;, const Num2DArray&lt; T &gt; &amp;)</arglist>
    </member>
    <member kind="friend">
      <type>friend NumArray&lt; T &gt;</type>
      <name>operator%</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>n4</anchor>
      <arglist>(const Num2DArray&lt; T &gt; &amp;, const NumArray&lt; T &gt; &amp;)</arglist>
    </member>
    <member kind="friend">
      <type>friend NumArray&lt; T &gt;</type>
      <name>operator%</name>
      <anchorfile>classutilib_1_1Num2DArray.html</anchorfile>
      <anchor>n5</anchor>
      <arglist>(const NumArray&lt; T &gt; &amp;, const Num2DArray&lt; T &gt; &amp;)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::NumArray</name>
    <filename>classutilib_1_1NumArray.html</filename>
    <templarg>T</templarg>
    <base>utilib::BasicArray</base>
    <member kind="function">
      <type></type>
      <name>NumArray</name>
      <anchorfile>classutilib_1_1NumArray.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>NumArray</name>
      <anchorfile>classutilib_1_1NumArray.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(const size_type len, T *d=((T *) 0), const EnumDataOwned o=DataNotOwned)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>NumArray</name>
      <anchorfile>classutilib_1_1NumArray.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const NumArray &amp;array)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~NumArray</name>
      <anchorfile>classutilib_1_1NumArray.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>NumArray&lt; T &gt; &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1NumArray.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(const BasicArray&lt; T &gt; &amp;array)</arglist>
    </member>
    <member kind="function">
      <type>NumArray&lt; T &gt; &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1NumArray.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(const NumArray&lt; T &gt; &amp;array)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>plus</name>
      <anchorfile>classutilib_1_1NumArray.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(const NumArray&lt; T &gt; &amp;x, const NumArray&lt; T &gt; &amp;y)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>minus</name>
      <anchorfile>classutilib_1_1NumArray.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>(const NumArray&lt; T &gt; &amp;x, const NumArray&lt; T &gt; &amp;y)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>times</name>
      <anchorfile>classutilib_1_1NumArray.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>(const NumArray&lt; T &gt; &amp;x, const NumArray&lt; T &gt; &amp;y)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>divide</name>
      <anchorfile>classutilib_1_1NumArray.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>(const NumArray&lt; T &gt; &amp;x, const NumArray&lt; T &gt; &amp;y)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>plus</name>
      <anchorfile>classutilib_1_1NumArray.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>(const NumArray&lt; T &gt; &amp;x, const T &amp;z)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>minus</name>
      <anchorfile>classutilib_1_1NumArray.html</anchorfile>
      <anchor>a11</anchor>
      <arglist>(const NumArray&lt; T &gt; &amp;x, const T &amp;z)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>times</name>
      <anchorfile>classutilib_1_1NumArray.html</anchorfile>
      <anchor>a12</anchor>
      <arglist>(const NumArray&lt; T &gt; &amp;x, const T &amp;z)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>divide</name>
      <anchorfile>classutilib_1_1NumArray.html</anchorfile>
      <anchor>a13</anchor>
      <arglist>(const NumArray&lt; T &gt; &amp;x, const T &amp;z)</arglist>
    </member>
    <member kind="function">
      <type>NumArray&lt; T &gt;</type>
      <name>operator-</name>
      <anchorfile>classutilib_1_1NumArray.html</anchorfile>
      <anchor>a14</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>NumArray&lt; T &gt; &amp;</type>
      <name>operator+=</name>
      <anchorfile>classutilib_1_1NumArray.html</anchorfile>
      <anchor>a15</anchor>
      <arglist>(const NumArray&lt; T &gt; &amp;x)</arglist>
    </member>
    <member kind="function">
      <type>NumArray&lt; T &gt; &amp;</type>
      <name>operator-=</name>
      <anchorfile>classutilib_1_1NumArray.html</anchorfile>
      <anchor>a16</anchor>
      <arglist>(const NumArray&lt; T &gt; &amp;x)</arglist>
    </member>
    <member kind="function">
      <type>NumArray&lt; T &gt; &amp;</type>
      <name>operator *=</name>
      <anchorfile>classutilib_1_1NumArray.html</anchorfile>
      <anchor>a17</anchor>
      <arglist>(const NumArray&lt; T &gt; &amp;x)</arglist>
    </member>
    <member kind="function">
      <type>NumArray&lt; T &gt; &amp;</type>
      <name>operator/=</name>
      <anchorfile>classutilib_1_1NumArray.html</anchorfile>
      <anchor>a18</anchor>
      <arglist>(const NumArray&lt; T &gt; &amp;x)</arglist>
    </member>
    <member kind="function">
      <type>NumArray&lt; T &gt; &amp;</type>
      <name>operator+=</name>
      <anchorfile>classutilib_1_1NumArray.html</anchorfile>
      <anchor>a19</anchor>
      <arglist>(const T &amp;z)</arglist>
    </member>
    <member kind="function">
      <type>NumArray&lt; T &gt; &amp;</type>
      <name>operator-=</name>
      <anchorfile>classutilib_1_1NumArray.html</anchorfile>
      <anchor>a20</anchor>
      <arglist>(const T &amp;z)</arglist>
    </member>
    <member kind="function">
      <type>NumArray&lt; T &gt; &amp;</type>
      <name>operator *=</name>
      <anchorfile>classutilib_1_1NumArray.html</anchorfile>
      <anchor>a21</anchor>
      <arglist>(const T &amp;z)</arglist>
    </member>
    <member kind="function">
      <type>NumArray&lt; T &gt; &amp;</type>
      <name>operator/=</name>
      <anchorfile>classutilib_1_1NumArray.html</anchorfile>
      <anchor>a22</anchor>
      <arglist>(const T &amp;z)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>initialize</name>
      <anchorfile>classutilib_1_1NumArray.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>(T *data, const size_type start, const size_type stop)</arglist>
    </member>
    <member kind="typedef" protection="private">
      <type>BasicArray&lt; T &gt;</type>
      <name>base_t</name>
      <anchorfile>classutilib_1_1NumArray.html</anchorfile>
      <anchor>y0</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend" protection="private">
      <type>friend class</type>
      <name>Num2DArray</name>
      <anchorfile>classutilib_1_1NumArray.html</anchorfile>
      <anchor>n0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::OrderedListItem</name>
    <filename>classutilib_1_1OrderedListItem.html</filename>
    <templarg>T</templarg>
    <templarg>V</templarg>
    <base>utilib::CachedAllocatorObject&lt; OrderedListItem&lt; T, V &gt; &gt;</base>
    <member kind="function">
      <type></type>
      <name>OrderedListItem</name>
      <anchorfile>classutilib_1_1OrderedListItem.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>OrderedListItem</name>
      <anchorfile>classutilib_1_1OrderedListItem.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(T Data_, V Key_)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~OrderedListItem</name>
      <anchorfile>classutilib_1_1OrderedListItem.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>T &amp;</type>
      <name>data</name>
      <anchorfile>classutilib_1_1OrderedListItem.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>V</type>
      <name>key</name>
      <anchorfile>classutilib_1_1OrderedListItem.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>classutilib_1_1OrderedListItem.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(const OrderedListItem&lt; T, V &gt; &amp;item)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>deallocate_derived</name>
      <anchorfile>classutilib_1_1OrderedListItem.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>T</type>
      <name>Data</name>
      <anchorfile>classutilib_1_1OrderedListItem.html</anchorfile>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>V</type>
      <name>Key</name>
      <anchorfile>classutilib_1_1OrderedListItem.html</anchorfile>
      <anchor>o1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>OrderedListItem&lt; T, V &gt; *</type>
      <name>next</name>
      <anchorfile>classutilib_1_1OrderedListItem.html</anchorfile>
      <anchor>o2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>OrderedListItem&lt; T, V &gt; *</type>
      <name>prev</name>
      <anchorfile>classutilib_1_1OrderedListItem.html</anchorfile>
      <anchor>o3</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend" protection="private">
      <type>friend class</type>
      <name>OrderedList&lt; T, V &gt;</name>
      <anchorfile>classutilib_1_1OrderedListItem.html</anchorfile>
      <anchor>n0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::OrderedList</name>
    <filename>classutilib_1_1OrderedList.html</filename>
    <templarg>T</templarg>
    <templarg>V</templarg>
    <member kind="function">
      <type></type>
      <name>OrderedList</name>
      <anchorfile>classutilib_1_1OrderedList.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>OrderedList</name>
      <anchorfile>classutilib_1_1OrderedList.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(const OrderedList&lt; T, V &gt; &amp;list)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~OrderedList</name>
      <anchorfile>classutilib_1_1OrderedList.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>OrderedList&lt; T, V &gt; &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1OrderedList.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(const OrderedList&lt; T, V &gt; &amp;list)</arglist>
    </member>
    <member kind="function">
      <type>OrderedListItem&lt; T, V &gt; *</type>
      <name>find</name>
      <anchorfile>classutilib_1_1OrderedList.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(V &amp;key)</arglist>
    </member>
    <member kind="function">
      <type>OrderedListItem&lt; T, V &gt; *</type>
      <name>head</name>
      <anchorfile>classutilib_1_1OrderedList.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>OrderedListItem&lt; T, V &gt; *</type>
      <name>tail</name>
      <anchorfile>classutilib_1_1OrderedList.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>OrderedListItem&lt; T, V &gt; *</type>
      <name>next</name>
      <anchorfile>classutilib_1_1OrderedList.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>(OrderedListItem&lt; T, V &gt; *item) const </arglist>
    </member>
    <member kind="function">
      <type>T &amp;</type>
      <name>top</name>
      <anchorfile>classutilib_1_1OrderedList.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>add</name>
      <anchorfile>classutilib_1_1OrderedList.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>(T &amp;data, V &amp;key)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>add</name>
      <anchorfile>classutilib_1_1OrderedList.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>(T &amp;data, V &amp;key, OrderedListItem&lt; T, V &gt; *&amp;item)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>remove</name>
      <anchorfile>classutilib_1_1OrderedList.html</anchorfile>
      <anchor>a11</anchor>
      <arglist>(T &amp;data, V &amp;key)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>remove</name>
      <anchorfile>classutilib_1_1OrderedList.html</anchorfile>
      <anchor>a12</anchor>
      <arglist>(OrderedListItem&lt; T, V &gt; *&amp;item)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>remove</name>
      <anchorfile>classutilib_1_1OrderedList.html</anchorfile>
      <anchor>a13</anchor>
      <arglist>(OrderedListItem&lt; T, V &gt; *&amp;item, T &amp;data, V &amp;key)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>update</name>
      <anchorfile>classutilib_1_1OrderedList.html</anchorfile>
      <anchor>a14</anchor>
      <arglist>(OrderedListItem&lt; T, V &gt; *item, V &amp;key)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>empty</name>
      <anchorfile>classutilib_1_1OrderedList.html</anchorfile>
      <anchor>a15</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>operator int</name>
      <anchorfile>classutilib_1_1OrderedList.html</anchorfile>
      <anchor>a16</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>size</name>
      <anchorfile>classutilib_1_1OrderedList.html</anchorfile>
      <anchor>a17</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>extract</name>
      <anchorfile>classutilib_1_1OrderedList.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>(OrderedListItem&lt; T, V &gt; *item, T &amp;data, V &amp;key)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>extract</name>
      <anchorfile>classutilib_1_1OrderedList.html</anchorfile>
      <anchor>b1</anchor>
      <arglist>(OrderedListItem&lt; T, V &gt; *item)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>OrderedListItem&lt; T, V &gt; *</type>
      <name>insert</name>
      <anchorfile>classutilib_1_1OrderedList.html</anchorfile>
      <anchor>b2</anchor>
      <arglist>(T &amp;data, V &amp;key)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>extract_all</name>
      <anchorfile>classutilib_1_1OrderedList.html</anchorfile>
      <anchor>b3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>OrderedListItem&lt; T, V &gt; *</type>
      <name>first</name>
      <anchorfile>classutilib_1_1OrderedList.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>OrderedListItem&lt; T, V &gt; *</type>
      <name>last</name>
      <anchorfile>classutilib_1_1OrderedList.html</anchorfile>
      <anchor>p1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>int</type>
      <name>Len</name>
      <anchorfile>classutilib_1_1OrderedList.html</anchorfile>
      <anchor>p2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected" static="yes">
      <type>static int</type>
      <name>counter</name>
      <anchorfile>classutilib_1_1OrderedList.html</anchorfile>
      <anchor>t0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::OrderedSet</name>
    <filename>classutilib_1_1OrderedSet.html</filename>
    <templarg>T</templarg>
    <member kind="function">
      <type></type>
      <name>OrderedSet</name>
      <anchorfile>classutilib_1_1OrderedSet.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~OrderedSet</name>
      <anchorfile>classutilib_1_1OrderedSet.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>add</name>
      <anchorfile>classutilib_1_1OrderedSet.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const T &amp;val, const bool unique=false)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>add</name>
      <anchorfile>classutilib_1_1OrderedSet.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(const OrderedSet&lt; T &gt; &amp;, const bool unique=false)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>remove</name>
      <anchorfile>classutilib_1_1OrderedSet.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(T &amp;val)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>empty</name>
      <anchorfile>classutilib_1_1OrderedSet.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>operator int</name>
      <anchorfile>classutilib_1_1OrderedSet.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>size</name>
      <anchorfile>classutilib_1_1OrderedSet.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>index</name>
      <anchorfile>classutilib_1_1OrderedSet.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>(const T &amp;val)</arglist>
    </member>
    <member kind="function">
      <type>T &amp;</type>
      <name>first</name>
      <anchorfile>classutilib_1_1OrderedSet.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>(OrderedSetIndex &amp;index, bool &amp;status)</arglist>
    </member>
    <member kind="function">
      <type>T &amp;</type>
      <name>next</name>
      <anchorfile>classutilib_1_1OrderedSet.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>(OrderedSetIndex &amp;index, bool &amp;status)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>is_member</name>
      <anchorfile>classutilib_1_1OrderedSet.html</anchorfile>
      <anchor>a11</anchor>
      <arglist>(const T &amp;val)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>is_disjoint</name>
      <anchorfile>classutilib_1_1OrderedSet.html</anchorfile>
      <anchor>a12</anchor>
      <arglist>(const OrderedSet&lt; T &gt; &amp;set)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1OrderedSet.html</anchorfile>
      <anchor>a13</anchor>
      <arglist>(ostream &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1OrderedSet.html</anchorfile>
      <anchor>a14</anchor>
      <arglist>(istream &amp;is)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1OrderedSet.html</anchorfile>
      <anchor>a15</anchor>
      <arglist>(PackBuffer &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1OrderedSet.html</anchorfile>
      <anchor>a16</anchor>
      <arglist>(UnPackBuffer &amp;is)</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>size_type</type>
      <name>Len</name>
      <anchorfile>classutilib_1_1OrderedSet.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>BasicArray&lt; T &gt;</type>
      <name>Data</name>
      <anchorfile>classutilib_1_1OrderedSet.html</anchorfile>
      <anchor>p1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::PersistentPointer</name>
    <filename>classutilib_1_1PersistentPointer.html</filename>
    <templarg>PointerT</templarg>
    <member kind="function">
      <type></type>
      <name>PersistentPointer</name>
      <anchorfile>classutilib_1_1PersistentPointer.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(PointerT *ptr_)</arglist>
    </member>
    <member kind="function">
      <type>PointerT *</type>
      <name>operator-&gt;</name>
      <anchorfile>classutilib_1_1PersistentPointer.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>PointerT &amp;</type>
      <name>operator *</name>
      <anchorfile>classutilib_1_1PersistentPointer.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>PointerT *</type>
      <name>ptr</name>
      <anchorfile>classutilib_1_1PersistentPointer.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::pvector</name>
    <filename>classutilib_1_1pvector.html</filename>
    <templarg>_Tp</templarg>
    <templarg>_RWSTD_COMPLEX_DEFAULT</templarg>
    <member kind="function">
      <type></type>
      <name>pvector</name>
      <anchorfile>classutilib_1_1pvector.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(const allocator_type &amp;__a=allocator_type())</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>pvector</name>
      <anchorfile>classutilib_1_1pvector.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(size_type __n, const _Tp &amp;__value, const allocator_type &amp;__a=allocator_type())</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>pvector</name>
      <anchorfile>classutilib_1_1pvector.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(size_type __n)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>pvector</name>
      <anchorfile>classutilib_1_1pvector.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(const pvector&lt; _Tp, _PAlloc &gt; &amp;__x)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>pvector</name>
      <anchorfile>classutilib_1_1pvector.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(const _Tp *__first, const _Tp *__last, const allocator_type &amp;__a=allocator_type())</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~pvector</name>
      <anchorfile>classutilib_1_1pvector.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>pvector&lt; _Tp, _PAlloc &gt; &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1pvector.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(const vector&lt; _Tp, _PAlloc &gt; &amp;__x)</arglist>
    </member>
    <member kind="function">
      <type>pvector&lt; _Tp, _PAlloc &gt; &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1pvector.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>(const _Tp &amp;val)</arglist>
    </member>
    <member kind="function">
      <type>reference</type>
      <name>operator[]</name>
      <anchorfile>classutilib_1_1pvector.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>(size_type __n)</arglist>
    </member>
    <member kind="function">
      <type>const_reference</type>
      <name>operator[]</name>
      <anchorfile>classutilib_1_1pvector.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>(size_type __n) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>push_back</name>
      <anchorfile>classutilib_1_1pvector.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>(const _Tp &amp;val)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>push_back</name>
      <anchorfile>classutilib_1_1pvector.html</anchorfile>
      <anchor>a11</anchor>
      <arglist>(const vector&lt; _Tp, _PAlloc &gt; &amp;vec)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>plus</name>
      <anchorfile>classutilib_1_1pvector.html</anchorfile>
      <anchor>a12</anchor>
      <arglist>(const pvector&lt; _Tp, _PAlloc &gt; &amp;x, const pvector&lt; _Tp, _PAlloc &gt; &amp;y)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>minus</name>
      <anchorfile>classutilib_1_1pvector.html</anchorfile>
      <anchor>a13</anchor>
      <arglist>(const pvector&lt; _Tp, _PAlloc &gt; &amp;x, const pvector&lt; _Tp, _PAlloc &gt; &amp;y)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>times</name>
      <anchorfile>classutilib_1_1pvector.html</anchorfile>
      <anchor>a14</anchor>
      <arglist>(const pvector&lt; _Tp, _PAlloc &gt; &amp;x, const pvector&lt; _Tp, _PAlloc &gt; &amp;y)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>divide</name>
      <anchorfile>classutilib_1_1pvector.html</anchorfile>
      <anchor>a15</anchor>
      <arglist>(const pvector&lt; _Tp, _PAlloc &gt; &amp;x, const pvector&lt; _Tp, _PAlloc &gt; &amp;y)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>plus</name>
      <anchorfile>classutilib_1_1pvector.html</anchorfile>
      <anchor>a16</anchor>
      <arglist>(const pvector&lt; _Tp, _PAlloc &gt; &amp;x, const _Tp &amp;z)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>minus</name>
      <anchorfile>classutilib_1_1pvector.html</anchorfile>
      <anchor>a17</anchor>
      <arglist>(const pvector&lt; _Tp, _PAlloc &gt; &amp;x, const _Tp &amp;z)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>times</name>
      <anchorfile>classutilib_1_1pvector.html</anchorfile>
      <anchor>a18</anchor>
      <arglist>(const pvector&lt; _Tp, _PAlloc &gt; &amp;x, const _Tp &amp;z)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>divide</name>
      <anchorfile>classutilib_1_1pvector.html</anchorfile>
      <anchor>a19</anchor>
      <arglist>(const pvector&lt; _Tp, _PAlloc &gt; &amp;x, const _Tp &amp;z)</arglist>
    </member>
    <member kind="function">
      <type>pvector&lt; _Tp, _PAlloc &gt;</type>
      <name>operator-</name>
      <anchorfile>classutilib_1_1pvector.html</anchorfile>
      <anchor>a20</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>pvector&lt; _Tp, _PAlloc &gt; &amp;</type>
      <name>operator+=</name>
      <anchorfile>classutilib_1_1pvector.html</anchorfile>
      <anchor>a21</anchor>
      <arglist>(const pvector&lt; _Tp, _PAlloc &gt; &amp;x)</arglist>
    </member>
    <member kind="function">
      <type>pvector&lt; _Tp, _PAlloc &gt; &amp;</type>
      <name>operator-=</name>
      <anchorfile>classutilib_1_1pvector.html</anchorfile>
      <anchor>a22</anchor>
      <arglist>(const pvector&lt; _Tp, _PAlloc &gt; &amp;x)</arglist>
    </member>
    <member kind="function">
      <type>pvector&lt; _Tp, _PAlloc &gt; &amp;</type>
      <name>operator *=</name>
      <anchorfile>classutilib_1_1pvector.html</anchorfile>
      <anchor>a23</anchor>
      <arglist>(const pvector&lt; _Tp, _PAlloc &gt; &amp;x)</arglist>
    </member>
    <member kind="function">
      <type>pvector&lt; _Tp, _PAlloc &gt; &amp;</type>
      <name>operator/=</name>
      <anchorfile>classutilib_1_1pvector.html</anchorfile>
      <anchor>a24</anchor>
      <arglist>(const pvector&lt; _Tp, _PAlloc &gt; &amp;x)</arglist>
    </member>
    <member kind="function">
      <type>pvector&lt; _Tp, _PAlloc &gt; &amp;</type>
      <name>operator+=</name>
      <anchorfile>classutilib_1_1pvector.html</anchorfile>
      <anchor>a25</anchor>
      <arglist>(const _Tp &amp;z)</arglist>
    </member>
    <member kind="function">
      <type>pvector&lt; _Tp, _PAlloc &gt; &amp;</type>
      <name>operator-=</name>
      <anchorfile>classutilib_1_1pvector.html</anchorfile>
      <anchor>a26</anchor>
      <arglist>(const _Tp &amp;z)</arglist>
    </member>
    <member kind="function">
      <type>pvector&lt; _Tp, _PAlloc &gt; &amp;</type>
      <name>operator *=</name>
      <anchorfile>classutilib_1_1pvector.html</anchorfile>
      <anchor>a27</anchor>
      <arglist>(const _Tp &amp;z)</arglist>
    </member>
    <member kind="function">
      <type>pvector&lt; _Tp, _PAlloc &gt; &amp;</type>
      <name>operator/=</name>
      <anchorfile>classutilib_1_1pvector.html</anchorfile>
      <anchor>a28</anchor>
      <arglist>(const _Tp &amp;z)</arglist>
    </member>
    <member kind="typedef" protection="private">
      <type>std::vector&lt; _Tp, _PAlloc &gt;</type>
      <name>base_t</name>
      <anchorfile>classutilib_1_1pvector.html</anchorfile>
      <anchor>y0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::QueueArray</name>
    <filename>classutilib_1_1QueueArray.html</filename>
    <templarg>T</templarg>
    <member kind="function">
      <type></type>
      <name>QueueArray</name>
      <anchorfile>classutilib_1_1QueueArray.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~QueueArray</name>
      <anchorfile>classutilib_1_1QueueArray.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual T &amp;</type>
      <name>top</name>
      <anchorfile>classutilib_1_1QueueArray.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>add</name>
      <anchorfile>classutilib_1_1QueueArray.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(const T &amp;)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>remove</name>
      <anchorfile>classutilib_1_1QueueArray.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(T &amp;val)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>empty</name>
      <anchorfile>classutilib_1_1QueueArray.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>operator int</name>
      <anchorfile>classutilib_1_1QueueArray.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>size</name>
      <anchorfile>classutilib_1_1QueueArray.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>clear</name>
      <anchorfile>classutilib_1_1QueueArray.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1QueueArray.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>(ostream &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1QueueArray.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>(istream &amp;is)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1QueueArray.html</anchorfile>
      <anchor>a11</anchor>
      <arglist>(PackBuffer &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1QueueArray.html</anchorfile>
      <anchor>a12</anchor>
      <arglist>(UnPackBuffer &amp;is)</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>size_type</type>
      <name>Len</name>
      <anchorfile>classutilib_1_1QueueArray.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>size_type</type>
      <name>First</name>
      <anchorfile>classutilib_1_1QueueArray.html</anchorfile>
      <anchor>p1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>size_type</type>
      <name>Last</name>
      <anchorfile>classutilib_1_1QueueArray.html</anchorfile>
      <anchor>p2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>BasicArray&lt; T &gt;</type>
      <name>Data</name>
      <anchorfile>classutilib_1_1QueueArray.html</anchorfile>
      <anchor>p3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>BasicArray&lt; size_t &gt;</type>
      <name>next</name>
      <anchorfile>classutilib_1_1QueueArray.html</anchorfile>
      <anchor>p4</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::QueueList</name>
    <filename>classutilib_1_1QueueList.html</filename>
    <templarg>T</templarg>
    <base>utilib::LinkedList&lt; T &gt;</base>
    <member kind="function">
      <type></type>
      <name>QueueList</name>
      <anchorfile>classutilib_1_1QueueList.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::Simple2DArray</name>
    <filename>classutilib_1_1Simple2DArray.html</filename>
    <templarg>T</templarg>
    <base>utilib::Basic2DArray</base>
    <member kind="typedef">
      <type>Basic2DArray&lt; T &gt;</type>
      <name>base_t</name>
      <anchorfile>classutilib_1_1Simple2DArray.html</anchorfile>
      <anchor>w0</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Simple2DArray</name>
      <anchorfile>classutilib_1_1Simple2DArray.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Simple2DArray</name>
      <anchorfile>classutilib_1_1Simple2DArray.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(const BasicArray&lt; T &gt; &amp;array, const size_type nrows=1, const EnumDataOwned own=DataNotOwned)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Simple2DArray</name>
      <anchorfile>classutilib_1_1Simple2DArray.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const size_type nrows, const size_type ncols, T *d=((T *) 0), const EnumDataOwned own=DataNotOwned)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Simple2DArray</name>
      <anchorfile>classutilib_1_1Simple2DArray.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(const size_type nrows, const size_type ncols, const BasicArray&lt; T &gt; &amp;array, const EnumDataOwned own=DataNotOwned)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Simple2DArray</name>
      <anchorfile>classutilib_1_1Simple2DArray.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(const Simple2DArray&lt; T &gt; &amp;array)</arglist>
    </member>
    <member kind="function">
      <type>Simple2DArray&lt; T &gt; &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1Simple2DArray.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(const Simple2DArray&lt; T &gt; &amp;array)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>classutilib_1_1Simple2DArray.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(const Simple2DArray&lt; T &gt; &amp;array) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator!=</name>
      <anchorfile>classutilib_1_1Simple2DArray.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>(const Simple2DArray&lt; T &gt; &amp;array) const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>compare</name>
      <anchorfile>classutilib_1_1Simple2DArray.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>(const Simple2DArray&lt; T &gt; &amp;array) const </arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual int</type>
      <name>write</name>
      <anchorfile>classutilib_1_1Simple2DArray.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>(ostream &amp;output) const </arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual int</type>
      <name>read</name>
      <anchorfile>classutilib_1_1Simple2DArray.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>(istream &amp;input)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual int</type>
      <name>write</name>
      <anchorfile>classutilib_1_1Simple2DArray.html</anchorfile>
      <anchor>a11</anchor>
      <arglist>(PackBuffer &amp;output) const </arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual int</type>
      <name>read</name>
      <anchorfile>classutilib_1_1Simple2DArray.html</anchorfile>
      <anchor>a12</anchor>
      <arglist>(UnPackBuffer &amp;input)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::SimpleHashTableItem</name>
    <filename>classutilib_1_1SimpleHashTableItem.html</filename>
    <templarg>T</templarg>
    <templarg>InfoT</templarg>
    <member kind="typedef">
      <type>size_t</type>
      <name>size_type</name>
      <anchorfile>classutilib_1_1SimpleHashTableItem.html</anchorfile>
      <anchor>w0</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>SimpleHashTableItem&lt; T, InfoT &gt;</type>
      <name>hash_item</name>
      <anchorfile>classutilib_1_1SimpleHashTableItem.html</anchorfile>
      <anchor>w1</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>ListItem&lt; hash_item * &gt; *</type>
      <name>hash_element</name>
      <anchorfile>classutilib_1_1SimpleHashTableItem.html</anchorfile>
      <anchor>w2</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1SimpleHashTableItem.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(ostream &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1SimpleHashTableItem.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(PackBuffer &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1SimpleHashTableItem.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(istream &amp;is)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1SimpleHashTableItem.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(UnPackBuffer &amp;is)</arglist>
    </member>
    <member kind="function">
      <type>T &amp;</type>
      <name>key</name>
      <anchorfile>classutilib_1_1SimpleHashTableItem.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const T &amp;</type>
      <name>key</name>
      <anchorfile>classutilib_1_1SimpleHashTableItem.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>InfoT &amp;</type>
      <name>info</name>
      <anchorfile>classutilib_1_1SimpleHashTableItem.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const InfoT &amp;</type>
      <name>info</name>
      <anchorfile>classutilib_1_1SimpleHashTableItem.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>compare</name>
      <anchorfile>classutilib_1_1SimpleHashTableItem.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>(const T &amp;key) const </arglist>
    </member>
    <member kind="variable">
      <type>InfoT</type>
      <name>Info</name>
      <anchorfile>classutilib_1_1SimpleHashTableItem.html</anchorfile>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>T</type>
      <name>Key</name>
      <anchorfile>classutilib_1_1SimpleHashTableItem.html</anchorfile>
      <anchor>o1</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" protection="protected">
      <type></type>
      <name>SimpleHashTableItem</name>
      <anchorfile>classutilib_1_1SimpleHashTableItem.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>(const T *Key_, const InfoT *info_)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type></type>
      <name>SimpleHashTableItem</name>
      <anchorfile>classutilib_1_1SimpleHashTableItem.html</anchorfile>
      <anchor>b1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="friend" protection="private">
      <type>friend class</type>
      <name>AbstractHashTable&lt; SimpleHashTableItem&lt; T, InfoT &gt;, T, InfoT &gt;</name>
      <anchorfile>classutilib_1_1SimpleHashTableItem.html</anchorfile>
      <anchor>n0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::SimpleHashTable</name>
    <filename>classutilib_1_1SimpleHashTable.html</filename>
    <templarg>T</templarg>
    <templarg>InfoT</templarg>
    <base>utilib::AbstractHashTable</base>
    <member kind="typedef">
      <type>SimpleHashTableItem&lt; T, InfoT &gt;</type>
      <name>item_type</name>
      <anchorfile>classutilib_1_1SimpleHashTable.html</anchorfile>
      <anchor>w0</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>AbstractHashTable&lt; item_type, T, InfoT &gt;::iterator</type>
      <name>iterator</name>
      <anchorfile>classutilib_1_1SimpleHashTable.html</anchorfile>
      <anchor>w1</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>AbstractHashTable&lt; item_type, T, InfoT &gt;::const_iterator</type>
      <name>const_iterator</name>
      <anchorfile>classutilib_1_1SimpleHashTable.html</anchorfile>
      <anchor>w2</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SimpleHashTable</name>
      <anchorfile>classutilib_1_1SimpleHashTable.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(const char *nameBuff=&quot;Unnamed&quot;)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SimpleHashTable</name>
      <anchorfile>classutilib_1_1SimpleHashTable.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(size_type init_size, const char *nameBuff=&quot;Unnamed&quot;)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::SimpleHeapItem</name>
    <filename>classutilib_1_1SimpleHeapItem.html</filename>
    <templarg>T</templarg>
    <member kind="function">
      <type></type>
      <name>SimpleHeapItem</name>
      <anchorfile>classutilib_1_1SimpleHeapItem.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(const T *Key_)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1SimpleHeapItem.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(ostream &amp;os)</arglist>
    </member>
    <member kind="function">
      <type>T &amp;</type>
      <name>key</name>
      <anchorfile>classutilib_1_1SimpleHeapItem.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>T</type>
      <name>Key</name>
      <anchorfile>classutilib_1_1SimpleHeapItem.html</anchorfile>
      <anchor>r0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>element</name>
      <anchorfile>classutilib_1_1SimpleHeapItem.html</anchorfile>
      <anchor>r1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>ctr</name>
      <anchorfile>classutilib_1_1SimpleHeapItem.html</anchorfile>
      <anchor>r2</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend" protection="private">
      <type>friend class</type>
      <name>SimpleHeapBase&lt; SimpleHeapItem&lt; T &gt; &gt;</name>
      <anchorfile>classutilib_1_1SimpleHeapItem.html</anchorfile>
      <anchor>n0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::SimpleHeapBase</name>
    <filename>classutilib_1_1SimpleHeapBase.html</filename>
    <templarg>ItemType</templarg>
    <member kind="function" protection="protected">
      <type>int &amp;</type>
      <name>element</name>
      <anchorfile>classutilib_1_1SimpleHeapBase.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>(ItemType *item)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>int &amp;</type>
      <name>ctr</name>
      <anchorfile>classutilib_1_1SimpleHeapBase.html</anchorfile>
      <anchor>b1</anchor>
      <arglist>(ItemType *item)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::SimpleHeap</name>
    <filename>classutilib_1_1SimpleHeap.html</filename>
    <templarg>T</templarg>
    <templarg>Compare</templarg>
    <base>utilib::AbstractHeap&lt; SimpleHeapItem&lt; T &gt;, T, Compare &gt;</base>
    <base>utilib::SimpleHeapBase</base>
    <member kind="typedef">
      <type>AbstractHeap&lt; SimpleHeapItem&lt; T &gt;, T, Compare &gt;::item_t</type>
      <name>item_t</name>
      <anchorfile>classutilib_1_1SimpleHeap.html</anchorfile>
      <anchor>w0</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SimpleHeap</name>
      <anchorfile>classutilib_1_1SimpleHeap.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(const char *nameBuff=&quot;Unnamed&quot;)</arglist>
    </member>
    <member kind="function">
      <type>bool &amp;</type>
      <name>duplicate</name>
      <anchorfile>classutilib_1_1SimpleHeap.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool &amp;</type>
      <name>check_duplicates</name>
      <anchorfile>classutilib_1_1SimpleHeap.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>int &amp;</type>
      <name>element</name>
      <anchorfile>classutilib_1_1SimpleHeap.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>(SimpleHeapItem&lt; T &gt; *item)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>SimpleHeapItem&lt; T &gt; *</type>
      <name>insert</name>
      <anchorfile>classutilib_1_1SimpleHeap.html</anchorfile>
      <anchor>b1</anchor>
      <arglist>(T *key)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>extract</name>
      <anchorfile>classutilib_1_1SimpleHeap.html</anchorfile>
      <anchor>b2</anchor>
      <arglist>(SimpleHeapItem&lt; T &gt; *item, bool &amp;status)</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>duplicate_flag</name>
      <anchorfile>classutilib_1_1SimpleHeap.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>check_duplicates_flag</name>
      <anchorfile>classutilib_1_1SimpleHeap.html</anchorfile>
      <anchor>p1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::SimpleSplayTreeItem</name>
    <filename>classutilib_1_1SimpleSplayTreeItem.html</filename>
    <templarg>T</templarg>
    <member kind="function">
      <type>int</type>
      <name>Size</name>
      <anchorfile>classutilib_1_1SimpleSplayTreeItem.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1SimpleSplayTreeItem.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(ostream &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>counter</name>
      <anchorfile>classutilib_1_1SimpleSplayTreeItem.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>T &amp;</type>
      <name>key</name>
      <anchorfile>classutilib_1_1SimpleSplayTreeItem.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const T &amp;</type>
      <name>key</name>
      <anchorfile>classutilib_1_1SimpleSplayTreeItem.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>compare</name>
      <anchorfile>classutilib_1_1SimpleSplayTreeItem.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(const T &amp;key) const </arglist>
    </member>
    <member kind="function">
      <type>SimpleSplayTreeItem&lt; T &gt; *</type>
      <name>clone</name>
      <anchorfile>classutilib_1_1SimpleSplayTreeItem.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private">
      <type></type>
      <name>SimpleSplayTreeItem</name>
      <anchorfile>classutilib_1_1SimpleSplayTreeItem.html</anchorfile>
      <anchor>d0</anchor>
      <arglist>(const T *Key_)</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>T</type>
      <name>Key</name>
      <anchorfile>classutilib_1_1SimpleSplayTreeItem.html</anchorfile>
      <anchor>r0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>ctr</name>
      <anchorfile>classutilib_1_1SimpleSplayTreeItem.html</anchorfile>
      <anchor>r1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>SimpleSplayTreeItem&lt; T &gt; *</type>
      <name>left</name>
      <anchorfile>classutilib_1_1SimpleSplayTreeItem.html</anchorfile>
      <anchor>r2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>SimpleSplayTreeItem&lt; T &gt; *</type>
      <name>right</name>
      <anchorfile>classutilib_1_1SimpleSplayTreeItem.html</anchorfile>
      <anchor>r3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>size</name>
      <anchorfile>classutilib_1_1SimpleSplayTreeItem.html</anchorfile>
      <anchor>r4</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend" protection="private">
      <type>friend class</type>
      <name>AbstractSplayTree&lt; SimpleSplayTreeItem&lt; T &gt;, T &gt;</name>
      <anchorfile>classutilib_1_1SimpleSplayTreeItem.html</anchorfile>
      <anchor>n0</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend" protection="private">
      <type>friend class</type>
      <name>SimpleSplayTree&lt; T &gt;</name>
      <anchorfile>classutilib_1_1SimpleSplayTreeItem.html</anchorfile>
      <anchor>n1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::SimpleSplayTree</name>
    <filename>classutilib_1_1SimpleSplayTree.html</filename>
    <templarg>T</templarg>
    <base>utilib::AbstractSplayTree</base>
    <member kind="typedef">
      <type>AbstractSplayTree&lt; SimpleSplayTreeItem&lt; T &gt;, T &gt;</type>
      <name>base_t</name>
      <anchorfile>classutilib_1_1SimpleSplayTree.html</anchorfile>
      <anchor>w0</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SimpleSplayTree</name>
      <anchorfile>classutilib_1_1SimpleSplayTree.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(const char *nameBuff=&quot;Unnamed&quot;)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SimpleSplayTree</name>
      <anchorfile>classutilib_1_1SimpleSplayTree.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(const SimpleSplayTree&lt; T &gt; &amp;tree)</arglist>
    </member>
    <member kind="function">
      <type>SimpleSplayTree&lt; T &gt; &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1SimpleSplayTree.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const SimpleSplayTree&lt; T &gt; &amp;tree)</arglist>
    </member>
    <member kind="function">
      <type>bool &amp;</type>
      <name>duplicate</name>
      <anchorfile>classutilib_1_1SimpleSplayTree.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool &amp;</type>
      <name>check_duplicates</name>
      <anchorfile>classutilib_1_1SimpleSplayTree.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>SimpleSplayTreeItem&lt; T &gt; *</type>
      <name>insert</name>
      <anchorfile>classutilib_1_1SimpleSplayTree.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>(T *key)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>extract</name>
      <anchorfile>classutilib_1_1SimpleSplayTree.html</anchorfile>
      <anchor>b1</anchor>
      <arglist>(SimpleSplayTreeItem&lt; T &gt; *item, bool &amp;status)</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>duplicate_flag</name>
      <anchorfile>classutilib_1_1SimpleSplayTree.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>check_duplicates_flag</name>
      <anchorfile>classutilib_1_1SimpleSplayTree.html</anchorfile>
      <anchor>p1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::SmartPtrInfoBase</name>
    <filename>classutilib_1_1SmartPtrInfoBase.html</filename>
    <templarg>Type</templarg>
    <templarg>cache_flag</templarg>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>deallocate_data</name>
      <anchorfile>classutilib_1_1SmartPtrInfoBase.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>(Type *data)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::SmartPtrInfo</name>
    <filename>classutilib_1_1SmartPtrInfo.html</filename>
    <templarg>Type</templarg>
    <base>utilib::SmartPtrInfoBase&lt; Type, is_cache_allocated&lt; Type &gt;::value &gt;</base>
    <base>utilib::CachedAllocatorObject&lt; SmartPtrInfo&lt; Type &gt; &gt;</base>
    <member kind="function">
      <type></type>
      <name>SmartPtrInfo</name>
      <anchorfile>classutilib_1_1SmartPtrInfo.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~SmartPtrInfo</name>
      <anchorfile>classutilib_1_1SmartPtrInfo.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>initialize</name>
      <anchorfile>classutilib_1_1SmartPtrInfo.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(Type *Data_, EnumDataOwned own_)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>allocate_exec</name>
      <anchorfile>classutilib_1_1SmartPtrInfo.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_data</name>
      <anchorfile>classutilib_1_1SmartPtrInfo.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(Type *Data_, EnumDataOwned own_)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>decrement</name>
      <anchorfile>classutilib_1_1SmartPtrInfo.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>deallocate_derived</name>
      <anchorfile>classutilib_1_1SmartPtrInfo.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>Type *</type>
      <name>Data</name>
      <anchorfile>classutilib_1_1SmartPtrInfo.html</anchorfile>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>EnumDataOwned</type>
      <name>own</name>
      <anchorfile>classutilib_1_1SmartPtrInfo.html</anchorfile>
      <anchor>o1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>nref</name>
      <anchorfile>classutilib_1_1SmartPtrInfo.html</anchorfile>
      <anchor>o2</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::SmartPtr</name>
    <filename>classutilib_1_1SmartPtr.html</filename>
    <templarg>Type</templarg>
    <member kind="function">
      <type></type>
      <name>SmartPtr</name>
      <anchorfile>classutilib_1_1SmartPtr.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SmartPtr</name>
      <anchorfile>classutilib_1_1SmartPtr.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(Type *data_, EnumDataOwned own_=AssumeOwnership)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SmartPtr</name>
      <anchorfile>classutilib_1_1SmartPtr.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const SmartPtr &amp;ptr)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~SmartPtr</name>
      <anchorfile>classutilib_1_1SmartPtr.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>reset</name>
      <anchorfile>classutilib_1_1SmartPtr.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>SmartPtr&lt; Type &gt; &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1SmartPtr.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(const SmartPtr&lt; Type &gt; &amp;ptr)</arglist>
    </member>
    <member kind="function">
      <type>SmartPtr&lt; Type &gt; &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1SmartPtr.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(Type *data)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>nref</name>
      <anchorfile>classutilib_1_1SmartPtr.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator!=</name>
      <anchorfile>classutilib_1_1SmartPtr.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>(const SmartPtr&lt; Type &gt; &amp;ptr)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>classutilib_1_1SmartPtr.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>(const SmartPtr&lt; Type &gt; &amp;ptr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_data</name>
      <anchorfile>classutilib_1_1SmartPtr.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>(Type *data_, EnumDataOwned own_=DataNotOwned)</arglist>
    </member>
    <member kind="function">
      <type>Type &amp;</type>
      <name>operator *</name>
      <anchorfile>classutilib_1_1SmartPtr.html</anchorfile>
      <anchor>a11</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const Type &amp;</type>
      <name>operator *</name>
      <anchorfile>classutilib_1_1SmartPtr.html</anchorfile>
      <anchor>a12</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>Type *</type>
      <name>operator-&gt;</name>
      <anchorfile>classutilib_1_1SmartPtr.html</anchorfile>
      <anchor>a13</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const Type *</type>
      <name>operator-&gt;</name>
      <anchorfile>classutilib_1_1SmartPtr.html</anchorfile>
      <anchor>a14</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>operator Type *</name>
      <anchorfile>classutilib_1_1SmartPtr.html</anchorfile>
      <anchor>a15</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>operator const Type *</name>
      <anchorfile>classutilib_1_1SmartPtr.html</anchorfile>
      <anchor>a16</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>Type *</type>
      <name>data</name>
      <anchorfile>classutilib_1_1SmartPtr.html</anchorfile>
      <anchor>a17</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const Type *</type>
      <name>data</name>
      <anchorfile>classutilib_1_1SmartPtr.html</anchorfile>
      <anchor>a18</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>SmartPtrInfo&lt; Type &gt; *</type>
      <name>info</name>
      <anchorfile>classutilib_1_1SmartPtr.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::SparseMatrix</name>
    <filename>classutilib_1_1SparseMatrix.html</filename>
    <templarg>T</templarg>
    <member kind="function">
      <type>int</type>
      <name>get_ncols</name>
      <anchorfile>classutilib_1_1SparseMatrix.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(void) const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>get_nrows</name>
      <anchorfile>classutilib_1_1SparseMatrix.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(void) const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>get_nnzero</name>
      <anchorfile>classutilib_1_1SparseMatrix.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(void) const </arglist>
    </member>
    <member kind="function">
      <type>NumArray&lt; int &gt; &amp;</type>
      <name>get_matbeg</name>
      <anchorfile>classutilib_1_1SparseMatrix.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>NumArray&lt; int &gt; &amp;</type>
      <name>get_matcnt</name>
      <anchorfile>classutilib_1_1SparseMatrix.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>NumArray&lt; int &gt; &amp;</type>
      <name>get_matind</name>
      <anchorfile>classutilib_1_1SparseMatrix.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>NumArray&lt; T &gt; &amp;</type>
      <name>get_matval</name>
      <anchorfile>classutilib_1_1SparseMatrix.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual T &amp;</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1SparseMatrix.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>(const int row, const int col)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual const T &amp;</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1SparseMatrix.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>(const int row, const int col) const =0</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual int</type>
      <name>pretty_print</name>
      <anchorfile>classutilib_1_1SparseMatrix.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>(ostream &amp;ostr) const </arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual int</type>
      <name>write</name>
      <anchorfile>classutilib_1_1SparseMatrix.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>(ostream &amp;ostr) const =0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual int</type>
      <name>read</name>
      <anchorfile>classutilib_1_1SparseMatrix.html</anchorfile>
      <anchor>a11</anchor>
      <arglist>(istream &amp;istr)=0</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>write</name>
      <anchorfile>classutilib_1_1SparseMatrix.html</anchorfile>
      <anchor>a12</anchor>
      <arglist>(PackBuffer &amp;ostr) const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>read</name>
      <anchorfile>classutilib_1_1SparseMatrix.html</anchorfile>
      <anchor>a13</anchor>
      <arglist>(UnPackBuffer &amp;istr)</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>delete_row</name>
      <anchorfile>classutilib_1_1SparseMatrix.html</anchorfile>
      <anchor>a14</anchor>
      <arglist>(const int i)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>delete_col</name>
      <anchorfile>classutilib_1_1SparseMatrix.html</anchorfile>
      <anchor>a15</anchor>
      <arglist>(const int i)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>delete_element</name>
      <anchorfile>classutilib_1_1SparseMatrix.html</anchorfile>
      <anchor>a16</anchor>
      <arglist>(const int row, const int col)=0</arglist>
    </member>
    <member kind="function" protection="protected">
      <type></type>
      <name>SparseMatrix</name>
      <anchorfile>classutilib_1_1SparseMatrix.html</anchorfile>
      <anchor>b0</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual</type>
      <name>~SparseMatrix</name>
      <anchorfile>classutilib_1_1SparseMatrix.html</anchorfile>
      <anchor>b1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="pure">
      <type>virtual void</type>
      <name>initialize</name>
      <anchorfile>classutilib_1_1SparseMatrix.html</anchorfile>
      <anchor>b2</anchor>
      <arglist>(int nrow, int ncol, int nnzero)=0</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>setup</name>
      <anchorfile>classutilib_1_1SparseMatrix.html</anchorfile>
      <anchor>b3</anchor>
      <arglist>(int ncol, int nrow, int nnzero, int flag)</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>int</type>
      <name>ncols</name>
      <anchorfile>classutilib_1_1SparseMatrix.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>int</type>
      <name>nrows</name>
      <anchorfile>classutilib_1_1SparseMatrix.html</anchorfile>
      <anchor>p1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>int</type>
      <name>nnzeros</name>
      <anchorfile>classutilib_1_1SparseMatrix.html</anchorfile>
      <anchor>p2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>NumArray&lt; int &gt;</type>
      <name>matbeg</name>
      <anchorfile>classutilib_1_1SparseMatrix.html</anchorfile>
      <anchor>p3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>NumArray&lt; int &gt;</type>
      <name>matcnt</name>
      <anchorfile>classutilib_1_1SparseMatrix.html</anchorfile>
      <anchor>p4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>NumArray&lt; int &gt;</type>
      <name>matind</name>
      <anchorfile>classutilib_1_1SparseMatrix.html</anchorfile>
      <anchor>p5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>NumArray&lt; T &gt;</type>
      <name>matval</name>
      <anchorfile>classutilib_1_1SparseMatrix.html</anchorfile>
      <anchor>p6</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::CMSparseMatrix</name>
    <filename>classutilib_1_1CMSparseMatrix.html</filename>
    <templarg>T</templarg>
    <base>utilib::SparseMatrix</base>
    <member kind="typedef">
      <type>SparseMatrix&lt; T &gt;</type>
      <name>base_t</name>
      <anchorfile>classutilib_1_1CMSparseMatrix.html</anchorfile>
      <anchor>w0</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>CMSparseMatrix</name>
      <anchorfile>classutilib_1_1CMSparseMatrix.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>CMSparseMatrix</name>
      <anchorfile>classutilib_1_1CMSparseMatrix.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(int nrow, int ncol, int nnzero)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>CMSparseMatrix</name>
      <anchorfile>classutilib_1_1CMSparseMatrix.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(CMSparseMatrix&lt; Ereal&lt; T &gt; &gt; &amp;mat)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~CMSparseMatrix</name>
      <anchorfile>classutilib_1_1CMSparseMatrix.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>initialize</name>
      <anchorfile>classutilib_1_1CMSparseMatrix.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(int nrow, int ncol, int nnzero)</arglist>
    </member>
    <member kind="function">
      <type>T &amp;</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1CMSparseMatrix.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(const int row, const int col)</arglist>
    </member>
    <member kind="function">
      <type>const T &amp;</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1CMSparseMatrix.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(const int row, const int col) const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>adjoinColumn</name>
      <anchorfile>classutilib_1_1CMSparseMatrix.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>(int count, int *rowPosition, T *value)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>write</name>
      <anchorfile>classutilib_1_1CMSparseMatrix.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>(ostream &amp;ostr) const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>read</name>
      <anchorfile>classutilib_1_1CMSparseMatrix.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>(istream &amp;istr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>convert</name>
      <anchorfile>classutilib_1_1CMSparseMatrix.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>(const RMSparseMatrix&lt; T &gt; &amp;rowmajor)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>delete_row</name>
      <anchorfile>classutilib_1_1CMSparseMatrix.html</anchorfile>
      <anchor>a11</anchor>
      <arglist>(const int i)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>delete_col</name>
      <anchorfile>classutilib_1_1CMSparseMatrix.html</anchorfile>
      <anchor>a12</anchor>
      <arglist>(const int i)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>delete_element</name>
      <anchorfile>classutilib_1_1CMSparseMatrix.html</anchorfile>
      <anchor>a13</anchor>
      <arglist>(const int row, const int col)</arglist>
    </member>
    <member kind="friend">
      <type>friend class</type>
      <name>RMSparseMatrix</name>
      <anchorfile>classutilib_1_1CMSparseMatrix.html</anchorfile>
      <anchor>n0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::RMSparseMatrix</name>
    <filename>classutilib_1_1RMSparseMatrix.html</filename>
    <templarg>T</templarg>
    <base>utilib::SparseMatrix</base>
    <member kind="typedef">
      <type>SparseMatrix&lt; T &gt;</type>
      <name>base_t</name>
      <anchorfile>classutilib_1_1RMSparseMatrix.html</anchorfile>
      <anchor>w0</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>RMSparseMatrix</name>
      <anchorfile>classutilib_1_1RMSparseMatrix.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>RMSparseMatrix</name>
      <anchorfile>classutilib_1_1RMSparseMatrix.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(int nrow, int ncol, int nnzero)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~RMSparseMatrix</name>
      <anchorfile>classutilib_1_1RMSparseMatrix.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>initialize</name>
      <anchorfile>classutilib_1_1RMSparseMatrix.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(int nrow, int ncol, int nnzero)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>resize</name>
      <anchorfile>classutilib_1_1RMSparseMatrix.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(const int rowndx, const int rowlen)</arglist>
    </member>
    <member kind="function">
      <type>T &amp;</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1RMSparseMatrix.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(const int row, const int col)</arglist>
    </member>
    <member kind="function">
      <type>const T &amp;</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1RMSparseMatrix.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(const int row, const int col) const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>adjoinRow</name>
      <anchorfile>classutilib_1_1RMSparseMatrix.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>(int count, int *colPosition, T *value)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>write</name>
      <anchorfile>classutilib_1_1RMSparseMatrix.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>(ostream &amp;ostr) const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>read</name>
      <anchorfile>classutilib_1_1RMSparseMatrix.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>(istream &amp;istr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>convert</name>
      <anchorfile>classutilib_1_1RMSparseMatrix.html</anchorfile>
      <anchor>a10</anchor>
      <arglist>(const CMSparseMatrix&lt; T &gt; &amp;colmajor)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>delete_row</name>
      <anchorfile>classutilib_1_1RMSparseMatrix.html</anchorfile>
      <anchor>a11</anchor>
      <arglist>(const int i)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>delete_col</name>
      <anchorfile>classutilib_1_1RMSparseMatrix.html</anchorfile>
      <anchor>a12</anchor>
      <arglist>(const int i)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>delete_element</name>
      <anchorfile>classutilib_1_1RMSparseMatrix.html</anchorfile>
      <anchor>a13</anchor>
      <arglist>(const int row, const int col)</arglist>
    </member>
    <member kind="friend">
      <type>friend class</type>
      <name>CMSparseMatrix</name>
      <anchorfile>classutilib_1_1RMSparseMatrix.html</anchorfile>
      <anchor>n0</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend">
      <type>friend void</type>
      <name>utilib::product</name>
      <anchorfile>classutilib_1_1RMSparseMatrix.html</anchorfile>
      <anchor>n1</anchor>
      <arglist>(NumArray&lt; T &gt; &amp;res, const RMSparseMatrix&lt; T &gt; &amp;mat, const NumArray&lt; T &gt; &amp;vec)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::StackArray</name>
    <filename>classutilib_1_1StackArray.html</filename>
    <templarg>T</templarg>
    <member kind="function">
      <type></type>
      <name>StackArray</name>
      <anchorfile>classutilib_1_1StackArray.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>StackArray</name>
      <anchorfile>classutilib_1_1StackArray.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(BasicArray&lt; T &gt; &amp;data)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~StackArray</name>
      <anchorfile>classutilib_1_1StackArray.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual T &amp;</type>
      <name>top</name>
      <anchorfile>classutilib_1_1StackArray.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>add</name>
      <anchorfile>classutilib_1_1StackArray.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(const T &amp;)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>remove</name>
      <anchorfile>classutilib_1_1StackArray.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(T &amp;)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>empty</name>
      <anchorfile>classutilib_1_1StackArray.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>operator int</name>
      <anchorfile>classutilib_1_1StackArray.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>size</name>
      <anchorfile>classutilib_1_1StackArray.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>BasicArray&lt; T &gt; &amp;</type>
      <name>get_data</name>
      <anchorfile>classutilib_1_1StackArray.html</anchorfile>
      <anchor>a9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>size_type</type>
      <name>Len</name>
      <anchorfile>classutilib_1_1StackArray.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>BasicArray&lt; T &gt;</type>
      <name>Data</name>
      <anchorfile>classutilib_1_1StackArray.html</anchorfile>
      <anchor>p1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::StackList</name>
    <filename>classutilib_1_1StackList.html</filename>
    <templarg>T</templarg>
    <base>utilib::LinkedList&lt; T &gt;</base>
    <member kind="function">
      <type></type>
      <name>StackList</name>
      <anchorfile>classutilib_1_1StackList.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::Tuple1</name>
    <filename>classutilib_1_1Tuple1.html</filename>
    <templarg>T1</templarg>
    <member kind="function">
      <type></type>
      <name>Tuple1</name>
      <anchorfile>classutilib_1_1Tuple1.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(T1 _val1)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>classutilib_1_1Tuple1.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(const Tuple1&lt; T1 &gt; &amp;tuple) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;</name>
      <anchorfile>classutilib_1_1Tuple1.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const Tuple1&lt; T1 &gt; &amp;tuple) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1Tuple1.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(ostream &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1Tuple1.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(PackBuffer &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1Tuple1.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(istream &amp;)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1Tuple1.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(UnPackBuffer &amp;is)</arglist>
    </member>
    <member kind="variable">
      <type>T1</type>
      <name>val1</name>
      <anchorfile>classutilib_1_1Tuple1.html</anchorfile>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>unsigned int</type>
      <name>num_elements</name>
      <anchorfile>classutilib_1_1Tuple1.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend">
      <type>friend class</type>
      <name>BasicArray&lt; Tuple1&lt; T1 &gt; &gt;</name>
      <anchorfile>classutilib_1_1Tuple1.html</anchorfile>
      <anchor>n0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::Tuple2</name>
    <filename>classutilib_1_1Tuple2.html</filename>
    <templarg>T1</templarg>
    <templarg>T2</templarg>
    <base>utilib::Tuple1</base>
    <member kind="typedef">
      <type>Tuple1&lt; T1 &gt;</type>
      <name>base_t</name>
      <anchorfile>classutilib_1_1Tuple2.html</anchorfile>
      <anchor>w0</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Tuple2</name>
      <anchorfile>classutilib_1_1Tuple2.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(T1 _val1, T2 _val2)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>classutilib_1_1Tuple2.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(const Tuple2&lt; T1, T2 &gt; &amp;tuple) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;</name>
      <anchorfile>classutilib_1_1Tuple2.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const Tuple2&lt; T1, T2 &gt; &amp;tuple) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1Tuple2.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(ostream &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1Tuple2.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(PackBuffer &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1Tuple2.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(istream &amp;)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1Tuple2.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(UnPackBuffer &amp;is)</arglist>
    </member>
    <member kind="variable">
      <type>T2</type>
      <name>val2</name>
      <anchorfile>classutilib_1_1Tuple2.html</anchorfile>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend">
      <type>friend class</type>
      <name>BasicArray&lt; Tuple2&lt; T1, T2 &gt; &gt;</name>
      <anchorfile>classutilib_1_1Tuple2.html</anchorfile>
      <anchor>n0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::Tuple3</name>
    <filename>classutilib_1_1Tuple3.html</filename>
    <templarg>T1</templarg>
    <templarg>T2</templarg>
    <templarg>T3</templarg>
    <base>utilib::Tuple2</base>
    <member kind="typedef">
      <type>Tuple2&lt; T1, T2 &gt;</type>
      <name>base_t</name>
      <anchorfile>classutilib_1_1Tuple3.html</anchorfile>
      <anchor>w0</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Tuple3</name>
      <anchorfile>classutilib_1_1Tuple3.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(T1 _val1, T2 _val2, T3 _val3)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>classutilib_1_1Tuple3.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(const Tuple3&lt; T1, T2, T3 &gt; &amp;tuple) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;</name>
      <anchorfile>classutilib_1_1Tuple3.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const Tuple3&lt; T1, T2, T3 &gt; &amp;tuple) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1Tuple3.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(ostream &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1Tuple3.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(PackBuffer &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1Tuple3.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(istream &amp;)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1Tuple3.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(UnPackBuffer &amp;is)</arglist>
    </member>
    <member kind="variable">
      <type>T3</type>
      <name>val3</name>
      <anchorfile>classutilib_1_1Tuple3.html</anchorfile>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend">
      <type>friend class</type>
      <name>BasicArray&lt; Tuple3&lt; T1, T2, T3 &gt; &gt;</name>
      <anchorfile>classutilib_1_1Tuple3.html</anchorfile>
      <anchor>n0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::Tuple4</name>
    <filename>classutilib_1_1Tuple4.html</filename>
    <templarg>T1</templarg>
    <templarg>T2</templarg>
    <templarg>T3</templarg>
    <templarg>T4</templarg>
    <base>utilib::Tuple3</base>
    <member kind="typedef">
      <type>Tuple3&lt; T1, T2, T3 &gt;</type>
      <name>base_t</name>
      <anchorfile>classutilib_1_1Tuple4.html</anchorfile>
      <anchor>w0</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Tuple4</name>
      <anchorfile>classutilib_1_1Tuple4.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(T1 _val1, T2 _val2, T3 _val3, T4 _val4)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>classutilib_1_1Tuple4.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(const Tuple4&lt; T1, T2, T3, T4 &gt; &amp;tuple) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;</name>
      <anchorfile>classutilib_1_1Tuple4.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const Tuple4&lt; T1, T2, T3, T4 &gt; &amp;tuple) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1Tuple4.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(ostream &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1Tuple4.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(PackBuffer &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1Tuple4.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(istream &amp;)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1Tuple4.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(UnPackBuffer &amp;is)</arglist>
    </member>
    <member kind="variable">
      <type>T4</type>
      <name>val4</name>
      <anchorfile>classutilib_1_1Tuple4.html</anchorfile>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend">
      <type>friend class</type>
      <name>BasicArray&lt; Tuple4&lt; T1, T2, T3, T4 &gt; &gt;</name>
      <anchorfile>classutilib_1_1Tuple4.html</anchorfile>
      <anchor>n0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::Tuple5</name>
    <filename>classutilib_1_1Tuple5.html</filename>
    <templarg>T1</templarg>
    <templarg>T2</templarg>
    <templarg>T3</templarg>
    <templarg>T4</templarg>
    <templarg>T5</templarg>
    <base>utilib::Tuple4</base>
    <member kind="typedef">
      <type>Tuple4&lt; T1, T2, T3, T4 &gt;</type>
      <name>base_t</name>
      <anchorfile>classutilib_1_1Tuple5.html</anchorfile>
      <anchor>w0</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Tuple5</name>
      <anchorfile>classutilib_1_1Tuple5.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(T1 _val1, T2 _val2, T3 _val3, T4 _val4, T5 _val5)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>classutilib_1_1Tuple5.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(const Tuple5&lt; T1, T2, T3, T4, T5 &gt; &amp;tuple) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;</name>
      <anchorfile>classutilib_1_1Tuple5.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const Tuple5&lt; T1, T2, T3, T4, T5 &gt; &amp;tuple) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1Tuple5.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(ostream &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1Tuple5.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(PackBuffer &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1Tuple5.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(istream &amp;)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1Tuple5.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(UnPackBuffer &amp;is)</arglist>
    </member>
    <member kind="variable">
      <type>T5</type>
      <name>val5</name>
      <anchorfile>classutilib_1_1Tuple5.html</anchorfile>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend">
      <type>friend class</type>
      <name>BasicArray&lt; Tuple5&lt; T1, T2, T3, T4, T5 &gt; &gt;</name>
      <anchorfile>classutilib_1_1Tuple5.html</anchorfile>
      <anchor>n0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::Tuple6</name>
    <filename>classutilib_1_1Tuple6.html</filename>
    <templarg>T1</templarg>
    <templarg>T2</templarg>
    <templarg>T3</templarg>
    <templarg>T4</templarg>
    <templarg>T5</templarg>
    <templarg>T6</templarg>
    <base>utilib::Tuple5</base>
    <member kind="typedef">
      <type>Tuple5&lt; T1, T2, T3, T4, T5 &gt;</type>
      <name>base_t</name>
      <anchorfile>classutilib_1_1Tuple6.html</anchorfile>
      <anchor>w0</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Tuple6</name>
      <anchorfile>classutilib_1_1Tuple6.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(T1 _val1, T2 _val2, T3 _val3, T4 _val4, T5 _val5, T6 _val6)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>classutilib_1_1Tuple6.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(const Tuple6&lt; T1, T2, T3, T4, T5, T6 &gt; &amp;tuple) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;</name>
      <anchorfile>classutilib_1_1Tuple6.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const Tuple6&lt; T1, T2, T3, T4, T5, T6 &gt; &amp;tuple) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1Tuple6.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(ostream &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1Tuple6.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(PackBuffer &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1Tuple6.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(istream &amp;)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1Tuple6.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(UnPackBuffer &amp;is)</arglist>
    </member>
    <member kind="variable">
      <type>T6</type>
      <name>val6</name>
      <anchorfile>classutilib_1_1Tuple6.html</anchorfile>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend">
      <type>friend class</type>
      <name>BasicArray&lt; Tuple6&lt; T1, T2, T3, T4, T5, T6 &gt; &gt;</name>
      <anchorfile>classutilib_1_1Tuple6.html</anchorfile>
      <anchor>n0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::Tuple7</name>
    <filename>classutilib_1_1Tuple7.html</filename>
    <templarg>T1</templarg>
    <templarg>T2</templarg>
    <templarg>T3</templarg>
    <templarg>T4</templarg>
    <templarg>T5</templarg>
    <templarg>T6</templarg>
    <templarg>T7</templarg>
    <base>utilib::Tuple6</base>
    <member kind="typedef">
      <type>Tuple6&lt; T1, T2, T3, T4, T5, T6 &gt;</type>
      <name>base_t</name>
      <anchorfile>classutilib_1_1Tuple7.html</anchorfile>
      <anchor>w0</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Tuple7</name>
      <anchorfile>classutilib_1_1Tuple7.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(T1 _val1, T2 _val2, T3 _val3, T4 _val4, T5 _val5, T6 _val6, T7 _val7)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>classutilib_1_1Tuple7.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(const Tuple7&lt; T1, T2, T3, T4, T5, T6, T7 &gt; &amp;tuple) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;</name>
      <anchorfile>classutilib_1_1Tuple7.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const Tuple7&lt; T1, T2, T3, T4, T5, T6, T7 &gt; &amp;tuple) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1Tuple7.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(ostream &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classutilib_1_1Tuple7.html</anchorfile>
      <anchor>a4</anchor>
      <arglist>(PackBuffer &amp;os) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1Tuple7.html</anchorfile>
      <anchor>a5</anchor>
      <arglist>(istream &amp;)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read</name>
      <anchorfile>classutilib_1_1Tuple7.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(UnPackBuffer &amp;is)</arglist>
    </member>
    <member kind="variable">
      <type>T7</type>
      <name>val7</name>
      <anchorfile>classutilib_1_1Tuple7.html</anchorfile>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend">
      <type>friend class</type>
      <name>BasicArray&lt; Tuple7&lt; T1, T2, T3, T4, T5, T6, T7 &gt; &gt;</name>
      <anchorfile>classutilib_1_1Tuple7.html</anchorfile>
      <anchor>n0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::TwoBitArray</name>
    <filename>classutilib_1_1TwoBitArray.html</filename>
    <base>utilib::BitArrayBase&lt; 1, int, TwoBitArray &gt;</base>
    <member kind="function">
      <type></type>
      <name>TwoBitArray</name>
      <anchorfile>classutilib_1_1TwoBitArray.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>TwoBitArray</name>
      <anchorfile>classutilib_1_1TwoBitArray.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(const size_type len, bitword *d=0, const EnumDataOwned o=DataNotOwned)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>TwoBitArray</name>
      <anchorfile>classutilib_1_1TwoBitArray.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const TwoBitArray &amp;array)</arglist>
    </member>
    <member kind="function">
      <type>TwoBitArray &amp;</type>
      <name>operator=</name>
      <anchorfile>classutilib_1_1TwoBitArray.html</anchorfile>
      <anchor>a3</anchor>
      <arglist>(const TwoBitArray &amp;array)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>utilib::Comparison</name>
    <filename>structutilib_1_1Comparison.html</filename>
  </compound>
  <compound kind="struct">
    <name>utilib::ComparisonBase</name>
    <filename>structutilib_1_1ComparisonBase.html</filename>
    <templarg>Type</templarg>
    <member kind="typedef">
      <type>Type</type>
      <name>argument_type</name>
      <anchorfile>structutilib_1_1ComparisonBase.html</anchorfile>
      <anchor>w0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>utilib::SimpleCompare</name>
    <filename>structutilib_1_1SimpleCompare.html</filename>
    <templarg>Type</templarg>
    <base>utilib::ComparisonBase</base>
    <base>utilib::Comparison</base>
    <member kind="function">
      <type>compare_type</type>
      <name>operator()</name>
      <anchorfile>structutilib_1_1SimpleCompare.html</anchorfile>
      <anchor>a0</anchor>
      <arglist>(const Type &amp;_arg1, const Type &amp;_arg2) const </arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>utilib::Reverse</name>
    <filename>classutilib_1_1Reverse.html</filename>
    <templarg>ComparisonType</templarg>
    <base>utilib::ComparisonBase&lt; ComparisonType::argument_type &gt;</base>
    <base>utilib::Comparison</base>
    <member kind="function">
      <type></type>
      <name>Reverse</name>
      <anchorfile>classutilib_1_1Reverse.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>(const ComparisonType &amp;_compare)</arglist>
    </member>
    <member kind="function">
      <type>compare_type</type>
      <name>operator()</name>
      <anchorfile>classutilib_1_1Reverse.html</anchorfile>
      <anchor>a2</anchor>
      <arglist>(const typename ComparisonType::argument_type &amp;_arg1, const typename ComparisonType::argument_type &amp;_arg2) const </arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>ComparisonType</type>
      <name>compare</name>
      <anchorfile>classutilib_1_1Reverse.html</anchorfile>
      <anchor>p0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>utilib::exception_mngr</name>
    <filename>namespaceutilib_1_1exception__mngr.html</filename>
    <member kind="typedef">
      <type>void(*</type>
      <name>null_fn_type</name>
      <anchorfile>namespaceutilib_1_1exception__mngr.html</anchorfile>
      <anchor>a1</anchor>
      <arglist>)(void)</arglist>
    </member>
    <member kind="enumeration">
      <name>handle_t</name>
      <anchor>a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Standard</name>
      <anchor>a9a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Abort</name>
      <anchor>a9a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Exit</name>
      <anchor>a9a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_mode</name>
      <anchorfile>namespaceutilib_1_1exception__mngr.html</anchorfile>
      <anchor>a6</anchor>
      <arglist>(handle_t)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_exit_function</name>
      <anchorfile>namespaceutilib_1_1exception__mngr.html</anchorfile>
      <anchor>a7</anchor>
      <arglist>(null_fn_type fn)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>handle_exception</name>
      <anchorfile>namespaceutilib_1_1exception__mngr.html</anchorfile>
      <anchor>a8</anchor>
      <arglist>(ostringstream &amp;msg)</arglist>
    </member>
    <member kind="variable">
      <type>handle_t</type>
      <name>mode</name>
      <anchorfile>namespaceutilib_1_1exception__mngr.html</anchorfile>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>null_fn_type</type>
      <name>exit_function</name>
      <anchorfile>namespaceutilib_1_1exception__mngr.html</anchorfile>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
  </compound>
</tagfile>
