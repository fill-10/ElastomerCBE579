#include <iostream>
#include <vector>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <assert.h>
using namespace std;
#include "Read.h"
#include "Random.h"
#include "Bead.h"
#include "Polymer.h"
#include "Box.h"
#include "VTF.h"


int main () {
	int number_of_steps;
	int printing_frequency;
	Box* box = new Box;
	Read read;
	string start_file = "/home/luohsubo/文档/CBE579/Elastomer/elastomer_input1.txt";
	read.setFileName(start_file);
	read.openFile();
	int number_of_beads;
	int number_of_stiffbeads;
	int number_of_softbeads;
	int number_of_polymers;
	int number_of_stiffpolymers;
	int number_of_softcrosslinkers;
	double diameter;
	double box_x,box_y,box_z;
	vector<double> line;
	Bead* bead;
	Polymer* polymer;
	vector <Bead*> beads_all;

	number_of_steps = read.getInt();
	printing_frequency = read.getInt();
	line = read.getLine();
	box_x = line[0];
	box_y = line[1];
	box_z = line[2];
	box->setDimensions(box_x,box_y,box_z);
	number_of_stiffbeads = read.getInt();
	number_of_softbeads = read.getInt();
	number_of_stiffpolymers =  read.getInt();
	number_of_softcrosslinkers = read.getInt();
	number_of_beads = number_of_stiffbeads+number_of_softbeads;
	number_of_polymers = number_of_stiffpolymers+number_of_softcrosslinkers;
    // load all the beads and store them in polymers
	for(int i =0; i<number_of_polymers ; i++) {
        polymer = new Polymer;
        int beads_in_polymer;
        if (i < number_of_stiffpolymers) {  beads_in_polymer = number_of_stiffbeads/number_of_stiffpolymers;    }
        else { beads_in_polymer = number_of_softbeads/number_of_softcrosslinkers;   }
        for (int j = 0 ; j< beads_in_polymer; j++) {
            diameter = read.getDouble();
            line = read.getLine6();     // read 4 to 6 numbers , first 3 numbers are position, 4th is the number of crosslinked neighbors, 5 & 6 are indices of crosslinked beads
            bead = new Bead;
            bead->setPosition(line[0],line[1],line[2]);
            bead->setDiameter(diameter);
            bead->setIndex( i * number_of_beads / number_of_polymers +j );
            // read the index(indices) of crosslinked neighbor beads
            int num_specialneighbors;
            num_specialneighbors = line[3];
            for (int k = 0; k< num_specialneighbors; k++) {
                bead->setCrosslink(line[k+4]);  // Note: line[k] is double in type, but crosslink vector in bead has type int. It works.
            }
            polymer -> addBead(bead);
            beads_all.push_back(bead); // a list of all beads, in the order of their indices. This is to convert the indices into pointers of special neighbor beads
        }
       // read and set the stiff condition
       if (i < number_of_stiffpolymers) {        polymer->ifstiff(true);  }
       else { polymer->ifstiff(false); }
       box->addPolymer(polymer);
    }
   	read.closeFile();
  	//  set up the  <vector> specialNeighbor in each bead
    for(int i =0; i<beads_all.size(); i++) {
            vector <int> crosslink_index_list;
            crosslink_index_list = beads_all[i]->getCrosslinkIndex();
            for (int j= 0; j< crosslink_index_list.size(); j++){
                beads_all[i]->addSpecialNeighbor(   beads_all[crosslink_index_list[j]  ]   );
            }
    }

   // For VTF/VMD output
	VTFFile* vtf_file;
	string visualization_output_file = "/home/luohsubo/文档/CBE579/Elastomer/elastomer_output.vtf";
	vtf_file = new VTFFile();
	vtf_file->setFileName(visualization_output_file);
	vtf_file->setBox(box);
    vtf_file->setNumberOfPolymers(number_of_polymers);
	vtf_file->writeHeader();
	vtf_file->writeBeadPositions();

	FILE* _stream = fopen("/home/luohsubo/文档/CBE579/Elastomer/output.txt", "w" );

    for(int i=0;i<number_of_steps;i++) {
		if( i%printing_frequency==0 ) {
			fprintf( _stream, "time_step: %d\n", i);
		}
		box->movePolymers();
		for(int j=0;j<number_of_polymers;j++) {
			polymer = box->getPolymer(j);
			for(int k=0;k<number_of_beads/number_of_polymers;k++) {
				bead = polymer->getBead(k);
				if(i%printing_frequency==0) {
					fprintf( _stream, "%lf\t%lf\t%lf\n", bead->getX(), bead->getY(), bead->getZ() );
				}
			}
		}
		if(i%printing_frequency==0) {
			fprintf( _stream, "\n" );
		}
		if(i%printing_frequency==0) {
			vtf_file->writeBeadPositions();
		}
	}
	fclose(_stream);
	delete vtf_file;

	return 0;
}
