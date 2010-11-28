#include "RWin.h"

void ReloadedWin::DirectConnected(direct_buffer_info *info) {
   if (!fConnected && fConnectionDisabled) {
      return;
   }
   locker->Lock();

   switch(info->buffer_state & B_DIRECT_MODE_MASK) {
      case B_DIRECT_START:
         fConnected = true;

      case B_DIRECT_MODIFY:
         // Get clipping information

         if (fClipList) {
            free(fClipList);
            fClipList = NULL;
         }

         fNumClipRects = info->clip_list_count;
         fClipList = (clipping_rect *)
               malloc(fNumClipRects*sizeof(clipping_rect));

         if (fClipList) {
            memcpy(fClipList, info->clip_list,
                  fNumClipRects*sizeof(clipping_rect));
            fBits = (uint8 *) info->bits;
            fRowBytes = info->bytes_per_row * 2;
				// The x2 here is to perform double-sizing
				// Then we copy each line to the one below it
            fFormat = info->pixel_format;
            fBounds = info->window_bounds;

			fBits += (fBounds.top+19) * info->bytes_per_row + fBounds.left * 4;
				// TODO : the 4 here is only valid for 32bpp mode
				// 19 = height of menubar

            fDirty = true;

         }
         break;

      case B_DIRECT_STOP:
         fConnected = false;
         break;
   }
   locker->Unlock();
}
