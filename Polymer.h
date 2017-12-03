class Polymer {
    public:
        void addBead(Bead *b){
            beads.push_back(b);
            }
        vector <Bead*> getBeads() {return beads;}

        Bead* getBead(int i) {
            assert(i<=beads.size());
            return beads[i];
        }

        void move(int i) {
            assert(i<=beads.size());
            beads[i] -> move();
	}

	void moveBack (int i){
 		assert(i<=beads.size());
		beads[i] -> goBack();
	}

	int size () { return beads.size(); }

	void ifstiff(bool b)  { if (b) stiff =true; else stiff = false ;}
	bool getStiff() { return stiff;};

    private:
        vector<Bead*>  beads;
        bool stiff;
};
