# ELF-read
- simple `readelf` inspired tool
- map sections to segments


# Example usage
```
$ ./elf-read ./elf-read
Segment	 Type	 Perms	 Sections
00	 PHDR           R--
01	 INTERP         R-- .interp
02	 LOAD           R-- .note.gnu.build-id .interp .gnu.hash .dynsym .dynstr .gnu.version .gnu.version_r .rela.dyn .rela.plt
03	 LOAD           R-X .init .plt .text .fini
04	 LOAD           R-- .rodata .eh_frame_hdr .eh_frame .note.gnu.property .note.ABI-tag
05	 LOAD           RW- .init_array .fini_array .dynamic .got .got.plt .data .bss
06	 DYNAMIC        RW- .dynamic
07	 NOTE           R-- .note.gnu.build-id
08	 NOTE           R-- .note.gnu.property
09	 NOTE           R-- .note.ABI-tag
10	 GNU_PROPERTY   R-- .note.gnu.property
11	 GNU_EH_FRAME   R-- .eh_frame_hdr
12	 GNU_STACK      RW-
13	 GNU_RELRO      R-- .init_array .fini_array .dynamic .got
```

