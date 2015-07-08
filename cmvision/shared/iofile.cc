// (C) 2005-2006 James R. Bruce, Carnegie Mellon University
// Licenced under the GNU General Public License (GPL) version 2,
//   or alternately by a specific written agreement.

#include <stdarg.h>
#include <stdio.h>

#include "iofile.h"


bool NormalFile::open(const char *filename,const char *mode)
{
  close();
  f = fopen(filename,mode);
  return(f != NULL);
}

bool NormalFile::close()
{
  if(!f) return(true);
  bool ok = fclose(f);
  f = NULL;
  return(ok);
}

int NormalFile::printf(char *fmt, ...)
{
  va_list al;
  va_start(al,fmt);
  int ret = vfprintf(f, fmt, al);
  va_end(al);

  return(ret);
}

//====================================================================//

bool GZipFile::open(const char *filename,const char *mode)
{
  close();
  f = gzopen(filename,mode);
  // if(f) gzsetparams(f,Z_BEST_COMPRESSION,Z_DEFAULT_STRATEGY);
  return(f != NULL);
}

bool GZipFile::close()
{
  if(!f) return(true);
  bool ok = gzclose(f);
  f = NULL;
  return(ok);
}

//====================================================================//

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int ReadBlob(void *data,int size,const char *filename)
{
  int fd = ::open(filename,O_RDONLY);
  if(fd < 0) return(-1);
  int n = ::read(fd,data,size);
  ::close(fd);
  return(n);
}

int WriteBlob(const void *data,int size,const char *filename)
{
  int fd = ::open(filename,O_CREAT|O_WRONLY|O_TRUNC,0644);
  if(fd < 0) return(-1);
  int n = ::write(fd,data,size);
  if(::close(fd) < 0) return(-1);
  return(n);
}
