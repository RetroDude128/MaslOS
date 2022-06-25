#include "taskbar.h"
#include "../../OSDATA/osdata.h"

namespace Taskbar
{
    Framebuffer* taskbarBuffer;
    kernelFiles::ImageFile* MButton;

    void InitTaskbar(kernelFiles::ImageFile* mButton)
    {
        MButton = mButton;
        taskbarBuffer = osData.windowPointerThing->taskbar;
    }

    void RenderTaskbar()
    {
        {
            uint32_t* endAddr = (uint32_t*)((uint64_t)taskbarBuffer->BaseAddress + taskbarBuffer->BufferSize);
            uint32_t col = osData.windowPointerThing->defaultTaskbarBackgroundColor;
            for (uint32_t* addr = (uint32_t*)taskbarBuffer->BaseAddress; addr < endAddr; addr++)
                *addr = col;
        }

        GlobDrawImage(MButton, 0, 0, 1, 1, taskbarBuffer);
        //GlobDrawImage(currentMouseImage, 10, 10, 1, 1, taskbarBuffer);





        int64_t ypos = osData.windowPointerThing->virtualScreenBuffer->Height - taskbarBuffer->Height;
        osData.windowPointerThing->UpdatePointerRect(0, ypos, osData.windowPointerThing->virtualScreenBuffer->Width - 1, ypos + taskbarBuffer->Height - 1);
    }



    
}