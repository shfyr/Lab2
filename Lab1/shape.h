//
//  shape.h
//  Lab1
//
//  Created by Liza Prokudina on 13.04.2021.
//

#ifndef shape_h
#define shape_h


#include <list>
#include <iostream>

using namespace std;
//==1. Поддержка экрана в форме матрицы символов ==
char screen[YMAX] [XMAX];

enum color {
    black = '*',
    white = '.' };

void screen_init( )
{
    for (auto y = 0; y < YMAX; ++y) {
        for (auto &x : screen[y]) {
            x = white;
        }
    }
}

void screen_destroy( )
{
    for (auto y = 0; y < YMAX; ++y) {
        for (auto &x : screen[y]) {
            x = black;
        }
    }
}

// проверка попадания точки на экран
bool on_screen(int a, int b)  {
    return 0 <= a && a < XMAX && 0 <= b && b < YMAX;
    
}

void put_point(int a, int b){
    if (on_screen(a,b)) {
        screen[b] [a] = black;
    }
}

/* Алгоритм Брезенхэма для прямой:
 рисование отрезка прямой от (x0,y0) до (x1,y1).
 Уравнение прямой: b(x-x0) + a(y-y0) = 0.
 Минимизируется величина abs(eps), где eps = 2*(b(x-x0)) + a(y-y0).  */
void put_line(int x0, int y0, int x1, int y1)

{
    int dx = 1;
    int dy = 1;
    
    int a = x1 - x0;
    int b = y1 - y0;
    if (a < 0) {
        dx = -1;
        a = -a;
    }
    if (b < 0) {
        dy = -1;
        b = -b;
    }
    
    int two_a = 2*a;
    int two_b = 2*b;
    
    int xcrit = -b + two_a;
    int eps = 0;
    
    
    
    //Формирование прямой линии по точкам
    for (;;) {
        put_point(x0, y0);
        if (x0 == x1 && y0 == y1) break;
        
        if (eps <= xcrit) {
            x0 += dx;
            eps += two_b;
        }
        
        if (eps >= a || a < b) {
            y0 += dy;
            eps -= two_a;
        }
    }
}
void screen_clear( ) {
    screen_init( );
    
} //Очистка экрана
void screen_refresh( ) // Обновление экрана
{
    for (int y = YMAX-1; 0 <= y; --y) { // с верхней строки до нижней
        for (auto x : screen[y])    // от левого столбца до правого
            std::cout << x;
        std::cout << '\n';
    }
}
//== 2. Библиотека фигур ==
// Виртуальный базовый класс "фигура"
struct shape {
    
    // Список фигур (один на все фигуры!)
    static list<shape*> shapes;
    //Фигура присоединяется к списку
    shape() {
        shapes.push_back(this);
        
    }
    //Точки для привязки
    virtual point north( ) const = 0;
    virtual point south( ) const = 0;
    virtual point east( ) const = 0; //восток
    virtual point west( ) const = 0;
    virtual point neast( ) const = 0;
    virtual point seast( ) const = 0;
    virtual point nwest( ) const = 0;
    virtual point swest( ) const = 0;
    
    
    //Рисование
    virtual void draw( ) = 0;
    //Перемещение
    virtual void move(int, int) = 0;
    //Изменение размера
    virtual void resize(int) = 0;
};

// Размещение списка фигур
list<shape*> shape::shapes;
// Перерисовка всех фигур на экране
void shape_refresh( )
{
    screen_clear( );
    //Динамическое связывание!!!
    for (auto p : shape :: shapes) {
        p->draw( );
    }
    screen_refresh( );
}

class rotatable : virtual public shape { //Фигуры, пригодные к повороту
public:
    virtual void rotate_left( ) = 0;
    virtual void rotate_right( ) = 0;
};

// Фигуры, пригодные
// к зеркальному отражению
class reflectable : virtual public shape {
public:
    // Отразить горизонтально
    virtual void flip_horisontally( ) = 0;
    virtual void flip_vertically( ) = 0;
};
class line : public shape {
    
    /* отрезок прямой ["w", "e"].
     north( ) определяет точку "выше центра отрезка и так далеко
     на север, как самая его северная точка", и т. п. */
protected:
    point w, e;
public:
    line(point a, point b) : w(a), e(b) {};
    //    line(point a, point b)
    //    {
    //
    //    }
    
    
    //    line(point a, int L) : w(point(a.x + L - 1, a.y)), e(a) {  };
    
    line (point a, int L)
    {
        if (a.x > XMAX ) {
            
            throw a.x;
       
        } else if (a.y > YMAX ) {
            throw a.y;
        } else if (L > XMAX) {
            throw L;
        }  else {
            w = (point(a.x + L - 1, a.y));
            e = a;
        }
    };
    
    point north( ) const {
        return point((w.x+e.x)/2, e.y<w.y? w.y : e.y);
        
    }
    point south( ) const { return point((w.x+e.x)/2, e.y<w.y? e.y : w.y); }
    point east( ) const { return point(e.x<w.x? w.x : e.x, (w.y+e.y)/2); }
    point west( ) const { return point(e.x<w.x? e.x : w.x, (w.y+e.y)/2); }
    point neast( ) const { return point(w.x<e.x? e.x : w.x, e.y<w.y? w.y : e.y); }
    point seast( ) const { return point(w.x<e.x? e.x : w.x, e.y<w.y? e.y : w.y); }
    point nwest( ) const { return point(w.x<e.x? w.x : e.x, e.y<w.y? w.y : e.y); }
    point swest( ) const { return point(w.x<e.x? w.x : e.x, e.y<w.y? e.y : w.y); }
    void move(int a, int b)     { w.x += a; w.y += b; e.x += a; e.y += b; }
    void draw( ) { put_line(w, e); }
    void resize(int d) // Увеличение длины линии в (d) раз
    { e.x += (e.x - w.x) * (d - 1); e.y += (e.y - w.y) * (d - 1); }
};

class RectangleException
{
private:
    string m_error;
//    point invalid_point;

public:
    RectangleException(string error) : m_error(error) { };
//    invalid_point(myPoint)

    const string getError() {
        return m_error.c_str();
    }
};
// Прямоугольник
class rectangle : public rotatable {

protected:
    point sw, ne;
public:
    rectangle(point a, point b) :  sw(a), ne(b) { }
    point north( ) const {
        point myPoint = point((sw.x + ne.x) / 2, ne.y);
        
        if (myPoint.x > XMAX || myPoint.y > YMAX ) {
            throw RectangleException("Invalid North");
        }
        return myPoint;
    }
    
    point south( ) const {
        point myPoint = point((sw.x + ne.x) / 2, sw.y);
        
        if (myPoint.x > XMAX || myPoint.y > YMAX ) {
            throw RectangleException("Invalid South");
        }
        return myPoint;
    }
    
    point east( ) const {
        point myPoint =  point(ne.x, (sw.y + ne.y) / 2);
        
        if (myPoint.x > XMAX || myPoint.y > YMAX ) {
            throw RectangleException("Invalid East");
        }
        return myPoint;
    }
    
  
    point west( ) const {
        point myPoint =  point(sw.x, (sw.y + ne.y) / 2);
        
        if (myPoint.x > XMAX || myPoint.y > YMAX ) {
            throw RectangleException("Invalid West");
        }
        return myPoint;
    }
    
    
    point neast( ) const {
        point myPoint = ne;
        
        if (myPoint.x > XMAX || myPoint.y > YMAX ) {
            throw "Invalid neast";
        }
        return myPoint;
    }
    
    point seast( ) const {
        point myPoint = point(ne.x, sw.y);
        
        if (myPoint.x > XMAX || myPoint.y > YMAX ) {
           throw "Invalid seast"; //ВОт именно на эту строку
        }
        return myPoint;
    }
    
    point nwest( ) const {
        point myPoint = point(sw.x, ne.y);
        
        if (myPoint.x > XMAX || myPoint.y > YMAX ) {
            throw RectangleException("Invalid nwest");
           
        } else {
            return myPoint;
        }
      
    }
    
    point swest( ) const {
        point myPoint = sw;
        
        if (myPoint.x > XMAX || myPoint.y > YMAX ) {
            throw RectangleException("Invalid swest");
        }
        return myPoint;
    }
    
    
    void rotate_right()
    { int w = ne.x - sw.x, h = ne.y - sw.y;
        sw.x = ne.x - h * 2; ne.y = sw.y + w / 2;
        
    }
    void rotate_left()
    { int w = ne.x - sw.x, h = ne.y - sw.y;
        ne.x = sw.x + h * 2; ne.y = sw.y + w / 2; }
    void move(int a, int b)
    { sw.x += a; sw.y += b; ne.x += a; ne.y += b; }
    void resize(int d)
    { ne.x += (ne.x - sw.x) * (d - 1); ne.y += (ne.y - sw.y) * (d - 1); }
    
    void draw( )
    {
        put_line(nwest( ), ne);
        put_line(ne, seast( ));
        put_line(seast( ), sw);
        put_line(sw, nwest( ));
    }
};



class h_circle: public rectangle, public reflectable {
    bool reflected;
public:
    h_circle(point a, point b, bool r=true) : rectangle(a, b), reflected(r) { }
    void draw();
    void flip_horisontally( ) { }; // Отразить горизонтально (пустая функция)
    void flip_vertically( ) { reflected = !reflected; };    // Отразить вертикально
    
    
};

void h_circle :: draw() //Алгоритм Брезенхэма для окружностей
{   //(выдаются два сектора, указываемые значением reflected)
    int x0 = (sw.x + ne.x)/2, y0 = reflected ? sw.y : ne.y;
    int radius = (ne.x - sw.x)/2;
    int x = 0, y = radius, delta = 2 - 2 * radius, error = 0;
    while(y >= 0) { // Цикл рисования
        if(reflected) { put_point(x0 + x, y0 + y*0.7); put_point(x0 - x, y0 + y*0.7); }
        else { put_point(x0 + x, y0 - y*0.7); put_point(x0 - x, y0 - y*0.7); }
        error = 2 * (delta + y) - 1;
        if(delta < 0 && error <= 0) { ++x; delta += 2 * x + 1; continue; }
        error = 2 * (delta - x) - 1;
        if(delta > 0 && error > 0) { --y; delta += 1 - 2 * y; continue; }
        ++x; delta += 2 * (x - y);  --y;
    }
}

void up(shape& p, const shape& q) // поместить p над q
{    //Это ОБЫЧНАЯ функция, не член класса! Динамическое связывание!!
    point n = q.north( );
    point s = p.south( );
    p.move(n.x - s.x, n.y - s.y + 1);
}

class obliqueCross: public shape {
    
protected:
    point sw, ne;
    
public:
    obliqueCross(point center, int len) :
    sw(point(center.x - len, center.y- len)), ne(point(center.x + len, center.y + len)) { }
    
    point north( ) const {
        return point((sw.x + ne.x) / 2, ne.y);
        
    }
    point south( ) const { return point((sw.x + ne.x) / 2, sw.y); }
    point east( ) const { return point(ne.x, (sw.y + ne.y) / 2); }
    point west( ) const { return point(sw.x, (sw.y + ne.y) / 2); }
    point neast( ) const { return ne; }
    point seast( ) const { return point(ne.x, sw.y); }
    point nwest( ) const { return point(sw.x, ne.y); }
    point swest( ) const { return sw; }
    
    void move(int a, int b)
    
    { sw.x += a;
        sw.y += b;
        ne.x += a;
        ne.y += b;
    }
    void resize(int d)
    {  }
    
    void draw( )
    {
        
        put_line(swest(), neast( ));
        put_line(nwest( ), seast( ));
    }
    
};

#endif /* shape_h */
