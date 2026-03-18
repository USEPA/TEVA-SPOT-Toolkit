import sys
import zipfile
from os.path import join
import os

if len(sys.argv) < 3:
   print "zip.py <zip-file> <file1> [<file2> ...]"
   sys.exit(0)

def zip(filename,fdlist):
    zf = zipfile.ZipFile(filename, 'w')
    for file in fdlist:
        if os.path.isdir(file):
            for root, dirs, files in os.walk(file):
                for fname in files:
                    if fname.endswith('exe'):
                        zf.external_attr = (0777 << 16L) | (010 << 28L)
                    else:
                        zf.external_attr = (0660 << 16L) | (010 << 28L)
                    zf.write(join(root,fname))
        else:
            if file.endswith('exe'):
            	zf.external_attr = (0777 << 16L) | (010 << 28L)
            else:
                zf.external_attr = (0660 << 16L) | (010 << 28L)
            zf.write(file)
    zf.close()


zip(sys.argv[1], sys.argv[2:])

