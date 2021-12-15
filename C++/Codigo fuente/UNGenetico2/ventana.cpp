#ifndef __VENTANA_CPP
#define __VENTANA_CPP

#include "ventana.h"


/*	AGFrame: IMPLEMENTACION	*/


/*!En el constructor se crean todos los objetos gr�ficos tales como barras de men�, 
barra de estado y ventanas que hacen parte de la aplicaci�n

\param title nombre que se mostrar� en la barra de t�tulo. \n
\param xpos posici�n horizontal de la ventana. \n 
\param ypos posici�n vertical de la ventana. \n
\param width ancho de la ventana. \n
\param height alto de la ventana.*/
AGFrame::AGFrame(const wxChar *title, int xpos, int ypos, int width, int height) :
       wxFrame((wxFrame *) NULL, -1, title, wxPoint(xpos, ypos), wxSize(width, height))
{
	m_congelar = false;

	//Se requiere para poder utilizar graficas en formato PNG
	wxImage::AddHandler(new wxPNGHandler);

	/*Creaci�n del notebook*/
	m_pPanel = new wxPanel(this, -1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxCLIP_CHILDREN | wxNO_BORDER);
	m_pNotebook = new wxNotebook(m_pPanel,NOTEBOOK);
    //m_notebook=new AGNotebook(m_pPanel,NOTEBOOK);
	m_pPagina = (AGVentana*)NULL;

    //m_notebook->AddPage(new AGVentana(m_notebook,ID_GEN_BOOL),"Gen Booleano");
    m_pNotebook->AddPage(new AGVentana(m_pNotebook,ID_CONSOLA),"Proceso");
	m_pNotebook->SetSelection(0);

	//Definici�n de la barra de menu
	m_pMenuBar = new wxMenuBar();

	// Definici�n del menu File
	m_pFileMenu = new wxMenu();
	//Definici�n de los Items del menu con nombre e indicaci�n en el statusbar
	m_pFileMenu->Append(MENU_FILE_SAVE, "&Guardar", "Guarda el contenido");
	m_pFileMenu->AppendSeparator();
	m_pFileMenu->Append(MENU_FILE_QUIT, "&Salir", "Cierra la aplicaci�n");
	m_pMenuBar->Append(m_pFileMenu, "&Archivo");

	//Definicion del menu Gr�fica
	m_pMenuGrafica = new wxMenu();
	//Definici�n de los Items
	m_pMenuGrafica->Append(MENU_GRAFICA_FONDO,"&Fondo","Cambia el color de fondo");
	m_pMenuGrafica->Append(MENU_GRAFICA_GRILLA,"&Grilla","Cambia el color de la grilla");
	m_pMenuGrafica->Append(MENU_GRAFICA_LINEA,"&Linea","Cambia el color de la grafica");
	m_pMenuGrafica->Append(MENU_GRAFICA_GRAFICAR,"G&raficar","Grafica Prueba.txt");
	m_pMenuBar->Append(m_pMenuGrafica,"&Gr�fica");

	//Definicion del menu Congelar
	m_pCongelarMenu = new wxMenu();
	m_pCongelarMenu->Append(MENU_CONGELAR,"&Congelar","Congela la Pantalla");
	m_pMenuBar->Append(m_pCongelarMenu,"&Congelar");

	// Definici�n del menu About
	m_pInfoMenu = new wxMenu();
	//Definici�n de los Items del menu con nombre e indicaci�n en el statusbar
	m_pInfoMenu->Append(MENU_INFO_ABOUT, "&Acerca de", "Muestra informaci�n acerca de la aplicaci�n");

	m_pMenuBar->Append(m_pInfoMenu, "&Info");

	SetMenuBar(m_pMenuBar);

	// Statusbar
	CreateStatusBar(1);
	SetStatusText("Inicio", 0);

	//Fija Sizers
	m_pSizerFrame = new wxBoxSizer(wxHORIZONTAL);
	m_pSizerNB = new wxNotebookSizer(m_pNotebook);
	m_pSizerFrame->Add(m_pSizerNB, 1, wxGROW | wxALL, 4);
	m_pSizerFrame->Layout();
	m_pPanel->SetSizer(m_pSizerFrame);
	m_pPanel->SetAutoLayout(TRUE);
}


///////////Procesamiento de los eventos del menu//////////

/*!Salva la gr�fica o el texto que se encuentra en la ventana actual*/
void AGFrame::OnMenuFileSave(wxCommandEvent &event)
{
	if (m_pNotebook->GetPageCount()>0)
	{
		m_pPagina = (AGVentana *) (m_pNotebook->GetPage(m_pNotebook->GetSelection()));
		/*wxFileDialog - Dialogo com�n para abrir o guardar un archivo*/
		if((m_pPagina->GetId())==ID_GRAFICA)
		{
			wxFileDialog *dlg = new wxFileDialog(this, "Guarda el archivo", "", "", "Gr�ficos de red port�tiles(*.png)|*.png", wxSAVE, wxDefaultPosition);
			if ( dlg->ShowModal() == wxID_OK )
			{
				m_pPagina->m_pBitmap->SaveFile( dlg->GetPath(), wxBITMAP_TYPE_PNG, (wxPalette*)NULL );
				SetStatusText(dlg->GetFilename(), 0);
			}
			dlg->Destroy();
		}
		else if((m_pPagina->GetId())==ID_CONSOLA)
		{
			wxFileDialog *dlg = new wxFileDialog(this, "Guarda el archivo", "", "", "Archivos de texto(*.txt)|*.txt", wxSAVE, wxDefaultPosition);
			if ( dlg->ShowModal() == wxID_OK )
			{
				m_pPagina->m_pTxtConsola->SaveFile(dlg->GetPath());
				SetStatusText(dlg->GetFilename(), 0);
			}
			dlg->Destroy();
		}		
		else
		{
			wxLogMessage("Todavia no se ha creado una gr�fica");
		}
	}
	else
	{
		wxLogMessage("Todavia no se ha creado una gr�fica");
	}
}

/*Este evento se ejecuta al escoger la opci�n cerrar de la barra de t�tulo o sus hom�logas 
dependiendo del sistema operativo*/
void AGFrame::OnMenuFileQuit(wxCommandEvent &event)
{
	Close(TRUE);
}

/*!Se ejecuta al escoger la opci�n <i>Acerca de</i> del men� de ayuda, muestra informaci�n
acerca de los autores de la aplicaci�n.*/
void AGFrame::OnMenuInfoAbout(wxCommandEvent &event)
{
	wxLogMessage("Aplicaci�n de UNGenetico 2.0\
		\n\n\nUniversidad Nacional de Colombia\
		\n\nAndr�s Delgadillo \t ardelgadillov@unal.edu.co\
		\nJorge Mario V�lez \t jmvelezg@unal.edu.co\
		\nSebasti�n Madrid  \t jsmadrida@unal.edu.co\
		\n\nDirigido por\
		\nIng. Oscar Duarte M.Sc., Ph.D.");
}

/*!Abre un dialogo para cambiar el color guardado en \a ColBrush y vuelve a realizar la gr�fica*/
void AGFrame::OnMenuFondo(wxCommandEvent &event)
{
	if (m_pNotebook->GetPageCount()>0)
	{
		SetStatusText("Cambia el color de fondo",0);
		m_pPagina = (AGVentana *) (m_pNotebook->GetPage(m_pNotebook->GetSelection()));
        if((m_pPagina->GetId())==ID_GRAFICA)
		{
			SelColor(m_pPagina->m_ColBrush);
			m_pPagina->GrafTexto();
		}
		else
		{
			wxLogMessage("Debe seleccionar una gr�fica");
		}
	}
	else
	{
		wxLogMessage("A�n no se ha creado ninguna gr�fica");
	}
}

/*!Abre un dialogo para cambiar el color definido en \a ColGrilla y vuelve a realizar la gr�fica */
void AGFrame::OnMenuGrilla(wxCommandEvent& event)
{
	if (m_pNotebook->GetPageCount()>0)
	{
		m_pPagina = (AGVentana *) (m_pNotebook->GetPage(m_pNotebook->GetSelection()));
		if((m_pPagina->GetId())==ID_GRAFICA)
		{
			SelColor(m_pPagina->m_ColGrilla);
			m_pPagina->GrafTexto();
		}
		else
		{
			wxLogMessage("Debe seleccionar una gr�fica");
		}
	}
	else
	{
		wxLogMessage("A�n no se ha creado ninguna gr�fica");
	}
}

/*!Abre un dialogo para cambiar el color definido en \a ColGraf y vuelve a realizar la gr�fica*/
void AGFrame::OnMenuLinea(wxCommandEvent& event)
{
	if (m_pNotebook->GetPageCount()>0)
	{
		m_pPagina = (AGVentana *) (m_pNotebook->GetPage(m_pNotebook->GetSelection()));
		if((m_pPagina->GetId())==ID_GRAFICA)
		{
			SelColor(m_pPagina->m_ColGraf);
			m_pPagina->GrafTexto();
		}
		else
		{
			wxLogMessage("Debe seleccionar una gr�fica");
		}
	}
	else
	{
		wxLogMessage("A�n no se ha creado ninguna gr�fica");
	}
}

/*!Se encarga de revisar cu�ntas columnas y datos existen en el archivo correspondiente 
a \a NomArch y grafica estos datos en p�ginas diferentes al invocar el m�todo \a GrafTexto()*/
void AGFrame::OnMenuGrafica(wxCommandEvent& event)
{
	if(m_NomArch.IsEmpty())
	{
		wxLogMessage("Para graficar, antes debe ejecutar el algoritmo gen�tico.");
		return;
	}
	wxTextFile arch(m_NomArch);
	if(arch.Exists()) 
		arch.Open();
	else
	{
		wxLogMessage("No existe archivo de salida.");		
		return;
	}

	int i=0,j,NumPag;
	//Comprueba el n�mero de columnas		
	wxString linea=arch.GetLine(0);
	if (linea.CompareTo("Columnas: {")==0)
	{
		while (linea.Find("}")==-1)
		{
			i++;
			linea=arch.GetNextLine();
		}
		//m_notebook->DeleteAllPages();
		NumPag=m_pNotebook->GetPageCount();
		for (j=0;j<i-2;j++)
		{
			m_pNotebook->AddPage(new AGVentana(m_pNotebook,ID_GRAFICA),arch.GetLine(j+2));
		}
		for(j=0;j<m_pNotebook->GetPageCount()-NumPag;j++)
		{
			m_pPagina = (AGVentana *) (m_pNotebook->GetPage(j+NumPag));
			m_pPagina->m_NameArch=m_NomArch;
			m_pPagina->m_Ini=i+1;
			m_pPagina->m_Titulo = arch.GetLine(j+2);
			m_pPagina->m_Columna=j+2;
			m_pPagina->GrafTexto();
		}
	}
	arch.Close();	
}


void AGFrame::OnCongelar(wxCloseEvent& event)
{
	m_pPagina = (AGVentana*)ObtenerPagina(ID_CONSOLA);
	m_congelar = !m_congelar;
	if(m_congelar)
		{
			m_pPagina->m_pTxtConsola->Freeze();
			m_pCongelarMenu->SetLabel(MENU_CONGELAR,"Des&congelar");
		}                                   
	else
		{
			m_pPagina->m_pTxtConsola->Thaw();
			m_pCongelarMenu->SetLabel(MENU_CONGELAR,"&Congelar");
		}
}


/*!Invoca el m�todo \a Destroy() para destruir todos los objetos descendientes del frame
luego de que han sido procesados con el fin de evitar problemas en la aplicaci�n.*/
void AGFrame::OnClose(wxCloseEvent& event)
{
	Destroy();
}

/*!\param color Objeto de define el color a modificar.*/
void AGFrame::SelColor(wxColour& color)
{	
	wxColourData data;
	wxColourDialog dialog(this, &data);

	if ( dialog.ShowModal() == wxID_OK )
	{
		color = dialog.GetColourData().GetColour();
	}	
}

/*!Realiza una b�squeda en todas las p�ginas contenidas en \a m_notebook,
y retorna la p�gina cuyo identificador coincida con \a ID.

\param ID Valor entero que identifica a cada p�gina.*/
wxWindow* AGFrame::ObtenerPagina(int ID)
{
	wxWindow *pagina1=NULL, *pagina2=NULL;
	int numPags = m_pNotebook->GetPageCount();
	for(int i=0;i<numPags;i++)
	{
		pagina1 = m_pNotebook->GetPage(i);
		if (pagina1->GetId()==ID)
		{
			i=numPags;
			pagina2=pagina1;
		}
	}
	return (pagina2);
}

/*!
\param p_pagina apuntador que especifica la nueva p�gina a insertar. \n
\param nombre cadena que especifica el t�tulo de la nueva p�gina.*/
void AGFrame::AgregarPagina(AGVentana *p_pagina, wxString nombre)
{
	m_pNotebook->InsertPage(0,p_pagina,nombre,true);
}

/*!Fija el foco en la p�gina identificada con \a ID retornando \a true si se encontro
la p�gina deseada

\param ID valor entero que identifica a cada p�gina.*/
bool AGFrame::SetPagina(int ID)
{
	bool exito=false;
	int numPags=m_pNotebook->GetPageCount();
	for (int i=0;i<numPags;i++)
	{
		if(m_pNotebook->GetPage(i)->GetId()==ID)
		{
			m_pNotebook->SetSelection(i);
			i = numPags;
			exito = true;
		}
	}
	return (exito);
}


//Tabla de eventos
BEGIN_EVENT_TABLE(AGFrame, wxFrame)
	//!Eventos del menu
	EVT_MENU(MENU_FILE_SAVE, AGFrame::OnMenuFileSave)
	EVT_MENU(MENU_FILE_QUIT, AGFrame::OnMenuFileQuit)
	EVT_MENU(MENU_INFO_ABOUT, AGFrame::OnMenuInfoAbout)
	EVT_MENU(MENU_GRAFICA_FONDO,AGFrame::OnMenuFondo)
	EVT_MENU(MENU_GRAFICA_GRILLA,AGFrame::OnMenuGrilla)
	EVT_MENU(MENU_GRAFICA_LINEA,AGFrame::OnMenuLinea)
	EVT_MENU(MENU_GRAFICA_GRAFICAR, AGFrame::OnMenuGrafica)
	EVT_MENU(MENU_CONGELAR, AGFrame::OnCongelar)
	//!Evento Cerrar la aplicaci�n
	EVT_CLOSE(AGFrame::OnClose)
END_EVENT_TABLE()

/*!En el constructor se coordinan las propiedades principales de la ventana
tales como posici�n y tama�o.

\param parent Apuntador a la ventana principal en el que esta contenida. 
\param id Valor entero que identifica a la ventana. 
\param pos Coordenadas que definen la posici�n de la ventana. 
\param size Conjunto de valores que definen el tama�o de la ventana. 
\param style Valor entero que define el estilo de la ventana.*/
AGVentana::AGVentana(wxWindow *parent, wxWindowID id,
    const wxPoint& pos, const wxSize& size, long style)
    : wxScrolledWindow(parent, id, pos, size, style)
{
	m_ColBrush=wxColour( 0 , 0 , 0 );
	m_ColGrilla=wxColour( 255 , 255 , 0 );
	m_ColGraf=*wxRED;

	if (id==ID_CONSOLA)
	{
		m_pSizer=new wxBoxSizer(wxVERTICAL);
		m_pTxtConsola = new wxTextCtrl(this,TEXT_CONSOLA,"",wxDefaultPosition,wxDefaultSize,wxTE_MULTILINE);
		m_pSizer->Add(m_pTxtConsola,1,wxGROW,wxALL);
		m_pSizer->Layout();
		SetSizer(m_pSizer);
		SetAutoLayout(TRUE);
	}
}


/*!Destruye las gr�ficas creadas por la aplicaci�n al cerrarla.*/
AGVentana::~AGVentana()
{
	if(GetId()==ID_GRAFICA)
		delete m_pBitmap;
}

void AGVentana::OnPaint(wxPaintEvent &event)
{
	//if(GetId()==ID_GRAFICA)
		if(m_dib.Ok())
		{
			wxPaintDC dc(this);
			dc.Blit(0, 0, m_ancho, m_alto, &m_dib, 0,0);
		}
	if (GetId()==ID_CONSOLA)
		m_pTxtConsola->Update();

}

void AGVentana::OnMaxLen(wxCommandEvent &event)
{
	m_pTxtConsola->Clear();
}

//Tabla de eventos
BEGIN_EVENT_TABLE(AGVentana, wxScrolledWindow)
	EVT_PAINT(AGVentana::OnPaint)
	EVT_TEXT_MAXLEN(TEXT_CONSOLA, AGVentana::OnMaxLen)
END_EVENT_TABLE()

/*!Los datos son extra�dos y validados a trav�s del m�todo \a ObtenerValoresGrafica().
Para cada columna de datos del archivo se crea un objeto de la clase \a wxBitmap 
que contiene una l�nea que indica la variaci�n de los valores de la columna correspondiente.*/
void AGVentana::GrafTexto()
{
	int i;
	long n1;
	double x1,y1,x2,y2;
	wxString cad;

	ObtenerValoresGrafica();
	ObtenerExtremos();
	//GetClientSize(&ancho,&alto);
	m_ancho=676;
	m_alto=394;
	m_pBitmap = new wxBitmap( m_ancho, m_alto );
	m_dib.SelectObject( *m_pBitmap );
	m_dib.Clear();
	n1=Extremo(m_NumGen);
	GrafGrilla();
	m_dib.SetPen(wxPen(m_ColGraf, 2 , wxSOLID));
	x1 = 0.2*m_ancho;
	y1 = m_alto*(0.1-0.7*(m_pValores[0]-m_max)/(m_max-m_min));
	for(i=1;i<m_tam;i++)
	{
		x2 = 0.2*m_ancho+0.7*i*m_ancho/n1;
		y2 = m_alto*(0.1-0.7*(m_pValores[i]-m_max)/(m_max-m_min));
		m_dib.DrawLine(x1,y1,x2,y2);
		x1=x2;
		y1=y2;
	}
	wxClientDC dibCopia(this);
	dibCopia.Blit(0, 0, m_ancho, m_alto, &m_dib,0,0);
}

/*!Convierte una cadena de caracteres presente en un archivo de texto
a un n�mero real.

\param n l�nea del archivo donde se encuentra el dato.
\param col columna del archivo donde se encuentra el dato.*/
double AGVentana::ObtenerY(int n, int col)
{
	wxString cad;
	//La separaci�n entre columnas es tab
	char tab='\t';
	int pos,i;
	double y;
	cad = m_pTextarch->GetLine(n);
	//wxLogMessage(cad);
	pos=cad.Freq(tab);
	//prueba si la columna existe
	if (col>pos+1)
	{
		wxLogMessage("Columna Inexistente");
		return 0;
	}
	//Escoge la cadena de la columna
	for(i=1;i<col;i++)
	{
		cad=cad.AfterFirst(tab);
	}
	cad=cad.BeforeFirst(tab);
	cad=cad.Trim(true);
	cad=cad.Trim(false);
	//ToDouble , realiza la conversi�n a double
	if (!cad.ToDouble(&y))
	{
		wxLogMessage("Debe contener solo caracteres n�mericos");
		return 0;
	}
	return(y);
}

/*!Crea los ejes principales y auxiliares tanto verticales como horizontales 
que se mostrar�n junto con las l�neas creadas por \a GrafTexto()*/
void AGVentana::GrafGrilla()
{
	long numlin;
	int i,eje_x;
	float x,y;
	double eje_y;
	wxString cad;
	m_dib.SetBrush(wxBrush(m_ColBrush,wxSOLID));
	m_dib.DrawRectangle(0 , 0, m_ancho, m_alto);
	m_dib.SetTextForeground(m_ColGrilla);
	m_dib.SetPen( wxPen(m_ColGrilla, 1, wxDOT) );
	m_dib.DrawText(m_Titulo, 0.4*m_ancho, 0.05*m_alto);
	numlin=Extremo(m_NumGen);
	x=0.2*m_ancho;
	y=0.8*m_alto;
	eje_y = m_min;
	eje_x=0;
	for(i=1;i<=11;i++)
	{
		m_dib.DrawLine(0.2*m_ancho,y,0.9*m_ancho,y);
		m_dib.DrawLine(x,0.1*m_alto,x,0.8*m_alto);
		cad.Empty();
		cad << eje_x;
		m_dib.DrawText(cad,x,0.8*m_alto);
		cad.Empty();
		cad << eje_y;
		m_dib.DrawText(cad,0.1*m_ancho,y-0.03*m_alto);
		eje_x += numlin/10;
		eje_y += (m_max - m_min)/10;
		x += 0.07*m_ancho;
		y -= 0.07*m_alto;
	}
}


/*!
\param y n�mero de datos que contiene el archivo de texto en cada columna.*/
long AGVentana::Extremo(long y)
{
	if ((y%10)!=0)
	{
		y=y+(10-(y%10));
	}
	return(y);
}

/*!Los datos se extraen del archivo cuyo nombre est� almacenado en la cadena \a NameArch
y se almacenan en el arreglo llamado \a p_valores*/
void AGVentana::ObtenerValoresGrafica()
{
	m_pTextarch = new wxTextFile(m_NameArch);
	if (m_pTextarch->Exists())
	{
		long n;
		int i;
		m_pTextarch->Open(m_NameArch);
		n = m_pTextarch->GetLineCount();
		m_NumGen=Extremo(ObtenerY(n-1,1));
		m_tam = n - m_Ini;
		m_pValores = new double[m_tam];
		for(i=0;i<m_tam;i++)
		{
			m_pValores[i] = ObtenerY( i+m_Ini, m_Columna);
		}
		m_pTextarch->Close();
	}
	else
	{
		wxLogMessage("Debe escoger un archivo de texto \n � \n Debe existir el archivo de texto especificado");
	}
}

/*!Cada uno de los datos l�mite se obtiene del arreglo \a p_valores.*/
void AGVentana::ObtenerExtremos()
{
	int i;
	m_max=-1e99;
	m_min=1e99;
	for(i=0;i<m_tam;i++)
	{
		if(m_pValores[i] > m_max) {m_max = m_pValores[i];}
		if(m_pValores[i] < m_min) {m_min = m_pValores[i];}
	}
	if (m_max==m_min)
	{
		m_max += abs(m_max);
		m_min -= abs(m_min);
		if (m_max == m_min)
		{
			m_max=5;
			m_min=-5;
		}
	}

}


#endif		//__VENTANA_CPP