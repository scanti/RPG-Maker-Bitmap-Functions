#include "stdafx.h"
#include "Your Code.h"
#include "RPG VX Ace SDK.h"

typedef union {
	unsigned int pixel;
	struct {
	unsigned char blue;
	unsigned char green;
	unsigned char red;
	unsigned char alpha;
	};
} RGSSRGBA;

typedef struct{
    DWORD unk1;
    DWORD unk2;
    BITMAPINFOHEADER *infoheader;
    RGSSRGBA *firstRow;
    RGSSRGBA *lastRow;
} RGSSBMINFO;
 
typedef struct{
    DWORD unk1;
    DWORD unk2;
    RGSSBMINFO *bminfo;
} BITMAPSTRUCT;
 
typedef struct{
    DWORD flags;
    DWORD klass;
    void (*dmark) (void*);
    void (*dfree) (void*);
    BITMAPSTRUCT *bm;
} RGSSBITMAP;

void RGSSYourCustomInitCode()
{

// Place your custom initialization code here.
	
	RGSSEval("class Bitmap;  InvertDLL=Win32API.new('BitmapFuncs.dll','Invert','l','v');  def Invert;    InvertDLL.call(self.__id__);  end;end;");
	RGSSEval("class Bitmap;  MonochromeDLL=Win32API.new('BitmapFuncs.dll','Monochrome','l','v');  def Monochrome;    MonochromeDLL.call(self.__id__);  end; end;");
	RGSSEval("class Bitmap;  SaturationDLL=Win32API.new('BitmapFuncs.dll','Saturation','ll','v');  def Saturation(amount);    if amount<0;    amount=0.0;    elsif amount>255;    amount=255.0;    end;    SaturationDLL.call(self.__id__,amount*1000);  end; end;");
	RGSSEval("class Bitmap;  BrightnessDLL=Win32API.new('BitmapFuncs.dll','Brightness','li','v');  def Brightness(amount);    BrightnessDLL.call(self.__id__,amount);  end; end;");
	RGSSEval("class Bitmap;  ContrastDLL=Win32API.new('BitmapFuncs.dll','Contrast','li','v');  def Contrast(amount);    ContrastDLL.call(self.__id__,amount);  end; end;");
	RGSSEval("class Bitmap;  Filter33DLL=Win32API.new('BitmapFuncs.dll','Filter33','liiiiiiiiii','v');  def Filter33(_11,_12,_13,_21,_22,_23,_31,_32,_33,scale);    Filter33DLL.call(self.__id__,_11,_12,_13,_21,_22,_23,_31,_32,_33,scale);  end; end;");
	RGSSEval("class Bitmap;  def Sharpen;  self.Filter33(0,-1,0,-1,5,-1,0,-1,0,1);  end;  def SharpenMore;  self.Filter33(-1,-1,-1,-1,9,-1,-1,-1,-1,1);  end; end;");
	RGSSEval("class Bitmap;  def EdgeDetect;  self.Filter33(0,-1,0,-1,4,-1,0,-1,0,1);  end;  def Emboss;  self.Filter33(-2,-1,0,-1,1,1,0,1,2,1);  end; end;");
	if(RGSSVersion==1)
		RGSSEval("class Bitmap;  def Blur;  self.Filter33(0,1,0,1,1,1,0,1,0,5);  end;  def BlurMore;  self.Filter33(1,1,1,1,1,1,1,1,1,9);  end; end;");
	RGSSEval("class Bitmap;  Mode7DLL=Win32API.new('BitmapFuncs.dll','Mode7','liii','v');  def Mode7(offset_x, offset_y, rotation);    Mode7DLL.call(self.__id__,offset_x,offset_y,rotation);  end; end;");
	RGSSEval("class Bitmap;  SepiaDLL=Win32API.new('BitmapFuncs.dll','Sepia','l','v');  def Sepia;    SepiaDLL.call(self.__id__);  end;end;");
	RGSSEval("class Bitmap;  Transform33DLL=Win32API.new('BitmapFuncs.dll','Transform33','liiiiiiiii','v');  def Transform33(_11,_12,_13,_21,_22,_23,_31,_32,_33);    Transform33DLL.call(self.__id__,_11,_12,_13,_21,_22,_23,_31,_32,_33);  end;  end;");
	RGSSEval("$REDMASK=1; $GREENMASK=2; $BLUEMASK=4; $ALPHAMASK=8");
	RGSSEval("class Bitmap;  NoiseDLL=Win32API.new('BitmapFuncs.dll','Noise','liii','v');  def Noise(range,iterations=1,mask=7);    NoiseDLL.call(self.__id__, range, iterations, mask);  end;end;");
	RGSSEval("class Bitmap;  VignetteDLL=Win32API.new('BitmapFuncs.dll','Vignette','li','v');  def Vignette(radius);    VignetteDLL.call(self.__id__,radius*1000);  end;end;");
	
	// End of custom initialization code.
	return;
}

// Place your DLL calls here.

// About as optimal as I can get it without unrolling the loop.
RGSSApi bool Invert(unsigned int object)
{
	#pragma warning (disable:4312)
    RGSSBMINFO *bitmap = ((RGSSBITMAP*)(object<<1))->bm->bminfo;
	#pragma warning (default:4312)

    long width, height;
    RGSSRGBA *row;
	RGSSRGBA *end;
 
    if(!bitmap) 
		return false;
    width = bitmap->infoheader->biWidth;
    height = bitmap->infoheader->biHeight;
    row = bitmap->lastRow;
	end=row+(width*height);
    for ( ; row < end; row++) 
	{
		row->pixel=row->pixel^0x00ffffff;
    }
    return true;
}

RGSSApi bool Monochrome(unsigned int object)
{
	#pragma warning (disable:4312)
    RGSSBMINFO *bitmap = ((RGSSBITMAP*)(object<<1))->bm->bminfo;
	#pragma warning (default:4312)

    long width, height;
    RGSSRGBA *row;
    long x, y;
    unsigned int grey;
	// Use perceptual brightness weights. (Green looks brighter than red and red brighter than blue).
	unsigned char RedWeight=77;			// approx 0.299 * 256
	unsigned char GreenWeight=150;		// approx 0.587 * 256
	unsigned char BlueWeight=29;		// approx 0.114 * 256

    if(!bitmap) 
		return false;
    width = bitmap->infoheader->biWidth;
    height = bitmap->infoheader->biHeight;

    row = bitmap->lastRow;
	
    for ( y = 0; y < height; y++) {
        
        for ( x = 0; x < width; x++) {
			grey = row->red*RedWeight+row->green*GreenWeight+row->blue*BlueWeight;
			grey = grey >> 8;	// Divide by 256
			row->red=grey;
			row->green=grey;
			row->blue=grey;
            row ++;
        }
    }
    return true;
}

RGSSApi bool Saturation(unsigned int object, unsigned int amount_in)
{
	#pragma warning (disable:4312)
    RGSSBMINFO *bitmap = ((RGSSBITMAP*)(object<<1))->bm->bminfo;
	#pragma warning (default:4312)

    long width, height;
    RGSSRGBA *row;
    long x, y;
    float red,green,blue,grey;
	float RedWeight=(float)0.299;			
	float GreenWeight=(float)0.587;		
	float BlueWeight=(float)0.114;
	float amount=(float)amount_in/1000;

    if(!bitmap) 
		return false;
    width = bitmap->infoheader->biWidth;
    height = bitmap->infoheader->biHeight;

    row = bitmap->lastRow;
	
    for ( y = 0; y < height; y++) {
        
        for ( x = 0; x < width; x++) {
			grey = row->red*RedWeight+row->green*GreenWeight+row->blue*BlueWeight;

			red=grey+((row->red-grey)*amount);
			green=grey+((row->green-grey)*amount);
			blue=grey+((row->blue-grey)*amount);
			row->red=(unsigned char)min(255,max(0,red));
			row->green=(unsigned char)min(255,max(0,green));
			row->blue=(unsigned char)min(255,max(0,blue));
            row ++;
        }
    }
    return true;
}

RGSSApi bool Brightness(unsigned int object, int amount)
{
	#pragma warning (disable:4312)
    RGSSBMINFO *bitmap = ((RGSSBITMAP*)(object<<1))->bm->bminfo;
	#pragma warning (default:4312)

    long width, height;
    RGSSRGBA *row;
    long x, y;
    int red, green, blue;
    if(!bitmap) 
		return false;
    width = bitmap->infoheader->biWidth;
    height = bitmap->infoheader->biHeight;

    row = bitmap->lastRow;
	
    for ( y = 0; y < height; y++) {
        
        for ( x = 0; x < width; x++) {
			red = row->red;
			green = row->green;
			blue = row->blue;
			red=red+amount;
			green=green+amount;
			blue=blue+amount;
			row->red=(unsigned char)min(255,max(0,red));
			row->green=(unsigned char)min(255,max(0,green));
			row->blue=(unsigned char)min(255,max(0,blue));
            row ++;
        }
    }
    return true;
}

RGSSApi bool Contrast(unsigned int object, int amount)
{
	#pragma warning (disable:4312)
    RGSSBMINFO *bitmap = ((RGSSBITMAP*)(object<<1))->bm->bminfo;
	#pragma warning (default:4312)

    long width, height;
    RGSSRGBA *row;
    long x, y;
    float red,green,blue;
	float factor=(float)(259*(amount+255))/(255*(259-amount));

    if(!bitmap) 
		return false;
    width = bitmap->infoheader->biWidth;
    height = bitmap->infoheader->biHeight;

    row = bitmap->lastRow;
	
    for ( y = 0; y < height; y++) {
        
        for ( x = 0; x < width; x++) {

			red=(float)(factor*(row->red-128))+128;
			green=(float)(factor*(row->green-128))+128;
			blue=(float)(factor*(row->blue-128))+128;
			row->red=(unsigned char)min(255,max(0,red));
			row->green=(unsigned char)min(255,max(0,green));
			row->blue=(unsigned char)min(255,max(0,blue));
            row ++;
        }
    }
    return true;
}

RGSSApi bool Filter33(unsigned int object, int _11, int _12, int _13, int _21, int _22, int _23, int _31, int _32, int _33, int scale)
{
	#pragma warning (disable:4312)
    RGSSBMINFO *bitmap = ((RGSSBITMAP*)(object<<1))->bm->bminfo;
	#pragma warning (default:4312)

    long width, height;
    RGSSRGBA *row;
	RGSSRGBA *rowcopy;
    long x, y;
    float red,green,blue;

	int xm,xp,ym,yp;

    if(!bitmap) 
		return false;
    width = bitmap->infoheader->biWidth;
    height = bitmap->infoheader->biHeight;

	rowcopy=new RGSSRGBA[width*height];
    row = bitmap->lastRow;
	
	RGSSRGBA *rowcopyOffset=rowcopy;
	memcpy(rowcopy,row,width*height*4);

	if(scale==0)
		scale=1;
	
    for ( y = 0; y < height; y++) 
	{    
        for ( x = 0; x < width; x++) 
		{
			if(x>0)
				xm=-1;
			else
				xm=0;
			if(x<width)
				xp=1;
			else
				xp=0;
			if(y>0)
				ym=-width;
			else
				ym=0;
			if(y<height)
				yp=width;
			else
				yp=0;

			red=(float)rowcopyOffset->red*_22;
			red+=(rowcopyOffset+ym+xm)->red*_11;
			red+=(rowcopyOffset+ym)->red*_12;
			red+=(rowcopyOffset+ym+xp)->red*_13;
			red+=(rowcopyOffset+xm)->red*_21;
			red+=(rowcopyOffset+xp)->red*_23;
			red+=(rowcopyOffset+yp+xm)->red*_31;
			red+=(rowcopyOffset+yp)->red*_32;
			red+=(rowcopyOffset+yp+xp)->red*_33;
			red = red/scale;

			green=(float)rowcopyOffset->green*_22;
			green+=(rowcopyOffset+ym+xm)->green*_11;
			green+=(rowcopyOffset+ym)->green*_12;
			green+=(rowcopyOffset+ym+xp)->green*_13;
			green+=(rowcopyOffset+xm)->green*_21;
			green+=(rowcopyOffset+xp)->green*_23;
			green+=(rowcopyOffset+yp+xm)->green*_31;
			green+=(rowcopyOffset+yp)->green*_32;
			green+=(rowcopyOffset+yp+xp)->green*_33;
			green = green/scale;

			blue=(float)rowcopyOffset->blue*_22;
			blue+=(rowcopyOffset+ym+xm)->blue*_11;
			blue+=(rowcopyOffset+ym)->blue*_12;
			blue+=(rowcopyOffset+ym+xp)->blue*_13;
			blue+=(rowcopyOffset+xm)->blue*_21;
			blue+=(rowcopyOffset+xp)->blue*_23;
			blue+=(rowcopyOffset+yp+xm)->blue*_31;
			blue+=(rowcopyOffset+yp)->blue*_32;
			blue+=(rowcopyOffset+yp+xp)->blue*_33;
			blue = blue/scale;

			row->red=(unsigned char)min(255,max(0,red));
			row->green=(unsigned char)min(255,max(0,green));
			row->blue=(unsigned char)min(255,max(0,blue));
            row++;
			rowcopyOffset++;
        }
    }

	delete[]rowcopy;

    return true;
}


// Definately a work in progress.

RGSSApi bool Mode7(unsigned int object,int offset_x, int offset_y, int rotation)
{
	#pragma warning (disable:4312)
    RGSSBMINFO *bitmap = ((RGSSBITMAP*)(object<<1))->bm->bminfo;
	#pragma warning (default:4312)

    long width, height, midx, midy;
    RGSSRGBA *row;
	RGSSRGBA *lastrow; 
	RGSSRGBA *sourcepos;

    long x, y;

	float posx, posy, posz;

	unsigned char red,green,blue,alpha;

    if(!bitmap) 
		return false;
    width = bitmap->infoheader->biWidth;
    height = bitmap->infoheader->biHeight;

	midx = width >> 1;
	midy = height >> 1;

    row = bitmap->lastRow;
	lastrow=new RGSSRGBA[width*height];
	memcpy(lastrow,row,width*height*4);

	float rot=(float)DegToRad(rotation);

	float rotcos=cos(rot);
	float rotsin=sin(rot);

	long horizon=-120;
	long fov=200;

	float scale=80;
	
    for ( y = 0; y < height; y++) 
	{
        
        for ( x = 0; x < width; x++) 
		{
			posx=(float)(x-midx);
			posy=(float)(y+midy-horizon-fov);
			posz=(float)(-y+midy-horizon);

			if(posz<1)
				posz=1;

			posx=posx/posz;
			posy=posy/posz;								
			
			float posyt=posy;
			float posxt=posx;

			posx=posxt*rotcos-posyt*rotsin;
			posy=posxt*rotsin+posyt*rotcos;	
			
			posx=(float)(posx*scale);
			posy=(float)(posy*scale);
							
			posx=posx+midx;
			posy=posy+midy;
			
			
			if (posx<0 || posx>width || posy<0 || posy>height)
			{
				red=0;
				green=0;
				blue=0;
				alpha=0;
			}
			else
			{
				sourcepos=lastrow+(int)posx+((int)posy*width);

				red = sourcepos->red;
				green = sourcepos->green;
				blue = sourcepos->blue;
				alpha = sourcepos->alpha;
			}
			
			row->red=red;
			row->green=green;
			row->blue=blue;
			row->alpha=alpha;
            row ++;
        }
    }

	delete[]lastrow;

    return true;
}

RGSSApi bool Sepia(unsigned int object)
{
	#pragma warning (disable:4312)
    RGSSBMINFO *bitmap = ((RGSSBITMAP*)(object<<1))->bm->bminfo;
	#pragma warning (default:4312)

    long width, height;
    RGSSRGBA *row;
    long x, y;
    float newred, newgreen, newblue;

	float Red_RedWeight=(float)0.393;		
	float Red_GreenWeight=(float)0.769;	
	float Red_BlueWeight=(float)0.189;	

	float Green_RedWeight=(float)0.349;	
	float Green_GreenWeight=(float)0.686;
	float Green_BlueWeight=(float)0.168;	

	float Blue_RedWeight=(float)0.272;		
	float Blue_GreenWeight=(float)0.534;		
	float Blue_BlueWeight=(float)0.131;	

    if(!bitmap) 
		return false;

    width = bitmap->infoheader->biWidth;
    height = bitmap->infoheader->biHeight;

    row = bitmap->lastRow;
	
    for ( y = 0; y < height; y++) 
	{
        
        for ( x = 0; x < width; x++) 
		{
			newred = row->red*Red_RedWeight+row->green*Red_GreenWeight+row->blue*Red_BlueWeight;
			newgreen = row->red*Green_RedWeight+row->green*Green_GreenWeight+row->blue*Green_BlueWeight;
			newblue = row->red*Blue_RedWeight+row->green*Blue_GreenWeight+row->blue*Blue_BlueWeight;

			row->red=(unsigned char)min(255,newred);
			row->green=(unsigned char)min(255,newgreen);
			row->blue=(unsigned char)min(255,newblue);
            row ++;
        }
    }
    return true;
}

RGSSApi bool Transform33(unsigned int object, int _11, int _12, int _13, int _21, int _22, int _23, int _31, int _32, int _33)
{
	#pragma warning (disable:4312)
    RGSSBMINFO *bitmap = ((RGSSBITMAP*)(object<<1))->bm->bminfo;
	#pragma warning (default:4312)

    long width, height;
    RGSSRGBA *row;
	RGSSRGBA *rowcopy;
    long x, y;
    unsigned char red,green,blue,alpha;

	float transx, transy, transw;

    if(!bitmap) 
		return false;
    width = bitmap->infoheader->biWidth;
    height = bitmap->infoheader->biHeight;

	rowcopy=new RGSSRGBA[width*height];
    row = bitmap->lastRow;
	memcpy(rowcopy,row,width*height*4);

	RGSSRGBA *sourcepos;
	
    for ( y = 0; y < height; y++) 
	{    
        for ( x = 0; x < width; x++) 
		{
			transx=(float)(_11*x+_12*y+_13);
			transy=(float)(_21*x+_22*y+_23);
			transw=(float)(_31*x+_32*y+_33);

			// Dividing by zero is bad

			if (transw==0)
				transw=(float)0.001;
			
			transx=transx/transw;
			transy=transy/transw;

			if (transx<0 || transx>width || transy<0 || transy>height)
			{
				red=0;
				green=0;
				blue=0;
				alpha=0;
			}
			else
			{
				sourcepos=rowcopy+(int)transx+((int)transy*width);

				red = sourcepos->red;
				green = sourcepos->green;
				blue = sourcepos->blue;
				alpha = sourcepos->alpha;
			}

			row->red=red;
			row->green=green;
			row->blue=blue;
			row->alpha=alpha;

            row++;
        }
    }

	delete[]rowcopy;

    return true;
}

RGSSApi bool Noise(unsigned int object,int range, int iterations, int Mask)
{
	#pragma warning (disable:4312)
    RGSSBMINFO *bitmap = ((RGSSBITMAP*)(object<<1))->bm->bminfo;
	#pragma warning (default:4312)

    long width, height;
    RGSSRGBA *row;
    long x, y, r;
    int red, green, blue;
	int RandomAmount,range2;

	unsigned short seed = rand()%0xffff;

	bool redMask, greenMask, blueMask;

	redMask=(Mask&1);
	greenMask=(Mask&2)==2;
	blueMask=(Mask&4)==4;

	if (range<1)
		range=1;
	range2=(range*2)+1;

	if(iterations<1)
		iterations=1;

    if(!bitmap) 
		return false;
    width = bitmap->infoheader->biWidth;
    height = bitmap->infoheader->biHeight;

    row = bitmap->lastRow;
	
    for ( y = 0; y < height; y++) 
	{
        
        for ( x = 0; x < width; x++) 
		{
			RandomAmount=0;
			for (r=0;r<iterations;r++)
			{
				seed=(seed >> 1) ^ (-(seed & 1u) & 0xB400u); // fast 16bit pseudo random number genertor.
				RandomAmount+=seed%range2;
			}
			RandomAmount=(RandomAmount/iterations)-range;

			red = row->red;
			green = row->green;
			blue = row->blue;
			if(redMask)
				red=red+RandomAmount;
			if(greenMask)
				green=green+RandomAmount;
			if(blueMask)
				blue=blue+RandomAmount;
			row->red=(unsigned char)min(255,max(0,red));
			row->green=(unsigned char)min(255,max(0,green));
			row->blue=(unsigned char)min(255,max(0,blue));
            row ++;
        }
    }
    return true;
}

RGSSApi bool Vignette(unsigned int object, int radius)
{
	#pragma warning (disable:4312)
    RGSSBMINFO *bitmap = ((RGSSBITMAP*)(object<<1))->bm->bminfo;
	#pragma warning (default:4312)

    long width, height;
	long hwidth, hheight;
    RGSSRGBA *row;
    long x, y;
	float xn, yn, r;
    float red, green, blue,alpha;

    if(!bitmap) 
		return false;
    width = bitmap->infoheader->biWidth;
    height = bitmap->infoheader->biHeight;

	hwidth=width/2;
	hheight=height/2;
	float factor=abs((float)radius/1000);

    row = bitmap->lastRow;
	
    for ( y = 0; y < height; y++) 
	{
        
        for ( x = 0; x < width; x++) 
		{
			// Normalise x and y values to -1 to +1
			xn=(float)x/hwidth-1;
			yn=(float)y/hheight-1;
			r=1-abs(((xn*xn)+(yn*yn))*factor);
			red=(float)row->red*r;
			green=(float)row->green*r;
			blue=(float)row->blue*r;
			alpha=(float)row->alpha*r;
			//row->red=(unsigned char)min(255,max(0,red));
			//row->green=(unsigned char)min(255,max(0,green));
			//row->blue=(unsigned char)min(255,max(0,blue));
			row->alpha=(unsigned char)min(255,max(0,alpha));
            row ++;
        }
    }
    return true;
}

RGSSApi bool Filter(long object, int kWidth, int kHeight, int* kernel, int scale)
{
	#pragma warning (disable:4312)
	RGSSBMINFO *bitmap = ((RGSSBITMAP*)(object<<1))->bm->bminfo;
	#pragma warning (default:4312)
	
	long width, height;
	RGSSRGBA *row;
	RGSSRGBA *rowcopy;
	long x, y;
	float red,green,blue;
	
	int xm,xp,ym,yp;
	
	kWidth += kWidth % 2 - 1;
	kHeight += kHeight % 2 - 1;
	
	if(!bitmap) 
		return false;
	width = bitmap->infoheader->biWidth;
	height = bitmap->infoheader->biHeight;
	
	rowcopy=new RGSSRGBA[width*height];
	row = bitmap->lastRow;
	
	RGSSRGBA *rowcopyOffset=rowcopy;
	memcpy(rowcopy,row,width*height*4);
	
	if(scale==0)
		scale=1;
	
	for ( y = 0; y < height; y++) 
	{    
		for ( x = 0; x < width; x++) 
		{
			red = 0;
			green = 0;
			blue = 0;
			xm = -min((kWidth - 1) / 2, x);
			ym = -min((kHeight - 1) / 2, y);
			xp = min((kWidth - 1) / 2, width - x - 1);
			yp = min((kHeight - 1) / 2, height - y - 1);
			for (int i = 0;i < kWidth;i++) {
				for (int e = 0;e < kHeight;e++) {
					red += (rowcopyOffset + min(xp, max(xm, i - kWidth / 2)) + width * min(yp, max(ym, e - kHeight / 2))) -> red * kernel[i + e * kWidth];
					green += (rowcopyOffset + min(xp, max(xm, i - kWidth / 2)) + width * min(yp, max(ym, e - kHeight / 2))) -> green * kernel[i + e * kWidth];
					blue += (rowcopyOffset + min(xp, max(xm, i - kWidth / 2)) + width * min(yp, max(ym, e - kHeight / 2))) -> blue * kernel[i + e * kWidth];
				}
			}
			red /= scale;
			green /= scale;
			blue /= scale;
			
			row->red=(unsigned char)min(255,max(0,red));
			row->green=(unsigned char)min(255,max(0,green));
			row->blue=(unsigned char)min(255,max(0,blue));
			row++;
			rowcopyOffset++;
		}
	}
	
	delete[]rowcopy;
	
	return true;
}
