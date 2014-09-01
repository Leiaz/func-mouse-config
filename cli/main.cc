#include "mouse_config_xml.h"
#include "mouse_profile.h"
#include "mouse_setup_interface.h"

#include <cstring>
#include <iostream>
#include <memory> // shared_ptr

using namespace std;

void printUsage(const string &name)
{
  cerr << "Usage: \n"
      << name << " interval <value> : Set polling interval (in ms).\n"
      << name << " switch <profile_num>  : Switch to profile_num (1,2,3).\n"
      << name << " set <p_num> <setting_path> : Read profile (from xml) "
                 "at setting_path and send to mouse profile p_num.\n"
      << name << " dpi <pro1> [pro2] [pro3] : Set selected DPI "
                 "setting for each profile. \n";
}

int main(int argc, char *argv[])
{
  char command;

  unsigned char p = 1; // profile number given in command
  unsigned char dpi1, dpi2, dpi3; // dpi slots
  dpi1 = dpi2 = dpi3 = 1;
  unsigned char val = 1;

  shared_ptr<MouseProfile> profile = nullptr;


  if (argc<3)
  {
    printUsage(argv[0]);
    return EXIT_FAILURE;
  }

  /* Checking the arguments */
  if (!strcmp(argv[1],"switch"))
  {
    command = 's';
    p = atoi(argv[2]);
    if ((p <1)||(p>3))
    {
      printUsage(argv[0]);
      return EXIT_FAILURE;
    }
  }
  else if (!strcmp(argv[1],"set"))
  {
    if (argc<4)
    {
      printUsage(argv[0]);
      return EXIT_FAILURE;
    }
    command = 'p';
    p = atoi(argv[2]);
    if ((p <1)||(p>3))
    {
      printUsage(argv[0]);
      return EXIT_FAILURE;
    }

    MouseConfigXml xml_file;
    if(!xml_file.loadFile(argv[3]))
    {
      cerr << "Couldn't read file." << endl;
      return EXIT_FAILURE;
    }
    profile = xml_file.extractProfile();
  }
  else if (!strcmp(argv[1],"interval"))
  {
    command = 'i';
    val = atoi(argv[2]);
    if (val > 8)
    {
      return EXIT_FAILURE;
    }
  }
  else if (!strcmp(argv[1],"dpi"))
  {
    command = 'd';
    dpi1 = dpi2 = dpi3 = atoi(argv[2]);
    if(argc>3)
    {
      dpi2 = dpi3 = atoi(argv[3]);
      if(argc>4)
      {
        dpi3 = atoi(argv[4]);
      }
    }
  }
  else
  {
    printUsage(argv[0]);
    return EXIT_FAILURE;
  }

  {
    MouseSetupInterface mouse;
    if (!mouse.isOpen())
    {
      cerr << "Error accessing mouse." << endl;
      cerr << mouse.getLastErrorMessage() << endl;
      return EXIT_FAILURE;
    }
    /* Do stuff */
    bool ok;
    switch (command)
    {
      case 's':
        ok = mouse.switchToProfile(p);
        break;
      case 'i':
        ok = mouse.setPollingInterval(val);
        break;
      case 'd':
        ok = mouse.setSelectedDPI(dpi1, dpi2, dpi3);
        break;
      case 'p':
        ok = mouse.setProfile(*profile, p);
        break;
    }
    if (!ok)
    {
      cerr << mouse.getLastErrorMessage() << endl;
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}
