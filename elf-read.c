/*
 * ian-proj1.c
 * author: Lukáš Navrátil - xnavral00
 */

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>

#include <elf.h>
#include <gelf.h>
#include <libelf.h>

const char *segment_types_names[] = {
    "NULL", "LOAD", "DYNAMIC", "INTERP", "NOTE", "SHLIB", "PHDR", "TLS", "NUM",
};

const char *segment_gnu_names[] = {
    "GNU_EH_FRAME", "GNU_STACK", "GNU_RELRO", "GNU_PROPERTY", "GNU_SFRAME",
};

const char *get_segment_name(unsigned int type_num) {
  if (type_num < sizeof(segment_types_names) / sizeof(*segment_types_names)) {
    return segment_types_names[type_num];

    // gnu segments
  } else if (PT_GNU_EH_FRAME <= type_num && type_num <= PT_GNU_SFRAME) {
    return segment_gnu_names[type_num - PT_GNU_EH_FRAME];
  }
  return NULL;
}

bool is_sec_in_seg(GElf_Phdr *phdr, GElf_Shdr *sec) {
  return sec->sh_addr >= phdr->p_vaddr &&
         sec->sh_addr + sec->sh_size <= phdr->p_vaddr + phdr->p_memsz;
}

void print_segment_entry(Elf *elf, size_t scnhdr_i, GElf_Phdr *seg,
                         unsigned long seg_id) {
  char flags[4];

  flags[0] = (seg->p_flags & PF_R) ? 'R' : '-';
  flags[1] = (seg->p_flags & PF_W) ? 'W' : '-';
  flags[2] = (seg->p_flags & PF_X) ? 'X' : '-';
  flags[3] = '\0';

  printf("%02lu\t %-14s %s ", seg_id, get_segment_name(seg->p_type), flags);

  Elf_Scn *scn = NULL;
  while ((scn = elf_nextscn(elf, scn)) != NULL) {

    Elf64_Shdr sec;
    gelf_getshdr(scn, &sec);
    char *sec_name = elf_strptr(elf, scnhdr_i, sec.sh_name);

    if (is_sec_in_seg(seg, &sec) && (sec.sh_flags & SHF_ALLOC) != 0) {
      printf("%s ", sec_name);
    }
  }

  puts("");
}

bool process_elf(Elf *elf) {

  int ret_val;
  size_t phdr_cnt;
  ret_val = elf_getphdrnum(elf, &phdr_cnt);
  if (ret_val != 0) {
    fprintf(stderr, "%s\n", elf_errmsg(-1));
    return false;
  }

  size_t scnhdr_i;
  ret_val = elf_getshdrstrndx(elf, &scnhdr_i);
  if (ret_val != 0) {
    fprintf(stderr, "%s\n", elf_errmsg(-1));
    return false;
  }

  puts("\
Segment\t Type\t Perms\t Sections\
");

  for (size_t phdr_id = 0; phdr_id < phdr_cnt; phdr_id++) {

    GElf_Phdr phdr;
    gelf_getphdr(elf, phdr_id, &phdr);

    print_segment_entry(elf, scnhdr_i, &phdr, phdr_id);
  }

  return true;
}

int main(int argc, const char *argv[]) {

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <file>\n", argv[0]);
    return EXIT_FAILURE;
  }

  int elf_fd = open(argv[1], O_RDONLY);
  if (elf_fd == -1) {
    perror(argv[1]);
    return EXIT_FAILURE;
  }

  // set current standard before any calls to libelf
  elf_version(EV_CURRENT);

  Elf *loaded_file = elf_begin(elf_fd, ELF_C_READ, NULL);
  if (!loaded_file) {
    // print most recent error
    fprintf(stderr, "%s: %s\n", argv[1], elf_errmsg(-1));

    close(elf_fd);
    return EXIT_FAILURE;
  }

  int file_class = gelf_getclass(loaded_file);
  if (file_class == ELFCLASSNONE) {
    fprintf(stderr, "%s: is not an elf\n", argv[1]);

    elf_end(loaded_file);
    close(elf_fd);
    return EXIT_FAILURE;
  }

  process_elf(loaded_file);

  elf_end(loaded_file);
  close(elf_fd);

  return EXIT_SUCCESS;
}
