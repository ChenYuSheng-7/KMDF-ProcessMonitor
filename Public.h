/*++

Module Name:

    public.h

Abstract:

    This module contains the common declarations shared by driver
    and user applications.

Environment:

    user and kernel

--*/

//
// Define an Interface Guid so that apps can find the device and talk to it.
//

DEFINE_GUID (GUID_DEVINTERFACE_ProcessMonitoring,
    0xdd15d3ea,0x4d95,0x43f6,0x84,0x23,0x90,0x4d,0xda,0x55,0x4c,0xff);
// {dd15d3ea-4d95-43f6-8423-904dda554cff}
