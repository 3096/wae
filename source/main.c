#include <string.h>
#include <stdio.h>
#include <switch.h>

#include "wae0_bin.h"
#include "wae1_bin.h"
#include "wae2_bin.h"

//See also libnx gfx.h.

int main(int argc, char **argv)
{
    u32* framebuf;
    u32  cnt=0;
    u8*  wae0ptr = (u8*)wae0_bin;
    u8*  wae1ptr = (u8*)wae1_bin;
    u8*  wae2ptr = (u8*)wae2_bin;

    //Enable max-1080p support. Remove for 720p-only resolution.
    //gfxInitResolutionDefault();

    gfxInitDefault();

    //Set current resolution automatically depending on current/changed OperationMode. Only use this when using gfxInitResolution*().
    //gfxConfigureAutoResolutionDefault(true);

    while(appletMainLoop())
    {
        //Scan all the inputs. This should be done once for each frame
        hidScanInput();

        //hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
        u32 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS) break; // break in order to return to hbmenu

        u32 width, height;
        u32 pos;
        framebuf = (u32*) gfxGetFramebuffer((u32*)&width, (u32*)&height);

        if(kDown & KEY_A || cnt > 0)
        {
            cnt++;
        }

        if(cnt > 60)
        {
            cnt = 0;
        }

        //Each pixel is 4-bytes due to RGBA8888.
        u32 x, y;
        for (y=0; y<height; y++)//Access the buffer linearly.
        {
            for (x=0; x<width; x++)
            {
                pos = y * width + x;
                if(cnt > 45 || (cnt > 15 && cnt < 30))
                    framebuf[pos] = RGBA8_MAXALPHA(wae1ptr[pos*3+2], wae1ptr[pos*3+1], wae1ptr[pos*3+0]);
                else if(cnt > 0)
                    framebuf[pos] = RGBA8_MAXALPHA(wae2ptr[pos*3+2], wae2ptr[pos*3+1], wae2ptr[pos*3+0]);
                else
                    framebuf[pos] = RGBA8_MAXALPHA(wae0ptr[pos*3+2], wae0ptr[pos*3+1], wae0ptr[pos*3+0]);

            }
        }

        gfxFlushBuffers();
        gfxSwapBuffers();
        gfxWaitForVsync();
    }

    gfxExit();
    return 0;
}
