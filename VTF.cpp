#include <iostream>
#include <time.h>
#include <vector>
#include <math.h>
#include <assert.h>
using namespace std;
#include "Random.h"
#include "Bead.h"
#include "Polymer.h"
#include "Box.h"
#include "VTF.h"

void VTFFile::writeHeader()
{
	Bead* bead;
	vector<Bead*> beads;
	// Just get the first bead
	bead = polymers[0]->getBead(0);
	stream = fopen(filename.c_str(), "w");
	assert(stream);
	fprintf(stream, "unitcell %lf %lf %lf\n", box->getX(), box->getY(), box->getZ());
	for(int i=0;i<polymers.size();i++) {
		beads = polymers[i]->getBeads();
		fprintf(stream, "atom %d:%d\tradius %lf segid %d\n", beads[0]->getIndex(), beads[0]->getIndex()+(beads.size()-1), beads[0]->getDiameter()/2.0, i);
	}
}

void VTFFile::writeBeadPositions()
{
	Bead* bead;
	fprintf(stream, "\ntimestep\n");
	fprintf(stream, "unitcell %lf %lf %lf\n", box->getX(), box->getY(), box->getZ());
	for(int i=0;i<polymers.size();i++) {
		for(int j=0;j<polymers[i]->size();j++) {
			bead = polymers[i]->getBead(j);
			fprintf(stream, "%lf\t %lf\t %lf\n", bead->getX(), bead->getY(), bead->getZ());
		}
	}
}

VTFFile::~VTFFile()
{
	fclose(stream);
}
