#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

extern int debugMode;

void   fatalError(char *message, ... );
void   debugMsg(char *message, ... );
void   *safe_malloc(size_t size);
FILE   *safe_fopen(const char *pathname, const char *mode);
uint8  readUint8(FILE *f);
uint16 readUint16(FILE *f);
uint32 readUint32(FILE *f);
char   *getString(FILE *f, int maxlen);
uint8  *readUint8Block(FILE *f, int len);
uint16 *readUint16Block(FILE *f, int len);
uint16 peekUint16(uint8 *data, uint32 *offset);
void   peekUint16Block(uint8 *data, uint32 *offset, uint16 *dest, int len);
void   hexdump(uint8 *data, uint32 len);

