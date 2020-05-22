void show_events_clog(string fname)
{
  gStyle->SetOptStat(0);
  TCanvas * c1 = new TCanvas("c", "c", 600, 600);
  
  TH2F* hxy = new TH2F("xy","xy",256,0,256,256,0,256);
  
  ifstream fin;
  fin.open(fname);

  int frameid = -1;
  
  while(! fin.eof())
    {
      // read one frame from clog file
      string line;
      
      getline (fin, line);
    
      char hname[20] = {"Press any key"};
      
      if( line.substr(0,5).compare("Frame")==0 ) // new frame
	{
	  if(gPad) { gPad->Clear();}
	  if(frameid >= 0)
	    {
	      sprintf(hname, "Frame %d",frameid);
	      hxy->SetTitle(hname);
	      hxy->Draw("colz");
	      if(gPad) gPad->Update();
	    }
	  do 
	    {
	      cout << '\n' << ". Press a key to read new frame ...";
	    } while (cin.get() != '\n');
	 
	  istringstream(line.substr(6,line.npos)) >> frameid;
	  cout << "Frame " << frameid << endl;

	  hxy->Reset();
	  continue;
	}
      
      // single cluster 
      auto begin = line.find_first_of("[");
      auto i = 0;
      
      if(begin != line.npos)
	{
	  cout << "New cluster " << line << endl;

	  string substr = line;
	  while(true) // read pixels
	    {
	      auto begin = substr.find_first_of("[");
	      auto end = substr.find_first_of("]");

	      if(begin == line.npos)
		break;
	      
	      auto pixel = substr.substr(begin, end);
	      
	      // cout << "PIXEL " << i++ << " " << pixel << endl;
	      int x, y, amp;
	      sscanf(pixel.c_str(),"[%d,%d,%d",&x,&y,&amp);
	      // cout << x << " " << y << " " << amp << endl;
	      hxy->Fill(x,y,amp);
	      substr = substr.substr(end+1);
	    }
	}
    }
}


