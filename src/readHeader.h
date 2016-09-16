#include<fstream>
class header
{

  enum parNum {parNum=1};
  std::string params[parNum] = {"USBNUM"};
  double params[parNum];

  header::header(const char* fName)
  {
    ifstream fin(fName);
    std::string sin, split1, split2;

    while (std::getline(fin,sin))
    {
      int length=sin.length();
   
      for (int i=0; i<length;i++)
      {
        if (sin[i]=='=')
        {
          split1=sin.substr(0,i);
          split2=sin.substr(i+1,length);
        }
      }

      for (int i=0; i<parNum; i++)
      {
        if (spli1==parNum[i]) params[i]=atof(split2);
      }
      
    }
    fin.close();
  }
};
