/* ---- Minimal FDT (Flattened Device Tree) parsing ---- */

#include <stdint.h>
#include "devicetree.h"
#include "uart.h"

#define FDT_MAGIC 0xd00dfeedu

#define FDT_BEGIN_NODE  0x1u
#define FDT_END_NODE    0x2u
#define FDT_PROP        0x3u
#define FDT_NOP         0x4u
#define FDT_END         0x9u

struct fdt_header {
    uint32_t magic;
    uint32_t totalsize;
    uint32_t off_dt_struct;
    uint32_t off_dt_strings;
    uint32_t off_mem_rsvmap;
    uint32_t version;
    uint32_t last_comp_version;
    uint32_t boot_cpuid_phys;
    uint32_t size_dt_strings;
    uint32_t size_dt_struct;
};

static uint32_t be32(const void *p) {
    const uint8_t *b = (const uint8_t *)p;
    return ((uint32_t)b[0] << 24) | ((uint32_t)b[1] << 16) | ((uint32_t)b[2] << 8) | (uint32_t)b[3];
}

static const char *strz(const char *s) { return s; } /* readability */

static int streq(const char *a, const char *b) {
    while (*a && *b) { if (*a++ != *b++) return 0; }
    return *a == 0 && *b == 0;
}

static int contains_compat(const char *compat_list, uint32_t len, const char *needle) {
    /* compat is a NUL-separated list of strings */
    uint32_t i = 0;
    while (i < len) {
        const char *s = compat_list + i;
        /* find end of this string */
        uint32_t j = i;
        while (j < len && compat_list[j] != '\0') j++;
        if (j < len && streq(s, needle)) return 1;
        i = j + 1;
    }
    return 0;
}

static uint32_t align4(uint32_t x) { return (x + 3u) & ~3u; }

void fdt_dump_virtio_mmio(const void *dtb) {
    const struct fdt_header *h = (const struct fdt_header *)dtb;

    if (be32(&h->magic) != FDT_MAGIC) {
        uart_puts("DTB: bad magic: ");
        uart_puthex32(be32(&h->magic));
        uart_puts("\n");
        return;
    }

    uint32_t off_struct  = be32(&h->off_dt_struct);
    uint32_t off_strings = be32(&h->off_dt_strings);

    const uint8_t *structp = (const uint8_t *)dtb + off_struct;
    const char    *strings = (const char *)dtb + off_strings;

    int in_virtio_mmio_node = 0;

    uart_puts("DTB OK. Scanning for virtio-mmio...\n");

    for (;;) {
        uint32_t token = be32(structp);
        structp += 4;

        if (token == FDT_BEGIN_NODE) {
            const char *name = (const char *)structp;
            /* skip node name (NUL-terminated), then align to 4 */
            uint32_t nlen = 0;
            while (name[nlen] != '\0') nlen++;
            structp += align4(nlen + 1);

            /* entering a node resets our “this node is virtio-mmio” flag */
            in_virtio_mmio_node = 0;

        } else if (token == FDT_END_NODE) {
            in_virtio_mmio_node = 0;

        } else if (token == FDT_PROP) {
            uint32_t len     = be32(structp); structp += 4;
            uint32_t nameoff = be32(structp); structp += 4;
            const char *pname = strings + nameoff;

            const uint8_t *val = structp;
            structp += align4(len);

            if (streq(pname, "compatible")) {
                if (contains_compat((const char *)val, len, "virtio,mmio")) {
                    in_virtio_mmio_node = 1;
                }
            }

            if (in_virtio_mmio_node && streq(pname, "reg")) {
                /* Common QEMU virt case: 2 cells addr + 2 cells size (64-bit each) */
                if (len >= 16) {
                    uint32_t a_hi = be32(val + 0);
                    uint32_t a_lo = be32(val + 4);
                    uint32_t s_hi = be32(val + 8);
                    uint32_t s_lo = be32(val + 12);

                    uart_puts("virtio-mmio reg addr=");
                    uart_puthex32(a_hi); uart_puts("_"); uart_puthex32(a_lo);
                    uart_puts(" size=");
                    uart_puthex32(s_hi); uart_puts("_"); uart_puthex32(s_lo);
                    uart_puts("\n");
                } else {
                    uart_puts("virtio-mmio reg: unexpected len\n");
                }
            }

        } else if (token == FDT_NOP) {
            /* ignore */
        } else if (token == FDT_END) {
            break;
        } else {
            uart_puts("DTB: unknown token ");
            uart_puthex32(token);
            uart_puts("\n");
            break;
        }
    }
}