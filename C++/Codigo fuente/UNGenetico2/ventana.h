#ifndef __VENTANA_H
#define __VENTANA_H

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/colordlg.h"
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/spinctrl.h>
#include <wx/textfile.h>
#include <wx/image.h>

#include <time.h>
#include <stdlib.h>

enum  Paginas
{
	ID_CONSOLA=2000,
	ID_GRAFICA,
	ID_OTRA
};

//!Clase derivada de \a wxScrolledWindow de la librer�a \a wxWindows que contiene objetos visibles por el usuario.
/*!Al establecer la utilizaci�n del ambiente gr�fico se crean dos tipos de ventana, una que contiene
un control de texto en el que aparece la ejecuci�n del algoritmo, y otras que contienen las gr�ficas
de desempe�o del algoritmo gen�tico. 
Sin embargo, el usuario puede crear una clase derivada de AGVentana y adicionarle cualquier control que pertenezca a la librer�a \a wxWindows. 
*/
class AGVentana : public wxScrolledWindow
{
public:
	//!Constructor de la clase AGVentana
	AGVentana(wxWindow *parent, wxWindowID id = -1,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = 0);

	//!Destructor de la clase AGVentana
	~AGVentana();
  
//FUNCIONES
	//!M�todo que retorna el objeto de la clase \a wxFrame en el que est� contenido
	wxWindow* ObtenerFrame()
	{
		return GetGrandParent()->GetParent();
	}
	
	//!M�todo que realiza gr�ficas descriptivas de datos almacenados en un archivo de texto
	void GrafTexto();

//CONTROLES
	//!Apuntador al control de texto donde se almacena la informaci�n seleccionada para cada iteraci�n del algoritmo
	wxTextCtrl *m_pTxtConsola;
	//!Apuntador al objeto donde se almacenan las gr�ficas creadas
	wxBitmap *m_pBitmap;
	//!Objeto que define el color de la l�nea de las gr�ficas creadas  
	wxColour m_ColBrush;
	//!Objeto que define el color de los ejes de las gr�ficas creadas  
	wxColour m_ColGrilla;
	//!Objeto que define el color del fondo de las gr�ficas creadas  
	wxColour m_ColGraf;
	//!Cadena de caracteres donde se almacena el nombre del archivo de texto del que se obtienen los datos para crear las gr�ficas
	wxString m_NameArch;
	//!Cadena de caracteres donde se almacena el nombre de la columna de datos que ser�n graficados
	wxString m_Titulo;
	//!Variable que indica la l�nea del archivo \a NameArch donde inician los datos que se van a graficar
	int m_Ini;
	//!Variable que indica la columna del archivo \a NameArch correspondiente a los datos que se van a graficar
	int m_Columna;

protected:

//EVENTOS

	DECLARE_EVENT_TABLE()
	//!M�todo que actualiza las ventanas que contienen las gr�ficas creadas
	void OnPaint(wxPaintEvent& event);
	//!M�todo que elimina la informaci�n presente en \a m_TxtConsola cuando su capacidad de almacenamiento llega al l�mite
	void OnMaxLen(wxCommandEvent& event);

  //FUNCIONES PROTEGIDAS
	//!M�todo que crea los ejes para las gr�ficas creadas
	void GrafGrilla();
	//!M�todo que halla el l�mite superior del eje horizontal de las gr�ficas creadas
	long Extremo(long valor);
	//!M�todo que valida los valores del archivo de texto para ser graficados
	double ObtenerY(int n, int col);
	//!M�todo que obtiene los valores del archivo de texto para ser graficados
	void ObtenerValoresGrafica();
	//!M�todo que obtiene y valida los extremos inferior y superior del eje vertical de cada gr�fica creada
	void ObtenerExtremos();

	
	//!Dispositivo propio de \a wxWindows que provee un medio para realizar una gr�fica en un mapa de bits
	wxMemoryDC m_dib;
	//!Dispositivo propio de \a wxWindows que permite mostrar la gr�fica creada en una ventana
	wxClientDC m_dibCopia;
	//!Variable que indica el ancho de la gr�fica
	int m_ancho;
	//!Variable que indica el alto de la gr�fica
	int m_alto;
	//!Apuntador a un arreglo de n�meros reales donde se almacenan los valores que ser�n graficados
	double *m_pValores;
	//!Apuntador al archivo de texto de donde se extraen los valores que ser�n graficados
	wxTextFile *m_pTextarch;
    //!Apuntador al objeto que ajusta el tama�o de los controles contenidos en una ventana
	wxSizer *m_pSizer;

private:

	//!Variable que almacena el n�mero de final de generaciones de la ejecuci�n del algoritmo gen�tico
	long m_tam;
	//!Variable que almacena el limite superior del eje horizontal de las gr�ficas creadas
	long m_NumGen;
	//!Variable que almacena el m�ximo valor presente en el arreglo \a p_valores
	double m_max;
	//!Variable que almacena el m�nimo valor presente en el arreglo \a p_valores
	double m_min;
	
	//!Enumeraci�n a los controles de la ventana
	enum Controles
	{
		TEXT_CONSOLA=3000,
		OTRO
	};

};

//!Clase derivada de \a wxFrame de la librer�a \a wxWindows que contiene las ventanas de la clase \a AGVentana.
/*! Un objeto de est� clase contiene una barra de men�, donde el usuario puede seleccionar varias
funciones como guardar los archivos creados, crear gr�ficas, cambiar los colores de las gr�ficas,
detener y restablecer la visualizaci�n de la ejecuci�n del algoritmo e informaci�n de la aplicaci�n.
Tambi�n contiene ventanas adicionales en las que aparecen el proceso del algoritmo y las gr�ficas
con las medidas de desempe�o del mismo. 
*/
class AGFrame : public wxFrame
{
public:
	//!Constructor de la clase AGFrame
	AGFrame(const wxChar *title, int xpos, int ypos, int width, int height);
	//!Destructor de la clase AGFrame
	~AGFrame(){}

//FUNCIONES
	//!M�todo que retorna la p�gina identificada por \a ID
	wxWindow* ObtenerPagina(int ID);
	//!M�todo que inserta una nueva p�gina en el control \a m_notebook
	void AgregarPagina(AGVentana* p_pagina, wxString nombre);
	//!M�todo que fija la p�gina identificada por \a ID
	bool SetPagina(int ID);

	
//CONTROLES
    //!Apuntador al control de texto donde se muestran los datos seleccionados para cada iteraci�n del algoritmo
    wxTextCtrl *m_pTextCtrl;
	//!Apuntador al objeto \a wxNoteBook que contiene las p�ginas que muestran las gr�ficas creadas
	wxNotebook *m_pNotebook;
	//!Apuntador al objeto derivado de AGVentana que contiene las gr�ficas creadas de la ejecuci�n del algoritmo gen�tico.
	AGVentana *m_pPagina;


	//!Cadena de caracteres correspondiente al nombre del archivo de donde se extraen los valores que ser�n graficados
    wxString m_NomArch;


protected:
	//Declaraci�n de la tabla de eventos
	DECLARE_EVENT_TABLE()
	
private:
  
//EVENTOS
	//!Evento que cambia el color de fondo de la gr�fica
	void OnMenuFondo(wxCommandEvent &event);
	//!Evento que cambia el color de los ejes de la gr�fica
	void OnMenuGrilla(wxCommandEvent &event);
	//!Evento que cambia el color de la l�nea de la gr�fica
	void OnMenuLinea(wxCommandEvent &event);
	//!Evento que crea las gr�ficas en cada p�gina
	void OnMenuGrafica(wxCommandEvent &event);
	//!Evento que guarda una gr�fica
	void OnMenuFileSave(wxCommandEvent &event);
	//!Evento que salva una gr�fica
	void OnMenuFileQuit(wxCommandEvent &event);
	//!Evento que muestra un cuadro de informaci�n de la aplicaci�n
	void OnMenuInfoAbout(wxCommandEvent &event);
	//!Evento que destruye la aplicaci�n
	void OnClose(wxCloseEvent &event);
	//!Evento que detiene la presentaci�n de la informaci�n en el control de texto  sin detener la ejecuci�n del algoritmo
	void OnCongelar(wxCloseEvent &event);

	//!M�todo que abre el cuadro de dialogo de selecci�n de colores
	void SelColor(wxColour& color);

	 //!Objeto que ajusta el tama�o del objeto notebook
	wxNotebookSizer *m_pSizerNB;
	//!Objeto que ajusta el tama�o del frame
	wxBoxSizer *m_pSizerFrame;

	//!Apuntador a la ventana que contiene al objeto notebook
	wxPanel *m_pPanel;
	//!Variable que indica si se encuentra detenida o no la actualizaci�n de informaci�n en el control de texto
	bool m_congelar;


//MENUS
	//!Apuntador a la barra de menus que se encuentra en el borde superior de la ventana
	wxMenuBar *m_pMenuBar;
	//!Apuntador al menu Archivo
	wxMenu *m_pFileMenu;
	//!Apuntador al menu Informaci�n
	wxMenu *m_pInfoMenu;
	//!Apuntador al menu Gr�fica
	wxMenu *m_pMenuGrafica;
	//!Apuntador al menu Congelar
	wxMenu *m_pCongelarMenu;

	//!Cada valor identifica a un menu de AGFrame
	enum menus
	{
		MENU_FILE_SAVE=1000,
		MENU_FILE_QUIT,
		MENU_INFO_ABOUT,
		MENU_GRAFICA_FONDO,
		MENU_GRAFICA_GRILLA,
		MENU_GRAFICA_LINEA,
		MENU_GRAFICA_GRAFICAR,
		MENU_CONGELAR,
		NOTEBOOK
	};
};


/*********************************************************
			DECLARACION DE MACROS
**********************************************************/

//!Macro que crea una aplicaci�n gr�fica para UNGen�tico
/*!Declara una clase derivada de \a wxApp que representa a la aplicaci�n 
por si misma junto con un apuntador al objeto que contiene a las otras ventanas y 
sus controles; tambi�n declara el m�todo \a OnInit() 
que corresponde a la funci�n principal donde se invocan las instrucciones que 
desarrolla la aplicaci�n. Por �ltimo usa la macro IMPLEMENT_APP(nombreApp) 
para crear una instancia de la clase derivada de \a wxApp.

\param nombreApp nombre a dar a la clase para la aplicaci�n, derivada de \a wxApp. \n
\param punteroFrame nombre del apuntador al objeto de la clase AGFrame.*/
#define DECLARAR_APLICACION(nombreApp)					\
	class nombreApp : public wxApp						\
	{													\
	public:												\
		/*Puntero al frame*/							\
		AGFrame *m_pFrame;								\
		/*Programa principal*/							\
		virtual bool OnInit();							\
	};													\
	IMPLEMENT_APP(nombreApp)


//!Ejecuta los eventos pendientes de la aplicaci�n.
/*!Debe usarse en medio de largos procesos para ejecutar eventos 
como la actualizaci�n de la visualizaci�n de la ventana*/
#define EJECUTAR_EVENTOS										\
	{ while(wxGetApp().Pending()) wxGetApp().Dispatch(); }


#endif		//__VENTANA_H