class Box {
public:
	void setPolymers(vector<Polymer*> pol) {
		polymers = pol;
	}
	vector<Polymer*> getPolymers() { return polymers; }
    Polymer* getPolymer(int i) {
		assert(i<=polymers.size());
		return polymers[i];
	}
	void setDimensions(double _x,double _y,double _z) {
		x = _x;
		y = _y;
		z = _z;
	}
    double getX() { return x; }
	double getY() { return y; }
	double getZ() { return z; }

	void addPolymer(Polymer*);
    void movePolymers();

private:
    bool checkPolymerMove(int, int);
    void periodic(Bead*);
    double periodicDistance(Bead*, Bead*);
    double calAngle(Bead*, Bead*, Bead*);
    void acceptPolymerMove() { }; // do nothing
	void rejectPolymerMove(int, int);
	bool positiveZ(Polymer*, Bead*);
private:
	vector<Polymer*> polymers;
	double x,y,z;
};
