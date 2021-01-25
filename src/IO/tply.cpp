#include "PlyLoader.hpp"

int main(int argc, char *argv[]){

  PlyLoader pl;

  pl.load(argv[1]);

  pl.print();

  return 1;

}
