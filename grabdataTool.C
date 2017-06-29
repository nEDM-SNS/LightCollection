// This file should be saved to the root directory 

#include "TFile.h"
#include "TH1.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;

typedef vector<double> Vec;

void grabBin(int x)
{
   double val;


   Vec output;
   Vec output2;    // output datasets

   int y = x+1;
   int z = x+2;    // select the bins from graph in rootfile
   int numOfFile = 0;
   const char *TestPath = "/home/panji3/github0401/singleFiber/build/isotest";   // location of root files



   double val2;
   TSystemDirectory dir(TestPath,TestPath);
   TList *files = dir.GetListOfFiles();                             //Store datas into vectors
   if (files) {
      TSystemFile *file;
      TString fname;
      TIter next(files);
      while ((file=(TSystemFile*)next())) {
         fname = file->GetName();
         if (!file->IsDirectory() && isRoot(fname)) {
            cout << fname.Data() << endl;
            TFile f1(fname.Data());


            val = Photons->GetBinContent(y);
            val2 = Photons->GetBinContent(z);
            cout << "+z det: "<< val << endl;
            cout << "-z det: "<< val2 << endl << endl << endl;  // print out the data
            output.push_back(val);

            output2.push_back(val2);

            count ++;
 
   }

         }


       sort(output.begin(), output.end());
       sort(output2.begin(),output.end());      // make datas in order

       numOfFile = output.size();
       cout <<"[" ;
       for(int i=0;i<output.size(); i++)
           {    cout<<output[i]<<" ";    }
       cout<<"]"<<endl;
       cout<<"number of files: "<< output.size() << endl;    //display sample information





       ofstream result1;                     // write data to datafile for octave or matlab analysis
       result1.open ("y1.dat");
       result1<<" ";
                for(int i=0;i<output.size(); i++)
           {    result1<<output[i]<<",";    }
       result1<<" "<<endl;

       result1.close();


       ofstream result2;
       result2.open ("y2.dat");
       result2<<" ";
                for(int i=0;i<output2.size(); i++)
           {    result2<<output2[i]<<",";    }
       result2<<" "<<endl;







      }
}


bool isRoot(const char* file) {                  // check if the file is root file   written by Gus
   string str(file);
   int i = str.length() - 5;
   if(str.substr(i,5).compare(".root")==0)
      return true;
   else return false;
}




