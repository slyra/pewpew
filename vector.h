#ifndef VECTOR_H
#define VECTOR_H
#include <math.h>
#define COS_THETA 0.997564050259824247613162680
#define SIN_THETA 0.069756473744125300775958835
#define SPEED_MAX 5000 
#define SPEED_MIN 20 
class vector
{
  private:
    double x;
    double y;
    double z;
    int remaining_deg;
    int remaining_inc;
  public:
    vector();
    vector(double a,double b,double c,int rd,int ri);
    void increase_vector(vector v);
    void set_vector(vector v);
    void scale(double a,double b, double c);
    void vincrease();
    void vdecrease();
    void soft_scale();
    void increase_reset();
    void soft_rotate();
    void rotate_reset();
    void rotater();
    void rotatel();
    double distance(vector );
    double getX();
    double getY();
    double getZ();
    int getRD();
    int getRI();

};
#endif


