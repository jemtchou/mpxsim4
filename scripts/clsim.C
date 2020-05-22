class pixel;
class cluster;
typedef vector <cluster> frame;

// integrate charge in a pixel
// (x,y - center position wrt pixel center in px [=0.5, 0.5], sigma in px)
// i,j - px number, (0,0) - central pixel

double integrate_gaussian(double x, double y, double sigma, int i, int j)
{
  if(fabs(x) > 0.5 || fabs(y)>0.5) return 0;
  
  double px = 1;
  double x1 = i*px - px/2;
  double x2 = i*px + px/2;
  double y1 = j*px - px/2;
  double y2 = j*px + px/2;
  
  double sigma2 = sigma*sigma;

  double N = 100;

  double h = px/N;
  
  double integral = 0;
  for(auto l=0; l<N; l++)
    {
      for(auto m=0; m<N; m++)
	{
	  double xi = x1 + l*h + h/2;
	  double yi = y1 + m*h + h/2;
	  
	  double f = exp(-((xi - x)*(xi-x) + (yi - y)*(yi - y))/2/sigma2);

	  integral += f*h*h;
	}
    }

  return integral/2/3.14159/sigma2;
}

// ========= Pixel =========
class pixel
{
public:
  pixel(int i, int j, float a = 1.)
  {
    X = i;
    Y = j;
    Amp = a;
  }
    
  ~pixel(){}

  int x() const {return X;} 
  int y() const {return Y;} 

  float amp() const {return Amp;}

  void amp(float amp){Amp = amp;}
  
  friend bool operator== (const pixel& a, const pixel& b)
  {
    if (a.x() == b.x() && a.y() == b.y()) return true;
    return false;
  }

  friend std::ostream& operator<< (std::ostream &out, const pixel &p)
  {
    out << "[" << p.x() << "," << p.y() << "," << p.amp() << "]"; 
    return out;
  }

private:
  int X;
  int Y;
  float Amp;
};

// ========== Cluster =========
class cluster
{
public:
  cluster()
  {
    px.clear();
  }

  ~cluster()
  {
    px.clear();
  }

  void add(pixel& p)
  {
    px.push_back(p);
  }

  void add(cluster& c)
  {
    for(auto x: c.pxv())
      {
        auto it = std::find(px.begin(), px.end(), x);
        if( it != px.end()) // pixels exists already
          (*it).amp(x.amp()+(*it).amp()); // add signal
        else
          px.push_back(x);
      }
  }

  void remove(pixel& p)
  {
    auto pend = std::remove(px.begin(), px.end(), p);
    px.erase(pend, px.end());
  }
  
  int size() const
  {
    return px.size();
 }

  void clear()
  {
    px.clear();
  }

  friend bool operator== (const cluster& a, const cluster& b)
  {
    if(a.size() != b.size()) return false;
    for (auto& p: a.pxv())
      {
        // pixel of a not found in b
        if (std::find(b.pxv().begin(), b.pxv().end(), p) == b.pxv().end() )
          return false;
      }
    // a and b have the same size and all pixels of a are present in b
    return true;
  }
  
  friend std::ostream& operator<< (std::ostream &out, const cluster &c)
  {
    for(auto& p : c.pxv()) out << p << " ";
    out << std::endl;
    return out;
  }
 const vector<pixel>& pxv() const {return px;}
  
private:
  vector <pixel> px;
};

// ====== Detector ======

class pxdet
{
  // pixels [0, nmax - 1], -1 - underflow, nmax - overflow
  
public:
  pxdet()
  {
    x0 = 0; // coords of the detector center
    y0 = 0; //
    nxmax = 256; // number of pixels in x
    nymax = 256; // ---------------- in y
    lenx = 14.1; // detector size in x [mm]
    leny = 14.1; // ------------- in y [mm]
 }

  ~pxdet(){}
  
  // pixel id in x, given the x coordinate
  int nx(double x)
  {
    return npx(x, x0, lenx, nxmax);
  }

  // pixel id in y, given the y coordinate
  int ny(double y)
  {
    return npx(y, y0, leny, nymax);
  }

  // return pixel (id_x, id_y) given (x, y) of a particle incident
  pixel px(double x, double y)
  {
    return pixel(nx(x), ny(y));
  }

  // max x of n-th pixel 
  double xup (int n)
  {
    return up(n, x0, lenx, nxmax);
  }

  // min x of n-th pixel
  double xlo (int n)
  {
    return lo(n, x0, lenx, nxmax);
  }

  // max y of n-th pixel 
  double yup (int n)
  {
    return up(n, y0, leny, nymax);
  }

  // min y of n-th pixel 
  double ylo (int n)
  {
    return lo(n, y0, leny, nymax);
  }

  // return vector of pixels in the circle r around (x,y) of the particle incident
  cluster clxy_circle(double x, double y, double r)
  {
    cluster cl;

    // define square around the particle incident, but do not go beyond the detector
    double xmin = x - r; int n_xmin = nx(xmin); if(n_xmin < 0) n_xmin = 0;
    double xmax = x + r; int n_xmax = nx(xmax); if(n_xmax > 0) n_xmax = nxmax-1;
    double ymin = y - r; int n_ymin = ny(ymin); if(n_ymin < 0) n_ymin = 0;
    double ymax = y + r; int n_ymax = ny(ymax); if(n_ymax < 0) n_ymax = nymax-1;
    double r2 = r*r;

    // check the distance from the closest pixel corner to the particle incident
    for (auto i = n_xmin; i<= n_xmax; i++)
      {
        for (auto j = n_ymin; j<= n_ymax; j++)
          {
            double lox = xlo(i);
            double upx = xup(i);
            double loy = ylo(j);
            double upy = yup(j);

            double xi, yi;
            xi = lox;
            if(lox < x) xi = upx;
            yi = loy;
            if(loy < y) yi = upy;

            if((xi-x)*(xi-x) + (yi-y)*(yi-y) <= r2) //accept
             {
                pixel* p = new pixel(i,j);
                cl.add(*p);
              }
          }
      }
    return cl;
  }

  
  double cluster_size(double edep)
  {
    double x = edep*1000; // MeV -> keV 
    return 0.79489 - 0.0216*x + 5.89473e-4*x*x; //px
  }
  
  // return vector of pixels around (x,y) of the particle incident according to the energy deposit
  cluster clxy_edep(double x, double y, double edep)
  {
     cluster cl;

    // define square around the particle incident, but do not go beyond the detector
    // double px = 0.055; // pixel size [mm]

     double X0 = (x - x0 + lenx/2)*nxmax/lenx - nx(x) - 0.5;
     double Y0 = (y - y0 + leny/2)*nymax/leny - ny(y) - 0.5;

     double sigma = cluster_size(edep);

     int n_x = nx(x);
     int n_y = ny(y);

     int delta = 1;
     int n_xmin = n_x - delta; if(n_xmin < 0) n_xmin = 0;
     int n_xmax = n_x + delta;  if(n_xmax >= nxmax) n_xmax = nxmax - 1;
     
     int n_ymin = n_y - delta; if(n_ymin < 0) n_ymin = 0;
     int n_ymax = n_y + delta;  if(n_ymax >= nymax) n_ymax = nymax - 1;

     double sum = 0;
     
     for (auto i = n_xmin; i<= n_xmax; i++)
       {
	 for (auto j = n_ymin; j<= n_ymax; j++)
	   {
	     int dx = i - n_x;
	     int dy = j - n_y;

	     double share = integrate_gaussian(X0, Y0, sigma, dx , dy);
	     sum += share;
	     
	     if(share > 0.1) //accept
	       {
		 pixel* p = new pixel(i,j);
		 p->amp(edep*share*1000); // MeV -> keV
		 cl.add(*p);

		 //	 cout << "ADD " << *p << endl;
		 // cout << "SHARE " << X0 << " " << Y0 << " " << dx << " " << dy << " " << share << " " << i << " " << j << " " << n_x << " " << n_y << endl;
		 //	 cout << x << " " << y << endl;
		 
		 
              }
          }
      }

     double check = 0;
     for(auto& p : cl.pxv()) check += p.amp();
     cout << "CHECK " << cl.size() << " " << check << " " << check/edep << endl;
     // cout << "SUM " << sum << endl;
     
    return cl;
  }
  
protected:
  int npx(double x, double x0, double len, int nmax)
  {
    if (x < x0 - len/2) return -1;
    if (x > x0 + len/2) return nmax;
    return floor((x - x0 + len/2)*nmax/len);
  }

  double up(int n,  double x0, double len, double nmax)
  {
    if(n < 0) return x0-len/2.;
    if(n >= nmax) return x0+len/2.;
    return x0 - len/2. + len/nmax*(n+1); 
  }

  double lo(int n,  double x0, double len, double nmax)
  {
    if(n < 0) return x0-len/2.;
   if(n >= nmax) return x0+len/2.;
    return x0 - len/2. + len/nmax*n; 
  }
  
private:
  double x0;
  double y0;
  double lenx;
  double leny;
  double nxmax;
  double nymax;
};

bool clusters_pileup(cluster& a, cluster& b)
{
  if( a == b) return false;
    
  for(auto& i : a.pxv())
    {
      for(auto& j : b.pxv())
        {
          auto x_orig = i.x();
          auto y_orig = i.y();
          auto x_pile = j.x();
          auto y_pile = j.y();

          // if clusters overlap or touch
          if( fabs(x_orig-x_pile) <= 1 && fabs(y_orig-y_pile)<=1) // pileup
            return true;
        }
    }
  
  return false;
}

void clsim(){
  TFile f("run_0.root");
  TTree* MpxSim = (TTree*)f.Get("MpxSim");
  Int_t nentries = MpxSim->GetEntries();

  //nentries = 10;
  
  Double_t edep, kine, x, y;
  Int_t frameno, pdg;

  TH1F* h1 = new TH1F("sme1","sme1", 100,0,0.2);
  
  MpxSim->SetBranchAddress("E",&kine);
  MpxSim->SetBranchAddress("X",&x);
  MpxSim->SetBranchAddress("Y",&y);
  MpxSim->SetBranchAddress("FRAMEID",&frameno);
  MpxSim->SetBranchAddress("PDG",&pdg);
  MpxSim->SetBranchAddress("Edep",&edep);

  int iframe = -1;
  frame frm;

  ofstream fout("fout.clog");
  
  pxdet px;
  
  for(auto i = 0; i<nentries; i++)
    {
      MpxSim->GetEntry(i);
      if(frameno == iframe)
      { // add to current frame
        if(edep>0)
	  {
	    double esmear = gRandom->Gaus(edep, edep*0.13);
	    // for the moment constant cluster size 140 um is used
	    cluster cl = px.clxy_edep(x, y, esmear);
	    if(cl.size()>0)
	       frm.push_back(cl);
	  }
      }
      else // new frame
	{
	  // merge clusters to form pile-up
	  for(auto& p : frm) {
	    for(auto& q : frm) {
	      if( q.size() > 0 && clusters_pileup(p, q)) {
		p.add(q);
		q.clear();
	      }
	    }
	  }
	  
	  auto pend = frm.end();
	  for (auto x = frm.begin(); x != frm.end(); x++)
	    { if((*x).size()==0)
		pend = std::remove(frm.begin(), frm.end(), *x); }
	  frm.erase(pend, frm.end());
	  if(iframe >= 0 && frm.size() > 0)
	    fout << "Frame " << iframe << " (1580216970.287083, 0.005000 s)" << endl;
	  for(auto q : frm) fout << q;
	  
	  frm.clear();
	  iframe = frameno;
	  
	  double esmear = gRandom->Gaus(edep, edep*0.13);
	  cluster cl = px.clxy_edep(x, y, esmear);
	  if(cl.size()>0)
	   frm.push_back(cl);
	}
    }
  
 f.Close();
}
