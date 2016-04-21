/* ===================== */
//	Nombre: Cristian Beltran
//	Carrera: Ing Civil Informatica
//	Profersor: Alejandro Mellado
//  Directorio Simple desde un archivo estructurado
//	Con Ncurses
/* ===================== */
#include <curses.h>
#include <string.h>


typedef struct
{
        char nombres[15];
        char apellidos[25];
        char edad[3];
        char direccion[40];
        char rut[13];
        char fono[20];
}Personas;

//    Prototipos   //
void plantilla();
void guardaRegistro();
void mostrarRegistros();
void cargarRegistros();
void buscaRegistro();
void eliminaRegistro();
void actualizaRegistro();

Personas persona[1000];
int nPersonas = 0, nInicio=0;// Numero incial de personas y pagina inicio

#define nHoja 15	// numero de  datos por hoja
//	==========		//
int main()
{
	initscr(); // crea la pantalla 
	keypad(stdscr,TRUE);
	
	
	start_color();
	int k=0; //tecla presionada
		
	init_pair(9,COLOR_WHITE, COLOR_CYAN);
	init_pair(8,COLOR_GREEN, COLOR_BLUE);
	init_pair(10,COLOR_WHITE, COLOR_RED);
	
	// Cuadros de datos y Alertas
	WINDOW *win = newwin(25,110,6,0);
	WINDOW *alert = newwin(2,110,30,0);
		
	wbkgd(win,COLOR_PAIR(9));		//da color a los cuadros
	wbkgd(alert,COLOR_PAIR(10));
	
	wbkgd(stdscr, COLOR_PAIR(8));
	wrefresh(win);
	refresh();
	
	
	while (k!=115 && k!=83 )	// hasta que presione S para salir
	{
			werase(win);//Limpia pantalla
			
			plantilla(); 	//Carga la plantilla con las opciones
			cargarRegistros();	//Carga los registros al arreglo 
			mostrarRegistros(win);	//  muestra en la pantalla
					
			move(34,0);		// mueve el cursor
			wrefresh(win);
			
			// Muestra las flechas para cambiar la pagina si es posible
			if(nInicio>0)
			{
				wattron(win,COLOR_PAIR(10));
				wattron(win, A_BOLD);
				mvwprintw(win, 24, 50," < ");
				wattroff(win,COLOR_PAIR(10));
				wattroff(win,A_BOLD);
				
			}
			if((nInicio+1)*nHoja< nPersonas)
			{
				wattron(win,COLOR_PAIR(10));
				wattron(win, A_BOLD);
				mvwprintw(win, 24, 54," > ");
				wattroff(win,COLOR_PAIR(10));
				wattroff(win,A_BOLD);
			}
			wrefresh(win);
			k=getch(); // captura la tecla
		
			if(k == KEY_LEFT)
			{
				if(nInicio>0)
				{
					nInicio--;	// cambia la pagina
				}
			}
			if(k == KEY_RIGHT)
			{
				if((nInicio+1)*nHoja< nPersonas)
					nInicio++;
				
			}
			if(k==105 || k==73 )  // si presiona i  Opcion Ingresar
			{
				
				attron(COLOR_PAIR(1));	// Blanco y Magenta
				
				mvprintw(31,0,"NOMBRES: ");
				getnstr(persona[nPersonas].nombres,15);
				mvprintw(32,0, "APELLIDOS: ");
				getnstr(persona[nPersonas].apellidos,25);
				mvprintw(33,0,"DIRECCION: ");
				getnstr(persona[nPersonas].direccion,40);
				mvprintw(34,0,"RUT: ");
				getnstr(persona[nPersonas].rut,13);
				mvprintw(35,0,"FONO: ");
				getnstr(persona[nPersonas].fono,20);
				mvprintw(36,0,"EDAD: ");
				getnstr(persona[nPersonas].edad,3);
				
				attroff(COLOR_PAIR(1));
				nPersonas++;
				
				guardaRegistro();
				
			}
			
			
			if(k==98 || k==66)		// si presiona B para buscar
			{
				char rut[13];
				mvprintw(30,0,"BUSCAR RUT:");
				getstr(rut);
				buscaRegistro(rut, alert);
				
				wrefresh(alert);
				getch();	// pause
				
			}
			
			if(k==101 || k==69 )		//si presiona E para eliminar
			{
				char rut[13];
				mvprintw(29,0,"ELIMINAR RUT:");
				getnstr(rut,13);
				
				eliminaRegistro(rut, alert);
				guardaRegistro();
				
				
			}
			
			if(k==97 || k==65)		// si presiona A para actualizar registro
			{
				char rut[13];
				mvprintw(29,0,"RUT ACTUALIZAR:");
				getnstr(rut, 13);
				actualizaRegistro(rut,alert);
				
			}	
		clear();
		
		wrefresh(win);	
		refresh();
	}
	
	endwin();// destruye la pantalla
	return 0;
}

void mostrarRegistros(WINDOW *win)
{
	int i,j;
	wclear(win);
	
	wattron(win, A_BOLD);
	mvwprintw(win, 1,1,"NOMBRES");
	mvwprintw(win, 1,25,"APELLIDOS");
	mvwprintw(win, 1,50,"RUT");
	mvwprintw(win, 1,70,"DIRECCION");
	mvwprintw(win, 1,90,"FONO");
	mvwprintw(win, 1,100,"EDAD");
	wattroff(win, A_BOLD);
	j=0;
	i=0;
	if(nInicio>0)
		i=nInicio*nHoja;
	
	while(i<nPersonas)
	{
		if(j< nHoja)
		{
			mvwprintw(win, 3+j, 1,"%s",persona[i].nombres);
			mvwprintw(win, 3+j, 25,"%s",persona[i].apellidos);
			mvwprintw(win, 3+j, 50,"%s",persona[i].rut);
			mvwprintw(win, 3+j, 70,"%s",persona[i].direccion);
			mvwprintw(win, 3+j, 90,"%s",persona[i].fono);
			mvwprintw(win, 3+j, 100,"%s",persona[i].edad);
		}
		i++;
		j++;
	}
	
	wrefresh(win);
}
void cargarRegistros()	// Carga los registros del archivo al arreglo
{
	FILE *hF = fopen("registros.bin","rb");
	Personas p;
	if(hF != NULL)
	{
		Personas p;
		fread(&p,sizeof(Personas),1,hF);
		
				
		int i=0;
		nPersonas=0;
		while(!feof(hF))
		{			
			strcpy(persona[nPersonas].nombres, p.nombres);
			strcpy(persona[nPersonas].apellidos, p.apellidos);
			strcpy(persona[nPersonas].direccion, p.direccion);
			strcpy(persona[nPersonas].rut, p.rut);
			strcpy(persona[nPersonas].edad, p.edad);
			strcpy(persona[nPersonas].fono, p.fono);
			
			i++;
			nPersonas++;
			fread(&p,sizeof(Personas),1,hF);
		}
		
		fclose(hF);
	}
	else
	{
		mvprintw(35,0,"Error abrir Archivo");
	}
}

void plantilla()	// plantilla con el menu y titulo del programa
{
	WINDOW *titulo = newwin(3,110, 0,0);
	
	box(titulo, '-','-');
	
	init_pair(1,COLOR_WHITE,COLOR_BLUE);
	
	wbkgd(titulo, COLOR_PAIR(1));
	
	init_pair(2,COLOR_WHITE,COLOR_RED);
	attron(COLOR_PAIR(1));
	wattron(titulo,A_BOLD);
	mvwprintw(titulo,1,45,"Directorio Personas");
	wrefresh(titulo);
	wattroff(titulo,A_BOLD);
	
	WINDOW *menu = newwin(1,110,4,0);
	wbkgd(menu, COLOR_PAIR(2));
	wattron(menu,A_BOLD);
	mvwprintw(menu,0,0,"\tIngresar Registro[I] | Eliminar[E] | Buscar[B] | Actualizar[a] | Salir[S]");
	
	wattroff(menu,A_BOLD);
	wrefresh(menu);
}

void guardaRegistro()		//guarda desde array al archivo
{
	FILE *hF = fopen("registros.bin","wb");
	fwrite(persona,sizeof(Personas),nPersonas,hF);
	fclose(hF);
}

void buscaRegistro(char rut[13], WINDOW *alert)
{
	int i, j=0;
	
	
	
	for(i=0;i<nPersonas;i++)
	{	
		if(strcmp(persona[i].rut, rut)==0)
		{
			wclear(alert);
			wattron(alert, A_BOLD);
			mvwprintw(alert, 0, 1,"NOMBRES");
			mvwprintw(alert, 0, 25,"APELLIDOS");
			mvwprintw(alert, 0, 50,"RUT");
			mvwprintw(alert, 0, 70,"DIRECCION");
			mvwprintw(alert, 0, 90,"FONO");
			mvwprintw(alert, 0, 100, "EDAD");
			wattroff(alert, A_BOLD);
			
			mvwprintw(alert, 1, 1,"%s", persona[i].nombres);
			mvwprintw(alert, 1,25,"%s",persona[i].apellidos);
			mvwprintw(alert, 1,50,"%s",persona[i].rut);
			mvwprintw(alert, 1,70,"%s",persona[i].direccion);
			mvwprintw(alert, 1,90,"%s",persona[i].fono);
			mvwprintw(alert, 1,100,"%s",persona[i].edad);
			
			j=1;
			
		}
	}
	
	if(j==0)
	{
		wattron(alert, A_BOLD);
		mvwprintw(alert, 0,20, "No se Encontro Registro");
		wattroff(alert, A_BOLD);
	}
	
}
void eliminaRegistro(char rut[13], WINDOW *alert)
{
	int i=0,j=0;
	while(i<nPersonas)
	{
		if(strcmp(persona[i].rut,rut)==0)
		{
			int j=i;
			while(j<nPersonas-1)
			{
				persona[j]=persona[j+1];
				j++;
			}
			nPersonas--;
			wattron(alert, A_BOLD);
			mvwprintw(alert, 0, 20, "Registro '%s' Eliminado",rut);
			wattroff(alert, A_BOLD);
			wrefresh(alert);
			getch();
			j=1;
		}
		i++;
	}
	if(j==0)
	{
		wattron(alert, A_BOLD);
		mvwprintw(alert, 0, 20, "Registro '%s' No encontrado",rut);
		wattroff(alert, A_BOLD);
		wrefresh(alert);
	}
	guardaRegistro();
}

void actualizaRegistro(char rut[13], WINDOW *alert)		// modifica un registro
{
	int i, j=0;
	for(i=0;i<nPersonas;i++)
	{
		
		if(strcmp(persona[i].rut, rut)==0)
		{
			int k;
			char input[40];
			mvprintw(30,0,"NOMBRES: %s",persona[i].nombres);
			mvprintw(15,0,"%d",strlen(persona[i].nombres));
			move(30,9);
			getnstr(input,15);
			k=0;
			while(k<strlen(input))
			{
				persona[i].nombres[k] = input[k];
				k++;
			}		
			
			mvprintw(31,0, "APELLIDOS: %s",persona[i].apellidos );
			move(31,11);
			getnstr(input,25);
			k=0;
			while(k<strlen(input))
			{
				persona[i].apellidos[k] = input[k];
				k++;
			}
			mvprintw(32,0,"DIRECCION: %s",persona[i].direccion);
			move(32,11);
			getnstr(input,40);
			k=0;
			while(k<strlen(input))
			{
				persona[i].direccion[k] = input[k];
				k++;
			}
			
			mvprintw(33,0,"FONO: %s",persona[i].fono);
			move(33,6);
            getnstr(input,20);
            k=0;
			while(k<strlen(input))
			{
				persona[i].fono[k] = input[k];
				k++;
			}
			mvprintw(34,0,"EDAD: %s",persona[i].edad);
			move(34,6);
            getnstr(input,3);
            k=0;
			while(k<strlen(input))
			{
				persona[i].edad[k] = input[k];
				k++;
			}
			j=1;
			guardaRegistro();
			
		}
	}
	
	if(j==0)
	{
		wattron(alert,A_BOLD);
		mvwprintw(alert,0,20, "Registro '%s' No encontrado", rut);
		wattroff(alert,A_BOLD);
		wrefresh(alert);
		getch();
	}
}
