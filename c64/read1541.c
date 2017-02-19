#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_BLOCKS 683
#define BLK_SIZE 256
#define DIR_TRACK 18
#define TRACKS 35

char *load_disk(const char *filename) {
  int fd = open(filename, O_RDONLY);
  assert (fd > 0);

  char *m = mmap(0, MAX_BLOCKS * BLK_SIZE, PROT_READ, MAP_PRIVATE, fd, 0); 
  assert (m != (char*)-1);
  return m;
}

char *BLOCK(char *disk, int track, int sector) {
  //printf("Reading (%d,%d) ", track, sector);
  assert(track >= 1 && track <= TRACKS);
  assert(sector >= 0);
  int block;
  if (track >=1 && track <= 17) {
    assert(sector < 21);
    block = 21 * track - 21;
  } else if (track >= 18 && track <=24) {
    assert(sector < 19);
    block = 19 * track + 15;
  } else if (track >= 25 && track <=30) {
    assert(sector < 18);
    block = 18 * track + 40;
  } else if (track >= 31 && track <=35) {
    assert(sector < 17);
    block = 17 * track + 71;
  }
  char *r = &disk[(block + sector) * BLK_SIZE];
  //printf("(offset 0x%lx)\n", r - disk);
  return r;
}

// Returns null if there is none.
char *nextblock(char *disk, char *b) {
  int track = b[0]; 
  int sector = b[1]; 
  if (track == 0)
    return NULL;
  return BLOCK(disk, track, sector);
}

// Dumps a 256 byte sector
void hexdump(char *b) {
  for (int j=0; j<BLK_SIZE; j+=16) {
    printf("%02x: ", j);
    for (int i=0; i<16; i+=2) {
      unsigned char c1 = b[i+j];
      unsigned char c2 = b[i+j+1];
      printf("%02x%02x ", c1, c2);
    }
    printf(" ");
    for (int i=0; i<16; i+=1) {
      unsigned char c = b[i+j];
      if (isprint(c)) {
        printf("%c", c);
      } else {
        printf(".");
      }
    }
    printf("\n");
  }
}

typedef unsigned char uchar;

struct dir_entry {
  uchar next_dir_track;
  uchar next_dir_sector;
  uchar type;
  uchar track;
  uchar sector;
  char filename[16];
  uchar rel_side_sector_track;
  uchar rel_side_sector_sector;
  uchar rel_record_length;
  uchar geos[6];
  uchar file_size_low;
  uchar file_size_high;
};

// trims trailing 0xa0's, copies no more than sz (including null)
// into dst buffer.
char *trim(char *dst, char *src, int sz) {
  int pad = 1;
  dst[sz-1] = 0;

  for (int i = sz-2; i>=0; i--) {
    if (pad && (unsigned char)src[i] == 0xa0) {
      dst[i] = 0;
    } else {
      dst[i] = src[i];
      pad = 0;
    }
  }
  return dst;
}

char *filetype(uchar type) {
  switch (type & 7) {
    case 0: return "DEL";
    case 1: return "SEQ";
    case 2: return "PRG";
    case 3: return "USR";
    case 4: return "REL";
  }
  return "???";
}
char *filebits(uchar type) {
  // TODO - I don't know if the precidence is correct.
  if (type & 0x20) return "@";
  if (type & 0x40) return "<";
  if (!type & 0x80) return "*";
  return " ";
}

void lsdir(char *d, char *b) {
  struct dir_entry *e;
  char fn[19]; // Includes null & quotes
  fn[0]= '"';
  while (b) {
    for (int i=0; i<BLK_SIZE; i+=sizeof(*e)) {
      #pragma GCC diagnostic push
      #pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
      e = (struct direntry*)(&b[i]);
      #pragma GCC diagnostic pop
      if (e->type) {
        int size = e->file_size_low + 256*e->file_size_high;
        trim(&fn[1], e->filename, sizeof(fn)-2);
        strcat(fn, "\"");
        printf("%-4d %-18s %s%s (%d,%d)\n",
            size, fn, filetype(e->type), filebits(e->type), e->track, e->sector);
      }
    }
    b = nextblock(d, b);
  }
}

int compute_blocks_free(char *bam) {
  int count = 0;
  for (int track = 1; track <= TRACKS; track++) {
    if (track != DIR_TRACK) {  // directory track
      count+=bam[track*4];
    }
  }
  return count;
}

void read_file(char *d, int track, int sector) {
  char *b = BLOCK(d, track, sector);
  while (b) {
    if (b[0]) {
      write(1, &b[2], 254);
    } else {
      write(1, &b[2], (unsigned char)b[1]);
    }
    b = nextblock(d, b);    
  }
}

int main(int argc, char* argv[]) {
  const char filename[] = "1541-demo0.d64";

  char *d = load_disk(filename);

  if (argc == 3) {
    read_file(d, atoi(argv[1]), atoi(argv[2]));
    return 0;
  }

  char label[17];  // Includes null & quotes

  // Read Disk header
  char *b = BLOCK(d, DIR_TRACK, 0);

  // Volume label
  trim(&label[1], &b[0x90], sizeof(label)-2);
  label[0] = '"';
  strcat(label, "\"");
  printf("0 %s    %2.2s %2.2s\n", label, &b[0xa2], &b[0xa5]);
  // Directory should begin on 18,1
  assert(b[0] == DIR_TRACK);
  assert(b[1] == 1);
  assert(b[2] == 'A');
  //hexdump(b);

  //write(1, &b[0x90], 16);

  lsdir(d, nextblock(d, b));
  printf("%d BLOCKS FREE.\n", compute_blocks_free(&b[0]));

}
