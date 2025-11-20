#define _GNU_SOURCE
#include <fcntl.h>
#include <linux/kvm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>

void setup_real_mode(int vcpu_fd) {
    struct kvm_sregs sregs;
    if (ioctl(vcpu_fd, KVM_GET_SREGS, &sregs) < 0) {
        perror("KVM_GET_SREGS");
        exit(1);
    }

    /* Real mode: all segments base = selector << 4 */
    sregs.cs.selector = 0;
    sregs.cs.base = 0;
    sregs.ds.selector = 0;
    sregs.ds.base = 0;
    sregs.es.selector = 0;
    sregs.es.base = 0;
    sregs.fs.selector = 0;
    sregs.fs.base = 0;
    sregs.gs.selector = 0;
    sregs.gs.base = 0;
    sregs.ss.selector = 0;
    sregs.ss.base = 0;

    /* Real mode CR0 */
    sregs.cr0 = 0x60000010;  // PE=0, MP=1, ET=1, NE=1

    if (ioctl(vcpu_fd, KVM_SET_SREGS, &sregs) < 0) {
        perror("KVM_SET_SREGS");
        exit(1);
    }
}

int main() {
    int kvm_fd = open("/dev/kvm", O_RDWR | O_CLOEXEC);
    if (kvm_fd < 0) { perror("open /dev/kvm"); exit(1); }

    // Ensure the kernel supports KVM API version
    int api_ver = ioctl(kvm_fd, KVM_GET_API_VERSION, 0);
    if (api_ver != 12) {
        fprintf(stderr, "KVM API version mismatch\n");
        exit(1);
    }

    // Create a virtual machine
    int vm_fd = ioctl(kvm_fd, KVM_CREATE_VM, 0);
    if (vm_fd < 0) { perror("KVM_CREATE_VM"); exit(1); }

    // Allocate one page of guest memory
    const size_t mem_size = 0x1000;
    void *mem = mmap(NULL, mem_size, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);

    // Place a tiny program in guest memory:
    //   0xF4 = HLT instruction
    //   0xEB 0xFE = JMP $ (infinite loop) — not reached here
    // unsigned char code[] = { 0xF4, 0xEB, 0xFE };

    unsigned char code[] = {
        0xF4  // HLT
    };
    memcpy(mem, code, sizeof(code));

    // Tell KVM to map the guest memory into the VM's physical address 0
    struct kvm_userspace_memory_region region = {
        .slot = 0,
        .guest_phys_addr = 0,
        .memory_size = mem_size,
        .userspace_addr = (uint64_t)mem
    };
    ioctl(vm_fd, KVM_SET_USER_MEMORY_REGION, &region);

    // Create a virtual CPU
    int vcpu_fd = ioctl(vm_fd, KVM_CREATE_VCPU, 0);
    if (vcpu_fd < 0) { perror("KVM_CREATE_VCPU"); exit(1); }

    // Map the vCPU's "run" structure
    int vcpu_mmap_size = ioctl(kvm_fd, KVM_GET_VCPU_MMAP_SIZE, 0);
    struct kvm_run *run = mmap(NULL, vcpu_mmap_size, PROT_READ | PROT_WRITE,
                               MAP_SHARED, vcpu_fd, 0);

    setup_real_mode(vcpu_fd);

    // Setup registers
    // struct kvm_regs regs = { .rip = 0x0, .rax = 2, .rbx = 3 };

    struct kvm_regs regs = { .rip = 0x0000 };
    ioctl(vcpu_fd, KVM_SET_REGS, &regs);

    printf("Entering guest...\n");

    // Run the virtual CPU
    while (1) {
        ioctl(vcpu_fd, KVM_RUN, 0);

        switch (run->exit_reason) {
        case KVM_EXIT_HLT:
            printf("KVM_EXIT_HLT — Guest executed HLT. Success.\n");
            return 0;

        case KVM_EXIT_IO:
            printf("KVM_EXIT_IO\n");
            break;

        case KVM_EXIT_FAIL_ENTRY:
            printf("KVM_EXIT_FAIL_ENTRY: hardware entry error\n");
            printf("failure reason = 0x%llx\n", run->fail_entry.hardware_entry_failure_reason);
            return 1;

        default:
            printf("Unhandled exit reason: %d\n", run->exit_reason);
            return 1;
        }
    }
}
