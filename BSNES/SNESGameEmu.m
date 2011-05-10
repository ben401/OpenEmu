/*
 Copyright (c) 2009, OpenEmu Team
 

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
     * Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
     * Neither the name of the OpenEmu Team nor the
       names of its contributors may be used to endorse or promote products
       derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY OpenEmu Team ''AS IS'' AND ANY
 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL OpenEmu Team BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#import "SNESGameEmu.h"
#import <OERingBuffer.h>

#include <snes/libsnes/libsnes.hpp>

#define SAMPLERATE 32040
#define SAMPLEFRAME 800
#define SIZESOUNDBUFFER SAMPLEFRAME*4



NSUInteger BSNESEmulatorValues[] = { SNES_DEVICE_ID_JOYPAD_R, SNES_DEVICE_ID_JOYPAD_L, SNES_DEVICE_ID_JOYPAD_X, SNES_DEVICE_ID_JOYPAD_A, SNES_DEVICE_ID_JOYPAD_RIGHT, SNES_DEVICE_ID_JOYPAD_LEFT, SNES_DEVICE_ID_JOYPAD_DOWN, SNES_DEVICE_ID_JOYPAD_UP, SNES_DEVICE_ID_JOYPAD_START, SNES_DEVICE_ID_JOYPAD_SELECT, SNES_DEVICE_ID_JOYPAD_Y, SNES_DEVICE_ID_JOYPAD_B };
NSString *BSNESEmulatorNames[] = { @"Joypad@ R", @"Joypad@ L", @"Joypad@ X", @"Joypad@ A", @"Joypad@ Right", @"Joypad@ Left", @"Joypad@ Down", @"Joypad@ Up", @"Joypad@ Start", @"Joypad@ Select", @"Joypad@ Y", @"Joypad@ B" };

BSNESGameEmu *current;

@implementation BSNESGameEmu


//BSNES callbacks
static void audio_callback(uint16_t left, uint16_t right)
{
	NSLog(@"audio callback");
	[[current ringBufferAtIndex:0] write:&left maxLength:2];
    [[current ringBufferAtIndex:0] write:&right maxLength:2];
}

static void video_callback(const uint16_t *data, unsigned width,
									unsigned height)
{
	NSLog(@"video callback");
	memcpy(current->videoBuffer, data, width * height * 2);
}

static void input_poll_callback(void)
{
	NSLog(@"poll callback");
}

static int16_t input_state_callback(bool port, unsigned device,
							  unsigned index, unsigned id)
{
	NSLog(@"state callback");
	return 0;
}

- (OEEmulatorKey)emulatorKeyForKey:(NSString *)aKey index:(NSUInteger)index player:(NSUInteger)thePlayer
{
    return OEMakeEmulatorKey(thePlayer - 1, BSNESEmulatorValues[index]);
}

- (void)pressEmulatorKey:(OEEmulatorKey)aKey
{
    pad[aKey.player][aKey.key] = 0xFFFF;
}

- (void)releaseEmulatorKey:(OEEmulatorKey)aKey
{
    pad[aKey.player][aKey.key] = 0;
}

- (id)init
{
	self = [super init];
    if(self != nil)
    {
		soundBuffer = (UInt16*)malloc(SIZESOUNDBUFFER* sizeof(UInt16));
		memset(soundBuffer, 0, SIZESOUNDBUFFER*sizeof(UInt16));
		videoBuffer = (unsigned char*) malloc(512 * 478 * 2);
	}
	
	current = self;

	return self;
}

#pragma mark Exectuion
- (void)executeFrame
{
	NSLog(@"executing a frame");
    snes_run();
}

- (BOOL)loadFileAtPath: (NSString*) path
{
	NSData* theData;
	theData = [NSData dataWithContentsOfFile:path];
	
	snes_init();
	
	snes_set_audio_sample(audio_callback);
	snes_set_video_refresh(video_callback);
	snes_set_input_poll(input_poll_callback);
	snes_set_input_state(input_state_callback);
	
	snes_set_controller_port_device(SNES_PORT_1, SNES_DEVICE_JOYPAD);
	snes_set_controller_port_device(SNES_PORT_2, SNES_DEVICE_JOYPAD);
	
	if (snes_load_cartridge_normal(NULL, [theData bytes], [theData length]))
	{

	}
	
    return YES;
}


#pragma mark Video
- (const void *)videoBuffer
{
    return videoBuffer;
}

//TODO: ask bsnes for proper screen size
- (OEIntRect)screenRect
{
    return OERectMake(0, 0, 256, 224);
}

- (OEIntSize)bufferSize
{
    return OESizeMake(512, 478);
}

- (void)setupEmulation
{
	
}

- (void)resetEmulation
{
	snes_reset();
}

- (void)stopEmulation
{
	snes_term();
    [super stopEmulation];
}

- (GLenum)pixelFormat
{
    return GL_RGB;
}

- (GLenum)pixelType
{
    return GL_UNSIGNED_SHORT_5_6_5;
}

- (GLenum)internalPixelFormat
{
    return GL_RGB5;
}

- (NSUInteger)soundBufferSize
{
    return SIZESOUNDBUFFER;
}

- (NSUInteger)frameSampleCount
{
    return SAMPLEFRAME;
}

- (NSUInteger)frameSampleRate
{
    return SAMPLERATE;
}

- (NSTimeInterval)frameInterval
{
    return (snes_get_region() == SNES_REGION_PAL) ? 50 : 60;
}

- (NSUInteger)channelCount
{
    return 2;
}

- (void)player:(NSUInteger)thePlayer didPressButton:(OEButton)gameButton;
{
	
}

- (void)player:(NSUInteger)thePlayer didReleaseButton:(OEButton)gameButton;
{
	
}

- (BOOL)saveStateToFileAtPath:(NSString *)fileName
{
	return YES;
}

- (BOOL)loadStateFromFileAtPath:(NSString *)fileName
{
    return YES;
}

@end
