//
// Copyright 2020 Electronic Arts Inc.
//
// TiberianDawn.DLL and RedAlert.dll and corresponding source code is free
// software: you can redistribute it and/or modify it under the terms of
// the GNU General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.

// TiberianDawn.DLL and RedAlert.dll and corresponding source code is distributed
// in the hope that it will be useful, but with permitted additional restrictions
// under Section 7 of the GPL. See the GNU General Public License in LICENSE.TXT
// distributed with this program. You should have received a copy of the
// GNU General Public License along with permitted additional restrictions
// with this program. If not, see https://github.com/electronicarts/CnC_Remastered_Collection

/***************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D   A S S O C I A T E S   **
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Westwood Win32 Library                   *
 *                                                                         *
 *                    File Name : DDRAW.CPP                                *
 *                                                                         *
 *                   Programmer : Philip W. Gorrow                         *
 *                                                                         *
 *                   Start Date : October 10, 1995                         *
 *                                                                         *
 *                  Last Update : October 10, 1995   []                    *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/*=========================================================================*/
/* The following PRIVATE functions are in this file:                       */
/*=========================================================================*/

/*= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =*/

#include "gbuffer.h"
#include "palette.h"
#include "video.h"
#include <cstdio>
#include <SDL.h>

static bool video_inited = false;
static int bit_depth;
static SDL_Window *window = NULL;
static SDL_Surface *screen = NULL;
static SDL_Surface *visible = NULL;
static SDL_Palette *vid_palette = NULL;

class SurfaceMonitorClassDummy : public SurfaceMonitorClass
{

public:
    SurfaceMonitorClassDummy()
    {
    }

    virtual void Restore_Surfaces()
    {
    }

    virtual void Set_Surface_Focus(bool in_focus)
    {
    }

    virtual void Release()
    {
    }
};

SurfaceMonitorClassDummy AllSurfacesDummy;           // List of all direct draw surfaces
SurfaceMonitorClass& AllSurfaces = AllSurfacesDummy; // List of all direct draw surfaces

/***********************************************************************************************
 * Set_Video_Mode -- Initializes Direct Draw and sets the required Video Mode                  *
 *                                                                                             *
 * INPUT:           int width           - the width of the video mode in pixels                *
 *                  int height          - the height of the video mode in pixels               *
 *                  int bits_per_pixel  - the number of bits per pixel the video mode supports *
 *                                                                                             *
 * OUTPUT:     none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/26/1995 PWG : Created.                                                                 *
 *=============================================================================================*/
bool Set_Video_Mode(int w, int h, int bits_per_pixel)
{
    int ret;
    SDL_Renderer *renderer;

    if ( !video_inited )
    {
        ret = SDL_InitSubSystem( SDL_INIT_VIDEO );
        if ( ret )
            return false;
        video_inited = true;
    }
    
    if ( !window )
    {
        window = SDL_CreateWindow( "C&C",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 0 );
        if ( !window )
            return false;
    }
    else
    {
        SDL_SetWindowSize( window, w, h );
    }
    
    bit_depth = bits_per_pixel;
    screen = SDL_GetWindowSurface( window );
    
    SDL_ShowCursor( SDL_DISABLE );
    
    return true;
}

/***********************************************************************************************
 * Reset_Video_Mode -- Resets video mode and deletes Direct Draw Object                        *
 *                                                                                             *
 * INPUT:		none                                                                            *
 *                                                                                             *
 * OUTPUT:     none                                                                            *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/26/1995 PWG : Created.                                                                 *
 *=============================================================================================*/
void Reset_Video_Mode(void)
{
}

/***********************************************************************************************
 * Get_Free_Video_Memory -- returns amount of free video memory                                *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   bytes of available video RAM                                                      *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    11/29/95 12:52PM ST : Created                                                            *
 *=============================================================================================*/
unsigned int Get_Free_Video_Memory(void)
{
    return 1000000000;
}

/***********************************************************************************************
 * Get_Video_Hardware_Caps -- returns bitmask of direct draw video hardware support            *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   hardware flags                                                                    *
 *                                                                                             *
 * WARNINGS: Must call Set_Video_Mode 1st to create the direct draw object                     *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    1/12/96 9:14AM ST : Created                                                              *
 *=============================================================================================*/
unsigned Get_Video_Hardware_Capabilities(void)
{
    unsigned video = 0;
    
    //video |= VIDEO_BLITTER;
    //video |= VIDEO_BLITTER_ASYNC;
    video |= VIDEO_COLOR_FILL;

    return video;
}

/***********************************************************************************************
 * Wait_Vert_Blank -- Waits for the start (leading edge) of a vertical blank                   *
 *                                                                                             *
 * INPUT:                                                                                      *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *=============================================================================================*/
void Wait_Vert_Blank(void)
{
}

/***********************************************************************************************
 * Set_Palette -- set a direct draw palette                                                    *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    ptr to 768 rgb palette bytes                                                      *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    10/11/95 3:33PM ST : Created                                                             *
 *=============================================================================================*/
void Set_DD_Palette(void* palette)
{
    int i;
    
    if ( !visible )
        return;
    
    if ( !vid_palette )
    {
        vid_palette = SDL_AllocPalette( 256 );
        if ( !vid_palette )
            return;
    }

    for ( i = 0; i < 256; i ++ )
    {
        unsigned char *p = palette + i * 3;
        SDL_Color c = { p[0] << 2, p[1] << 2, p[2] << 2, 0xFF };
        SDL_SetPaletteColors( vid_palette, &c, i, 1 );
    }
    SDL_SetSurfacePalette( visible, vid_palette );
}

/***********************************************************************************************
 * Wait_Blit -- waits for the DirectDraw blitter to become idle                                *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07-25-95 03:53pm ST : Created                                                             *
 *=============================================================================================*/

void Wait_Blit(void)
{
}

/***********************************************************************************************
 * SMC::SurfaceMonitorClass -- constructor for surface monitor class                           *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    11/3/95 3:23PM ST : Created                                                              *
 *=============================================================================================*/

SurfaceMonitorClass::SurfaceMonitorClass()
{
    SurfacesRestored = false;
}

/*
** VideoSurfaceDDraw
*/

class VideoSurfaceSDL2 : public VideoSurface
{
public:
    VideoSurfaceSDL2(int w, int h, GBC_Enum flags);
    virtual ~VideoSurfaceSDL2();

    virtual void* GetData() const;
    virtual long GetPitch() const;
    virtual bool IsAllocated() const;

    virtual void AddAttachedSurface(VideoSurface* surface)
    {
    }

    virtual bool IsReadyToBlit();

    virtual bool LockWait();
    virtual bool Unlock();

    virtual void Blt(const Rect& destRect, VideoSurface* src, const Rect& srcRect, bool mask);
    virtual void FillRect(const Rect& rect, unsigned char color);

private:
    SDL_Surface *surface;
    bool primary;
};

VideoSurfaceSDL2::VideoSurfaceSDL2(int w, int h, GBC_Enum flags) :
    primary(false)
{
     surface = SDL_CreateRGBSurfaceWithFormat( 0, w, h, bit_depth,
                                    SDL_PIXELFORMAT_INDEX8 );
                                    
    if ( !surface )
    {
        fprintf(stderr,"Failed to create surface!\n" );
        return;
    }

    if ( flags & GBC_VISIBLE )
    {
        primary = true;
        visible = surface;
    }
}

VideoSurfaceSDL2::~VideoSurfaceSDL2()
{
    if ( surface )
        SDL_FreeSurface( surface );
}

void* VideoSurfaceSDL2::GetData() const
{
    if ( !surface )
        return nullptr;
//fprintf(stderr,"%s %u: %p\n",__FUNCTION__,__LINE__,surface->pixels);
    return surface->pixels;
}

long VideoSurfaceSDL2::GetPitch() const
{
    if ( !surface )
        return 0;
//fprintf(stderr,"%s %u: %d\n",__FUNCTION__,__LINE__,surface->pitch);
    return surface->pitch;
}

bool VideoSurfaceSDL2::IsAllocated() const
{
    /* why this should return true if surface IS allocated??? */
    return (surface == NULL);
}

bool VideoSurfaceSDL2::IsReadyToBlit()
{
    if ( !surface )
        return false;
    return true;
}

bool VideoSurfaceSDL2::LockWait()
{
    if ( !surface )
        return false;
//fprintf(stderr,"%s %u\n",__FUNCTION__,__LINE__);
    return (SDL_LockSurface( surface ) == 0);
}

bool VideoSurfaceSDL2::Unlock()
{
    if ( !surface )
        return false;
//fprintf(stderr,"%s %u\n",__FUNCTION__,__LINE__);
    SDL_UnlockSurface( surface );
    
    if ( (surface == visible) && window )
    {
        SDL_BlitSurface( visible, NULL, screen, NULL );
        SDL_UpdateWindowSurface( window );
    }
    
    return true;
}

void VideoSurfaceSDL2::Blt(const Rect& destRect, VideoSurface* src, const Rect& srcRect, bool mask)
{//fprintf(stderr,"%s %u\n",__FUNCTION__,__LINE__);
    VideoSurfaceSDL2* srcSurface = reinterpret_cast<VideoSurfaceSDL2*>(src);
    SDL_Rect srect, drect;
    
    if ( !surface || !srcSurface || !srcSurface->surface )
        return;
    
    srect.x = srcRect.X;
    srect.y = srcRect.Y;
    srect.w = srcRect.Width;
    srect.h = srcRect.Height;
    drect.x = destRect.X;
    drect.y = destRect.Y;
    drect.w = destRect.Width;
    drect.h = destRect.Height;
    SDL_BlitSurface( srcSurface->surface, &srect, surface, &drect );
}

void VideoSurfaceSDL2::FillRect(const Rect& rect, unsigned char color)
{//fprintf(stderr,"%s %u\n",__FUNCTION__,__LINE__);
    SDL_Rect drect;
    
    if ( !surface )
        return;

    drect.x = rect.X;
    drect.y = rect.Y;
    drect.w = rect.Width;
    drect.h = rect.Height;
    SDL_FillRect(surface, &drect, color);
}

/*
** Video
*/

Video::Video()
{
}

Video::~Video()
{
}

Video& Video::Shared()
{
    static Video video;
    return video;
}

VideoSurface* Video::CreateSurface(int w, int h, GBC_Enum flags)
{
    return new VideoSurfaceSDL2(w, h, flags);
}
