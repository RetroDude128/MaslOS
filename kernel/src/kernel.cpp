#include "kernelStuff/stuff/kernelUtil.h"


/*
if (osData.enableStackTrace)
    osData.stackPointer.add(MStack("_start", "kernel.cpp"));
if (osData.enableStackTrace)
    osData.stackPointer.removeLast();

*/

extern "C" void _start(BootInfo* bootInfo)
{  
    osData.booting = false;
    osData.maxNonFatalCrashCount = 3;
    MStackData::stackPointer = 0;
    for (int i = 0; i < 1000; i++)
        MStackData::stackArr[i] = MStack();
    
    osData.enableStackTrace = RECORD_STACK_TRACE;
    AddToStack();
    osData.crashCount = 0;
    //
    //while(true);

    KernelInfo kernelInfo = InitializeKernel(bootInfo);
    PageTableManager* pageTableManager = kernelInfo.pageTableManager;
    //AddToStack();
    osData.osTasks = List<Task*>(4);


    osData.kernelInfo = &kernelInfo;
    osData.exit = false;

    Window* debugTerminalWindow = osData.debugTerminalWindow;
    //Window* realMainWindow = osData.realMainWindow;
    //Window* realMainWindow2 = osData.realMainWindow2;
    
    osData.drawBackground = true;
    
    
    GlobalRenderer->Clear(Colors.black);
    GlobalRenderer->DrawImage(bootInfo->bootImage, 0, 0, 1, 1);
    osData.booting = true;

    {
        uint64_t endTime = PIT::TimeSinceBootMS() + 1000;
        while (PIT::TimeSinceBootMS() < endTime && osData.booting)
            asm("hlt");
        osData.booting = false;
    }
    
    GlobalRenderer->Clear(Colors.black);

//    while(true);

    //GlobalRenderer->Clear(Colors.black);
    
    // {
    //     int xdiff = 20;
    
    //     GlobalRenderer->Println("Filecount: {}\n", to_string(bootInfo->windowIconZIP->fileCount), Colors.yellow);
    //     int ydiff = GlobalRenderer->CursorPosition.y + 10;
    //     for (int i = 0; i < bootInfo->windowIconZIP->fileCount; i++)
    //     {
    //         kernelFiles::ImageFile* image = kernelFiles::ConvertFileToImage(&bootInfo->windowIconZIP->files[i]);
    //         GlobalRenderer->DrawImage(image, xdiff, ydiff, 1, 1);
    //         //GlobalRenderer->DrawImage(image, xdiff + image->xOff, ydiff + image->yOff + 20, 1, 1);
    //         xdiff += image->width + 20;
    //     }
    //     // {
    //     //     kernelFiles::ImageFile* image = kernelFiles::ConvertFileToImage(kernelFiles::ZIP::GetFileFromFileName(bootInfo->mouseZIP, "default.mbif"));
    //     //     if (image != NULL)
    //     //         GlobalRenderer->DrawImage(image, xdiff, ydiff, 2, 2);
    //     // }
    // }
    
    // while (true);

    // if (bootInfo->testImage != NULL)
    // {
    //     dispVar vars[] = {dispVar(bootInfo->testImage->width), dispVar(bootInfo->testImage->height)};
    //     GlobalRenderer->Println("Image: {0}x{1}", vars);
    // }
    // else
    //     GlobalRenderer->Println("No Image found!");

    //GlobalRenderer->DrawImage(bootInfo->testImage, 100, 10);
    

    //while(true);



    //CopyFrameBuffer(osData.realMainWindow->framebuffer, osData.realMainWindow2->framebuffer);

    Window* mainWindow;
    {
        mainWindow = (Window*)malloc(sizeof(Window), "Main window");
        TerminalInstance* terminal = (TerminalInstance*)malloc(sizeof(TerminalInstance), "Terminal Instance for main window");
        *terminal = TerminalInstance(&adminUser, mainWindow);
        *(mainWindow) = Window((DefaultInstance*)terminal, Size(600, 500), Position(5, 30), "Main Window", true, true, true);
        osData.windows.add(mainWindow);

        activeWindow = mainWindow;
        osData.mainTerminalWindow = mainWindow;
    }


    // {
    //     Window* window = (Window*)malloc(sizeof(Window), "Window");
    //     TerminalInstance* terminal = (TerminalInstance*)malloc(sizeof(TerminalInstance), "Terminal Instance");
    //     *terminal = TerminalInstance(&guestUser, window);
    //     *(window) = Window((DefaultInstance*)terminal, Size(400, 360), Position(700, 60), "Testing Window", true, true, true);
    //     osData.windows.add(window);
            
    //     window->renderer->Cls();  
    //     window->renderer->DrawImage(bootInfo->testImage, 150, 0, 2, 2);
    //     //osData.windows[1]->renderer->Println("Hello, world!");
    //     KeyboardPrintStart(window);
    // }
 
    NewTerminalInstance* newTerminaltest;
    {
        Window* window = (Window*)malloc(sizeof(Window), "Window");
        NewTerminalInstance* terminal = (NewTerminalInstance*)malloc(sizeof(NewTerminalInstance), "Terminal Instance");
        *terminal = NewTerminalInstance(&guestUser, window);
        *(window) = Window((DefaultInstance*)terminal, Size(400, 360), Position(500, 60), "Testing new Terminal", true, true, true);
        osData.windows.add(window);
            
        newTerminaltest = terminal;
        terminal->Clear();
        terminal->WriteText("This \\B1100FFis a \\FFF00FFtest!");
        terminal->WriteText("Oh ma go\nsh");
        //osData.windows[1]->renderer->Println("Hello, world!");
        //KeyboardPrintStart(window);
    }
    

    
    osData.windows.add(osData.debugTerminalWindow);


    debugTerminalWindow->Log("Kernel Initialised Successfully!");

    debugTerminalWindow->Log("<STAT>");
    debugTerminalWindow->Log("<STAT>");
    debugTerminalWindow->Log("<STAT>");
    debugTerminalWindow->Log("<STAT>");
    debugTerminalWindow->Log("<STAT>");
    debugTerminalWindow->Log("<STAT>");
    debugTerminalWindow->Log("<STAT>");

    activeWindow->renderer->Cls();
    KeyboardPrintStart(mainWindow);
    //mainWindow->Render();

    
    osData.windowPointerThing->Clear();
    osData.windowPointerThing->RenderWindows();
    bool updateBorder = true;
    bool bgm = osData.drawBackground;
    int frame = 0;
    int tFrame = 0;
    uint64_t oldTime = PIT::TimeSinceBootMS();
    //double fps = 1;
    uint64_t fps = 1;
    while(!osData.exit)
    {
        AddToStack();
        freeCount = 0;
        mallocCount = 0;


        if (++tFrame >= 1000)
            tFrame = 0;

        frame++;  
        if (frame >= 30)
        {
            uint64_t currTime = PIT::TimeSinceBootMS();
            //fps = ((frame - 1)*1000.0) / (currTime - oldTime);
            if ((currTime - oldTime) == 0)
            {
                fps = 9999;
            }
            else
                fps = ((frame - 1)*1000) / (currTime - oldTime);
            oldTime = currTime;
            frame = 0;
        }
        RemoveFromStack();

        ProcessMousePackets();

        AddToStack();
        {
            // newTerminaltest->scrollX = 60 - ((tFrame % 300) * 2);
            // newTerminaltest->scrollY = 50 - ((tFrame/3 % 250) * 1);
            // newTerminaltest->Render();
        }
        RemoveFromStack();

        if (bgm != osData.drawBackground)
        {
            osData.windowPointerThing->RenderWindows();
            bgm = osData.drawBackground;
        }

        // if (osData.drawBackground)
        //     realMainWindow->renderer->DrawImage(bootInfo->bgImage, 0, 0, 2, 2);
        // else
        //     realMainWindow->renderer->Clear(Colors.dblue);


        if (activeWindow != NULL)
        {
            updateBorder = true;
            if (activeWindow->moveToFront)
            {
                activeWindow->moveToFront = false;
                int index = osData.windows.getIndexOf(activeWindow);
                if (index == osData.windows.getCount() - 1)
                {
                    osData.windowPointerThing->UpdateWindowBorder(activeWindow);
                    osData.windowPointerThing->RenderWindow(activeWindow);
                }
                else if (index != -1)
                {
                    Window* oldActive = osData.windows[osData.windows.getCount() - 1];
                    osData.windows.removeAt(index);
                    osData.windows.add(activeWindow);
                    
                    osData.windowPointerThing->RenderWindow(activeWindow);
                    osData.windowPointerThing->UpdateWindowBorder(oldActive);
                }
            }
        }
        else
        {
            if (updateBorder)
            {
                updateBorder = false;
                {
                    Window* oldActive = osData.windows[osData.windows.getCount() - 1];
                    
                    osData.windowPointerThing->UpdateWindowBorder(oldActive);
                }
            }
        }

        AddToStack();
        for (int i = 0; i < osData.windows.getCount(); i++)
        {            
            Window* window = osData.windows[i];

            if (window == osData.debugTerminalWindow && !osData.showDebugterminal)
                continue;
            
            if (window->hidden != window->oldHidden)
            {
                window->oldHidden = window->hidden;
                osData.windowPointerThing->UpdatePointerRect(
                    window->position.x - 1, 
                    window->position.y - 23, 
                    window->position.x + window->size.width, 
                    window->position.y + window->size.height
                    );
            }

            {
                int x1 = window->position.x - 1;
                int y1 = window->position.y - 23;
                int sx1 = window->size.width + 3;
                int sy1 = window->size.height + 25;

                bool update = false;

                int x2 = x1;
                int y2 = y1;
                int sx2 = sx1;
                int sy2 = sy2;

                Size nSize = window->newSize;
                Position nPos = window->newPosition;


                if (window->size != nSize)
                {
                    window->Resize(nSize);
                    {
                        x2 = window->position.x - 1;
                        y2 = window->position.y - 23;
                        sx2 = window->size.width + 3;
                        sy2 = window->size.height + 25;

                        update = true;
                    }
                }

                if (window->position != nPos)
                {
                    window->position = nPos;

                    x2 = window->position.x - 1;
                    y2 = window->position.y - 23;
                    sx2 = window->size.width + 3;
                    sy2 = window->size.height + 25;

                    update = true;
                }

                if (update)
                {
                    int rx1 = min(x1, x2);
                    int ry1 = min(y1, y2);
                    int rx2 = max(x1 + sx1, x2 + sx2);
                    int ry2 = max(y1 + sy1, y2 + sy2);

                    int AR = (rx2 - rx1) * (ry2 - ry1);
                    int A1 = sx1 * sy1;
                    int A2 = sx2 * sy2;

                    if (AR <= A1+A2)
                    {
                        osData.windowPointerThing->UpdatePointerRect(rx1, ry1, rx2, ry2);
                    }
                    else
                    {
                        osData.windowPointerThing->UpdatePointerRect(x1, y1, x1 + sx1, y1 + sy1);
                        osData.windowPointerThing->UpdatePointerRect(x2, y2, x2 + sx2, y2 + sy2);
                    }
                }
            }
        }
        RemoveFromStack();

        AddToStack();
        Taskbar::RenderTaskbar();
        MPoint mPos = MousePosition;
        DrawMousePointer2(osData.windowPointerThing->virtualScreenBuffer, mPos);
        osData.windowPointerThing->fps = fps;
        osData.windowPointerThing->Render();
        osData.windowPointerThing->UpdatePointerRect(mPos.x - 32, mPos.y - 32, mPos.x + 32, mPos.y + 32);
        RemoveFromStack();

        AddToStack();
        //double endTime = PIT::TimeSinceBoot + 0.02;
        for (int ax = 0; ax < 10; ax++)
        {
            //GlobalRenderer->Print("A");
            for (int i = 0; i < osData.windows.getCount(); i++)
            {     
                //GlobalRenderer->Print("B");
                   
                Window* window = osData.windows[i];
                if (window->instance == NULL)
                    continue;
                if (window->instance->instanceType != InstanceType::Terminal)
                    continue;

                TerminalInstance* terminal = (TerminalInstance*)window->instance;

                if (terminal->tasks.getCount() != 0)
                {
                    Task* task = terminal->tasks[0];
                    DoTask(task);
                    if (task->GetDone())
                    {
                        terminal->tasks.removeFirst();
                        FreeTask(task);
                        //GlobalRenderer->Println("TASK DONE");
                        terminal->PrintUserIfNeeded();
                    }
                    else
                    {
                        //GlobalRenderer->Println("TASK NOT DONE");
                    }
                }
            }
            //PIT::Sleep(10);
            //asm("hlt");
        }
        

        while(osData.osTasks.getCount() > 0)
        {
            Task* task = osData.osTasks[0];
            DoTask(task);
            if (task->GetDone())
            {
                osData.osTasks.removeFirst();
                FreeTask(task);
            }
        }
        RemoveFromStack();

        //GlobalRenderer->Print("C");
        //asm("hlt");
    }

    GlobalRenderer->Clear(Colors.black);
    GlobalRenderer->color = Colors.white;
    GlobalRenderer->Println("Goodbye.");
    PIT::Sleep(1000);
    GlobalRenderer->Clear(Colors.black);


    RemoveFromStack();
    return;
}













/*

while(!osData.exit)
    {    
        if (osData.drawBackground)
            realMainWindow->renderer->DrawImage(bootInfo->bgImage, 0, 0, 2, 2);
        else
            realMainWindow->renderer->Clear(Colors.dblue);

        if (activeWindow != NULL)
        {
            if (activeWindow->moveToFront)
            {
                activeWindow->moveToFront = false;
                int index = osData.windows.getIndexOf(activeWindow);
                if (index != -1)
                {
                    osData.windows.removeAt(index);
                    osData.windows.add(activeWindow);
                }
            }
        }

        //DrawMousePointer1(realMainWindow->framebuffer);
        for (int i = 0; i < osData.windows.getCount(); i++)
        {            
            Window* window = osData.windows[i];

            if (window == osData.debugTerminalWindow && !osData.showDebugterminal)
                continue;
            
            window->position = window->newPosition;

            if (window->size != window->newSize)
                window->Resize(window->newSize);

            window->Render();
        }
        DrawMousePointer2(realMainWindow->framebuffer);

        


        CopyFrameBuffer(realMainWindow->framebuffer, realMainWindow2->framebuffer, GlobalRenderer->framebuffer);
        //CopyFrameBuffer(realMainWindow->framebuffer, realMainWindow2->framebuffer);
        //realMainWindow->Render();
        

        //double endTime = PIT::TimeSinceBoot + 0.02;
        //while (PIT::TimeSinceBoot < endTime)
        {
            //GlobalRenderer->Print("A");
            for (int i = 0; i < osData.windows.getCount(); i++)
            {     
                //GlobalRenderer->Print("B");
                   
                Window* window = osData.windows[i];
                if (window->instance == NULL)
                    continue;
                if (window->instance->instanceType != InstanceType::Terminal)
                    continue;

                TerminalInstance* terminal = (TerminalInstance*)window->instance;

                if (terminal->tasks.getCount() != 0)
                {
                    Task* task = terminal->tasks[0];
                    //GlobalRenderer->Println("DOING TASK");
                    DoTask(task);
                    if (task->GetDone())
                    {
                        terminal->tasks.removeFirst();
                        FreeTask(task);
                        //GlobalRenderer->Println("TASK DONE");
                        terminal->PrintUserIfNeeded();
                    }
                    else
                    {
                        //GlobalRenderer->Println("TASK NOT DONE");
                    }
                }
            }
            //PIT::Sleep(10);
            //asm("hlt");
        }
        //GlobalRenderer->Print("C");
        //asm("hlt");
    }


*/

/*
                    // int x1 = oldActive->position.x;
                    // int y1 = oldActive->position.y;
                    // int x2 = x1 + oldActive->size.width;
                    // int y2 = y1 + oldActive->size.height;
                    
                    // osData.windowPointerThing->UpdatePointerRect(x1-1, y1-22, x1-1, y2);
                    // osData.windowPointerThing->UpdatePointerRect(x2, y1-22, x2, y2);

                    // osData.windowPointerThing->UpdatePointerRect(x1-1, y1-22, x2, y1-1);
                    // osData.windowPointerThing->UpdatePointerRect(x1-1, y2, x2, y2);
                    //osData.windowPointerThing->UpdatePointerRect(x1-1, y1-22, x2, y1-22);


                    // VirtualRenderer::Border border = VirtualRenderer::Border(osData.windowPointerThing->virtualScreenBuffer);
                    // uint32_t col1 = Colors.red;
                    // uint32_t col2 = Colors.orange;
                    // VirtualRenderer::Clear(x1-1, y1-22, x1-1, y2, border, osData.windowPointerThing->virtualScreenBuffer, &col1);
                    // VirtualRenderer::Clear(x2, y1-22, x2, y2, border, osData.windowPointerThing->virtualScreenBuffer, &col2);
                    // VirtualRenderer::Clear(x1-1, y1-1, x2, y1-1, border, osData.windowPointerThing->virtualScreenBuffer, &col1);
                    // VirtualRenderer::Clear(x1-1, y2, x2, y2, border, osData.windowPointerThing->virtualScreenBuffer, &col2);
                    // VirtualRenderer::Clear(x1-1, y1-22, x2, y1-22, border, osData.windowPointerThing->virtualScreenBuffer, &col1);

*/


/*



    // GlobalRenderer->Print("Memory Size: ");
    // GlobalRenderer->Print(to_string(GetMemorySize(bootInfo->mMap, (bootInfo->mMapSize / bootInfo->mMapDescSize), bootInfo->mMapDescSize)));
    // GlobalRenderer->Println(" Bytes.");
    // GlobalRenderer->Println();


    // GlobalRenderer->Println("Free: {} Bytes.", to_string(GlobalAllocator->GetFreeRAM()), Colors.bgreen);
    // GlobalRenderer->Println("");

        //GlobalRenderer->Println("ADDR Window:   {}", ConvertHexToString((uint64_t)window), Colors.bgreen);
        //GlobalRenderer->Println("ADDR Terminal: {}", ConvertHexToString((uint64_t)terminal), Colors.bgreen);

    // {
    //     Window* window = (Window*)malloc(sizeof(Window));
    //     TerminalInstance* terminal = (TerminalInstance*)malloc(sizeof(TerminalInstance));
    //     *terminal = TerminalInstance(&guestUser);
    //     *(window) = Window((DefaultInstance*)terminal, Size(480, 360), Position(100, 20), GlobalRenderer->framebuffer);
    //     osData.windows.add(window);

    //     //GlobalRenderer->Println("ADDR Window:   {}", ConvertHexToString((uint64_t)window), Colors.bgreen);
    //     //GlobalRenderer->Println("ADDR Terminal: {}", ConvertHexToString((uint64_t)terminal), Colors.bgreen);
    // }


    //GlobalRenderer->Println("Free: {} Bytes.", to_string(GlobalAllocator->GetFreeRAM()), Colors.bgreen);
    //GlobalRenderer->Println("");
    //GlobalRenderer->Println("");

    // {
    //     dispVar vars[] = 
    //     {
    //         dispVar((uint64_t)GlobalRenderer->framebuffer->Width), 
    //         dispVar((uint64_t)GlobalRenderer->framebuffer->PixelsPerScanLine),
    //         dispVar((uint64_t)GlobalRenderer->framebuffer->Height), 
    //         dispVar((uint64_t)(GlobalRenderer->framebuffer->Width*GlobalRenderer->framebuffer->Height)), 
    //         dispVar((uint64_t)GlobalRenderer->framebuffer->BufferSize), 
    //     };
    //     GlobalRenderer->Println("Width:  {0}, PPS: {1}.", vars);
    //     GlobalRenderer->Println("Height: {2}.", vars);
    //     GlobalRenderer->Println("H * W:  {3}.", vars);
    //     GlobalRenderer->Println("Size:   {4}.", vars);
    //     GlobalRenderer->Println();
    // }
    
    // {
    //     Window* window = osData.windows[0];
    //     dispVar vars[] = 
    //     {
    //         dispVar((uint64_t)window->framebuffer->Width), 
    //         dispVar((uint64_t)window->framebuffer->PixelsPerScanLine),
    //         dispVar((uint64_t)window->framebuffer->Height), 
    //         dispVar((uint64_t)(window->framebuffer->Width * window->framebuffer->Height)), 
    //         dispVar((uint64_t)window->framebuffer->BufferSize), 
    //     };
    //     GlobalRenderer->Println("Width:  {0}, PPS: {1}.", vars);
    //     GlobalRenderer->Println("Height: {2}.", vars);
    //     GlobalRenderer->Println("H * W:  {3}.", vars);
    //     GlobalRenderer->Println("Size:   {4}.", vars);
    //     GlobalRenderer->Println();
    // }

    // osData.windows[0]->renderer->Clear(Colors.blue);
    // osData.windows[0]->renderer->color = Colors.white;
    // osData.windows[0]->renderer->Println("Hello, world!");
    // osData.windows[0]->Render();

    //osData.windows[1]->Render();

    // osData.windows[2]->renderer->Clear(Colors.green);
    // osData.windows[2]->renderer->color = Colors.white;
    // osData.windows[2]->renderer->Println("Hello, world!");
    // //osData.windows[2]->Render();
*/



/*

    //GlobalRenderer->Println("TEST: {}", to_string(to_int("1024")), Colors.bblue);
    //GlobalRenderer->Println("TEST: {}", to_string(to_int("-1024")), Colors.bblue);

    {
        dispVar vars[] = {dispVar("test"), dispVar(2), dispVar(0)};
        GlobalRenderer->Println("This is a {0}! Also {2}+2={1}.", vars);
    }

    {
        dispVar vars[] = {dispVar("cool thing"), dispVar(4), dispVar(2)};
        GlobalRenderer->Println("This is a {0}! Also {2}+2={1}.", vars);
    }

*/

/*
    //PIT::SetDivisor(65535);

    //GlobalRenderer->delChar(0, 0);
 
    //Panic("Panic go brrrt"); 
    //asm("int $0x0e");

*/

/*
    GlobalRenderer->Println("TIME: {} s", to_string(PIT::TimeSinceBoot), Colors.bred);
    GlobalRenderer->Println("TIME: {} ms", to_string((int)(PIT::TimeSinceBoot*1000)), Colors.bred);
    GlobalRenderer->Println("DIV:  {}", to_string(PIT::Divisor), Colors.bred);
    GlobalRenderer->Println("FREQ: {} Hz", to_string(PIT::freq), Colors.bred);

    GlobalRenderer->Println();

    for (int i = 0; i < 20; i++)
    {
        GlobalRenderer->Print("hoi! ");
        PIT::Sleep(100);
    }

    GlobalRenderer->Println();
    GlobalRenderer->Println();

    GlobalRenderer->Println("TIME: {} s", to_string(PIT::TimeSinceBoot), Colors.bred);
    GlobalRenderer->Println("TIME: {} ms", to_string((int)(PIT::TimeSinceBoot*1000)), Colors.bred);
    GlobalRenderer->Println("DIV:  {}", to_string(PIT::Divisor), Colors.bred);
    GlobalRenderer->Println("FREQ: {} Hz", to_string(PIT::freq), Colors.bred);

*/


/*

    GlobalRenderer->Println("TEST: {}", to_string(0.0), Colors.bred);
    GlobalRenderer->Println("TEST: {}", to_string(0.123), Colors.bred);
    GlobalRenderer->Println("TEST: {}", to_string(0.12345), Colors.bred);
    GlobalRenderer->Println("TEST: {}", to_string(10.0), Colors.bred);
    GlobalRenderer->Println("TEST: {}", to_string(10.123), Colors.bred);
    GlobalRenderer->Println("TEST: {}", to_string(10.12345), Colors.bred);
    GlobalRenderer->Println();
    GlobalRenderer->Println("TEST: {}", to_string(-0.0), Colors.bred);
    GlobalRenderer->Println("TEST: {}", to_string(-0.123), Colors.bred);
    GlobalRenderer->Println("TEST: {}", to_string(-0.12345), Colors.bred);
    GlobalRenderer->Println("TEST: {}", to_string(-10.0), Colors.bred);
    GlobalRenderer->Println("TEST: {}", to_string(-10.123), Colors.bred);
    GlobalRenderer->Println("TEST: {}", to_string(-10.12345), Colors.bred);
    GlobalRenderer->Println();

    GlobalRenderer->Println("TEST: {}", to_string(0.0, 4), Colors.bred);
    GlobalRenderer->Println("TEST: {}", to_string(0.123, 4), Colors.bred);
    GlobalRenderer->Println("TEST: {}", to_string(0.12345, 4), Colors.bred);
    GlobalRenderer->Println("TEST: {}", to_string(10.0, 4), Colors.bred);
    GlobalRenderer->Println("TEST: {}", to_string(10.123, 4), Colors.bred);
    GlobalRenderer->Println("TEST: {}", to_string(10.12345, 4), Colors.bred);
    GlobalRenderer->Println();
    GlobalRenderer->Println("TEST: {}", to_string(-0.0, 4), Colors.bred);
    GlobalRenderer->Println("TEST: {}", to_string(-0.123, 4), Colors.bred);
    GlobalRenderer->Println("TEST: {}", to_string(-0.12345, 4), Colors.bred);
    GlobalRenderer->Println("TEST: {}", to_string(-10.0, 4), Colors.bred);
    GlobalRenderer->Println("TEST: {}", to_string(-10.123, 4), Colors.bred);
    GlobalRenderer->Println("TEST: {}", to_string(-10.12345, 4), Colors.bred);
    GlobalRenderer->Println();
*/


/*

    

    // debugTerminalWindow->Log("Address of RSDP: {}", ConvertHexToString((uint64_t)bootInfo->rsdp));

    // {
    //     char* substr = StrSubstr((char*)bootInfo->rsdp, 0, 8);
    //     debugTerminalWindow->Log("RSDP: \"{}\"", substr);
    //     free(substr);
    // }
    
    //PrepareACPI(bootInfo);



    Array<int> arr = Array<int>(10);

    arr[0] = 5; 
    GlobalRenderer->Println("ARR 1:");
    GlobalRenderer->Println("Size:  {}", to_string(arr.getSize()), Colors.silver);
    GlobalRenderer->Println("Value: {}", to_string((uint64_t)arr[0]), Colors.silver);
    GlobalRenderer->Println("Value: {}", to_string((uint64_t)arr[2]), Colors.silver);
    GlobalRenderer->Println();


    arr.free();

    List<int> list = List<int>();
    GlobalRenderer->Println("Size:  {}", to_string(list.getCount()), Colors.silver);
    GlobalRenderer->Println("Cap:   {}", to_string(list.getCapacity()), Colors.silver);
    
    GlobalRenderer->Println("Items:", Colors.silver);
    for (uint64_t i = 0; i < list.getCount(); i++)
        GlobalRenderer->Println(" - {}", to_string(list[i]), Colors.silver);
    GlobalRenderer->Println();

    list.add(10);
    list.add(20);
    list.add(5);
    list.add(8);
    list.add(99);
    list.add(1000);
    list.add(1);
    list.add(151);
    list.add(7);


    GlobalRenderer->Println("Size:  {}", to_string(list.getCount()), Colors.silver);
    GlobalRenderer->Println("Cap:   {}", to_string(list.getCapacity()), Colors.silver);

    GlobalRenderer->Println("Items:", Colors.silver);
    for (uint64_t i = 0; i < list.getCount(); i++)
        GlobalRenderer->Println(" - {}", to_string(list[i]), Colors.silver);
    GlobalRenderer->Println();

    list[3] = 1000000;


    list.removeAt(4);



    GlobalRenderer->Println("Size:  {}", to_string(list.getCount()), Colors.silver);
    GlobalRenderer->Println("Cap:   {}", to_string(list.getCapacity()), Colors.silver);

    GlobalRenderer->Println("Items:", Colors.silver);
    for (uint64_t i = 0; i < list.getCount(); i++)
        GlobalRenderer->Println(" - {}", to_string(list[i]), Colors.silver);
    GlobalRenderer->Println();
    GlobalRenderer->Println();

    List<int> list2 = list.clone();
    list.clear();

    GlobalRenderer->Println("Size:  {}", to_string(list.getCount()), Colors.silver);
    GlobalRenderer->Println("Cap:   {}", to_string(list.getCapacity()), Colors.silver);

    GlobalRenderer->Println("Items:", Colors.silver);
    for (uint64_t i = 0; i < list.getCount(); i++)
        GlobalRenderer->Println(" - {}", to_string(list[i]), Colors.silver);
    GlobalRenderer->Println();
    GlobalRenderer->Println();


    GlobalRenderer->Println("Size:  {}", to_string(list2.getCount()), Colors.silver);
    GlobalRenderer->Println("Cap:   {}", to_string(list2.getCapacity()), Colors.silver);

    GlobalRenderer->Println("Items:", Colors.silver);
    for (uint64_t i = 0; i < list2.getCount(); i++)
        GlobalRenderer->Println(" - {}", to_string(list2[i]), Colors.silver);
    GlobalRenderer->Println();


*/


/*

    Array<int> arr = Array<int>(10);

    arr[0] = 5;
    GlobalRenderer->Println("ARR 1:");
    GlobalRenderer->Println("Size:  {}", to_string(arr.size), Colors.silver);
    GlobalRenderer->Println("Value: {}", to_string((uint64_t)arr[0]), Colors.silver);
    GlobalRenderer->Println("Value: {}", to_string((uint64_t)arr[2]), Colors.silver);
    GlobalRenderer->Println("Freed: {}", to_string(arr.freed), Colors.silver);
    GlobalRenderer->Println();

    // arr.free();
    
    // GlobalRenderer->Println("Size:  {}", to_string(arr.size), Colors.silver);
    // GlobalRenderer->Println("Value: {}", to_string((uint64_t)arr[0]), Colors.silver);
    // GlobalRenderer->Println("Value: {}", to_string((uint64_t)arr[2]), Colors.silver);
    // GlobalRenderer->Println("Value: {}", to_string((uint64_t)arr[11]), Colors.silver);
    // GlobalRenderer->Println("Freed: {}", to_string(arr.freed), Colors.silver);

    Array<int> arr2 = arr.clone();

    arr[0] = 15;
    GlobalRenderer->Println("ARR 1:");
    GlobalRenderer->Println("Size:  {}", to_string(arr.size), Colors.silver);
    GlobalRenderer->Println("Value: {}", to_string((uint64_t)arr[0]), Colors.silver);
    GlobalRenderer->Println("Value: {}", to_string((uint64_t)arr[2]), Colors.silver);
    GlobalRenderer->Println("Freed: {}", to_string(arr.freed), Colors.silver);

    GlobalRenderer->Println("ARR 2:");
    GlobalRenderer->Println("Size:  {}", to_string(arr2.size), Colors.silver);
    GlobalRenderer->Println("Value: {}", to_string((uint64_t)arr2[0]), Colors.silver);
    GlobalRenderer->Println("Value: {}", to_string((uint64_t)arr2[2]), Colors.silver);
    GlobalRenderer->Println("Freed: {}", to_string(arr2.freed), Colors.silver);
    GlobalRenderer->Println();

    arr.free();

    GlobalRenderer->Println("ARR 1:");
    GlobalRenderer->Println("Size:  {}", to_string(arr.size), Colors.silver);
    GlobalRenderer->Println("Value: {}", to_string((uint64_t)arr[0]), Colors.silver);
    GlobalRenderer->Println("Value: {}", to_string((uint64_t)arr[2]), Colors.silver);
    GlobalRenderer->Println("Freed: {}", to_string(arr.freed), Colors.silver);

    GlobalRenderer->Println("ARR 2:");
    GlobalRenderer->Println("Size:  {}", to_string(arr2.size), Colors.silver);
    GlobalRenderer->Println("Value: {}", to_string((uint64_t)arr2[0]), Colors.silver);
    GlobalRenderer->Println("Value: {}", to_string((uint64_t)arr2[2]), Colors.silver);
    GlobalRenderer->Println("Freed: {}", to_string(arr2.freed), Colors.silver);
    GlobalRenderer->Println();
*/


/*

    // GlobalRenderer->Print("Memory Size: ");
    // GlobalRenderer->Print(to_string(GetMemorySize(bootInfo->mMap, (bootInfo->mMapSize / bootInfo->mMapDescSize), bootInfo->mMapDescSize)));
    // GlobalRenderer->Println(" Bytes.");
    // GlobalRenderer->Println();


    // GlobalRenderer->Println("Free: {} Bytes.", to_string(GlobalAllocator->GetFreeRAM()), Colors.bgreen);
    // GlobalRenderer->Println("");

    // int* arr[100];// = (int*)GlobalAllocator->RequestPage();
    // for (int i = 0; i < 100; i++)
    //     arr[i] = (int*)GlobalAllocator->RequestPage();

    // GlobalRenderer->Println("Free: {} Bytes.", to_string(GlobalAllocator->GetFreeRAM()), Colors.bgreen);
    // GlobalRenderer->Println("");

    // arr[0][1] = 100;


    // for (int i = 0; i < 100; i++)
    //     GlobalAllocator->FreePage(arr[i]);

    // GlobalRenderer->Println("Free: {} Bytes.", to_string(GlobalAllocator->GetFreeRAM()), Colors.bgreen);
    // GlobalRenderer->Println("");
 
    // GlobalRenderer->Println("ADDR OF VRAM:       {}", ConvertHexToString((uint64_t)GlobalRenderer->framebuffer->BaseAddress), Colors.bgreen);
    // GlobalRenderer->Println("ADDR OF NEW DATA 1: {}", ConvertHexToString((uint64_t)heapStart), Colors.silver);
    // GlobalRenderer->Println("ADDR OF NEW DATA:   {}", ConvertHexToString((uint64_t)malloc(0x100)), Colors.silver);
    // GlobalRenderer->Println("ADDR OF NEW DATA:   {}", ConvertHexToString((uint64_t)malloc(0x100)), Colors.silver);
    // GlobalRenderer->Println("ADDR OF NEW DATA:   {}", ConvertHexToString((uint64_t)malloc(0x100)), Colors.silver);
    // GlobalRenderer->Println("");

    //GlobalRenderer->Println("ADDR OF VRAM:       {}", ConvertHexToString((uint64_t)GlobalRenderer->framebuffer->BaseAddress), Colors.bgreen);
    //GlobalRenderer->Println("ADDR OF NEW DATA 1: {}", ConvertHexToString((uint64_t)heapStart), Colors.silver);
    //GlobalRenderer->Println("ADDR OF NEW DATA:   {}", ConvertHexToString((uint64_t)malloc(0x10)), Colors.silver);
    //GlobalRenderer->Println("ADDR OF NEW DATA:   {}", ConvertHexToString((uint64_t)malloc(0x20)), Colors.silver);
    //GlobalRenderer->Println("ADDR OF NEW DATA:   {}", ConvertHexToString((uint64_t)malloc(0x30)), Colors.silver);
    //GlobalRenderer->Println("ADDR OF NEW DATA:   {}", ConvertHexToString((uint64_t)malloc(0x40)), Colors.silver);
    //GlobalRenderer->Println("");

    //GlobalRenderer->Println("Username: {}", adminUser.userName);
    //GlobalRenderer->Println("Username: {}", currentUser->userName);

    // GlobalRenderer->Println("Free: {} Bytes.", to_string(GlobalAllocator->GetFreeRAM()), Colors.bgreen);
    // GlobalRenderer->Println("Free Page Count: {} pages.", to_string(GlobalAllocator->GetFreePageCount()), Colors.bgreen);

    // for (int i = 0; i < 5; i++)
    // {
    //     //GlobalAllocator->RequestPage(); //works
    //     GlobalRenderer->Println("ADDR OF NEW DATA: {}", ConvertHexToString((uint64_t)malloc(0x1000)), Colors.silver); // makes the page count function crash
    //     GlobalRenderer->Println("Free Page Count: {} pages.", to_string(GlobalAllocator->GetFreePageCount()), Colors.bgreen);
    //     //GlobalRenderer->Println("Free: {} Bytes.", to_string(GlobalAllocator->GetFreeRAM()), Colors.bgreen);
    //     //GlobalRenderer->Println("");
    // }

    // GlobalRenderer->Println("Free: {} Bytes.", to_string(GlobalAllocator->GetFreeRAM()), Colors.bgreen);
    // GlobalRenderer->Println("Free Page Count: {} pages.", to_string(GlobalAllocator->GetFreePageCount()), Colors.bgreen);
    // GlobalRenderer->Println("Done.");

*/


/*
   //GlobalRenderer->Println("ADDR OF NEW DATA: {}", ConvertHexToString((uint64_t)malloc(0x100)), Colors.silver);

    GlobalRenderer->Println();

    GlobalRenderer->Print("Memory Size: ");
    GlobalRenderer->Print(to_string(GetMemorySize(bootInfo->mMap, (bootInfo->mMapSize / bootInfo->mMapDescSize), bootInfo->mMapDescSize)));
    GlobalRenderer->Println(" Bytes.");
    GlobalRenderer->Println();

    GlobalRenderer->Println("Free: {} Bytes.", to_string(GlobalAllocator->GetFreeRAM()), Colors.bgreen);
    GlobalRenderer->Println("");

    int* arr = (int*)GlobalAllocator->RequestPage();

    GlobalRenderer->Println("Free: {} Bytes.", to_string(GlobalAllocator->GetFreeRAM()), Colors.bgreen);
    GlobalRenderer->Println("");

    arr[0] = 100;

    GlobalAllocator->FreePage(arr);

    GlobalRenderer->Println("Free: {} Bytes.", to_string(GlobalAllocator->GetFreeRAM()), Colors.bgreen);
    GlobalRenderer->Println("");


    GlobalRenderer->Println("ADDR OF NEW DATA: {}", ConvertHexToString((uint64_t)malloc(0x100)), Colors.silver);

    GlobalRenderer->Println("Free: {} Bytes.", to_string(GlobalAllocator->GetFreeRAM()), Colors.bgreen);
    GlobalRenderer->Println("");
*/

/*
    GlobalRenderer->Print("Memory Size: ");
    GlobalRenderer->Print(to_string(GetMemorySize(bootInfo->mMap, (bootInfo->mMapSize / bootInfo->mMapDescSize), bootInfo->mMapDescSize)));
    GlobalRenderer->Println(" Bytes.");
    GlobalRenderer->Println("Free: {} Bytes.", to_string(GlobalAllocator->GetFreeRAM()), Colors.bgreen);
    GlobalRenderer->Println("");

    int* arr = (int*)GlobalAllocator->RequestPage();

    GlobalRenderer->Print("Memory Size: ");
    GlobalRenderer->Print(to_string(GetMemorySize(bootInfo->mMap, (bootInfo->mMapSize / bootInfo->mMapDescSize), bootInfo->mMapDescSize)));
    GlobalRenderer->Println(" Bytes.");
    GlobalRenderer->Println("Free: {} Bytes.", to_string(GlobalAllocator->GetFreeRAM()), Colors.bgreen);
    GlobalRenderer->Println("");

    arr[0] = 100;

    GlobalAllocator->FreePage(arr);

    GlobalRenderer->Print("Memory Size: ");
    GlobalRenderer->Print(to_string(GetMemorySize(bootInfo->mMap, (bootInfo->mMapSize / bootInfo->mMapDescSize), bootInfo->mMapDescSize)));
    GlobalRenderer->Println(" Bytes.");
    GlobalRenderer->Println("Free: {} Bytes.", to_string(GlobalAllocator->GetFreeRAM()), Colors.bgreen);
    GlobalRenderer->Println("");

*/
 
/*
    GlobalRenderer->Print("Memory Size: ");
    GlobalRenderer->Print(to_string(GetMemorySize(bootInfo->mMap, (bootInfo->mMapSize / bootInfo->mMapDescSize), bootInfo->mMapDescSize)));
    GlobalRenderer->Println(" Bytes.");
    GlobalRenderer->Println("Free: {} Bytes.", to_string(GlobalAllocator.GetFreeRAM()), Colors.bgreen);
    GlobalRenderer->Println("");

    int* arr = (int*)GlobalAllocator.RequestPage();

    GlobalRenderer->Print("Memory Size: ");
    GlobalRenderer->Print(to_string(GetMemorySize(bootInfo->mMap, (bootInfo->mMapSize / bootInfo->mMapDescSize), bootInfo->mMapDescSize)));
    GlobalRenderer->Println(" Bytes.");
    GlobalRenderer->Println("Free: {} Bytes.", to_string(GlobalAllocator.GetFreeRAM()), Colors.bgreen);
    GlobalRenderer->Println("");
*/




/*
    GlobalRenderer->Println("Test 1: {}", to_string((uint64_t)100));
    GlobalRenderer->Println("Test 2: {}", to_string(100.5, 2));
    GlobalRenderer->Clear(Colors.orange);
    
    
    asm("int $0x0e");
    
    for (unsigned int y = 10; y < bootInfo->framebuffer->Height / 4; y++)
    {
        for (unsigned int x = 0; x < bootInfo->framebuffer->Width / 2 * 4; x+=4)
        {
            *(unsigned int*)(x + (y * (bootInfo->framebuffer->PixelsPerScanLine * 4) + bootInfo->framebuffer->BaseAddress)) = 0xff00ffff;
        }
    }
 
    temp.Print("Memory Size: ");
    temp.Print(to_string(GetMemorySize(bootInfo->mMap, (bootInfo->mMapSize / bootInfo->mMapDescSize), bootInfo->mMapDescSize)));
    temp.Println(" Bytes.");
    temp.Println("");


    // {
    //     const char* a[] = {to_string(-7.5, 1), to_string((int64_t)-10000)};
    //     temp.Println("The values are: {} and {}.", (const char**)a);
    // }

    temp.Println("New Page Map loaded!");

    pageTableManager->MapMemory((void*)0x600000000, (void*)0x80000);

    uint64_t* test = (uint64_t*)0x600000000;

    *test = 26;

    temp.Println("Testing virtual Memory Adress: {}", to_string(*test));



    //temp.Println("Hello, \\CFF00FFWorld!\\CFFFFFF");
    PageMapIndexer pageIndexer = PageMapIndexer(0x1000 * 52 + 0x50000 * 7);

    temp.Println("P_i: {}", to_string(pageIndexer.P_i));
    temp.Println("PT_i: {}", to_string(pageIndexer.PT_i));
    temp.Println("PD_i: {}", to_string(pageIndexer.PD_i));
    temp.Println("PDP_i: {}", to_string(pageIndexer.PDP_i));
    temp.Println("Size: {} Pages.", to_string(kernelPages));

    temp.Println("Free RAM:     {} KB", to_string(newAllocator.GetFreeRAM() / 1024));
    temp.Println("Used RAM:     {} KB", to_string(newAllocator.GetUsedRAM() / 1024));
    temp.Println("Reserved RAM: {} KB", to_string(newAllocator.GetReservedRAM() / 1024));
    temp.Println();
    temp.Println("Total RAM:    {} KB", to_string((newAllocator.GetFreeRAM() + newAllocator.GetUsedRAM() + newAllocator.GetReservedRAM()) / 1024));
    temp.Println();


    for (int t = 0; t < 20; t++)
    {
        void* adress = newAllocator.RequestPage();
        temp.Println("Received Page address: {}", ConvertHexToString((uint64_t)adress));
    }


    temp.Println("Total RAM:    {} KB", to_string(GetMemorySize(bootInfo->mMap, (bootInfo->mMapSize / bootInfo->mMapDescSize), bootInfo->mMapDescSize)/ 1024));
    temp.Println();
    temp.Println("-----------------------------");
    temp.Println();
    //PrintEFIMemData(bootInfo->mMap, (bootInfo->mMapSize / bootInfo->mMapDescSize), bootInfo->mMapDescSize, &temp);

    uint8_t testbuffer[20];
    Bitmap testBitMap;
    testBitMap.Buffer = &testbuffer[0];
    // testBitMap.Set(0, true); 
    // testBitMap.Set(2, true); 
    // testBitMap.Set(3, true);  
    // testBitMap.Set(5, true); 
    // testBitMap.Set(11, true);  
    // testBitMap.Set(12, true); 
    // testBitMap.Set(14, true);
    // testBitMap.Set(16, true);

    for (uint64_t i = 0; i < 20; i ++)
    {
        testBitMap.Set(i, false);
    }

    for (uint64_t i = 0; i < 20; i++)
    {
        temp.Print(to_string(i));
        temp.Print(": ");
        temp.color = BasicRenderer::Colors::red;
        if (testBitMap[i]) 
            temp.color = BasicRenderer::Colors::green;
        temp.Println("#");
        temp.color = BasicRenderer::Colors::white;
    }

    for (uint64_t i = 0; i < 20; i += 1 + (i % 3))
    {
        testBitMap.Set(i, true);
    }
    temp.Println("");

    for (uint64_t i = 0; i < 20; i++)
    {
        temp.Print(to_string(i));
        temp.Print(": ");
        temp.color = BasicRenderer::Colors::red;
        if (testBitMap[i])
            temp.color = BasicRenderer::Colors::green;
        temp.Println("#");
        temp.color = BasicRenderer::Colors::white;
    }
	unsigned int BBP = 4;

    for (unsigned int y = 10; y < framebuffer->Height / 4; y++)
    {
        for (unsigned int x = 0; x < framebuffer->Width / 2 * BBP; x+=BBP)
        {
            *(unsigned int*)(x + (y * (framebuffer->PixelsPerScanLine * BBP) + framebuffer->BaseAddress)) = 0xff00ffff;
        }
    }
    //putChar(framebuffer, psf1_font, 0xffff0000, 'A', 0, 0);

     //putStr(framebuffer, psf1_font, 0xffff00ff, "Hello, World!", 16, 16);

    temp.printStr("Hello, World!\n\r");

    temp.printStr(ConvertHexToString((uint64_t)0x002244668811AACC));
    temp.printStr("\n\r");
    temp.printStr(ConvertHexToString((uint32_t)0x0022AA66));
    temp.printStr("\n\r"); 
    temp.printStr(ConvertHexToString((uint16_t)0xA056));
    temp.printStr("\n\r");
    temp.printStr(ConvertHexToString((uint8_t)0x0A));
    temp.printStr("\n\r"); 
    temp.printStr(to_string((uint64_t)125));
    temp.printStr("\n\r");   
    temp.printStr(to_string((int64_t)-125)); 
    temp.printStr("\n\r");
    temp.printStr(to_string((int64_t)125));  
    temp.printStr("\n\r");
    temp.printStr(to_string(10.623, 1));
    temp.printStr("\n\r");
    temp.printStr(to_string(-10.615, 2));
    temp.printStr("\n\r");
    temp.printStr(to_string(-10.615, 4));
    temp.printStr("\n\r");
    temp.printStr(to_string(-10.615, 0));
    temp.printStr("\n\r");
    temp.printStr(to_string(510.623, 3));
    temp.printStr("\n\r");

    

    if (ConvertStringToHex("FF0AFF", temp) == 0x00FF0AFF)
        temp.printStr("String conversion works!\n\r");
    else 
        temp.printStr("String conversion doesn't work.\n\r");

    temp.printStr(to_string((uint64_t)125));
    temp.printStr("\n\r");
    temp.printStr(to_string((int64_t)-125));
    temp.printStr("\n\r");
    

    //printStr(framebuffer, psf1_font, 0xffff00ff, "Hello,");
    //printStr(framebuffer, psf1_font, 0xffff0000, " World!\n\r");
    //printStr(framebuffer, psf1_font, 0xff00ff00, "Hoi!");
    //printStr(framebuffer, psf1_font, 0xff0000ff, "\nHello, World!\n\r");

    //printStr(framebuffer, psf1_font, 0xffff00ffff, "Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi Hoi ");







*/