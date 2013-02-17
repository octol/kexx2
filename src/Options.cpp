//    Kexx2
//    Copyright (C) 2001, 2013 Jon Häggblad
//
//    This file is part of Kexx2.
//
//    Kexx2 is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    Kexx2 is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Kexx2.  If not, see <http://www.gnu.org/licenses/>.

#include "Options.h"

// system headers
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

Options::Options()
{
    dataPath = "";
    m_fullscreen = false;
    m_fpsCounter = false;
    howManyLevelsAvailable = 0;
    numOfPlayers = 0;
}

Options::~Options()
{
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void Options::load(string path)
{
    int filePlayers, fileFullscreen, fileFpsCounter;

    // read the file information
    ifstream optionsFile(path.c_str());
    if (!optionsFile) {
        cerr << "Error opening " << path << " for reading, ";
        cerr << "using default options!" << endl;
    } else {
        optionsFile.ignore(80, '=');
        optionsFile >> filePlayers;
        optionsFile.ignore(80, '=');
        optionsFile >> fileFullscreen;
        optionsFile.ignore(80, '=');
        optionsFile >> fileFpsCounter;
    }
    optionsFile.close();

    // use the file information
    setHowManyPlayers(filePlayers);

    if (fileFullscreen == 1)
        fullscreen(true);
    else fullscreen(false);

    if (fileFpsCounter == 1)
        fpsCounter(true);
    else fpsCounter(false);
}

void Options::write(string path)
{
    int filePlayers, fileFullscreen, fileFpsCounter;

    // set up the information in variables
    filePlayers = getHowManyPlayers();

    if (fullscreen())
        fileFullscreen = 1;
    else fileFullscreen = 0;

    if (fpsCounter())
        fileFpsCounter = 1;
    else fileFpsCounter = 0;

    // write the information to disk
    ofstream optionsFile(path.c_str());
    if (!optionsFile)
        cerr << "Error opening file " << path << " for writing!" << endl;
    else {
        optionsFile << "NumPlayers=" << filePlayers << "\n";
        optionsFile << "Fullscreen=" << fileFullscreen << "\n";
        optionsFile << "FPSCounter=" << fileFpsCounter << "\n";
    }
    optionsFile.close();
}
/*
int Options::getHowManyLevelsAvailable()
{
    if(howManyLevelsAvailable == 0)
    {
        cerr << "warning: check disk for amount of levels before calling ";
        cerr << "Options::getHowManyLevelsAvailable()!" << endl;
    }
    return howManyLevelsAvailable;
}

int Options::setHowManyLevelsAvailable(int value)
{
    if(value < 1)
        cerr << "warning: Options::setHowManyLevelsAvailable() set to less than one!" << endl;
    return (howManyLevelsAvailable = value);
}
*/
int Options::setHowManyPlayers(int value)
{
    if (value < 0)
        cerr << "warning: Options::setHowManyPlayers() set to negative!" << endl;
    return (numOfPlayers = value);
}

int Options::getHowManyLevels()
{
    std::vector<std::string> levelFilename;
    int i = 0;
    ifstream levelsFile((dataPath + "levels/levels.cfg").c_str());

    string tmpFilename;
    levelFilename.clear();

    while (!levelsFile.eof()) {
        levelsFile >> tmpFilename;
        levelFilename.push_back(tmpFilename);
//      cout << levelFilename[i] << endl;
        i++;

        if (levelsFile.eof())
            break;
    }

    levelsFile.close();
    return levelFilename.size() - 1; // number of levels
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------
