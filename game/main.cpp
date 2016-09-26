// Brian Malloy        Game Construction
#include "manager.h"

int main(int, char*[]) {
   try {
   	Manager manager;
     int reset = 1;
      while (reset) {
      	
      	reset = manager.play();
      	manager.~Manager();
      	new(&manager) Manager();

      }
   }
   catch (const string& msg) { std::cout << msg << std::endl; }
   catch (...) {
      std::cout << "Oops, someone threw an exception!" << std::endl;
   }
   return 0;
}

//Main function with two parameters - int and char pointer array