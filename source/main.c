#include "../header/picross.h"

int main(int argc,char ** argv)
{
/*Initialisation de l'environnement du terminal pour une plus grande zone cliquable*/
//putenv("TERM=xterm-1006");
/*Réinitialisation de la graine du temps pour l'aléatoire*/
srand(time(NULL));
/**NCURSES START**/
ncurses_initialiser();
ncurses_souris();
ncurses_couleurs();
char replay;
int choice;
do
{
	/*Musique du menu*/
	system("play -q soundtrack/VIEWER1_Final4AAC.mp3 repeat 100 &");
	int width,heigth,start_y,start_x;
	/*Récupération des coordonnées maximales de la fenêtre*/
	getmaxyx(stdscr,heigth,width);
	start_x = start_y = 0;
	/*Création de la fenêtre principale*/
	WINDOW * win = NULL;
	win = newwin(heigth,width,start_y,start_x);
	refresh();
	/*Initialisation du caractère de bordure et casting vers int*/
	char c = '#';
	box(win,(int)c,(int)c);
	wrefresh(win);
	wbkgd(win,COLOR_PAIR(3)); // Attribution de la couleur bleu à la fenêtre
	int elt_y,elt_x;
	/*Récupération des coordonnées maximales de la fenêtre*/
	getmaxyx(win,elt_y,elt_x);
	/*Affichage du menu*/
	printMenu(win,elt_y/2 - 20,elt_x/2 - 40);
	/*Création de la box Menu*/
	WINDOW * win_in = NULL;
	win_in = newwin(10,100,elt_y/2,elt_x/2 - 40);
	refresh();
	wrefresh(win);
	box(win_in,0,0);
	wrefresh(win_in);
	/*Récupération du choix*/
	choice = choiceMenu(win_in,win);
	if(choice == 0) // Si on choisit le mode aléatoire
	{
		do  
		{
			/*Kill tout les sons en cours*/
			system("killall play");
			/*Joue un son*/
			system("play -q soundtrack/27.SharpFearShort.mp3 &");
			/*On efface l'affichage précédent pour un affichage dynamique*/
			clear();
			/*On supprime les fenêtre créées pour libérer de la mémoire*/
			delwin(win_in);
			delwin(win);
			/*On récupère la taille de la ligne et de la colonne*/
			int nbLin = getIntegerGreaterThanL(5),nbCol = getIntegerGreaterThanC(5);
			/*Création du plateau de jeu*/
			int iArray[nbLin][nbCol];
			/*Initialisation du plateau de jeu à 0*/
			initArray(nbLin, nbCol,iArray, 0);
			/*Chargement aléatoire du plateau de jeu*/ 
			loadArray(nbLin, nbCol,iArray);
			replay = 'N';
			/*On lance le jeu et on récupère si on re-joue*/
			replay = LANCEMENT_JEU(nbLin,nbCol,iArray); 
		}while(replay == 'Y' || replay == 'y'); // Tant qu'on re-joue
	}
	else if(choice == 1) // Si on choisit le mode image
	{
		do  
		{
			/*Kill tout les sons en cours*/
			system("killall play");
			/*Joue un son*/
			system("play -q soundtrack/27.SharpFearShort.mp3 &");
			/*On efface l'affichage précédent pour un affichage dynamique*/
			clear();
			delwin(win_in);
			delwin(win);
			/*Création de la fenêtre de choix pour image*/
			WINDOW * ChoiceBox = NULL;
			ChoiceBox = newwin(40,40,1,1);
			refresh();
			wrefresh(ChoiceBox);
			const char* c = choicePic(ChoiceBox);
			int nbLin,nbCol;
			if(strcmp("picture/Tour_eiffel.txt",c) == 0)
			{
				/*Attribution de la ligne et colonne maximale pour l'image*/
				nbLin = 22;nbCol = 30;
			}
			else if(strcmp("picture/PSG_Ascii.txt",c) == 0)
			{
				/*Attribution de la ligne et colonne maximale pour l'image*/
				nbLin = 29;nbCol = 59;
			}
			else if(strcmp("BACK",c) == 0)
			{
				replay = 'n';
				goto stop;
			}
			{
			/*Création du plateau de jeu*/
			int iArray[nbLin][nbCol];
			/*Initialisation à 0 du plateau de jeu*/
			initArray(nbLin, nbCol,iArray, 0);
			/*Charge le tableau en fonction de l'image choisie*/
			loadFromFile(c,nbLin,nbCol,iArray);
			replay = 'N';
			replay = LANCEMENT_JEU(nbLin,nbCol,iArray);
			/*On supprime la fenêtre créée pour libérer de la mémoire*/
				delwin(ChoiceBox);
			}
			stop: delwin(ChoiceBox);
		}while(replay == 'Y' || replay == 'y');
	}
	else if(choice == 3)
	{
		/*On s'assure d'éteindre tout sons avant de quitter le programme*/
		system("killall play");
		/*On supprime les fenêtres créées pour libérer de la mémoire*/
		delwin(win_in);
		delwin(win);
		endwin();
		goto stopN;
	}
}while((replay == 'N' || replay == 'n') && choice != 3);
/*On s'assure d'éteindre tout sons avant de quitter le programme*/
stopN: system("killall play");
/**END NCURSES**/
endwin();
return EXIT_SUCCESS;
}
