/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

namespace BinaryData
{

//================== FaustReverb.dsp ==================
static const unsigned char temp_binary_data_0[] =
"import(\"stdfaust.lib\");\n"
"\n"
"process = _ <: dm.zita_rev1;\n";

const char* FaustReverb_dsp = (const char*) temp_binary_data_0;


const char* getNamedResource (const char*, int&) throw();
const char* getNamedResource (const char* resourceNameUTF8, int& numBytes) throw()
{
    unsigned int hash = 0;
    if (resourceNameUTF8 != 0)
        while (*resourceNameUTF8 != 0)
            hash = 31 * hash + (unsigned int) *resourceNameUTF8++;

    switch (hash)
    {
        case 0x5c98f70f:  numBytes = 54; return FaustReverb_dsp;
        default: break;
    }

    numBytes = 0;
    return 0;
}

const char* namedResourceList[] =
{
    "FaustReverb_dsp"
};

}
