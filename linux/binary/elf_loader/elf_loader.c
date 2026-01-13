#define _GNU_SOURCE
#include <elf.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define PAGE 4096
#define ALIGN_DOWN(x) ((x) & ~(PAGE - 1))
#define ALIGN_UP(x)   (((x) + PAGE - 1) & ~(PAGE - 1))

static void die(const char *msg)
{
    write(2, msg, strlen(msg));
    write(2, "\n", 1);
    _exit(1);
}

static void load_and_run(const char *path)
{
    int fd = open(path, O_RDONLY);
    if (fd < 0) die("open failed");

    Elf64_Ehdr eh;
    if (read(fd, &eh, sizeof(eh)) != sizeof(eh))
        die("read ehdr failed");

    if (memcmp(eh.e_ident, ELFMAG, SELFMAG))
        die("not ELF");

    Elf64_Phdr *phdrs = malloc(eh.e_phnum * sizeof(*phdrs));
    lseek(fd, eh.e_phoff, SEEK_SET);
    read(fd, phdrs, eh.e_phnum * sizeof(*phdrs));

    for (int i = 0; i < eh.e_phnum; i++) {
        Elf64_Phdr *ph = &phdrs[i];
        if (ph->p_type != PT_LOAD)
            continue;

        uintptr_t map_start = ALIGN_DOWN(ph->p_vaddr);
        uintptr_t map_end   = ALIGN_UP(ph->p_vaddr + ph->p_memsz);
        size_t map_size = map_end - map_start;

        void *addr = mmap(
            (void *)map_start,
            map_size,
            PROT_READ | PROT_WRITE,
            MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED,
            -1,
            0
        );

        if (addr == MAP_FAILED)
            die("mmap failed");

        lseek(fd, ph->p_offset, SEEK_SET);
        read(fd, (void *)ph->p_vaddr, ph->p_filesz);

        memset(
            (void *)(ph->p_vaddr + ph->p_filesz),
            0,
            ph->p_memsz - ph->p_filesz
        );

        int prot = 0;
        if (ph->p_flags & PF_R) prot |= PROT_READ;
        if (ph->p_flags & PF_W) prot |= PROT_WRITE;
        if (ph->p_flags & PF_X) prot |= PROT_EXEC;

        mprotect((void *)map_start, map_size, prot);
    }

    void (*entry)(void) = (void (*)(void))eh.e_entry;
    entry();

    _exit(0);
}

int main(int argc, char **argv)
{
    if (argc != 2)
        die("usage: elf_loader <elf>");

    pid_t pid = fork();
    if (pid == 0)
        load_and_run(argv[1]);

    waitpid(pid, NULL, 0);
    return 0;
}
