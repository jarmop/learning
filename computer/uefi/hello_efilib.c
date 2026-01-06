#include <efi.h>
#include <efilib.h>

EFI_STATUS
EFIAPI
efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    InitializeLib(ImageHandle, SystemTable);

    SystemTable->ConOut->OutputString(
        SystemTable->ConOut,
        L"Hello from UEFI!\r\n"
    );

    // Hang forever (like your jmp hang)
    while (1);

    return EFI_SUCCESS;
}
