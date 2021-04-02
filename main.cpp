#include<cstdio>
#include<cstdlib>

#include<time.h>
#include <simplecpp>
#include <string>
#include<fstream>
#include "MovingObject.h"
#include "lasso.h"
#include "coin.h"



using namespace simplecpp;

main_program {

  initCanvas("Lasso", WINDOW_X, WINDOW_Y);
  int stepCount = 0;
  float stepTime = STEP_TIME;
  float runTime = -1; // sec; -ve means infinite
  float currTime = 0;
  int hs ; //The high Score
  bool newHS =false; //if new high score produced
  bool newHSTxt = false ; //indicates whether "NEW HIGH SCORE!!" is being printed

  // Draw lasso at start position
  double release_speed = INIT_RELEASE_SPEED; // m/s
  double release_angle_deg = INIT_RELEASE_ANGLE_DEG; // degrees
  double lasso_ax = 0;
  double lasso_ay = LASSO_G;
  bool paused = true;
  bool rtheta = true;

  Rectangle background(WINDOW_X/2,WINDOW_Y/2,WINDOW_X,WINDOW_Y) ; //setting background color
  background.setColor(0xffbbbb);
  background.setFill(true);

  Rectangle ground(WINDOW_X/2,(PLAY_Y_HEIGHT+WINDOW_Y)/2,WINDOW_X,(-PLAY_Y_HEIGHT+WINDOW_Y)/2 + 40 ); //making a ground
  ground.setColor(0x3366aa);
  ground.setFill(true);

  Lasso lasso(release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);

  Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
  b1.setColor(0x009900);

  Rectangle body(100,450,25,50);// cowboy body
  body.setColor(0x000099);
  body.setFill(true);

  Rectangle leg1(90,500,3,50);//cowboy leg
  leg1.setColor(0x000000);
  leg1.setFill(true);

  Rectangle leg2(110,500,3,50);//cowboy leg
  leg2.setColor(0x000000);
  leg2.setFill(true);

  Line hand1(90,430,150,450);//cowboy hand

  Line hand2(112,430,150,450); //cowboy hand

  Circle head(100,410,15); //cowboy head
  head.setColor(0x3388ee);
  head.setFill(true);

  Rectangle hat1(100,396,35,5); //cowboy hat1
  hat1.setColor(0x3366aa);
  hat1.setFill(true);

  Rectangle hat2(100,391,10,10);//cowboy hat2
  hat2.setColor(0x3366aa);
  hat2.setFill(true);

  Turtle tur[5]; //sheriff star
  for(int i=0; i<5 ; i++){
    tur[i].penUp();
    tur[i].moveTo(WINDOW_X/2 , WINDOW_Y*2);
  }

  string msg("Cmd: _");
  Text charPressed(PLAY_X_START+50, PLAY_Y_HEIGHT+20, msg);
  char coinScoreStr[256];
  sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
  Text coinScore(PLAY_X_START+50, PLAY_Y_HEIGHT+50, coinScoreStr);

  fstream file_in;
  fstream file_out ;
  file_in.open("Scores.csv" , ios::in);



  paused = true; rtheta = true;
  double coin_speed = COIN_SPEED;
  double coin_angle_deg = COIN_ANGLE_DEG  ;
  double coin_ax = 0;
  double coin_ay = COIN_G;
  Coin coin(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);

  // After every COIN_GAP sec, make the coin jump
  double last_coin_jump_end = 0;

  {
    Text t(WINDOW_X/2,WINDOW_Y/2,"READY COWBOY?");
    Rectangle rect(WINDOW_X/2,WINDOW_Y/2,textWidth("READY COWBOY?")+5,20);
    rect.setColor(0x000099);
    wait(2);
  }
  {
    Text t(WINDOW_X/2,WINDOW_Y/2,"3");
    Circle circ(WINDOW_X/2,WINDOW_Y/2,textWidth("3")+4);
    circ.setColor(0x000099);
    wait(1);
  }
  {
    Text t(WINDOW_X/2,WINDOW_Y/2,"2");
    Circle circ(WINDOW_X/2,WINDOW_Y/2,textWidth("3")+4);
    circ.setColor(0x000099);
    wait(1);
  }
  {
    Text t(WINDOW_X/2,WINDOW_Y/2,"1");
    Circle circ(WINDOW_X/2,WINDOW_Y/2,textWidth("3")+4);
    circ.setColor(0x000099);
    wait(1);
  }
  {
    Text t(WINDOW_X/2,WINDOW_Y/2,"LASSO");
    Rectangle rect(WINDOW_X/2,WINDOW_Y/2,textWidth("LASSO")+5,20);
    rect.setColor(0x000099);
    wait(1);
  }


  // When t is pressed, throw lasso
  // If lasso within range, make coin stick
  // When y is pressed, yank lasso
  // When l is pressed, loop lasso
  // When q is pressed, quit

  for(;;) {
    if((runTime > 0) && (currTime > runTime)) { break; }

    XEvent e;
    bool pendingEv = checkEvent(e);
    if(pendingEv) {
      char c = charFromEvent(e); // *** captures key press if event has occured
      msg[msg.length()-1] = c;  // *** String msg declared before , it is treated as char array and char is changed like setCharAt()
      charPressed.setMessage(msg);
      switch(c) {
      case 't':
	lasso.unpause();
	break;
      case 'y':
	lasso.yank();
	break;
      case 'l':
	lasso.loopit();
	lasso.check_for_coin(&coin);
	wait(STEP_TIME*5);
	break;
      case'r':
      if(lasso.isPaused() && lasso.rocketLassoNum>0){ // *** For rocket lasso
     lasso.releaseRocketLasso();

     lasso.unpause();}
    break;
      case 'b':
      if(!lasso.isPaused() && lasso.boomerangLassoNum>0){
      lasso.setvx(-50);
      lasso.boomerangLassoReleased = true ;
      }
    break;
      case 'm' :
      if(!lasso.isPaused() && lasso.magneticLassoNum>0){
      lasso.magneticLassoReleased = true ;

      }
      break;


      case '[':
	if(lasso.isPaused()) { lasso.addAngle(-RELEASE_ANGLE_STEP_DEG);	}
	break;
      case ']':
	if(lasso.isPaused()) { lasso.addAngle(+RELEASE_ANGLE_STEP_DEG); }
	break;
      case '-':
	if(lasso.isPaused()) { lasso.addSpeed(-RELEASE_SPEED_STEP); }
	break;
      case '=':
	if(lasso.isPaused()) { lasso.addSpeed(+RELEASE_SPEED_STEP); }
	break;
      case 'q': //If you want your high score saved close by clicking the 'q' button
    file_in.close();
    if(newHS){
    file_out.open("Scores.csv" , ios::out); //saving the current highscore
    file_out << hs ;
    file_out.close();
    }

	exit(0);
      default:
	break;
      }
    }
    char lifeStr[256];
  sprintf(lifeStr, "Lives :%d", lasso.life);
  Text lives(PLAY_X_START+150 , PLAY_Y_HEIGHT+20 ,lifeStr); //printing number of lives

    char rocketLassoStr[256];
    sprintf(rocketLassoStr , "Rocket Lassos :%d" , lasso.rocketLassoNum);
    Text rct(PLAY_X_START+150 , PLAY_Y_HEIGHT+50 ,rocketLassoStr); //printing number of rocket lassos

    char boomerangLassoStr[256];
    sprintf(boomerangLassoStr , "Boomerang Lassos :%d" , lasso.boomerangLassoNum);
    Text bct(PLAY_X_START+350 , PLAY_Y_HEIGHT+50 ,boomerangLassoStr); //printing number of boomerang lassos

    char magneticLassoStr[256];
    sprintf(magneticLassoStr , "Magnetic Lassos :%d" , lasso.magneticLassoNum);
    Text mct(PLAY_X_START+350 , PLAY_Y_HEIGHT+20 ,magneticLassoStr); //printing number of magnetic lassos

    if(!newHS){
        while(1){
        file_in >> hs ;
        if(file_in.eof()){
            break ;
        }
        }

        if(lasso.getNumCoins()>hs){
            newHS = true ;
            newHSTxt = true ;
        }
    }else{
        hs = lasso.getNumCoins() ;
    }


    char highScoreStr[256];
    sprintf(highScoreStr, "High Score: %d" , hs);
    Text hst(PLAY_X_START+550 , PLAY_Y_HEIGHT+20 ,highScoreStr); //printing the current highscore


    if(newHS && newHSTxt){
        {
        Text t(WINDOW_X/2,WINDOW_Y/2,"NEW HIGH SCORE"); //prints when new high score occurs
        Rectangle rect(WINDOW_X/2,WINDOW_Y/2,textWidth("NEW HIGH SCORE")+5,20);
        rect.setColor(0x000099);
        wait(1);
        }
        {
        Text tw(WINDOW_X/2,WINDOW_Y/2,"YOU EARN A SHERIFF BATCH"); //prints that you earn sheriff batch
        Rectangle rectw(WINDOW_X/2,WINDOW_Y/2,textWidth("YOU EARN A SHERIFF BATCH")+5,20);
        rectw.setColor(0x000099);

        //making the sheriff star
        for(int i=0 ; i<5 ; i++){
            tur[i].moveTo(WINDOW_X/2,WINDOW_Y/2+30);
            tur[i].left(72*i);
            tur[i].setColor(0x00aaaa);
            tur[i].penUp();
        }
        wait(1);
        for(int i=0 ; i<5 ; i++){
                tur[i].scale(0.3);
            }
        for(int j=1 ; j<=10 ;j++){
            for(int i=0 ; i<5 ; i++){
                tur[i].move((110-WINDOW_X/2)/10 , (430-(WINDOW_Y/2+30))/10);
            }
        }


        wait(1);
        }
        newHSTxt = false;
    }


  if(lasso.life == 0){
  file_in.close();
    if(newHS){
    file_out.open("Scores.csv" , ios::out); //saving the current highscore
    file_out << hs ;
    file_out.close();
    }
    Text tg(WINDOW_X/2,WINDOW_Y/2,"GAME OVER"); //prints game over
        Rectangle rectg(WINDOW_X/2,WINDOW_Y/2,textWidth("GAME OVER")+5,20);
        rectg.setColor(0x000099);
        wait(1);
  exit(0); //lives ran out
  }

    lasso.nextStep(stepTime);

    coin.nextStep(stepTime);
    if(coin.isPaused()) {
      if((currTime-last_coin_jump_end) >= COIN_GAP) {
	coin.unpause();
	coin.setAngle(COIN_ANGLE_DEG + 20*(sine(56*currTime))); //generating pseudo random numbers
      }
    }

    if(coin.getYPos() > PLAY_Y_HEIGHT || coin.getYPos()<10) {
      coin.resetCoin();
      coin.setAngle(COIN_ANGLE_DEG + 20*(sine(56*currTime)));
      last_coin_jump_end = currTime;
    }

    sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
    coinScore.setMessage(coinScoreStr);

    stepCount++;
    currTime += stepTime;
    wait(stepTime);
  } // End for(;;)

  wait(3);
} // End main_program
