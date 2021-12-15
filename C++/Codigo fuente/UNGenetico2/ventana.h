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

//!Clase derivada de \a wxScrolledWindow de la librería \a wxWindows que contiene objetos visibles por el usuario.
/*!Al establecer la utilización del ambiente gráfico se crean dos tipos de ventana, una que contiene
un control de texto en el que aparece la ejecución del algoritmo, y otras que contienen las gráficas
de desempeño del algoritmo genético. 
Sin embargo, el usuario puede crear una clase derivada de AGVentana y adicionarle cualquier control que pertenezca a la librería \a wxWindows. 
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
	//!Método que retorna el objeto de la clase \a wxFrame en el que está contenido
	wxWindow* ObtenerFrame()
	{
		return GetGrandParent()->GetParent();
	}
	
	//!Método que realiza gráficas descriptivas de datos almacenados en un archivo de texto
	void GrafTexto();

//CONTROLES
	//!Apuntador al control de texto donde se almacena la información seleccionada para cada iteración del algoritmo
	wxTextCtrl *m_pTxtConsola;
	//!Apuntador al objeto donde se almacenan las gráficas creadas
	wxBitmap *m_pBitmap;
	//!Objeto que define el color de la línea de las gráficas creadas  
	wxColour m_ColBrush;
	//!Objeto que define el color de los ejes de las gráficas creadas  
	wxColour m_ColGrilla;
	//!Objeto que define el color del fondo de las gráficas creadas  
	wxColour m_ColGraf;
	//!Cadena de caracteres donde se almacena el nombre del archivo de texto del que se obtienen los datos para crear las gráficas
	wxString m_NameArch;
	//!Cadena de caracteres donde se almacena el nombre de la columna de datos que serán graficados
	wxString m_Titulo;
	//!Variable que indica la línea del archivo \a NameArch donde inician los datos que se van a graficar
	int m_Ini;
	//!Variable que indica la columna del archivo \a NameArch correspondiente a los datos que se van a graficar
	int m_Columna;

protected:

//EVENTOS

	DECLARE_EVENT_TABLE()
	//!Método que actualiza las ventanas que contienen las gráficas creadas
	void OnPaint(wxPaintEvent& event);
	//!Método que elimina la información presente en \a m_TxtConsola cuando su capacidad de almacenamiento llega al límite
	void OnMaxLen(wxCommandEvent& event);

  //FUNCIONES PROTEGIDAS
	//!Método que crea los ejes para las gráficas creadas
	void GrafGrilla();
	//!Método que halla el límite superior del eje horizontal de las gráficas creadas
	long Extremo(long valor);
	//!Método que valida los valores del archivo de texto para ser graficados
	double ObtenerY(int n, int col);
	//!Método que obtiene los valores del archivo de texto para ser graficados
	void ObtenerValoresGrafica();
	//!Método que obtiene y valida los extremos inferior y superior del eje vertical de cada gráfica creada
	void ObtenerExtremos();

	
	//!Dispositivo propio de \a wxWindows que provee un medio para realizar una gráfica en un mapa de bits
	wxMemoryDC m_dib;
	//!Dispositivo propio de \a wxWindows que permite mostrar la gráfica creada en una ventana
	wxClientDC m_dibCopia;
	//!Variable que indica el ancho de la gráfica
	int m_ancho;
	//!Variable que indica el alto de la gráfica
	int m_alto;
	//!Apuntador a un arreglo de números reales donde se almacenan los valores que serán graficados
	double *m_pValores;
	//!Apuntador al archivo de texto de donde se extraen los valores que serán graficados
	wxTextFile *m_pTextarch;
    //!Apuntador al objeto que ajusta el tamaño de los controles contenidos en una ventana
	wxSizer *m_pSizer;

private:

	//!Variable que almacena el número de final de generaciones de la ejecución del algoritmo genético
	long m_tam;
	//!Variable que almacena el limite superior del eje horizontal de las gráficas creadas
	long m_NumGen;
	//!Variable que almacena el máximo valor presente en el arreglo \a p_valores
	double m_max;
	//!Variable que almacena el mínimo valor presente en el arreglo \a p_valores
	double m_min;
	
	//!Enumeración a los controles de la ventana
	enum Controles
	{
		TEXT_CONSOLA=3000,
		OTRO
	};

};

//!Clase derivada de \a wxFrame de la librería \a wxWindows que contiene las ventanas de la clase \a AGVentana.
/*! Un objeto de está clase contiene una barra de menú, donde el usuario puede seleccionar varias
funciones como guardar los archivos creados, crear gráficas, cambiar los colores de las gráficas,
detener y restablecer la visualización de la ejecución del algoritmo e información de la aplicación.
También contiene ventanas adicionales en las que aparecen el proceso del algoritmo y las gráficas
con las medidas de desempeño del mismo. 
*/
class AGFrame : public wxFrame
{
public:
	//!Constructor de la clase AGFrame
	AGFrame(const wxChar *title, int xpos, int ypos, int width, int height);
	//!Destructor de la clase AGFrame
	~AGFrame(){}

//FUNCIONES
	//!Método que retorna la página identificada por \a ID
	wxWindow* ObtenerPagina(int ID);
	//!Método que inserta una nueva página en el control \a m_notebook
	void AgregarPagina(AGVentana* p_pagina, wxString nombre);
	//!Método que fija la página identificada por \a ID
	bool SetPagina(int ID);

	
//CONTROLES
    //!Apuntador al control de texto donde se muestran los datos seleccionados para cada iteración del algoritmo
    wxTextCtrl *m_pTextCtrl;
	//!Apuntador al objeto \a wxNoteBook que contiene las páginas que muestran las gráficas creadas
	wxNotebook *m_pNotebook;
	//!Apuntador al objeto derivado de AGVentana que contiene las gráficas creadas de la ejecución del algoritmo genético.
	AGVentana *m_pPagina;


	//!Cadena de caracteres correspondiente al nombre del archivo de donde se extraen los valores que serán graficados
    wxString m_NomArch;


protected:
	//Declaración de la tabla de eventos
	DECLARE_EVENT_TABLE()
	
private:
  
//EVENTOS
	//!Evento que cambia el color de fondo de la gráfica
	void OnMenuFondo(wxCommandEvent &event);
	//!Evento que cambia el color de los ejes de la gráfica
	void OnMenuGrilla(wxCommandEvent &event);
	//!Evento que cambia el color de la línea de la gráfica
	void OnMenuLinea(wxCommandEvent &event);
	//!Evento que crea las gráficas en cada página
	void OnMenuGrafica(wxCommandEvent &event);
	//!Evento que guarda una gráfica
	void OnMenuFileSave(wxCommandEvent &event);
	//!Evento que salva una gráfica
	void OnMenuFileQuit(wxCommandEvent &event);
	//!Evento que muestra un cuadro de información de la aplicación
	void OnMenuInfoAbout(wxCommandEvent &event);
	//!Evento que destruye la aplicación
	void OnClose(wxCloseEvent &event);
	//!Evento que detiene la presentación de la información en el control de texto  sin detener la ejecución del algoritmo
	void OnCongelar(wxCloseEvent &event);

	//!Método que abre el cuadro de dialogo de selección de colores
	void SelColor(wxColour& color);

	 //!Objeto que ajusta el tamaño del objeto notebook
	wxNotebookSizer *m_pSizerNB;
	//!Objeto que ajusta el tamaño del frame
	wxBoxSizer *m_pSizerFrame;

	//!Apuntador a la ventana que contiene al objeto notebook
	wxPanel *m_pPanel;
	//!Variable que indica si se encuentra detenida o no la actualización de información en el control de texto
	bool m_congelar;


//MENUS
	//!Apuntador a la barra de menus que se encuentra en el borde superior de la ventana
	wxMenuBar *m_pMenuBar;
	//!Apuntador al menu Archivo
	wxMenu *m_pFileMenu;
	//!Apuntador al menu Información
	wxMenu *m_pInfoMenu;
	//!Apuntador al menu Gráfica
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

//!Macro que crea una aplicación gráfica para UNGenético
/*!Declara una clase derivada de \a wxApp que representa a la aplicación 
por si misma junto con un apuntador al objeto que contiene a las otras ventanas y 
sus controles; también declara el método \a OnInit() 
que corresponde a la función principal donde se invocan las instrucciones que 
desarrolla la aplicación. Por último usa la macro IMPLEMENT_APP(nombreApp) 
para crear una instancia de la clase derivada de \a wxApp.

\param nombreApp nombre a dar a la clase para la aplicación, derivada de \a wxApp. \n
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


//!Ejecuta los eventos pendientes de la aplicación.
/*!Debe usarse en medio de largos procesos para ejecutar eventos 
como la actualización de la visualización de la ventana*/
#define EJECUTAR_EVENTOS										\
	{ while(wxGetApp().Pending()) wxGetApp().Dispatch(); }


#endif		//__VENTANA_H