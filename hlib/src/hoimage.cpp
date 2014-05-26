#include <stdlib.h>
#include "hoset.h"
#if hplatform==XW
#include <netinet/in.h>
#include <X11/Xutil.h>
#endif

#include "hoimage.h"
#include "hoapp.h"

HoImage::HoImage(int w, int h) {
  this->w = w;
  this->h = h;
#if hplatform==MW
  
  HDC cdc = GetDC(NULL);
  handle = CreateCompatibleBitmap(cdc, w, h);
  ReleaseDC(NULL, cdc);
  
  
#else
  handle = XCreatePixmap(
    HoMainApp->dpy,
    DefaultRootWindow(HoMainApp->dpy),
    w, h,
    DefaultDepth(HoMainApp->dpy, DefaultScreen(HoMainApp->dpy))
  );
#endif
}

HoImage::~HoImage() {
#if hplatform==XW
  XFreePixmap(HoMainApp->dpy, handle);
#elif hplatform==MW
  DeleteObject(handle);
#endif
};

void HoImage::init(int w, int h, void *rgbData) {
  this->w = w;
  this->h = h;

#if hplatform==XW
  handle = XCreatePixmap(
    HoMainApp->dpy,
    DefaultRootWindow(HoMainApp->dpy),
    w, h,
    DefaultDepth(HoMainApp->dpy, DefaultScreen(HoMainApp->dpy))
  );
  int depth;
  XImage *img = NULL;
  int outIndex = 0;	
  int i;
  int numBufBytes = (3 * (w * h));
  depth = DefaultDepth(HoMainApp->dpy, DefaultScreen(HoMainApp->dpy));
    size_t numNewBufBytes = (4 * (w * h));
    unsigned *newBuf = (unsigned *) malloc(numNewBufBytes);
    for (i = 0; i < numBufBytes;) {
      newBuf[outIndex] = ((unsigned char *) rgbData)[i++] << 16;
      newBuf[outIndex] |= ((unsigned char *) rgbData)[i++] << 8;
      newBuf[outIndex] |= ((unsigned char *) rgbData)[i++];
      ++outIndex;
    }		
		
    img = XCreateImage (HoMainApp->dpy, 
      CopyFromParent, depth, 
      ZPixmap, 0, 
      (char *) newBuf,
      w, h,
      32, 0
    );
		

	XInitImage (img);
	if (htons(1) != 1) {
          img->byte_order = LSBFirst;
	} else {
	  img->byte_order = MSBFirst;
	}
	
	/*The bitmap_bit_order doesn't matter with ZPixmap images.*/
	img->bitmap_bit_order = MSBFirst;

  GC dc = XCreateGC(HoMainApp->dpy, handle, 0, NULL);
  XPutImage(HoMainApp->dpy, handle, dc, img, 0, 0, 0, 0, w, h);
  XFreeGC(HoMainApp->dpy, dc);
  XFlush(HoMainApp->dpy);
  XDestroyImage(img);

#elif hplatform==MW

  BITMAPINFO *bmi = (BITMAPINFO *)malloc(sizeof(BITMAPINFO));
  bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi->bmiHeader.biWidth = w;
  bmi->bmiHeader.biHeight = h;
  bmi->bmiHeader.biPlanes = 1;
  bmi->bmiHeader.biBitCount = 32;
  bmi->bmiHeader.biCompression = BI_RGB;
  bmi->bmiHeader.biSizeImage = 0;
  bmi->bmiHeader.biXPelsPerMeter = 0;
  bmi->bmiHeader.biYPelsPerMeter = 0;
  bmi->bmiHeader.biClrUsed = 0;
  bmi->bmiHeader.biClrImportant = 0;
  void *data;
  unsigned char *bdata;
  
  handle = CreateDIBSection(NULL, bmi, DIB_RGB_COLORS, &data, NULL, NULL);
  bdata = (unsigned char *) data;

  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      bdata[(i * w + j) * 4 + 2] = ((unsigned char *) rgbData)[((h - i - 1) * w + j) * 3];
      bdata[(i * w + j) * 4 + 1] = ((unsigned char *) rgbData)[((h - i - 1) * w + j) * 3 + 1];
      bdata[(i * w + j) * 4] = ((unsigned char *) rgbData)[((h - i - 1) * w + j) * 3 + 2];
    }
  }
#endif

}

HoImage::HoImage(int w, int h, void *rgbData) {
  init(w, h, rgbData);
}

HoImage::HoImage(HoImage &image, int zvetseni) {
#if hplatform==MW
  BITMAP bmStara;
  GetObject(image.handle, sizeof(BITMAP), &bmStara);

  BITMAPINFO *bmi = (BITMAPINFO *)malloc(sizeof(BITMAPINFO));
  bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi->bmiHeader.biWidth = image.w * zvetseni;
  bmi->bmiHeader.biHeight = image.h * zvetseni;
  bmi->bmiHeader.biPlanes = 1;
  bmi->bmiHeader.biBitCount = 32;
  bmi->bmiHeader.biCompression = BI_RGB;
  bmi->bmiHeader.biSizeImage = 0;
  bmi->bmiHeader.biXPelsPerMeter = 0;
  bmi->bmiHeader.biYPelsPerMeter = 0;
  bmi->bmiHeader.biClrUsed = 0;
  bmi->bmiHeader.biClrImportant = 0;
  
  w = image.w * zvetseni;
  h = image.h * zvetseni;
  unsigned char *nova;
  unsigned char *stara = (unsigned char *) bmStara.bmBits;
  handle =  CreateDIBSection(NULL, bmi, DIB_RGB_COLORS, (void **)&nova, NULL, NULL);
  if (zvetseni == 1) {
    memcpy(nova, stara, w * h * 4);
  } else {
    for (int j = 0; j < image.h; j++) {
      for (int i = 0; i < image.w; i++) {
        for (int k = 0; k < 3; k++) {
          unsigned char *n = nova + ((j * image.w * zvetseni  + i) * zvetseni * 4 + k);
          for (int z = 0; z < zvetseni; z++) {
            for (int z2 = 0; z2 < zvetseni; z2++) {
              *(n + z * 4 + z2 * 4 * image.w * zvetseni) = *stara;
            }
          }
          stara++;
        }
        stara++;
      }
    }
  }
#else
    handle = XCreatePixmap(HoMainApp->dpy,
      DefaultRootWindow(HoMainApp->dpy),
      w = image.w * zvetseni,
      h = image.h * zvetseni,
      DefaultDepth(HoMainApp->dpy,
      DefaultScreen(HoMainApp->dpy))
  );

#endif
}

