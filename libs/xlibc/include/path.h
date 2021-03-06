#ifndef __XLIBC_PATH_H__
#define __XLIBC_PATH_H__

#ifdef __cplusplus
extern "C" {
#endif

static inline int is_absolute_path(const char * path)
{
	if(path && (*path == '/'))
		return 1;
	return 0;
}

static inline int is_relative_path(const char * path)
{
	if(path && (*path != '/'))
		return 1;
	return 0;
}

char * basename(char * path);
char * dirname(char * path);

#ifdef __cplusplus
}
#endif

#endif /* __XLIBC_PATH_H__ */
