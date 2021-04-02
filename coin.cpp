#include "coin.h"
#include "lasso.h"
#include<cstdio>
#include<cstdlib>

#include<time.h>
#include <simplecpp>


void Coin::initCoin() {
  coin_start_x = (PLAY_X_START+WINDOW_X)/2; // *** comming from lasso.h
  coin_start_y = PLAY_Y_HEIGHT;
  coin_circle.reset(coin_start_x, coin_start_y, COIN_SIZE);
  coin_circle.setColor(0x009999);
  coin_circle.setFill(true);
  addPart(&coin_circle);
  type = 'g' ; //gold coin
}

void Coin::resetCoin() {

  double coin_speed = COIN_SPEED;
  double coin_angle_deg = COIN_ANGLE_DEG + 20*(sine(56*resetCount));
  coin_ax = 0;
  if(abs(sin(50*resetCount*resetCount)) < 0.65)
  {coin_ay = COIN_G;coin_ax = 0;
  coin_circle.setColor(0x009999);
  type = 'g' ;
   isBoomerangCoin=false;isRocketCoin=false ;}
  else if(abs(sin(50*resetCount*resetCount)) < 0.88)
  { coin_ay = 0;
  coin_ax = 0;
  coin_circle.setColor(0x990000);
   type='r';
   isRocketCoin=true ;
   isBoomerangCoin=false;}
  else
  {coin_ay = COIN_G;
  coin_ax = -(coin_speed/5.0)*cosine(coin_angle_deg);
  coin_circle.setColor(0x009900);
   type='b'; isBoomerangCoin=true ;
    isRocketCoin=false ;}

  bool paused = true, rtheta = true;
  reset_all(coin_start_x + 100*(sine(63*resetCount)), coin_start_y , coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);



  resetCount ++ ;
}

void Coin::setAngle(double angle_deg){
    release_angle_deg = angle_deg ;

}
