
#include <iostream>
#include "screen.h"
#include "shape.h"

using namespace std;


// ПРИМЕР ДОБАВКИ: дополнительная функция присоединения…
void down(shape &p, const shape &q) {
    point n = q.south();
    point s = p.north();
    p.move(n.x - s.x, n.y - s.y - 1);
    cout << n.x << " " << n.y << endl;
    cout << s.x << " " << s.y << endl;

}

void leftAndDown(shape &p, const shape &q) {
    point n = q.south();
    point s = p.north();
    point w = q.west();
    point e = p.east();
    p.move(w.x - e.x, n.y - s.y);

}

void rightAndDown(shape &p, const shape &q) {
    point n = q.south();
    point s = p.north();
    point e = q.east();
    point w = p.west();
    p.move(e.x - w.x, n.y - s.y);

}


/* nw ------ n ------ ne
 |               |
 |               |
 w       c            e
 |               |
 |               |
 sw ------- s ------ se */

// Cборная пользовательская фигура - физиономия
class myshape : public rectangle { // Моя фигура ЯВЛЯЕТСЯ
    int w, h;                         //        прямоугольником
    line l_eye; // левый глаз – моя фигура СОДЕРЖИТ линию
    line r_eye; // правый глаз
    line mouth; // рот

    obliqueCross l_cross;
    obliqueCross r_cross;

public:
    myshape(point, point);

    void draw();

    void move(int, int);

    void resize(int) {}
};

myshape::myshape(point a, point b)
        : rectangle(a, b),
          w(neast().x - swest().x + 1),
          h(neast().y - swest().y + 1),
          l_eye(point(swest().x + 2, swest().y + h * 3 / 4), 2),


          r_eye(point(swest().x + w - 4, swest().y + h * 3 / 4), 2),

          mouth(point(swest().x + 2, swest().y + h / 4), w - 4),


          l_cross(point(5, 5), max(1, h / 10)),
          r_cross(point(5, 5), max(1, h / 10)) {}

void myshape::draw() {
    rectangle::draw();
    int a = (swest().x + neast().x) / 2;
    int b = (swest().y + neast().y) / 2;
    cout << "draw cross" << endl;
    down(l_cross, l_eye);
    down(r_cross, r_eye);
    put_point(point(a, b));
}

void myshape::move(int a, int b) {
    rectangle::move(a, b);
    l_eye.move(a, b);
    r_eye.move(a, b);
    mouth.move(a, b);
}

int main() {
    setlocale(LC_ALL, "Rus");
    screen_init();

    obliqueCross l_bigCross(point(5, 5), 2);
    obliqueCross r_bigCross(point(10, 10), 2);
    myshape face(point(15, 10), point(27, 18));
    h_circle beard(point(40, 10), point(50, 20));


    face.resize(2);

//    rectangle hat(point(0, 0), point(14, 5));
//    hat.rotate_right( );

    line brim(point(0, 15), 17);
    brim.resize(2);
    up(brim, face);
//    up(hat, brim);
    beard.flip_vertically();

    try {
        rectangle hat(point(0, 0), point(14, 300));
        hat.rotate_right();
        up(hat, brim);
//    throw std::exception();
        throw RectangleException("1");
//    } catch (RectangleException &exception) {
//    cerr << "Rectange Exception occured: " << exception.getError() << "\n";
//    } catch (const char* exception){
//    cerr << "Rectange Exception occured: " << exception << "\n" ;
//    }
    }
//    catch(std::exception& e)
//        {
//           cout << "norm";
//        }
    catch (RectangleException &e) {
        cout << "RectangleException norm";
    }

//    try
//    {
//      throw std::exception();
//      throw RectangleException("1"); // поочередно коментируя эти строки
//    }
//    catch(std::exception& e)
//    {
//       cout << "norm";
//    }
//    catch(RectangleException& e)
//    {
//       cout << "RectangleException norm";
//    }


//    try {
//    line brim(point(1000,300),300);
//        brim.resize(2);
//        up(brim, face);
//        up(hat, brim);
//    } catch(int a)  {
//        std::cerr << "Error: " << a << " Is not on the screen" <<'\n';
//
//    }


    down(beard, face);
    leftAndDown(l_bigCross, face);
    rightAndDown(r_bigCross, face);

    shape_refresh();
    std::cout << "=== Ready! ===\n";
    std::cin.get();
    screen_destroy();
    return 0;
}

