#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <string>

void displayCurrentDirectory();
void listDirContents();
void changeDir(const std::string& newPath);
void goToParentDirectory();

#endif // NAVIGATION_H