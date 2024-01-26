#!/bin/bash
sudo apt-get update
echo "installation de freeglut3-dev"
sudo apt-get install freeglut3-dev;
echo "installation de Netpbm"
sudo apt-get install netpbm;
echo "installation de mencoder"
sudo apt-get install mencoder;
echo "installation de libg3x"
sudo mkdir /usr/lib/g3x/;
sudo mkdir /usr/include/g3x/;
sudo cp include/* /usr/include/g3x/;
sudo chmod a+rx /usr/include/g3x/
sudo chmod a+r  /usr/include/g3x/*.h
printf '#---------------------------------------------------------------------------------\n# ' cat >> $HOME/.bashrc
date | cat >> $HOME/.bashrc
echo "# Variables d'environnement pour la lib. graphique <g3x> - Version 6e - 2022
#---------------------------------------------------------------------------------
# libG3X - installée comme une lib. standard (/usr/lib/g3x/)
export G3X_VERSION='6e-23'
export G3X_PATH='/usr/lib/g3x'
export GL_PATH='/usr/lib/x86_64-linux-gnu/'
export libG3X='-lm -L\$(GL_PATH) -lGL -lGLU -lglut -L\$(G3X_PATH) -lg3x.\$(G3X_VERSION)'
export incG3X='-I/usr/include/g3x/'
export LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:\$G3X_PATH
#---------------------------------------------------------------------------------
"| cat >> $HOME/.bashrc;
make cleanall;
make clean; export GDB=1; make g3x; # compilation 'C'   en mode debug    -> libg3x.6e.gdb.{so,a}
make clean; export CPP=1; make g3x; # compilation 'C++' en mode debug    -> libg3x.6e++.gdb.{so,a}
make clean; export GDB=0; make g3x; # compilation 'C++' en mode optimisé -> libg3x.6e++.{so,a}
make clean; export CPP=0; make g3x; # compilation 'C'   en mode optimisé -> libg3x.6e.{so,a}  -- VERSION PAR DEFAUT
sudo mv /bin/libg3x*.* /usr/lib/g3x/;
bash;
