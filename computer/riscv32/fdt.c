#include <stdint.h>
#include "uart.h"
#include "fdt.h"

#define FDT_MAGIC 0xd00dfeedu

/**
 * All the header fields are 32-bit integers, stored in big-endian format.
 * @param magic 0xd00dfeed
 * @param totalsize the total size in bytes of the devicetree data structure. Includes 
 * the possible free space gaps between the blocks or after the final block.
 * @param off_dt_struct the offset in bytes of the structure block from the beginning 
 * of the header.
 * @param off_dt_strings the offset in bytes of the structure block from the beginning 
 * of the header.
 * @param off_mem_rsvmap the offset in bytes of the Memory reservation block from the 
 * beginning of the header.
 * @param version The version of the devicetree data structure. The version is 17 if 
 * using the structure as defined in this document.
 * @param last_comp_version The lowest version of the devicetree data structure with 
 * which the version used is backwards compatible. So, for the structure as defined in 
 * this document (version 17), this field shall contain 16 because version 17 is 
 * backwards compatible with version 16.
 * @param boot_cpuid_phys The physical ID of the system’s boot CPU. Identical to the 
 * physical ID given in the reg property of that CPU node within the devicetree.
 * @param size_dt_strings The length in bytes of the strings block section of the 
 * devicetree blob.
 * @param size_dt_struct The length in bytes of the structure block section of the 
 * devicetree blob
 */
struct fdt_header {
    uint32_t magic; // 0xd00dfeed
    uint32_t totalsize; // includes any possible free space
    // Offset (in bytes) for the structure block from the beginning of the header section
    uint32_t off_dt_struct;
    uint32_t off_dt_strings;
    uint32_t off_mem_rsvmap;
    uint32_t version;
    uint32_t last_comp_version; // the oldest compatible version
    uint32_t boot_cpuid_phys; // the physical ID of the boot CPU
    uint32_t size_dt_strings; // Size (in bytes) of the strings block
    uint32_t size_dt_struct;
};

/**
 * The memory reservation block consists of a list of pairs of 64-bit big-endian 
 * integers, each pair being represented by the following C structure. On 32-bit CPUs 
 * the upper 32-bits of the value are ignored. 
 * 
 * The memory reservation block is located at an 8-byte aligned offset from the 
 * beginning of the devicetree blob
 */
struct fdt_mem_res {
    uint32_t address;
    uint32_t size;
};

/**
 * The structure block describes the structure and contents of the devicetree itself. It 
 * is composed of a sequence of tokens with data organized into a linear tree structure.
 * 
 * Located at a 4-byte aligned offset from the beginning of the 
 * devicetree blob.
 * 
 * Composed of a sequence of pieces, each beginning with a token (a big-endian 32-bit 
 * integer). Some tokens are followed by extra data, the format of which is determined 
 * by the token value. All tokens are aligned on a 32-bit boundary, which may require 
 * padding bytes (with a value of 0x0) to be inserted after the previous token’s data. 
 * The five token types are:
 *
 * FDT_BEGIN_NODE (0x00000001)
 * FDT_END_NODE (0x00000002)
 * FDT_PROP (0x00000003)
 * FDT_NOP (0x00000004)
 * FDT_END (0x00000009)
 * 
 * The tree structure
 * - (Any number of FDT_NOP tokens)
 * - FDT_BEGIN_NODE
 *   - The node’s name as a null-terminated string
 *   - [zeroed padding bytes to align to a 4-byte boundary]
 * - For each property of the node:
 *   - (optionally) any number of FDT_NOP tokens
 *   - FDT_PROP token
 *     - property information
 *     - [zeroed padding bytes to align to a 4-byte boundary]
 * - Representations of all child nodes in this format
 * - (optionally) any number of FDT_NOP tokens
 * - FDT_END_NODE token 
 */

// Followed by the node's unit name, and maybe by the unit address. After possible 
// padding zeroes, followed by any token except FDT_END
#define FDT_BEGIN_NODE  0x1u
// Followed by any token except FDT_PROP
#define FDT_END_NODE    0x2u
// Followed by the fdt_prop struct, which is followed by the value string of len size 
#define FDT_PROP        0x3u
#define FDT_NOP         0x4u
#define FDT_END         0x9u

struct fdt_prop {
    uint32_t len; // The total length of the property (may be 0)
    // an offset to the property’s string block (starts with the name)
    uint32_t nameoff; 
};

// Flip 32-bit big-endian into little-endian
static uint32_t le32(const void *p) {
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

void fdt_dump(const void *dtb) {
    const struct fdt_header *header = (const struct fdt_header *)dtb;

    // Check that the magic is correct
    if (le32(&header->magic) != FDT_MAGIC) {
        uart_puts("Invalid DTB magic: ");
        uart_puthex32(le32(&header->magic));
        uart_puts("\n");
        return;
    }

    uart_puts("DTB Header:\n");
    uart_puts("Version: ");
    uart_putdec32(le32(&header->version));
    uart_puts("\n");

    uint32_t off_struct  = le32(&header->off_dt_struct);
    uint32_t off_strings = le32(&header->off_dt_strings);

    const uint8_t *structp = (const uint8_t *)dtb + off_struct;
    const char    *strings = (const char *)dtb + off_strings;

    // uart_puthex32(header->off_dt_struct);
    // uart_puts("\n");
    // uart_puthex32(off_struct);
    // uart_puts("\n");
    // uart_puthex32(header->off_dt_strings);
    // uart_puts("\n");
    // uart_puthex32(off_strings);
    // uart_puts("\n");
    uart_puts("Strings: "); uart_puts(strings); uart_puts("\n");

    int in_virtio_mmio_node = 0;


    for (;;) {
        uint32_t token = le32(structp);
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
            uint32_t len     = le32(structp); structp += 4;
            uint32_t nameoff = le32(structp); structp += 4;
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
                    uint32_t a_hi = le32(val + 0);
                    uint32_t a_lo = le32(val + 4);
                    uint32_t s_hi = le32(val + 8);
                    uint32_t s_lo = le32(val + 12);

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