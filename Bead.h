class Bead {
public:
	void setPosition(double _x, double _y, double _z) { x = _x; y = _y; z = _z; }
	void setDiameter(double d) { diameter = d; }
	void setIndex(int _index) { index = _index; }
	double getX() { return x; }
	double getY() { return y; }
	double getZ() { return z; }
	double getDiameter() { return diameter; }
	int getIndex() { return index; }
	int crosslinksize(){ return crosslink.size();}
	void setCrosslink(int i){ crosslink.push_back(i);	}
	vector <int> getCrosslinkIndex(){ return crosslink;}
    void addSpecialNeighbor(Bead* b) { specialNeighbors.push_back(b); }
    int Num_SpecialNeighbors() {return specialNeighbors.size();}
    vector <Bead* >getSpecialNeighbors() { return specialNeighbors;}
public:
	void move() {
		x_old = x;
		y_old = y;
		z_old = z;
		double delta_x,delta_y,delta_z;
		delta_x = random.get()*diameter;
		delta_y = random.get()*diameter;
		delta_z = random.get()*diameter;
        if(random.get()>0.5) { x = x + delta_x; }
		else { x = x - delta_x; }
		if(random.get()>0.5) { y = y + delta_y; }
		else { y = y - delta_y; }
		if(random.get()>0.5) { z = z + delta_z; }
		else { z = z - delta_z; }
	}
	void goBack() {
		x = x_old;
		y = y_old;
		z = z_old;
	}

private:
	double x,y,z;
	double x_old,y_old,z_old;
	double diameter;
	Random random;
	int index;
	vector <int> crosslink; // the indices of crosslinked special neighbors
    vector <Bead*> specialNeighbors;
};
