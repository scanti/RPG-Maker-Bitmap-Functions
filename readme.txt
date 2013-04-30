**** RPG Maker Bitmap Function Library

This is dll extension for the RPG Maker series that adds bitmap manipulation functions to the standard
Ruby library. It will automagically monkey patch any existing classes to add the extra functionality.

At the moment this library has only been tested using RPG Maker VX Ace and has had limited test on
RPG Maker XP.


**** How to load the library.

For RPG Maker XP use:

Win32API.new('BitmapFuncs.dll','Initialize','','').call()

For RPG Maker VX Ace use:

DL.dlopen('BitmapFuncs.dll')


**** How to load the library securely

Unfortunately loading an external dll file into RPG Maker is insecure. I won't go into the reasons why
because I don't want to help any hackers out there. In order to make the loading of the library a bit 
more secure you can use the following code:

For RPG Maker XP:

sysDir=' '*260
GetSysDir=Win32API.new('kernel32.dll','GetSystemDirectoryA','PI','')
GetSysDir.call(sysDir,260)

sysDir=sysDir.rstrip.chop+'\\'
puts sysDir

csum=Win32API.new(sysDir+'imagehlp.dll','MapFileAndCheckSumA','PPP','I')
headerSum=[0].pack('l')
checkSum=[0].pack('l')

csum.call('BitmapFuncs.dll',headerSum,checkSum)


# Replace <checksum of library> with the checksum of the dll file. You'll need to extract it from checkSum.
if (headerSum.unpack('l')!=checkSum.unpack('l') || checkSum.unpack('l')!=<checksum of library> )
  Win32API.new('user32.dll','MessageBoxA','IPPI','').call(0,"BitmapFuncs.dll corrupt. Please reinistall.","Error",0)
  exit
end

For RPG Maker VX Ace:

crc32 = File.open('BitmapFuncs.dll') { |f| Zlib.crc32 f.read }

# Replace <crc32 of library> with the value in crc32.
# puts crc32
if (crc32!=<crc32 of library>)
  msgbox 'Corrupt BitmapFuncs.dll. Please re-install.'
  exit
end

**** New bitmap functions

This is a rundown of all the functions that are added by the library.

Global variables added:

$REDMASK
$GREENMASK
$BLUEMASK
$ALPHAMASK

Used by some functions to select which channels you want the filter to effect.

$RGSSVERSION

The version of RPG MAker that called the library.
0 = Unknown version.
1 = RPG Maker XP
2 = RPG Maker VX
3 = RPG Maker VX Ace

Extensions to the Bitmap class:

> Invert

Inverts the colors in the bitmap.

> Monochrome

Converts the bitmap to grey scale.

> Saturation (amount)

Adjusts the amount of color in a bitmap. 0 is grey scale. 1 means no charge. 2 means twice as colorfull.
Amount has the range of 0 to 255 (anything over 255 will be the same as 255) and can be a floating point number.

> Brightness (amount)

Adjusts the brightness of the bitmap. A negative number decreases the brightness. 0 means no change and a positive number
increase the brightness.

> Contrast (amount)

Adjusts the contrast of the bitmap. A negative number decreases the contrast. 0 means no change and a positive number
increase the contrast.

> Filter33 (_11,_12,_13,_21,_22,_23,_31,_32,_33,divisor)

Applies a 3x3 convolution matrix to the bitmap.

> Sharpen

Sharpens a bitmap.

> SharpenMore

Sharpens a bitmap more than Sharpen.

> EdgeDetect

Detects areas of high contrast.

> Emboss

Applies a pseudo 3d type effect to the bitmap.

> Blur

(RPG Maker XP Only) Blurs the bitmap.

> BlurMore

(RPG Maker XP Only) Blur the bitmap a bit more than Blur.

> Mode7 (offset_x, offset_y, rotation)

(Experimental) Applies a mode 7 type effect to the bitmap. Currently offset_x and offset_y don't do anything. rotation is the
rotation of the bitmap in degrees.

> Sepia

Changes the tone of the bitmap to a sepia tone.
 
> Transform33 (_11,_12,_13,_21,_22,_23,_31,_32,_33)

Applies a 3x3 transformation matrix to the pixels in the bitmap. You can scale, rotate, translate and perspectively transform
the bitmap with this function.

> Noise (range, iterations=1, mask=$REDMASK|$GREENMASK|$BLUEMASK)


Adds noise to the bitmap. range is the intensity of the noise. The higher the number of iterations the more the noise follows
a guassian distribution. You can also choose which channels to add the noise to using the mask parameter and the $REDMASK,
$GREENMASK, $BLUEMASK, $ALPHAMASK global variables.

> Vignette (radius)

Applies a Vignette filter to the bitmap. i.e. the edges of the bitmap get darker.


