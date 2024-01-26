#!/bin/bash
sudo apt-get update
echo "installation de freeglut3-dev"
sudo apt-get install freeglut3-dev;
echo "installation de Netpbm"
sudo apt-get install netpbm;
echo "installation de mencoder"
sudo apt-get install mencoder;
echo "installation de libg2x"
sudo mkdir /usr/lib/g2x/;
sudo mkdir /usr/include/g2x/;
sudo cp include/* /usr/include/g2x/;
sudo chmod a+rx /usr/include/g2x/
sudo chmod a+r  /usr/include/g2x/*.h
printf '#---------------------------------------------------------------------------------\n# ' cat >> $HOME/.bashrc
date | cat >> $HOME/.bashrc
echo "# Variables d'environnement pour la lib. graphique <g2x> - Version 6e - 2022
#---------------------------------------------------------------------------------
# libG2X - installée comme une lib. standard (/usr/lib/g2x/)
export G2X_VERSION='6e-23'
export GL_PATH='/usr/lib/x86_64-linux-gnu/'
export G2X_PATH='/usr/lib/g2x'
export libG2X='-lm -L\$(GL_PATH) -lGL -lGLU -lglut -L\$(G2X_PATH) -lg2x'
export incG2X='-I/usr/include/g2x/'
export LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:\$G2X_PATH
#---------------------------------------------------------------------------------
"| cat >> $HOME/.bashrc;
make cleanall;
make clean; export GDB=1; make g2x; # compilation 'C'   en mode debug    -> libg2x.6e.gdb.{so,a}
make clean; export CPP=1; make g2x; # compilation 'C++' en mode debug    -> libg2x.6e++.gdb.{so,a}
make clean; export GDB=0; make g2x; # compilation 'C++' en mode optimisé -> libg2x.6e++.{so,a}
make clean; export CPP=0; make g2x; # compilation 'C'   en mode optimisé -> libg2x.6e.{so,a}  -- VERSION PAR DEFAUT
sudo mv libg2x*.* /usr/lib/g2x/;
bash;
