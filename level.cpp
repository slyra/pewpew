#include "level.h"
level::level()
{
  WINDOW_SIZEX=800;
  WINDOW_SIZEY=600;
  windowX=0;
  windowY=0;
  font = GLUT_BITMAP_TIMES_ROMAN_24;
  esp.set_vector(vector(0.001,0.005,0.005,0,0));
  pos.set_vector(vector(-0.5,0,0.5,0,0));
  enemie_killed = 0;
  enemie_before = 0;
  enemie_after = 0;
  score = 0;
  time = 0;
  ftime = 0;
  startingSpeed.set_vector(vector(0.001,0.005,0.005,0,0));
  playerShip.set_speed(startingSpeed);
  lifeDraw=playerShip.get_life();
}

void level::shipExplode(vector position)
{
  drawableList.push_back(new xplosion(position));
}
void level::drawScene()
{
  glClearColor(0,0,0,0);
  glClear(GL_COLOR_BUFFER_BIT);
  //glPushMatrix();

  //glEnable(GL_CONVOLUTION_2D);
	glEnable(GL_SEPARABLE_2D);
	//GLfloat filter[7][7] = {
	//	{0.00000067, 0.00002292, 0.00019117, 0.00038771, 0.00019117, 0.00002292, 0.00000067},
	//	{0.00002292, 0.00078633, 0.00655965, 0.01330373, 0.00655965, 0.00078633, 0.00002292},
	//	{0.00019117, 0.00655965, 0.05472157, 0.11098164, 0.05472157, 0.00655965, 0.00019117},
	//	{0.00038771, 0.01330373, 0.11098164, 0.22508352, 0.11098164, 0.01330373, 0.00038771},
	//	{0.00019117, 0.00655965, 0.05472157, 0.11098164, 0.05472157, 0.00655965, 0.00019117},
	//	{0.00002292, 0.00078633, 0.00655965, 0.01330373, 0.00655965, 0.00078633, 0.00002292},
	//	{0.00000067, 0.00002292, 0.00019117, 0.00038771, 0.00019117, 0.00002292, 0.00000067}
	//};
	//GLfloat filter[7][7] = {
	//	{0.00000067, 0.00002292, 0.00019117, 0.00038771, 0.00019117, 0.00002292, 0.00000067},
	//	{0.00002292, 0.00078633, 0.00655965, 0.01330373, 0.00655965, 0.00078633, 0.00002292},
	//	{0.00019117, 0.00655965, 0.05472157, 0.11098164, 0.05472157, 0.00655965, 0.00019117},
	//	{0.00038771, 0.01330373, 0.11098164, 0.22508352, 0.11098164, 0.01330373, 0.00038771},
	//	{0.00019117, 0.00655965, 0.05472157, 0.11098164, 0.05472157, 0.00655965, 0.00019117},
	//	{0.00002292, 0.00078633, 0.00655965, 0.01330373, 0.00655965, 0.00078633, 0.00002292},
	//	{0.00000067, 0.00002292, 0.00019117, 0.00038771, 0.00019117, 0.00002292, 0.00000067}
	//};
	//glConvolutionFilter2D(GL_CONVOLUTION_2D, GL_LUMINANCE, 6, 6, GL_LUMINANCE, GL_FLOAT, filter);
	glWindowPos2i(0,0);
	glCopyPixels(0,0,WINDOW_SIZEX,WINDOW_SIZEY,GL_COLOR);
	glDisable(GL_SEPARABLE_2D);
	//glDisable(GL_CONVOLUTION_2D);

  glLineWidth(5);
  GLfloat  mycolor[]={1.0,0.0,0.0};
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,mycolor);
  glutWireCube(1.5);
  glDisable(GL_LINE_STIPPLE);
  show_score(score);
  if (playerShip.isAlive()) 
  {
    glLineWidth(10);
    displayLife();
    playerShip.draw();
  }
  //drawDrawableList(drawableList);
  for_each(drawableList.begin(),drawableList.end(),[](game_object *p)->void{p->draw();});
  //drawEnemyList(fireList); 
  for_each(fireList.begin(),fireList.end(),[](game_ship *p)->void{p->draw();});
  //drawEnemyList(enemyList);
  for_each(enemyList.begin(),enemyList.end(),[](game_ship *p)->void{p->draw();});
  glutSwapBuffers();
}
void level::displayLife()
{
    int mlife = playerShip.getLife();
    if (lifeDraw<mlife) 
    {
      lifeDraw+=10;
    }
    else if (lifeDraw > mlife ) 
    {
      lifeDraw-=10;
    }
    glBegin(GL_LINES);
    GLfloat  my1color[]={0.0,1.0,0.0};
    GLfloat shiny[]={100.0};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,my1color);
    glVertex3f(-0.9,0.8,0);  
    GLfloat  mycolor[]={1.0,0.0,0.0};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,mycolor);
    glVertex3f(lifeDraw/3000.0-0.9,0.8,0);  
    glEnd();
}
void level::collisionDetect(std::list<game_ship *> bullets,std::list<game_ship *> enemies)
{
  std::list<game_ship *>::iterator enemy = enemies.begin();
  std::list<game_object *> newdrawList;
  while (enemy !=enemies.end())
  {
    newdrawList = (*enemy)->collisions(bullets);
    drawableList.insert
      (drawableList.end(),newdrawList.begin(),newdrawList.end());
    enemy++;
  }
}

void level::play()
{
  enemie_before = (int) enemyList.size();
  collisionDetect(fireList,enemyList);
  fireList.remove_if([](game_object *p)->bool {return p->get_life()<0;});
  enemyList.remove_if([](game_object *p)->bool {return p->get_life()<0;});
  drawableList.remove_if([](game_object *p)->bool {return p->get_life()<0;});
  enemie_after = (int) enemyList.size();
  enemie_killed+=enemie_before-enemie_after;

  if (enemie_killed > 1)
  {
    enemie_killed = 0;
    playerShip.set_life(playerShip.get_life()+50);
    score+=10;
  }
  if (playerShip.isAlive())
  {
    std::list<game_object *> newdrawList;
    newdrawList = playerShip.collisions(enemyList);
    drawableList.insert(drawableList.end(),newdrawList.begin(),newdrawList.end());
    playerShip.move(); 
    if (ftime==10)
    {
      ftime=0;
      double firespeed=0.015;
      double angle = shipMouseAngle();
      vector fspeed(firespeed*cos(angle),firespeed*sin(angle),0,0,0);
      fire * bullet = new fire(playerShip.get_pos(),fspeed);
      fireList.push_back(bullet);
    }
  }
  else 
  {
    enemie_killed =0 ;
    score = 0;           
    time = 0;
    ftime=0;     
  }
  for_each(drawableList.begin(),drawableList.end(),[](game_object *p)->void{p->move();});
  for_each(enemyList.begin(),enemyList.end(),[](game_ship *p)->void{p->move();});
  for_each(fireList.begin(),fireList.end(),[](game_ship *p)->void{p->move();});
  usleep(10000);
  time++;
  ftime++;
  if (time==100)
  {
    time=0;
    enemyList.push_back(new dummyship(pos,esp));
  }
  
  drawScene();
}



void level::output(double x, double y, char *string)
{
  int len, i;
  glRasterPos2f(x, y);
  len = (int) strlen(string);
  for (i = 0; i < len; i++) 
  {
    glutBitmapCharacter(font, string[i]);
  }
}

void level::show_score(int score)
{
  char c_score[20];
  sprintf(c_score,"%d",score);
  output(0.8,0.9,c_score);
}
void level::reshape(int w,int h)
{
  GLsizei minSize=w>h ? (GLsizei) h : (GLsizei) w;

  GLsizei startX=((w-h)/2.0);
  if (startX>0)
  {
    glViewport(startX,0,minSize,minSize);
  }
  else
  {
    glViewport(0,-startX,minSize,minSize);
  }
  WINDOW_SIZEX=w;
  WINDOW_SIZEY=h;
}
double level::shipMouseAngle()
{
  vector pos = playerShip.get_pos();
  double x = pos.getX();
  double y = pos.getY();
  x=mX-x;
  y=-mY-y;
  //std::cout <<" x y = --|#>> " << x << " " << y << std::endl;
  if(x>0) 
  {
    return atan(y/x);
  }
  else if (x<0)
  {
    return atan(y/x)+M_PI;
  }
  else if (y>0)
  {
    return M_PI/2;
  }
  else 
  {
    return -M_PI/2;
  }
}
void level::keyboardReleaseFunction(unsigned char key,int x, int y)
{
  vector current_speed;
  current_speed.set_vector(playerShip.get_speed());
  switch (key)
  {
    case 'w':
      //current_speed.increase_vector(0,0.001,0);
      current_speed.increase_reset();//scale(1.2,1.2,1.2);
      playerShip.set_speed(current_speed);
      break;

    case 'a':
      //current_speed.increase_vector(-0.001,0,0);
      current_speed.rotate_reset();
      playerShip.set_speed(current_speed);
      break;
    case 's':
      //current_speed.increase_vector(0,-0.001,0);
      current_speed.increase_reset();//scale(1.2,1.2,1.2);
      playerShip.set_speed(current_speed);
      break;

    case 'd':
      //current_speed.increase_vector(0.001,0,0);
      current_speed.rotate_reset();
      playerShip.set_speed(current_speed);
      break;
    case 'q':
      current_speed.scale(0.25,0.25,0.25);
      playerShip.set_speed(current_speed);
      break;
    case 'e':
      current_speed.scale(4,4,4);
      playerShip.set_speed(current_speed);
      break;
  }
}
void level::myMouseFunction(int x,int y)
{
  mX=3*((double(x)/WINDOW_SIZEX)-0.5);
  mY=2*((double(y)/WINDOW_SIZEY)-0.5);
}
void level::keyboardFunction(unsigned char key,int x,int y)
{
  vector current_speed;
  current_speed.set_vector(playerShip.get_speed());
  switch (key)
  {
    case 27:
      exit (0);
      break;
    case 'w':
      //current_speed.increase_vector(0,0.001,0);
      current_speed.vincrease();//scale(1.2,1.2,1.2);
      playerShip.set_speed(current_speed);
      break;
    case 'a':
      //current_speed.increase_vector(-0.001,0,0);
      current_speed.rotatel();
      current_speed.scale(1.01,1.01,1.01);
      playerShip.set_speed(current_speed);
      break;
    case 's':
      //current_speed.increase_vector(0,-0.001,0);
      current_speed.vdecrease();//scale(0.8,0.8,0.8);
      playerShip.set_speed(current_speed);
      break;
    case 'd':
      //current_speed.increase_vector(0.001,0,0);
      current_speed.rotater();
      current_speed.scale(1.01,1.01,1.01);
      playerShip.set_speed(current_speed);
      break;
    case 'q':
      current_speed.scale(4,4,4);
      playerShip.set_speed(current_speed);
      break;
    case 'e':
      current_speed.scale(0.25,0.25,0.25);
      playerShip.set_speed(current_speed);
      break;
    case 'r':
      if (!playerShip.isAlive())
      {
        playerShip = *(new ship);
        vector sp(0.001,0.005,0.005,0,0);
        playerShip.set_speed(sp);
      }
      break;
  }
}

