#include <ntifs.h>

NTSTATUS DriverEntry() {
	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "Hello, World!\n");
	return STATUS_SUCCESS;
}