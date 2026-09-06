/*
 * Direct C references to original firmware routines.
 *
 * Function addresses are supplied as absolute symbols in the linker script (e.g. sceOpen = 0x1001b854).
 */

#ifndef PFS_FW_REFS_H
#define PFS_FW_REFS_H

typedef struct {
  unsigned int mode;
  unsigned int attr;
  unsigned int size;
  unsigned char ctime[8];
  unsigned char atime[8];
  unsigned char mtime[8];
  unsigned int hisize;
  unsigned int private[6];
} sce_stat_t;

typedef struct {
  sce_stat_t stat;
  char name[256];
  void *private;
} sce_dirent_t;

extern int sceOpen(const char *path, unsigned short flags, unsigned short mode);
extern int sceWrite(int fd, const void *buf, unsigned int len);
extern int sceClose(int fd);
extern int sceLseek(int fd, int offset, int whence);
extern long sceLseek64(int fd, long offset, int whence);
extern int sceDopen(const char *path);
extern int sceMkdir(const char *path, unsigned short mode);
extern int sceDclose(int res);
extern int sceRemove(const char *path);
extern int sceRename(const char *src, const char *dst);
extern int sceGetstat(const char *path, sce_stat_t *st);
extern int sceSync(const char *path, unsigned short mode);
extern int sceMount(char *path, char *devname, int flag, void *arg, int arglen);
extern int sceDevctl(char *name, int cmd, void *arg, unsigned int arglen, void *bufp, unsigned int buflen);
extern int sceDread(int fd, sce_dirent_t *dirent);
extern int sceRead(int fd, void *buf, int count);
extern int sceIoctl(int fd, int cmd, void *param);
extern int sceIoctl2(int fd, int cmd, void *arg, unsigned int arglen, void *bufp, unsigned int buflen);
extern int sceChdir(const char *name);
extern int sceChstat(const char *name, sce_stat_t *buf, unsigned int cbit);
extern int sceFormat(const char *devname, char *blockdevname, void *arg, int arglen);
extern int sceReadlink(const char *path, char *buf, unsigned int bufsize);
extern int sceRmdir(const char *name);
extern int sceSymlink(const char *oldname, const char *newname);
extern int sceUmount(const char *name, int flag);

#endif
