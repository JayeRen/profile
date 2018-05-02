/*

  USC/Viterbi/Computer Science
  "Jello Cube" Assignment 1 starter code

*/

#include "jello.h"
#include "physics.h"

#include <vector>
#include <iostream>

using  namespace std;
/* Computes acceleration to every control point of the jello cube, 
   which is in state given by 'jello'.
   Returns result in array 'a'. */
static int isCol=0;

// computes struct force at any point
// takes point i,j,k; updates Fstruct.
void computeStructForce(struct world * jello, int i, int j, int k, struct point * Fstruct)
{
  //get the neighbour point indexes in neighbours list vector.
  vector<struct index> neighbours;
  struct index current;
  
  if(i!=0){ 
    current.i = i-1;
    current.j = j;
    current.k = k;
    neighbours.push_back(current); 
  }
  if(i!=7){   
    current.i = i+1;
    current.j = j;
    current.k = k;
    neighbours.push_back(current); 
  }
  if(j!=0){
    current.i = i;
    current.j = j-1;
    current.k = k;
    neighbours.push_back(current); 
  }
  if(j!=7){
    current.i = i;
    current.j = j+1;
    current.k = k;
    neighbours.push_back(current); 
  }
  if(k!=0){
    current.i = i;
    current.j = j;
    current.k = k-1;
    neighbours.push_back(current); 
  }
  if(k!=7){
    current.i = i;
    current.j = j;
    current.k = k+1;
    neighbours.push_back(current); 
  }
  
  vector<struct index>::iterator iter; 
  struct point F1Struct;      //force of 1 struct spring
  
  Fstruct->x = 0;
  Fstruct->y = 0;
  Fstruct->z = 0;
  
  F1Struct.x = 0;
  F1Struct.y = 0;
  F1Struct.z = 0;
  
  //  iterate through neighbours 
    //  find force exerted by neighbour(i,j,k) on jello->p(i,j,k)
  for(iter = neighbours.begin(); iter!=neighbours.end(); iter++){
    
    int ni,nj,nk; 
    //neighbour i,j,k
    ni = iter->i;
    nj = iter->j;
    nk = iter->k;
    
    //vector L from neighbour to this point    
    point Np;
    pDIFFERENCE(jello->p[i][j][k], jello->p[ni][nj][nk], Np);
    //velocity difference for damping
    point difVel;
    pDIFFERENCE(jello->v[i][j][k], jello->v[ni][nj][nk], difVel);
    //magnitude of L, abs(L)
    double lenNp;
    lenNp = sqrt((Np).x * (Np).x + (Np).y * (Np).y + (Np).z * (Np).z);
    //dot product for damping
    double vDotNp;
    DOTPRODUCT(difVel, Np, vDotNp);
    //normalize L vector
    double length;
    pNORMALIZE(Np);
    //Rest length of struct springs
    double RestLen;
    RestLen = 0.142857; //1/7
    double dX;
    dX = lenNp - RestLen;

    //force magni
    double structFM;
    structFM = -1 * jello->kElastic * dX ;
    //force vector
    point structFV;
    pMULTIPLY(Np, structFM, structFV);
    pSUM(F1Struct,structFV,F1Struct);

    //damping force
    double dampFM;
    //damp force magnitude
    dampFM = (-1 * jello -> dElastic * vDotNp) / lenNp;
    //direction of Fdamp = Fdamp * normalized L
    point dampFV;
    pMULTIPLY(Np, dampFM, dampFV);
    pSUM(F1Struct, dampFV, F1Struct);
  }
  
  Fstruct->x = F1Struct.x;
  Fstruct->y = F1Struct.y;
  Fstruct->z = F1Struct.z;
}
//compute shear force
void computeShearForce(struct world * jello, int i, int j, int k, struct point * Fshear)
{
  vector<struct index> ShNeighbours;
  struct index current;
  
  //i,j-1,k-1; i,j-1,k+1
  if(j-1>=0){
    if(k-1>=0){
      current.i = i;
      current.j = j-1;
      current.k = k-1;
      ShNeighbours.push_back(current); 
    }
    if(k+1<=7){
      current.i = i;
      current.j = j-1;
      current.k = k+1;
      ShNeighbours.push_back(current); 
    }
  }
  //i,j+1,k-1;i,j+1,k+1
  if(j+1<=7){
    if(k-1>=0){
      current.i = i;
      current.j = j+1;
      current.k = k-1;
      ShNeighbours.push_back(current); 
    }
    if(k+1<=7){
      current.i = i;
      current.j = j+1;
      current.k = k+1;
      ShNeighbours.push_back(current); 
    }
  }
  //i-1
  if(i-1>=0){   
    if(k-1>=0){   
      //i-1,k-1
      current.i = i-1;
      current.j = j;
      current.k = k-1;
      ShNeighbours.push_back(current); 
    }
    if(k+1<=7){   
      //i-1,k+1
      current.i = i-1;
      current.j = j;
      current.k = k+1;
      ShNeighbours.push_back(current); 
    }
    if(j-1>=0){
      {  //i-1,j-1,k
        current.i = i-1;
        current.j = j-1;
        current.k = k;
        ShNeighbours.push_back(current); 
      }
      if(k-1>=0){   
        //i-1,j-1,k-1
        current.i = i-1;
        current.j = j-1;
        current.k = k-1;
        ShNeighbours.push_back(current); 
      }
      if(k+1<=7){   
        //i-1,j-1,k+1
        current.i = i-1;
        current.j = j-1;
        current.k = k+1;
        ShNeighbours.push_back(current); 
      }
    }
    if(j+1<=7){  
      {   //i-1,j+1,k
        current.i = i-1;
        current.j = j+1;
        current.k = k;
        ShNeighbours.push_back(current);  
      }
      if(k-1>=0){   
        current.i = i-1;
        current.j = j+1;
        current.k = k-1;
        ShNeighbours.push_back(current);  
      }
      if(k+1<=7){   
        current.i = i-1;
        current.j = j+1;
        current.k = k+1;
        ShNeighbours.push_back(current);  
      }
    }
  }
  if(i+1<=7){
    if(k-1>=0){   
      current.i = i+1;
      current.j = j;
      current.k = k-1;
      ShNeighbours.push_back(current);  
    }
    if(k+1<=7){   
      current.i = i+1;
      current.j = j;
      current.k = k+1;
      ShNeighbours.push_back(current);  
    }
    if(j-1>=0){ 
      {   //i+1,j-1,k
        current.i = i+1;
        current.j = j-1;
        current.k = k;
        ShNeighbours.push_back(current); 
      }
      if(k-1>=0){   
        current.i = i+1;
        current.j = j-1;
        current.k = k-1;
        ShNeighbours.push_back(current); 
      }
      if(k+1<=7){   
        current.i = i+1;
        current.j = j-1;
        current.k = k+1;
        ShNeighbours.push_back(current); 
      }
    }
    if(j+1<=7){ 
      {   //i+1,j+1,k
        current.i = i+1;
        current.j = j+1;
        current.k = k;
        ShNeighbours.push_back(current); 
      }
      if(k-1>=0){   
        current.i = i+1;
        current.j = j+1;
        current.k = k-1;
        ShNeighbours.push_back(current); 
      }
      if(k+1<=7){   
        current.i = i+1;
        current.j = j+1;
        current.k = k+1;
        ShNeighbours.push_back(current); 
      }
    }
  }

  vector<struct index>::iterator iter; 
  point F1Shear;
  
  Fshear->x = 0;
  Fshear->y = 0;
  Fshear->z = 0;
  
  F1Shear.x = 0;
  F1Shear.y = 0;
  F1Shear.z = 0;
  
  //iterate through shear neighbours
  for (iter = ShNeighbours.begin(); iter!=ShNeighbours.end(); iter++){
    int ni,nj,nk; 
    ni = iter->i;
    nj = iter->j;
    nk = iter->k;
    
    point Sp;
    pDIFFERENCE(jello->p[i][j][k],jello->p[ni][nj][nk],Sp );
    
    //velocity difference for damping
    point difVel;
    pDIFFERENCE(jello->v[i][j][k],jello->v[ni][nj][nk],difVel );
    
    //magnitude of L, len(Sp)
    double lenSp;
    lenSp = sqrt((Sp).x * (Sp).x + (Sp).y * (Sp).y + (Sp).z * (Sp).z);
    
    //dot product for damping
    double vDotSp;
    DOTPRODUCT(difVel, Sp, vDotSp);
    
    //normalize point Sp
    double length;
    pNORMALIZE(Sp);    
    
    //Rest length of shear springs
    double RestShlen1,RestShlen2;
               RestShlen1 = sqrt(2) * 0.142857; //square diagonal
               RestShlen2 = sqrt(3) * 0.142857; //cube diagonal
    
    double dX;
    if((abs(i-ni)==1)&&(abs(j-nj)==1)&&(abs(k-nk)==1)) //cube diagonal
      dX = lenSp - RestShlen2;
    else
      dX = lenSp - RestShlen1;
    
    //force magnitude
    double shFM;
    shFM = -1 * jello->kElastic * dX ;
    //force vector
    point shFV;
    pMULTIPLY(Sp, shFM, shFV); 
    //sum
    pSUM(F1Shear,shFV,F1Shear); //Fstruct is final total strurct force on the point
  
    //damping force
    double dampFM;
    //damp force magnitude
    dampFM = (-1 * jello->dElastic * vDotSp) / lenSp;
    //Direction of Fdamp = Fdamp1 * normalized L1
    point dampFV;
    pMULTIPLY(Sp, dampFM, dampFV);  
    //add Fdamp to Fstruct
    pSUM(F1Shear, dampFV, F1Shear);
  }

  Fshear->x = F1Shear.x;
  Fshear->y = F1Shear.y;
  Fshear->z = F1Shear.z;
}
//computebendForce
void computeBendForce(struct world * jello, int i, int j, int k, struct point * Fbend) //current point i, j, k
{
  vector<struct index> Bneighbours;
  struct index current;
  
  if(i>=2){
       current.i = i-2;
       current.j = j;
       current.k = k;
       Bneighbours.push_back(current); 
  }
  if(i<=5){
       current.i = i+2;
       current.j = j;
       current.k = k;
       Bneighbours.push_back(current); 
  }
  if(j>=2){
       current.i = i;
       current.j = j-2;
       current.k = k;
       Bneighbours.push_back(current); 
      }
  if(j<=5){
       current.i = i;
       current.j = j+2;
       current.k = k;
       Bneighbours.push_back(current); 
  }
  if(k>=2){
       current.i = i;
       current.j = j;
       current.k = k-2;
       Bneighbours.push_back(current); 
  }
  if(k<=5){
       current.i = i;
       current.j = j;
       current.k = k+2;
       Bneighbours.push_back(current); 
  }
  
  vector<struct index>::iterator iter; 
  Fbend->x = 0;
  Fbend->y = 0;
  Fbend->z = 0;
  
  point F1Bend;  
  F1Bend.x = 0;
  F1Bend.y = 0;
  F1Bend.z = 0;
  
  //iterate through bend neighbours
  for (iter = Bneighbours.begin(); iter!=Bneighbours.end(); iter++){
    //neighbour i,j,k
    int ni,nj,nk; 
    ni = iter->i;
    nj = iter->j;
    nk = iter->k;
    //L vector
    point Bp;
    pDIFFERENCE(jello->p[i][j][k],jello->p[ni][nj][nk],Bp );
    //v dif, damping
    point difVel;
    pDIFFERENCE(jello->v[i][j][k],jello->v[ni][nj][nk],difVel );
    
    //magnitude of L, abs(L)
    double lenBp;
    lenBp = sqrt((Bp).x * (Bp).x + (Bp).y * (Bp).y + (Bp).z * (Bp).z);
    //dot product for damping
    double vDotBp;
    DOTPRODUCT(difVel, Bp, vDotBp);   
    //normalize L1 
    double length;
    pNORMALIZE(Bp);   
    //Rest length of bend springs
    double RestBlen;
    RestBlen = 2 * 0.142857;
    //dx for Hooks law
    double dX;
    dX = lenBp - RestBlen;
    
    //force magnitude
    double bendFM;
    bendFM = -1 * jello->kElastic * dX ; 
    //Force vector
    point bendFV;
    pMULTIPLY(Bp, bendFM, bendFV);   
    //add up single forces to Fstruct
    pSUM(F1Bend,bendFV,F1Bend); //Fstruct is final total strurct force on the point  
    
    //damping force
    double dampFM;
    //damp force magnitude
    dampFM = (-1 * jello->dElastic * vDotBp) / lenBp;
    //Direction of Fdamp = Fdamp1 * normalized L1
    point dampFV;
    pMULTIPLY(Bp, dampFM, dampFV);
    //add Fdamp to Fstruct
    pSUM(F1Bend, dampFV, F1Bend);
  } 
  Fbend->x = F1Bend.x;
  Fbend->y = F1Bend.y;
  Fbend->z = F1Bend.z;
}
//compute externalForce
void computeExternalForce(struct world * jello, int pti, int ptj, int ptk, struct point * Fext)
{
  double px  , py  , pz   ,
         EFx0, EFy0, EFz0 ,
         EFx1, EFy1, EFz1 ;
    
  point  EFxyz  [2][2][2],
         FVector[2][2][2];
    
  int    found = 0;
  
  //point x, y, z.
  px = jello->p[pti][ptj][ptk].x;
  py = jello->p[pti][ptj][ptk].y;
  pz = jello->p[pti][ptj][ptk].z;
  
    
  int i,j,k;
  for (i=0; i< jello->resolution; i++){
    EFx0 = -2 + 4 * (1.0 * i     / ( jello->resolution-1));
    EFx1 = -2 + 4 * (1.0 * (i+1) / ( jello->resolution-1));
    
    if ((px>=EFx0) && (px<=EFx1)) break;
  }
  for (j=0; j< jello->resolution; j++){
    EFy0 = -2 + 4 * (1.0 * j   / (jello->resolution-1));
    EFy1 = -2 + 4 * (1.0 *(j+1)/ (jello->resolution-1));
  
    if ((py>=EFy0) && (py<=EFy1)) break;
  }
  for (k=0; k< jello->resolution; k++){
    EFz0 = -2 + 4 * (1.0 * k     / (jello->resolution-1));
    EFz1 = -2 + 4 * (1.0 * (k+1) / (jello->resolution-1));
    
    if((pz>=EFz0)&&(pz<=EFz1)) break;
  }
    
  // corner forces, force inside the cube.
  if((i<jello->resolution-1)&&(j<jello->resolution-1)&&(k<jello->resolution-1)) // point inside force field
  {
    int a,b,c,d;
    a = i * jello->resolution * jello->resolution;
    b = j * jello->resolution;
    c = (j + 1) * jello->resolution;    
    d = (i + 1) * jello->resolution * jello->resolution;
    
    //8 vector coord 
    FVector[0][0][0].x = jello->forceField[a + b + k].x;
    FVector[0][0][0].y = jello->forceField[a + b + k].y;
    FVector[0][0][0].z = jello->forceField[a + b + k].z;
    
    FVector[0][0][1].x = jello->forceField[a + b + (k+1)].x;
    FVector[0][0][1].y = jello->forceField[a + b + (k+1)].y;
    FVector[0][0][1].z = jello->forceField[a + b + (k+1)].z;
    
    FVector[0][1][0].x = jello->forceField[a + c + k].x;
    FVector[0][1][0].y = jello->forceField[a + c + k].y;
    FVector[0][1][0].z = jello->forceField[a + c + k].z;
    
    FVector[0][1][1].x = jello->forceField[a + c + (k+1)].x;
    FVector[0][1][1].y = jello->forceField[a + c + (k+1)].y;
    FVector[0][1][1].z = jello->forceField[a + c + (k+1)].z;
    
    FVector[1][0][0].x = jello->forceField[d + b + k].x;
    FVector[1][0][0].y = jello->forceField[d + b + k].y;
    FVector[1][0][0].z = jello->forceField[d + b + k].z;
    
    FVector[1][0][1].x = jello->forceField[d + b + (k+1)].x;
    FVector[1][0][1].y = jello->forceField[d + b + (k+1)].y;
    FVector[1][0][1].z = jello->forceField[d + b + (k+1)].z;
    
    FVector[1][1][0].x = jello->forceField[d + c + k].x;
    FVector[1][1][0].y = jello->forceField[d + c + k].y;
    FVector[1][1][0].z = jello->forceField[d + c + k].z;
    
    FVector[1][1][1].x = jello->forceField[d + c + (k+1)].x;
    FVector[1][1][1].y = jello->forceField[d + c + (k+1)].y;
    FVector[1][1][1].z = jello->forceField[d + c + (k+1)].z;
    
    //trilinear interp
    double alpha,beta,gama,GL;
    alpha = px   - EFx0;
    beta  = py   - EFy0;
    gama  = pz   - EFz0;
    GL    = EFx1 - EFx0;
    
    double GLaGL, GLbGL, GLgGL, 
             aGL,   bGL,   gGL;
    GLaGL = (GL - alpha)/GL ;
    GLbGL = (GL - beta)/GL ;
    GLgGL = (GL - gama)/GL ;
    
    aGL = alpha / GL;
    bGL =  beta / GL;
    gGL =  gama / GL;
    
    //interp
    point Fpoint;
    Fpoint.x = FVector[0][0][0].x * GLaGL * GLbGL * GLgGL +
               FVector[1][0][0].x * aGL   * GLbGL * GLgGL +
               FVector[0][1][0].x * GLaGL * bGL   * GLgGL +
               FVector[0][0][1].x * GLaGL * GLbGL * gGL   +
               FVector[1][0][1].x * aGL   * GLbGL * gGL   +
               FVector[0][1][1].x * GLaGL * bGL   * gGL   +
               FVector[1][1][0].x * aGL   * bGL   * GLgGL +
               FVector[1][1][1].x * aGL   * bGL   * gGL   ; 
    
    Fpoint.y = FVector[0][0][0].y * GLaGL * GLbGL * GLgGL +
               FVector[1][0][0].y * aGL   * GLbGL * GLgGL +
               FVector[0][1][0].y * GLaGL * bGL   * GLgGL +
               FVector[0][0][1].y * GLaGL * GLbGL * gGL   +
               FVector[1][0][1].y * aGL   * GLbGL * gGL   +
               FVector[0][1][1].y * GLaGL * bGL   * gGL   +
               FVector[1][1][0].y * aGL   * bGL   * GLgGL +
               FVector[1][1][1].y * aGL   * bGL   * gGL   ; 
    
    Fpoint.z = FVector[0][0][0].z * GLaGL * GLbGL * GLgGL +
               FVector[1][0][0].z * aGL   * GLbGL * GLgGL +
               FVector[0][1][0].z * GLaGL * bGL   * GLgGL +
               FVector[0][0][1].z * GLaGL * GLbGL * gGL   +
               FVector[1][0][1].z * aGL   * GLbGL * gGL   +
               FVector[0][1][1].z * GLaGL * bGL   * gGL   +
               FVector[1][1][0].z * aGL   * bGL   * GLgGL +
               FVector[1][1][1].z * aGL   * bGL   * gGL   ; 
    
    Fext->x  = Fpoint.x;
    Fext->y  = Fpoint.y;
    Fext->z  = Fpoint.z;
    
    }
}


void computeAcceleration(struct world * jello, struct point a[8][8][8])
{
  /* for you to implement ... */
  for(int i =0; i<8; i++){
      for(int j =0; j<8; j++){
        for(int k =0; k<8; k++){
            //Fstruct, Fshear, Fbend, spring force  
            point Fstruct;
            computeStructForce(jello, i, j, k, &Fstruct);
            point Fshear;
            computeShearForce(jello, i, j, k, &Fshear);
            point Fbend;
            computeBendForce(jello, i, j, k, &Fbend);
            //ex forces   
            point Fexternal;  
            computeExternalForce(jello, i, j, k, &Fexternal); 
            //Total force:str+sh+b+ex
            point totalF;
            pSUM(Fstruct, Fshear, totalF);
            pSUM(totalF, Fbend, totalF);
            pSUM(totalF, Fexternal, totalF);
  
            //check collission w/ box
            //point inside box
            if(jello->inCube[i][j][k]){ 
               isCol = 0; 
               //collision?   
               if((jello->p[i][j][k].x<=-2)||(jello->p[i][j][k].x>=2) || 
                    (jello->p[i][j][k].y<=-2)||(jello->p[i][j][k].y>=2) || 
                    (jello->p[i][j][k].z<=-2)||(jello->p[i][j][k].z>=2)  ){
                     //collided - point change to outside  
                     jello->inCube[i][j][k] = false;             
                     isCol++;
                     //coll point: first collision
                     if (isCol==1){   
                        jello->pCol[i][j][k].x =jello->p[i][j][k].x;   
                        jello->pCol[i][j][k].y =jello->p[i][j][k].y;   
                        jello->pCol[i][j][k].z =jello->p[i][j][k].z;
                    }
                }
            }
            //point outside box
            else{
               //check inside now?
                if((jello->p[i][j][k].x>=-2) && (jello->p[i][j][k].x<=2)&&
                  (jello->p[i][j][k].y>=-2) && (jello->p[i][j][k].y<=2)&&
                  (jello->p[i][j][k].z>=-2) && (jello->p[i][j][k].z<=2)){   
                    jello->inCube[i][j][k] = true;
                }
                //point outside now, find force after col
                //calcu spring force btw surface - cur position
                //pCol[0][0][0] - current p[0][0][0]
                else{
                  point L;
                  pDIFFERENCE(jello->p[i][j][k],jello->pCol[i][j][k],L );
                
                  //velocity dif for damping
                  point difVel, vPCol;
                  pMAKE(0, 0, 0, vPCol);
                  pDIFFERENCE(jello->v[i][j][k],vPCol,difVel );
              
                  //len of L
                  double lenL;
                  lenL = sqrt((L).x * (L).x + (L).y * (L).y + (L).z * (L).z);
                
                  //normal vector
                  struct point floorN; //normal to floor
                  if(jello->p[i][j][k].x<=-2){
                    pMAKE(1,0,0,floorN);
                  }
                  else if(jello->p[i][j][k].x>=2){
                    pMAKE(-1,0,0,floorN);
                  }
                  else if(jello->p[i][j][k].y<=-2){
                    pMAKE(0,1,0,floorN);
                  }
                  else if(jello->p[i][j][k].y>=2){
                    pMAKE(0,-1,0,floorN);
                  } 
                  else if(jello->p[i][j][k].z<=-2){
                    pMAKE(0,0,1,floorN);
                  }
                  else if(jello->p[i][j][k].z>=2){
                    pMAKE(0,0,-1,floorN);
                  }
            
                  //dot product for damping
                  double vDotL;
                  DOTPRODUCT(difVel, L, vDotL);
                  //collision force magnitude    
                  double colFM; 
                  colFM = jello-> kCollision * lenL;
                  //collision force vector
                  point colFV;
                  pMULTIPLY(floorN, colFM, colFV);
                  //damping force magnitude
                  double colFD;
                  colFD = ( jello-> dCollision * vDotL) / lenL;
                  //direction of Fdamp = Fdamp1 * normal of floor
                  point colFDDir;
                  pMULTIPLY(floorN, colFD , colFDDir);
                  pSUM(colFV , colFDDir, colFV);       
                  //total force += col F
                  pSUM(totalF, colFV, totalF);
                } 
            } 
            point acc;
            pMULTIPLY( totalF, (1/jello->mass), acc);
            // Keep the system stable by cutting the forces in half when it goes beyond control 
            if((totalF.x>70)||(totalF.x<-70)) totalF.x/=2;             
            if((totalF.y>70)||(totalF.y<-70)) totalF.y/=2;  
            if((totalF.z>70)||(totalF.z<-70)) totalF.z/=2;
            //Update a[][][] matrix
            a[i][j][k] = acc;
          }
      }
  }
}

/* performs one step of Euler Integration */
/* as a result, updates the jello structure */
void Euler(struct world * jello)
{
  int i,j,k;
  point a[8][8][8];

  computeAcceleration(jello, a);
  
  for (i=0; i<=7; i++)
    for (j=0; j<=7; j++)
      for (k=0; k<=7; k++)
      {
        jello->p[i][j][k].x += jello->dt * jello->v[i][j][k].x;
        jello->p[i][j][k].y += jello->dt * jello->v[i][j][k].y;
        jello->p[i][j][k].z += jello->dt * jello->v[i][j][k].z;
        jello->v[i][j][k].x += jello->dt * a[i][j][k].x;
        jello->v[i][j][k].y += jello->dt * a[i][j][k].y;
        jello->v[i][j][k].z += jello->dt * a[i][j][k].z;

      }
}

/* performs one step of RK4 Integration */
/* as a result, updates the jello structure */
void RK4(struct world * jello)
{
  point F1p[8][8][8], F1v[8][8][8], 
        F2p[8][8][8], F2v[8][8][8],
        F3p[8][8][8], F3v[8][8][8],
        F4p[8][8][8], F4v[8][8][8];

  point a[8][8][8];


  struct world buffer;

  int i,j,k;

  buffer = *jello; // make a copy of jello

  computeAcceleration(jello, a);

  for (i=0; i<=7; i++)
    for (j=0; j<=7; j++)
      for (k=0; k<=7; k++)
      {
         pMULTIPLY(jello->v[i][j][k],jello->dt,F1p[i][j][k]);
         pMULTIPLY(a[i][j][k],jello->dt,F1v[i][j][k]);
         pMULTIPLY(F1p[i][j][k],0.5,buffer.p[i][j][k]);
         pMULTIPLY(F1v[i][j][k],0.5,buffer.v[i][j][k]);
         pSUM(jello->p[i][j][k],buffer.p[i][j][k],buffer.p[i][j][k]);
         pSUM(jello->v[i][j][k],buffer.v[i][j][k],buffer.v[i][j][k]);
      }

  computeAcceleration(&buffer, a);

  for (i=0; i<=7; i++)
    for (j=0; j<=7; j++)
      for (k=0; k<=7; k++)
      {
         // F2p = dt * buffer.v;
         pMULTIPLY(buffer.v[i][j][k],jello->dt,F2p[i][j][k]);
         // F2v = dt * a(buffer.p,buffer.v);     
         pMULTIPLY(a[i][j][k],jello->dt,F2v[i][j][k]);
         pMULTIPLY(F2p[i][j][k],0.5,buffer.p[i][j][k]);
         pMULTIPLY(F2v[i][j][k],0.5,buffer.v[i][j][k]);
         pSUM(jello->p[i][j][k],buffer.p[i][j][k],buffer.p[i][j][k]);
         pSUM(jello->v[i][j][k],buffer.v[i][j][k],buffer.v[i][j][k]);
      }

  computeAcceleration(&buffer, a);

  for (i=0; i<=7; i++)
    for (j=0; j<=7; j++)
      for (k=0; k<=7; k++)
      {
         // F3p = dt * buffer.v;
         pMULTIPLY(buffer.v[i][j][k],jello->dt,F3p[i][j][k]);
         // F3v = dt * a(buffer.p,buffer.v);     
         pMULTIPLY(a[i][j][k],jello->dt,F3v[i][j][k]);
         pMULTIPLY(F3p[i][j][k],0.5,buffer.p[i][j][k]);
         pMULTIPLY(F3v[i][j][k],0.5,buffer.v[i][j][k]);
         pSUM(jello->p[i][j][k],buffer.p[i][j][k],buffer.p[i][j][k]);
         pSUM(jello->v[i][j][k],buffer.v[i][j][k],buffer.v[i][j][k]);
      }
         
  computeAcceleration(&buffer, a);


  for (i=0; i<=7; i++)
    for (j=0; j<=7; j++)
      for (k=0; k<=7; k++)
      {
         // F3p = dt * buffer.v;
         pMULTIPLY(buffer.v[i][j][k],jello->dt,F4p[i][j][k]);
         // F3v = dt * a(buffer.p,buffer.v);     
         pMULTIPLY(a[i][j][k],jello->dt,F4v[i][j][k]);

         pMULTIPLY(F2p[i][j][k],2,buffer.p[i][j][k]);
         pMULTIPLY(F3p[i][j][k],2,buffer.v[i][j][k]);
         pSUM(buffer.p[i][j][k],buffer.v[i][j][k],buffer.p[i][j][k]);
         pSUM(buffer.p[i][j][k],F1p[i][j][k],buffer.p[i][j][k]);
         pSUM(buffer.p[i][j][k],F4p[i][j][k],buffer.p[i][j][k]);
         pMULTIPLY(buffer.p[i][j][k],1.0 / 6,buffer.p[i][j][k]);
         pSUM(buffer.p[i][j][k],jello->p[i][j][k],jello->p[i][j][k]);

         pMULTIPLY(F2v[i][j][k],2,buffer.p[i][j][k]);
         pMULTIPLY(F3v[i][j][k],2,buffer.v[i][j][k]);
         pSUM(buffer.p[i][j][k],buffer.v[i][j][k],buffer.p[i][j][k]);
         pSUM(buffer.p[i][j][k],F1v[i][j][k],buffer.p[i][j][k]);
         pSUM(buffer.p[i][j][k],F4v[i][j][k],buffer.p[i][j][k]);
         pMULTIPLY(buffer.p[i][j][k],1.0 / 6,buffer.p[i][j][k]);
         pSUM(buffer.p[i][j][k],jello->v[i][j][k],jello->v[i][j][k]);
      }

  return;  
}
