#include "../header/picross.h"

/**Variable globale pour la souris**/
int L,C;
/**Initialise un tableau d'une dimension avec une valeur**/
void initArray1D(int size,int iArray[size], int value)
{
	for(int i = 0; i < size; i++)
	{
		iArray[i] = value;
	}
}
/**Stocke dans un tableau le nombre de bombes de la colonne**/
void stackInCols(int nbLin,int nbCol,int iArray[nbLin][nbCol],int size,int stack[size],int index)
{
	/*Initialisation du tableau de bombes*/
	initArray1D(size,stack,0);
	int k = 0;
	for(int i = 0; i < nbLin; i++)
	{
		/*Si il y a une bombe on augmente le compteur de bombe*/
		if(iArray[i][index] == 1)
			stack[k]++;
		if(iArray[i][index] == 0 && iArray[i - 1][index] == 1 && i != 0)
			++k;
	}
}
/**Stocke dans un tableau le nombre de bombes de la ligne**/
void stackInLines(int nbLin,int nbCol,int iArray[nbLin][nbCol],int size,int stack[size], int index)
{
	initArray1D(size,stack,0);
	int k = 0;
	for(int j = 0; j < nbCol; j++)
	{
		if(iArray[index][j] == 1)
			stack[k]++;
		if(iArray[index][j] == 0 && iArray[index][j - 1] == 1 && j != 0)
			++k;
	}
}
/**Compte le nombre de séquence de bombes de la ligne**/
int nbSeqLines(int nbLin,int nbCol,int iArray[nbLin][nbCol],int index)
{
	int cmpt = 0;
	for(int j = 0; j < nbCol; j++)
	{
		if(j == 0 && iArray[index][j] == 1)
			cmpt++;
		else if(iArray[index][j] == 1 && iArray[index][j - 1] == 0 && j != 0)
			cmpt++;
	}
	return cmpt;
}
/**Compte le nombre de séquence de bombes de la colonne**/
int nbSeqCols(int nbLin,int nbCol,int iArray[nbLin][nbCol],int index)
{
	int cmpt = 0;
	for(int i = 0; i < nbLin; i++)
	{
		if(i == 0 && iArray[i][index] == 1)
			cmpt++;
		else if(iArray[i][index] == 1 && iArray[i - 1][index] == 0 && i != 0)
			cmpt++;
	}
	return cmpt;
}
/**Initialise un tableau 2D avec une valeur**/
void initArray(int nbLin, int nbCol, int iArray[nbLin][nbCol], int value) 
{
	int i,j;
	for ( i = 0; i < nbLin; i++)
	{
		for ( j = 0; j < nbCol; j++)
		{
			iArray[i][j] = value;
		}
	}
}

/**Initialisation des couleurs**/
void ncurses_couleurs() 
{
  /* Vérification du support de la couleur */
  if(has_colors() == FALSE) 
  {
    endwin();
    fprintf(stderr, "Le terminal ne supporte pas les couleurs.\n");
    exit(EXIT_FAILURE);
  }

  /* Activation des couleurs */
  start_color();
  /*Création de paire de couleur*/
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_WHITE, COLOR_BLUE);
  init_pair(4, COLOR_BLACK, COLOR_WHITE);
  init_pair(5, COLOR_BLUE,  COLOR_BLACK);
  init_pair(6, COLOR_YELLOW,COLOR_BLACK);

}
/**Initialisation de NCURSES**/
void ncurses_initialiser() 
{
  initscr();	        /* Démarre le mode ncurses */
  cbreak();	        /* Pour les saisies clavier (desac. mise en buffer) */
  noecho();             /* Désactive l'affichage des caractères saisis */
  keypad(stdscr, TRUE);	/* Active les touches spécifiques */
  refresh();            /* Met a jour l'affichage */
  curs_set(FALSE);      /* Masque le curseur */
}
/**Dessine le plateau de jeu**/
void draw_grid(int nbLin,int nbCol,int iArray[nbLin][nbCol],int iArrayEx[nbLin][nbCol])
{
/*Initialisation des variables de deplacement du curseur*/
int y = 0,x = ((nbCol - 3) * 3);
move(y,x);
/*Dessine le couvercle des collonnes pour les bombes*/
for(int j = 0; j < nbCol; j++)
{
			if(j == 0)
			{
				addch(ACS_ULCORNER);
				addch(ACS_HLINE);
				addch(ACS_HLINE);
				addch(ACS_TTEE);
				addch(ACS_HLINE);
			}
			else if(j > 0 && j < nbCol - 1)
			{
				addch(ACS_HLINE);
				addch(ACS_TTEE);
				addch(ACS_HLINE);
			}
			else if(j == nbCol - 1)
			{
				addch(ACS_HLINE);
				addch(ACS_URCORNER);
				move(++y,x);
			}
}
/*Intérieur des colonnes pour les bombes*/
for(int j = 0;j < nbCol; j++)
{
	addch(ACS_VLINE);
	++x;
	/*Initialisation de la taille du tableau des bombes*/
	int size = nbSeqCols(nbLin,nbCol,iArrayEx,j);
	int stack[size];
	/*Stockage des nombres de bombes dans le tableau*/
	stackInCols(nbLin,nbCol,iArrayEx,size,stack,j);
	/*Affichage dans chaque colonne des bombes*/
	for(int i = 0; i < size; i++)
	{
		if(i == 0)
		{
			/*Check si la colonne est bien remplie avant de mettre les chiffres*/
			if(is_OKCols(nbLin,nbCol,iArrayEx,iArray,j) == false)
				printw("%d",stack[i]);
			else
			{
				attron(COLOR_PAIR(2));//Attribution couleur verte au texte
				printw("V");
				refresh();
				attron(COLOR_PAIR(1));//Attribution couleur blanche au texte
				refresh();
			}
		if(size > 1)
			move(++y,--x);
		else
			move(++y,x);
		}
		else
		{
			if(size <= 2)
			{
				addch(ACS_VLINE);
				/*Check si la colonne est bien remplie avant de mettre les chiffres*/
				if(is_OKCols(nbLin,nbCol,iArrayEx,iArray,j) == false)
					printw("%d",stack[i]);
				else
				{
					attron(COLOR_PAIR(2));//Attribution couleur verte au texte
					printw("V");
					refresh();
					attron(COLOR_PAIR(1));//Attribution couleur blanche au texte
					refresh();
				}
				move(++y,++x);
			}
			else
			{
				addch(ACS_VLINE);
				/*Check si la colonne est bien remplie avant de mettre les chiffres*/
				if(is_OKCols(nbLin,nbCol,iArrayEx,iArray,j) == false)
					printw("%d",stack[i]);
				else
				{
					attron(COLOR_PAIR(2));//Attribution couleur verte au texte
					printw("V");
					refresh();
					attron(COLOR_PAIR(1));//Attribution couleur blanche au texte
					refresh();
				}
				move(++y,x);
			}
		}
		/*Remplissage des barres verticales manquantes*/
		if(size <= 2 && size < (nbLin - 2) && i == size - 1)
		{
			move(y,--x);
				for(int u = size; u < nbLin - 2; ++u)
				{
					addch(ACS_VLINE);
					move(++y,x);
				}
			move(y,++x);
		}
		else if(size > 2 && size < nbLin - 2 && i == size - 1)
		{
			for(int u = size; u < nbLin - 2; ++u)
			{
				addch(ACS_VLINE);
				move(++y,x);
			}
		}
		if(i == size - 1 && size > 2)
			move(y,++x);
	}
	/*Réinitialisation des variables de déplacement*/
	y -= nbLin - 2;
	x += 2;
	move(y,x);
	/*Si on est au bord du tableau on remplit les barres verticales manquantes*/
	if(j == nbCol - 1)
	{
		while(y < nbLin - 2)
		{
			addch(ACS_VLINE);
			move(++y,x);
		}
		addch(ACS_VLINE);
	}	
}
printw("\n");
/**Dessine le Couvercle-Haut du tableau**/
for(int j = 0; j < nbCol; j++)
{
			if(j == 0)
			{
				/*Dessine la première ligne des cases pour chiffres*/
				addch(ACS_ULCORNER);
				for(int cmpt = 0; cmpt < ((nbCol - 3) * 3) - 1; cmpt++)
					addch(ACS_HLINE);
				/*Fin de la ligne pour les cases des chiffres*/
				/*Début du Couvercle-Haut du tableau*/
				addch(110 | A_ALTCHARSET);
				addch(ACS_HLINE);
				addch(ACS_HLINE);
				addch(110 | A_ALTCHARSET);
				addch(ACS_HLINE);
			}
			else if(j > 0 && j < nbCol - 1)
			{
				addch(ACS_HLINE);
				addch(110 | A_ALTCHARSET);
				addch(ACS_HLINE);
			}
			else if(j == nbCol - 1)
			{
				addch(ACS_HLINE);
				addch(ACS_RTEE);
				/*Fin du Couvercle-Haut du tableau*/
				printw("\n");
			}
}
/**Intérieur du tableau**/
 for(int i = 0; i < nbLin; i++)
 {
	for(int j = 0; j < nbCol; j++)
	{
		if(j == 0)
		{
		/*Mise en place des chiffres pour trouver les bombes(Lignes)*/
		addch(ACS_VLINE);
		int size = nbSeqLines(nbLin,nbCol,iArrayEx,i);
		int stack[size];
		stackInLines(nbLin,nbCol,iArrayEx,size,stack,i);
		for(int k = 0; k < size; k++)
		{
			/*Check si la ligne est bien remplie avant de mettre les chiffres*/
			if(is_OKLines(nbLin,nbCol,iArrayEx,iArray,i) == false)
			{
				if(stack[k] > 9)
				{
					printw("%d ",stack[k]);
				}
				else
					printw(" %d ",stack[k]);//Affichage des nombres
			}
			else
			{
				attron(COLOR_PAIR(2));
				printw(" V ");
				refresh();
				attron(COLOR_PAIR(1));//Attribution couleur blanche au texte
				refresh();
			}
		}
		while(size < nbCol - 3)//On remplit jusqu'à la ligne du tableau
		{
		printw("   "); 
		size++;
		}
		getyx(stdscr,y,x);//Verification de la position curseur pour mettre la ligne verticale
		move(y,--x);//On recule le curseur pour afficher la ligne verticale
		}
		addch(ACS_VLINE);
		/*Verification de la case*/
		if(iArray[i][j] == 0) printw("  ");//Cas de la case vide
		if(iArray[i][j] == 1)//Cas de la case noircie 
		{
			addch(ACS_CKBOARD);//Deux rectangles pour faire un carré
			addch(ACS_CKBOARD);
		}
		if(iArray[i][j] == 2)//Cas du drapeau(Flag)
		{
			addch(ACS_DIAMOND);//Diamant en guise de drapeau
			printw(" ");
		}
	}
	addch(ACS_VLINE);
	printw("\n");
	/*Trace les lignes*/
	if(i != nbLin -1)
		for(int x = 0; x < nbCol; x++)
		{
			if(x == 0)
			{
	/*Dessine les lignes des cases pour les chiffres à l'exception de la première et dernière*/
				addch(ACS_LTEE);
				for(int cmpt = 0; cmpt < ((nbCol - 3) * 3) - 1; cmpt++)
					addch(ACS_HLINE);
				/*Fin de la ligne des cases pour les chiffres*/
				/*Début des lignes du tableau*/
				addch(110 | A_ALTCHARSET);
				addch(ACS_HLINE);
				addch(ACS_HLINE);
				addch(110 | A_ALTCHARSET);
			}
			else if(x > 0 && x < nbCol - 1)
			{
				addch(ACS_HLINE);
				addch(ACS_HLINE);
				addch(110 | A_ALTCHARSET);
			}
			else if(x == nbCol - 1)
			{
				addch(ACS_HLINE);
				addch(ACS_HLINE);
				addch(ACS_RTEE);
				/*Fin des lignes du tableau*/
				printw("\n");
			}
		}
	/*Dessine le Couvercle-Bas du tableau*/
	else if(i == nbLin - 1)
		for(int x = 0; x < nbCol; x++)
		{
			if(x == 0)
			{
		/*Dessinne la dernière ligne des cases pour les chiffres*/
				addch(ACS_LLCORNER);
				for(int cmpt = 0; cmpt < ((nbCol - 3) * 3) - 1; cmpt++)
					addch(ACS_HLINE);
				/*Fin de la dernière ligne des cases pour les chiffres*/
				/*Début du Couvercle-Bas du tableau*/
				addch(ACS_BTEE);
				addch(ACS_HLINE);
				addch(ACS_HLINE);
				addch(ACS_BTEE);
			}	
			else if(x > 0 && x < nbCol - 1)
			{
				addch(ACS_HLINE);
				addch(ACS_HLINE);
				addch(ACS_BTEE);
			}
			else if(x == nbCol - 1)
			{
				addch(ACS_HLINE);
				addch(ACS_HLINE);
				addch(ACS_LRCORNER);
				/*Fin du Couvercle-Bas du tableau*/
				printw("\n");
			}
			
		}
 }
	printw("\n");
}
/**Charge le tableau aléatoirement avec nombres consécutifs récursivement**/
void loadArrayRec(int nbLin,int nbCol,int iArray[nbLin][nbCol],int acci,int accj)
{
	if(acci != nbLin)
	{
		if(accj != nbCol)
		{
			iArray[acci][accj] = rand()%2;
			loadArrayRec(nbLin,nbCol,iArray,acci,accj + 1);
		}
		accj = 0;
		loadArrayRec(nbLin,nbCol,iArray,acci + 1,accj);
	}

} 
/**Charge le tableau aléatoirement avec nombres consécutifs itérativement**/
void loadArray(int nbLin,int nbCol,int iArray[nbLin][nbCol])
{
	for(int i = 0; i < nbLin; i++)
		for(int j = 0; j < nbCol; j++)
		{
			iArray[i][j] = rand()%2;
		}
	/*Vérification si il existe une colonne de 0*/
	for(int j = 0; j < nbCol; j++)
		for(int i = 0; i < nbLin; i++)
		{
			if(iArray[i][j] == 1)
				break;
			/*Attribut une bombe à un indice aléatoire de la colonne*/
			else if(iArray[i][j] == 0)
			{
				iArray[rand()%nbLin][j] = 1;
			}
		}

}
/**Affichage normalisé d'un tableau 2D**/
void showArray(int nbLin, int nbCol, int iArray[nbLin][nbCol]) 
{
	int i,j;
	for(i = 0; i < nbLin; i++)
	{
		for(j = 0; j < nbCol; j++)
			printw("%3d ", iArray[i][j]);
		printw("\n");
	}
}
/**Initialisation de la souri*s*/
void ncurses_souris() 
{
  if(!mousemask(ALL_MOUSE_EVENTS, NULL)) 
  {
    endwin();
    fprintf(stderr, "Erreur lors de l'initialisation de la souris.\n");
    exit(EXIT_FAILURE);
  }
  /*Détection de la souris*/
  if(has_mouse() != TRUE) 
  {
    endwin();
    fprintf(stderr, "Aucune souris n'est détectée.\n");
    exit(EXIT_FAILURE);
  }
}
/**Récupère le click de la souris**/
int click_souris(int nbLin,int nbCol)
{
  MEVENT event;
  int ch;

  while((ch = getch()) != KEY_F(1)) 
  {
    switch(ch) 
    {
      case KEY_F(2): /*Pour quitter la boucle*/
	return 1;
      case KEY_MOUSE:
        if(getmouse(&event) == OK) 
	{
	  C = event.x;
	  L = event.y;
	  if(event.bstate & BUTTON1_CLICKED)
	  { 
	    /*Si le click est une case du tableau*/
	    if((L >= nbLin && L <= (nbLin * 2 + nbLin)) && (C > (nbCol - 3)*3 && C <= (nbCol * 3 + (nbCol - 3)*3)))
	    return 0;
	  }
	}
    }
  }
  return 0;
}
/**Fonction principale du jeu**/
char LANCEMENT_JEU(int nbLin,int nbCol,int iArray[nbLin][nbCol])
{
 /*On efface l'affichage précédent pour avoir un affichage dynamique*/
 clear();
 /*Création du tableau d'exemple et initialisation*/
 int iArrayEx[nbLin][nbCol];
 initArray(nbLin,nbCol,iArrayEx,0);
 /*Affichage de la grille*/
 draw_grid(nbLin,nbCol,iArrayEx,iArray);
  while (click_souris(nbLin,nbCol) == 0) //Tant qu'on clique sur une case
  {
    /*On efface l'affichage précédent pour avoir un affichage dynamique*/
    clear();
    /*Calcul de la position dans le tableau par rapport au clique-souris*/
    int y1 = ceil((L - nbLin)/2),x1 = ceil((C-((nbCol - 3) * 3))/3);
    /*Si c'est une case de base*/
    if(iArrayEx[(int)y1][(int)x1] == 0)
	iArrayEx[(int)y1][(int)x1] = 1;
    /*Si c'est une case noircie*/
    else if(iArrayEx[(int)y1][(int)x1] == 1)
    	iArrayEx[(int)y1][(int)x1] = 2;
    /*Si non c'est un drapeau*/
    else
       iArrayEx[(int)y1][(int)x1] = 0;
    /*Affichage de la grille*/
    draw_grid(nbLin, nbCol, iArrayEx,iArray);
    refresh();
    /*Check si le joueur à gagné*/
    if(hasWin(nbLin,nbCol,iArray,iArrayEx))
		break;
  }
	/*Affichage de la partie gagnée et retourne la réponse pour re-faire une partie*/
  	return printWin();
}
/**Check si il y autant de cases noircies dans le premier tableau que dans le deuxième pour une ligne**/
bool is_OKLines(int nbLin,int nbCol,int iArray[nbLin][nbCol],int iArrayEx[nbLin][nbCol],int eltLin)
{
/*Initialisation des compteurs de bombes*/
int cmpt = 0,cmptEx = 0;
	for(int j = 0; j < nbCol; j++)
	{
		/*Si on a une bombe dans notre vrai tableau*/
		if(iArray[eltLin][j] == 1)
			cmpt++;
		/*Si on noircit une case où il y a une bombe*/
		if((iArrayEx[eltLin][j] == 1) && (iArray[eltLin][j] == 1))
				cmptEx++;
		/*Si on noircit une case où il n'y a pas de bombes*/
		if((iArrayEx[eltLin][j] == 1) && (iArray[eltLin][j] == 0))
				cmptEx--;
	}
	/*Check si la ligne est égale*/
	if((cmpt > 0) && (cmpt == cmptEx))
		return true;
return false;
}
/**Check si il y autant de cases noircies dans le premier tableau que dans le deuxième pour une colonne**/
bool is_OKCols(int nbLin,int nbCol,int iArray[nbLin][nbCol],int iArrayEx[nbLin][nbCol],int eltCol)
{
/*Initialisation des compteurs de bombes*/
int cmpt = 0,cmptEx = 0;
	for(int i = 0; i < nbLin; i++)
	{
		/*Si on a une bombe dans notre vrai tableau*/
		if(iArray[i][eltCol] == 1)
			cmpt++;
		/*Si on noircit une case où il y a une bombe*/
		if((iArrayEx[i][eltCol] == 1) && (iArray[i][eltCol] == 1))
			cmptEx++;
		/*Si on noircit une case où il n'y a pas de bombes*/
		if((iArrayEx[i][eltCol] == 1) && (iArray[i][eltCol] == 0))
			cmptEx--;
	}
	/*Check si la colonne est égale*/
	if((cmpt > 0) && (cmpt == cmptEx))
		return true;
return false;
}
/*Check si le joueur a gagné*/
bool hasWin(int nbLin,int nbCol,int iArray[nbLin][nbCol],int iArrayEx[nbLin][nbCol])
{
/*Check si toutes les lignes sont OK*/
for(int i = 0; i < nbLin; i++)
{
	if(is_OKLines(nbLin,nbCol,iArray,iArrayEx,i));
	else return false;
}
/*Check si toutes les colonnes sont OK*/
for(int j = 0; j < nbCol; j++)
{
	if(is_OKCols(nbLin,nbCol,iArray,iArrayEx,j));
	else return false;
}
/*Si tout est vrai alors il gagne*/
return true;
}
/*Affichage de la partie gagnée et retourne si on re-joue*/
char printWin()
{
/*Joue un son si on gagne*/
system("play -q soundtrack/COMPLETE3_0529.dspadpcm.mp3 &");
/*Initialisation de la demande*/
char replay = ' ';
		/*Echappement des backslashs*/
		attron(COLOR_PAIR(5));// Attribution de la couleur bleue au terminal
		attron(A_BOLD); // Attribution du type gras au texte
		/*You Won Ascii ART*/
		printw("           ___________    ____  \n");
		printw("    ______/   \\__//   \\__/____\\ \n");                             
		printw("  _/   \\_/  :           //____\\\\ \n");                             
		printw(" /|      :  :  ..      /        \\ \n");                       
		printw("| |     ::     ::      \\        /\n");                           
		printw("| |     :|     ||     \\ \\______/\n");                            
		printw("| |     ||     ||      |\\  /  |\n");                                   
		printw(" \\|     ||     ||      |   / | \\ \n");   
		printw("  |     ||     ||      |  / /_\\ \\ \n");
		printw("  | ___ || ___ ||      | /  /    \\ \n"); 
		printw("   \\_-_/  \\_-_/ | ____ |/__/      \\ \n");
		printw("                _\\_--_/    \\      /\n"); 
		printw("               /____             /\n");
		printw("              /     \\           /\n"); 
		printw("              \\______\\_________/\n");
		/*End ART*/
		attroff(A_BOLD); // Désattribution
		attron(COLOR_PAIR(6)); // Attribution couleur jaune au terminal
		attron(A_REVERSE); // Attribution du type surligné au texte
		printw("\n            YOU WON !           \n");
		attroff(A_REVERSE); // Désattribution
		attron(COLOR_PAIR(1)); // Réattribution couleur blanche au terminale
		printw("Do you want to play another game ?(Y/N)\n");
  		refresh();
  		echo(); // Activation de l'écriture
  		if(scanw("%c",&replay) != 1) // Sécurisation du retour
  		{
			endwin();
			fprintf(stderr,"Input error\n");
			exit(EXIT_FAILURE);
  		}
  		noecho(); // Désactivation de l'écriture
return replay;
}
/*Demande un chiffre plus grand ou égale à un seuil pour la ligne*/
int getIntegerGreaterThanL(int treshold)
{
	int n;
	do
	{
	printw("Enter an integer greater or equal to %d for lines : ",treshold);
	refresh();
	echo(); // Activation de l'écriture
	if(scanw("%d",&n) != 1) // Sécurisation du retour
	{
		endwin();
		fprintf(stderr,"Input error\n");
		exit(EXIT_FAILURE);
	}
	noecho(); // Désactivation de l'écriture
	}while(n < treshold);
return n;
}
/*Demande un chiffre plus grand ou égale à un seuil pour la colonne*/
int getIntegerGreaterThanC(int treshold)
{
	int n;
	do
	{
	printw("Enter an integer greater or equal to %d for columns : ",treshold);
	refresh();
	echo(); // Activation de l'écriture
	if(scanw("%d",&n) != 1) // Sécurisation du retour
	{
		endwin();
		fprintf(stderr,"Input error\n");
		exit(EXIT_FAILURE);
	}
	noecho(); // Désactivation de l'écriture
	}while(n < treshold);
return n;
}
/*Charge un tableau 2D à partir d'un fichier*/
void loadFromFile(const char * File,int nbLin,int nbCol,int iArray[nbLin][nbCol])
{
	FILE * file = NULL;
	file = fopen(File,"r"); // Ouverture en lecture seule du fichier
	if(file == NULL) // S'il pointe toujours sur NULL
	{
		endwin();
		fprintf(stderr,"Impossible d'ouvrir %s, le fichier est peut-être inexistant.\n",File);
		exit(EXIT_FAILURE);
	}
	else
	{
	// On replace le curseur au tout début avant manipulation sur le fichier
	rewind(file);
	for(int i = 0; i < nbLin; i++)
		for(int j = 0; j < nbCol; j++)
		{
			fscanf(file,"%d",&iArray[i][j]); // Stocke chaque nombre dans la matrice
		}
	fclose(file); // On ferme le fichier pour la sécurité
	}
}
/*Affichage du menu*/
void printMenu(WINDOW * win,int y,int x)
{
	/**PICROSS ASCII ART**/
mvwprintw(win,y,x,"    ``         ``....````````````` `                         ` `:::::`````````....:''';;;;;;;;;;,");
mvwprintw(win,++y,x,"    ###########@@####@#@#######@##@###########################################@@#;::,,,,,::,::,,:");
mvwprintw(win,++y,x,"    +@,,,,,,,,,,@+,,:#:,,,,,,,,,:;,,,,,,,,,:@@,,,,,,,,,:@@,,,,,,,,,,@#,,,,,,,,,,@;:,```````````,:");
mvwprintw(win,++y,x,"    +@          ,#  ,+          :;          ';          +:          @,         `@;:,` ` ``` ` `::");
mvwprintw(win,++y,x,"    +@      `   ,#  :+          ;;  ``````  ':  ``` ``  +,  ````````#,  ````````@;:,``,,:,:,,.`::");
mvwprintw(win,++y,x,"    +@  :####+  :#  :+  ;#######+;  ;####;  ':` +####:` +:  +#######@:  #########;:,``,:,,,:,``::");
mvwprintw(win,++y,x,"    +@  ::::::``:#  ;+ `;########;  ::,::: `;;  #,:,,:``+:``,,:::::;#,  :::::::'#;:,``,,,,,,,``::");
mvwprintw(win,++y,x,"    +@ `````````:# `;+``;########;``````` ``';` #,,,,:``+: ````````.#:``````````@;::` ``.`` ` `,:");
mvwprintw(win,++y,x,"    +@ `````````'# `;+``;########;``` ``` `.#;` #,::,:``+'.`````````#;`````````.@;;,````.``````::");
mvwprintw(win,++y,x,"    +#``;######### `;+``;#######@;``+##;``'@#;``+;:::;``+########:``#########:`.@;:,``,::,::,::::");
mvwprintw(win,++y,x,"    +#``;#########``;+``:;::::;:''``'@#'``::''``;;:;::``+;::::::;:``#:::::::::`.@;:,``,:,,:,:::::");
mvwprintw(win,++y,x,"    +#``;#########``;+````````.`''``+##'````';``````````+;`````````.#;`````````.@;:,```````````,: ");
mvwprintw(win,++y,x,"    +#``;#########``;@.`````````''``+###````++.````````,@;`````````:#;`````````:@:;,```````````:: ");
mvwprintw(win,++y,x,"    +#@@##########@###@##@#########@##@#@@#@##@@##@@###@###@#@#@#@#@###@@@@@@@@@+`:::,:,::::::::: ");
mvwprintw(win,++y,x,"    +''''''''''';'''''''''''''''''''''''''''''''''''''''''''''''''';''''''''''';..;,,,,,,,,,,,,,; ");
/**END ART**/
}
/*Affichage sur une fenêtre un menu et retourne le choix de l'utilisateur*/
int choiceMenu(WINDOW * win_in, WINDOW * win)
{
	/**Box MENU**/
int elt_y1,elt_x1;
getmaxyx(win_in,elt_y1,elt_x1); // Récupère les coordonées maximales de la fenêtre
wattron(win_in,A_BOLD); // Attribution type gras au texte
mvwprintw(win_in,0,elt_x1/2 - 5,"###MENU###");
wattroff(win_in,A_BOLD); // Désattribution
wrefresh(win_in);
keypad(win_in,TRUE); // Activation des touches
int i,highlight = 0,choice;
/*Initialisation du menu*/
char choices[3][15] = {"» Aléatoire",
		       "» Image",
		       "» Quitter"};
while(true)
{
	for(i = 0; i < 3; i++)
	{
		/* Si on est sur le highlight */
		if(i == highlight)
		wattron(win_in,A_REVERSE); // On surligne le texte
		mvwprintw(win_in,elt_y1/2 - 1 + i,1,"%s",choices[i]); // Affichage en ligne des choix
		wattroff(win_in,A_REVERSE); // Désattribution
			
	}
	choice = wgetch(win_in); // On récupère le choix
	switch(choice)
	{
		/*Si on appuie fleche haut*/
		case KEY_UP:
			/*On joue un son*/
			system("play -q soundtrack/beep-07.mp3 &");
			highlight--;
			/* Replacement de l'highlight si on sort du tableau */
			if(highlight == -1) highlight = 2;
		break;
		/*Si on appuie fleche bas*/
		case KEY_DOWN:
			/*On joue un son*/
			system("play -q soundtrack/beep-07.mp3 &");
			highlight++;
			/* Replacement de l'highlight si on sort du tableau */
			if(highlight == 3) highlight = 0;
		break;
		/*Par défaut on ne fait rien*/
		default:
		break;
	}
	/**10 is the numerical representation for LF (LineFeed / '/n')**/
	if(choice == 10) 
	{
			return highlight; //Retourne le choix
	}
}
/**END Box MENU**/
}
/*Menu pour choisir la photo*/
const char * choicePic(WINDOW * win)
{
keypad(win,TRUE); //On active les touches dans la fenêtre
int y,x;
getmaxyx(win,y,x); //Récupération des coordonnées maximales de la fenêtre
int i,highlight = 0, choice;
/*Initialisation du menu*/
char choices[3][15] = {"» Tour eiffel",
		       "» PSG        ",
		       "» Retour     "};
	while(true)
	{
		for(i = 0; i < 3; i++)
		{
			/* Si on est sur le highlight */
			if(i == highlight)
			wattron(win,A_REVERSE); // On surligne le texte
			mvwprintw(win,y/2 - 1 + i,x/x,"%s",choices[i]); // Affichage en ligne des choix
			wattroff(win,A_REVERSE); // Désattribution
		}
		choice = wgetch(win);  // On récupère le choix
		switch(choice)
		{
			/*Si on appuie fleche haut*/
			case KEY_UP:
				/*On joue un son*/
				system("play -q soundtrack/beep-07.mp3 &");
				highlight--;
				/* Replacement de l'highlight si on sort du tableau */
				if(highlight == -1) highlight = 2;
			break;
			/*Si on appuie fleche bas*/
			case KEY_DOWN:
				/*On joue un son*/
				system("play -q soundtrack/beep-07.mp3 &");
				highlight++;
				/* Replacement de l'highlight si on sort du tableau */
				if(highlight == 3) highlight = 0;
			break;
			/*Par défaut on ne fait rien*/
			default:
			break;	
		}
			/**10 is the numerical representation for LF (LineFeed / '/n')**/
		if(choice == 10) 
		{
			/*On retourne le fichier*/
			if(highlight == 0)
				return "picture/Tour_eiffel.txt";
			if(highlight == 1)
				return "picture/PSG_Ascii.txt";
			if(highlight == 2)
				return "BACK";
		}
	}
	

}
