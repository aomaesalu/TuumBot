// (C) 2005-2006 James R. Bruce, Carnegie Mellon University
// Licenced under the GNU General Public License (GPL) version 2,
//   or alternately by a specific written agreement.

#ifndef _INCLUDED_IO_FILE_H_
#define _INCLUDED_IO_FILE_H_

#include <stdio.h>
#include <string.h>
#include <zlib.h>

class IOFile{
public:
  IOFile() {}
  virtual ~IOFile() {}

  virtual bool open(const char *filename,const char *mode) = 0;
  virtual bool close() = 0;
  virtual bool valid() = 0;

  virtual int read(void *buf,int len) = 0;
  virtual int write(const void *buf,int len) = 0;

  virtual void flush() {}

  virtual bool gets(char *str,int len) = 0;
  int puts(const char *str)
    {return(write(str,strlen(str)));}
};

class NormalFile : public IOFile {
  FILE *f;
public:
  NormalFile() {f=NULL;}
  ~NormalFile() {close();}

  bool open(const char *filename,const char *mode);
  bool close();
  bool valid()
    {return(f != NULL);}

  int read(void *buf,int len)
    {return(fread(buf,1,len,f));}
  int write(const void *buf,int len)
    {return(fwrite(buf,1,len,f));}

  int printf(char *fmt, ...);

  void flush()
    {fflush(f);}
  FILE *getFile()
    {return(f);}

  bool gets(char *str,int len)
    {return(fgets(str,len,f) != NULL);}
};

class GZipFile : public IOFile {
  gzFile f;
public:
  GZipFile() {f=NULL;}
  ~GZipFile() {close();}

  bool open(const char *filename,const char *mode);
  bool close();
  bool valid()
    {return(f != NULL);}

  int read(void *buf,int len)
    {return(gzread(f,buf,len));}
  int write(const void *buf,int len)
    {return(gzwrite(f,buf,len));}

  bool gets(char *str,int len)
    {return(gzgets(f,str,len) != NULL);}
};


//====================================================================//
//  Direct whole-file IO for fixed-size objects
//====================================================================//

int ReadBlob(void *data,int size,const char *filename);
int WriteBlob(const void *data,int size,const char *filename);

template <class data>
bool ReadFile(data &item,const char *filename)
{
  int s = sizeof(item);
  return(ReadBlob(&item,s,filename) == s);
}

template <class data>
int ReadFile(data *item,int num,const char *filename)
{
  int s = sizeof(item);
  return(ReadBlob(item,s*num,filename)/s);
}

template <class data>
bool WriteFile(const data &item,const char *filename)
{
  int s = sizeof(item);
  return(WriteBlob(&item,s,filename) == s);
}

template <class data>
int WriteFile(const data *item,int num,const char *filename)
{
  int s = sizeof(item);
  return(WriteBlob(item,s*num,filename)/s);
}

#endif
