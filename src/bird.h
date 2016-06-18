#ifndef BIRD_H
#define BIRD_H

#include <gameitem.h>
#include <QPixmap>
#include <QGraphicsScene>
#include <QTimer>
#include <QMouseEvent>
#include <iostream>

#define BIRD_DENSITY 10.0f
#define BIRD_FRICTION 0.2f
#define BIRD_RESTITUTION 0.5f


#define red    0
#define blue   1
#define black  2
#define yellow 3
#define pig    4
class Bird : public GameItem
{
public:
    Bird(float x, float y, float radius, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene,int type);
    void setLinearVelocity(b2Vec2 velocity);
    void SetPosion(QMouseEvent *pos);
    void SetPosion(QPoint *pos);
    void shoot();
    int type;
    float X,Y;
};

#endif // BIRD_H
