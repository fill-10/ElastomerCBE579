#include <vector>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <math.h>
using namespace std;
#include "Random.h"
#include "Bead.h"
#include "Polymer.h"
#include "Box.h"


void Box::addPolymer(Polymer* pol) {
    polymers.push_back(pol);
}

void Box::movePolymers(){
	for (int i=0 ; i<polymers.size();i++) {
		for (int j = 0; j<polymers[i]->size(); j++ ) {
			polymers[i] ->move(j);
			if (checkPolymerMove(i,j)) {
				acceptPolymerMove();
			}
			else {
				rejectPolymerMove(i, j );
			}
		}
    }
}

bool Box::checkPolymerMove(int polymer_index, int bead_index)  {    //Here, bead_index is only a local variable but not the  'index'/SetIndex  in class Bead.
    double distanceA, distanceB, angle;
    Bead* bead_moved;
    Bead* bead_next, *bead_pre;// bead pre and bead next
    Bead* bead_prepre;
    Bead* bead_nextnext;    // bead pre pre, bead next next
    bead_moved = polymers[polymer_index] ->getBead(bead_index);
    periodic(bead_moved); // apply preriodic BC
    if (positiveZ(polymers[polymer_index], bead_moved)) {
        return false;
    }

// Neighbours
    if (bead_index == 0 ) {
        bead_next = polymers[polymer_index] ->getBead(bead_index+1);
        distanceA = periodicDistance(bead_next, bead_moved);
        if (distanceA < (bead_next->getDiameter()/2.0   + bead_moved -> getDiameter()/2.0) )    return false;
        if (distanceA > 1.2)       return false;

        if (polymers[polymer_index]->getStiff()) {  //angle constrain
            bead_nextnext = polymers[polymer_index] ->getBead(bead_index+2);
            angle = calAngle(bead_moved, bead_next, bead_nextnext);
            if (angle < 3.0) return false;
        }
    }
    else if (bead_index== polymers[polymer_index]->size()-1) {
        bead_pre = polymers[polymer_index] ->getBead(bead_index-1);

        distanceA = periodicDistance(bead_pre, bead_moved);
        if (distanceA < (bead_pre->getDiameter()/2.0 +bead_moved -> getDiameter()/2.0) )    return false;
        if (distanceA >1.2)       return false;

        if (polymers[polymer_index]->getStiff()) {  //angle constrain
            bead_prepre = polymers[polymer_index] ->getBead(bead_index-2);
           angle = calAngle(bead_moved, bead_pre, bead_prepre);
            if (angle < 3.0) return false;
        }
    }
    else   {
        bead_pre =  polymers[polymer_index] ->getBead(bead_index-1);
        bead_next = polymers[polymer_index] ->getBead(bead_index+1);
        distanceA = periodicDistance(bead_pre, bead_moved);
        distanceB = periodicDistance(bead_next, bead_moved);
        if (distanceA < (bead_pre->getDiameter()/2.0 +bead_moved -> getDiameter()/2.0) )    return false;
        if (distanceA >1.2)       return false;
        if (distanceB < (bead_next->getDiameter()/2.0 +bead_moved -> getDiameter()/2.0) )    return false;
        if (distanceB >1.2)       return false;
     // The constrain for stiff polymers
        if (polymers[polymer_index]->getStiff()) {
            angle = calAngle(bead_pre, bead_moved, bead_next);
            if (angle < 3.0)  return false;

            if  (bead_index== 1){
                bead_nextnext = polymers[polymer_index] ->getBead(bead_index+2);
                angle = calAngle(bead_moved, bead_next, bead_nextnext);
                if (angle < 3.0)  return false;
            }
            else if (bead_index == polymers[polymer_index]->size()-2 ){
                bead_prepre =  polymers[polymer_index] ->getBead(bead_index-2);
                angle = calAngle(bead_moved, bead_pre, bead_prepre);
                if (angle < 3.0)  return false;
            }
            else {
                bead_nextnext = polymers[polymer_index] ->getBead(bead_index+2);
                bead_prepre =  polymers[polymer_index] ->getBead(bead_index-2);
                angle = calAngle(bead_moved, bead_pre, bead_prepre);
                if (angle < 3.0)  return false;
                angle = calAngle(bead_moved, bead_next, bead_nextnext);
                if (angle < 3.0)  return false;
            }
        }
    }
    //crosslinked special neighbor
    vector <Bead*> CrosslinkedNeighbors;
    CrosslinkedNeighbors = bead_moved->getSpecialNeighbors();
    for (int i=0 ; i < CrosslinkedNeighbors.size(); i++)
    {
        if (periodicDistance(bead_moved, CrosslinkedNeighbors[i]) > 1.2 ) {return false;}
    }
    //non neighbour
    Bead* bead;
  	for (int i=0 ; i<polymers.size();i++) {
		for (int j = 0; j<polymers[i]->size(); j++ ) {
            if (i != polymer_index && j != bead_index)  {
                bead = polymers[i] -> getBead(j);
            if( periodicDistance(bead, bead_moved)  < (bead->getDiameter()/2.0 + bead_moved->getDiameter()/2.0) )
                    return false;
            }
        }
    }
return true;
}

void Box::rejectPolymerMove(int i , int j){
    polymers[i]->moveBack(j) ;
}

void Box::periodic(Bead* b ) {
    if(b->getX() <   -x/2.0)   b->setPosition(b->getX()+x,     b->getY(),           b->getZ()          );
    if(b->getY() <   -y/2.0)   b->setPosition(b->getX(),          b->getY()+y,      b->getZ()          );
	if(b->getZ() <   -z/2.0)   b->setPosition(b->getX(),          b->getY(),           b->getZ()+z      );
	if(b->getX() >= x/2.0)    b->setPosition(b->getX()-x,      b->getY(),            b->getZ()          );
	if(b->getY() >= y/2.0)    b->setPosition(b->getX(),          b->getY()-y,        b->getZ()          );
	if(b->getZ() >= z/2.0)    b->setPosition(b->getX(),          b->getY(),            b->getZ()-z       );
}
double Box::periodicDistance(Bead* B1, Bead* B2 ) {
    double dx, dy, dz;
    dx = fabs (B1->getX() - B2->getX());
    dy = fabs (B1->getY() - B2->getY());
    dz = fabs (B1->getZ() - B2->getZ());
    if(dx > 0.5*x) { dx =   x-    dx; }
    if(dy > 0.5*y) { dy =   y-    dy ; }
    if(dz > 0.5*z) { dz =    z-    dz; }
    return sqrt(pow (dx,2.0)+ pow(dy,2.0)+pow(dz, 2.0));
}

double Box::calAngle(Bead* B_p, Bead* B_m, Bead* B_n)   {
    double dist_A, dist_B, dist_C, angle_cal;
    dist_A = periodicDistance(B_p, B_m);
    dist_B = periodicDistance(B_m, B_n);
    dist_C = periodicDistance(B_p, B_n);
    angle_cal  = acos ( (pow(dist_A, 2.0) + pow(dist_B, 2.0) - pow(dist_C,2.0))/(2* dist_A *dist_B) );
    return angle_cal;
}

bool Box::positiveZ(Polymer* P, Bead* BB){
      if (P->getStiff() ) {
        BB->setPosition(BB->getX(),  BB->getY(),   0);
        return false;
    }
    else if (BB->getZ()>=0) {return false;}
    else return true;
}
