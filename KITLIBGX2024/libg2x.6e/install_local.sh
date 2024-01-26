#!/bin/bash
printf '#---------------------------------------------------------------------------------\n# ' cat >> $HOME/.bashrc
date | cat >> $HOME/.bashrc
echo "# Variables d'environnement pour la lib. graphique <g2x> - Version 6e - 2022
#---------------------------------------------------------------------------------
# libG2X - installée comme une lib. locale ($PWD)
export G2X_VERSION='6e-23'
export G2X_PATH=$PWD
export GL_PATH='/usr/lib/x86_64-linux-gnu/'
export libG2X='-lm -L\$(GL_PATH) -lGL -lGLU -lglut -L\$(G2X_PATH)/bin -lg2x'
export incG2X='-I\$(G2X_PATH)/include'
export LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:\$G2X_PATH/bin
#---------------------------------------------------------------------------------
"| cat >> $HOME/.bashrc;
make cleanall;
make cleanall;
make clean; export GDB=1; make g2x; # compilation 'C'   en mode debug    -> libg2x.6e.gdb.{so,a}
make clean; export CPP=1; make g2x; # compilation 'C++' en mode debug    -> libg2x.6e++.gdb.{so,a}
make clean; export GDB=0; make g2x; # compilation 'C++' en mode optimisé -> libg2x.6e++.{so,a}
make clean; export CPP=0; make g2x; # compilation 'C'   en mode optimisé -> libg2x.6e.{so,a}  -- VERSION PAR DEFAUT
make clean;
bash;
