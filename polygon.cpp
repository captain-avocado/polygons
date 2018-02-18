#include "polygon.h"

polygon::polygon(int x, int y, int r)
{
    x0 = x;
    y0 = y;
    R = r;
}
//void Model_main::split(int R, int P)
//{
//    for (int count = 1; count < 256; count *= 2) {
//        for (int x = 0; x < R; x += R/count) {
//            for (int y = 0; y < R; y += R/count)
//                if (reqSplit(x, y, R/count, P)) {
//                    draw(x, y, R/count);
//                }
//        }
//    }
//}
