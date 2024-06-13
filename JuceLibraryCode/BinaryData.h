/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   backgroundWithModules_png;
    const int            backgroundWithModules_pngSize = 19513;

    extern const char*   BypassBackground_png;
    const int            BypassBackground_pngSize = 336;

    extern const char*   BypassSwitchOff_png;
    const int            BypassSwitchOff_pngSize = 789;

    extern const char*   BypassSwitchOn_png;
    const int            BypassSwitchOn_pngSize = 808;

    extern const char*   LinkOff_png;
    const int            LinkOff_pngSize = 846;

    extern const char*   LinkOn_png;
    const int            LinkOn_pngSize = 755;

    extern const char*   mbrBackground_png;
    const int            mbrBackground_pngSize = 6361;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 7;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
