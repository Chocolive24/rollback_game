#include "raylib_wrapper.h"

namespace raylib {

void DrawRaylibText(const char* text, int posX, int posY, int fontSize,
                    Color color) {
  DrawText(text, posX, posY, fontSize, color);
}

} // namespace raylib