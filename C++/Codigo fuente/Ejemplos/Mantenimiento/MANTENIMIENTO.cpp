
#include "MANTENIMIENTO.h"


/************************************************************************/
/*				AGMantenimiento:   Implementacion						*/
/************************************************************************/

void AGMantenimiento::inicializarParametros()
{
	m_TamanoPoblacion= GNumIndiv;
	m_GeneracionMaxima= GNumGeneraciones;
}


void AGMantenimiento::codificacion(Individuo* pInd, int estado)
{
	ADICIONAR_GENARREGLO_ENTERO	(pInd, 0, SemMantenimiento, GNumMaq, GNumMaq, 0, NP-1, 0)
}


void AGMantenimiento::definirOperadores()
{
	DEFINIR_OPERADOR_PROBABILIDAD( OperadorProbabilidadProporcional ) //OperadorProbabilidadHomogenea,OperadorProbabilidadLineal(0.5),OperadorProbabilidadProporcional
	DEFINIR_OPERADOR_SELECCION( OperadorSeleccionEstocasticaRemplazo )
	DEFINIR_OPERADOR_PAREJAS( OperadorParejasAdyacentes )	//OperadorParejasExtremos, OperadorParejasAleatoria
	DEFINIR_OPERADOR_REPRODUCCION(OperadorReproduccionMejoresEntrePadresEHijos() )//OperadorReproduccionMejorPadreMejorHijo(),OperadorReproduccionCruceSimple(),OperadorReproduccionDosPadresDosHijos()
	ADICIONAR_OPERADOR_ADAPTACION(	OperadorAdaptacionElitismo()	)	//OperadorAdaptacionElitismo()	OperadorAdaptacionProbMutacion(ADAPTACION_PROBMUTACION_EXPONENCIAL)		OperadorAdaptacionNumIndividuos()	OperadorAdaptacionProbMutacion()	
	ADICIONAR_OPERADOR_MUTACION( OperadorMutacionArregloEntero(GProbMutacion) )
}


//Funcion Objetivo
double AGMantenimiento::objetivo()
{
	int i, j;
	double obj=0, prioridadi, prioridadj;
	wxString cod;
	bool condicion;
	int *CantidadMan = new int[NP];
	for(i=0 ; i < NP ; i++)
	{
		CantidadMan[i]=0;
	}
	for(i=0 ; i<GNumMaq; i++)
	{
		prioridadi = Codigos[i][3]*0.01;
		cod=CodEquipo[i].Left(1);
		for(j=i+1;j<GNumMaq; j++)
		{
			condicion=false;
			if ( (SemMantenimiento[i]==SemMantenimiento[j]) ) 
				condicion=true;
			else if( (SemMantenimiento[i]==(SemMantenimiento[j]+NP2)%NP) )
				condicion=true;
			if(condicion)
			{
				if( cod==CodEquipo[j].Left(1) )
				{
					prioridadj = Codigos[j][3]*0.01;
					obj+= CASTIGO*(prioridadi+prioridadj);
				}
			}
		}
		
		CantidadMan[SemMantenimiento[i]]++;
	}
	int maxMant = 1 + (GNumMaq-1)/NP;
	for(i=0; i<NP; i++)
	{
		if(CantidadMan[i]>maxMant)
			obj+=(CantidadMan[i]-maxMant);		
	}	
	delete [] CantidadMan;
	return obj;
}



void AGMantenimiento::guardarProgramacion()
{

	int i,j, SemM, SemT, SemS;

	FILE *pArch = fopen("ProgrSemanal.xls","wt");
	if(!pArch) { m_pFrame->m_pTextCtrl->AppendText("\nNO SE PUDO GUARDAR LA PROGRAMACION. PUEDE QUE EL ARCHIVO ESTÉ EN USO POR OTRO PROGRAMA.\n\n"); return;}
	for(i=0;i<NP;i++)
		fprintf( pArch, "\t%2d", i+1);
	fprintf(pArch,"\n");


	wxString (*Prog)[NP] = new wxString[GNumMaq][NP];
	bool W,M,T,S,A;
	wxString Cad;
	codificacion(m_pMejorEnLaHistoria, ESTADO_DECODIFICAR);
	for(i=0;i<GNumMaq;i++)
	{
		SemM = SemMantenimiento[i] % 4;
		SemT = SemMantenimiento[i] % 12;
		SemS = SemMantenimiento[i] % 24;		

		W= (Codigos[i][2]&1  != 0);
		M= (Codigos[i][2]&2  != 0);
		T= (Codigos[i][2]&4  != 0);
		S= (Codigos[i][2]&8  != 0);
		A= (Codigos[i][2]&16 != 0);

		if(W)
			Cad = "W";
		else
			Cad = " ";
			
		for(j=0;j<NP;j++)
			Prog[i][j]=Cad;
		
		if(M)
		{			
			for(j=0; j<12; j++)
			{
				Prog[i][ SemM+4*j ] = "M";
			}
		}
		
		if(T)
		{
			for(j=0; j<4; j++)
			{
				Prog[i][ SemT+12*j ] = "T";
			}
		}

		if(S)
		{
			for(j=0; j<2; j++)
			{
				Prog[i][ SemS+24*j ] = "S";
			}
		}

		if(A)
			Prog[i][ SemMantenimiento[i] ] = "A";

		fprintf(pArch, CodEquipo[i]);
		for(j=0;j<NP;j++)					
			fprintf(pArch, "\t%s", Prog[i][j]);
		fprintf(pArch,"\n");

	}
	fclose(pArch);
	delete [] Prog;
}


void AGMantenimiento::guardar(Individuo& Ind, FILE *pArch, int generacion)
{	
	pArch = fopen("Individuos.txt","at");
	if(!pArch) return;
	fprintf(pArch,"\nGeneracion:\t %2d", generacion);
	fprintf(pArch,"\n\tFuncion Objetivo: %g\t\n\tIndividuo:", Ind.objetivo(true));
	for(int i=0; i < GNumMaq; i++)
	{
		fprintf(pArch, "  %2d", SemMantenimiento[i]);
	}
	fclose(pArch);
}


void AGMantenimiento::mostrar(Individuo& Ind)
{
	wxString Cad;
	Cad << "\nFunción Objetivo: " << Ind.objetivo() << "\n";
	m_pFrame->m_pTextCtrl->AppendText(Cad);
}


/****************************************************************************/
/*				AGApp:   Implementacion	de la aplicación					*/
/****************************************************************************/
DECLARAR_APLICACION(AGApp)


//!Se crea el frame para la aplicación
bool AGApp::OnInit()
{
	//Se debe crear un frame
	m_pFrame = new AGFrame("Aplicación de UNGenetico 2.0. Programación de mantenimiento", 50, 50, 700, 500);
	SetTopWindow(m_pFrame);
	//Si el frame se muestra aca se muestran cada salida pero se demora mas
	m_pFrame->AgregarPagina(new MiVentana(m_pFrame->m_pNotebook,ID_ENTRADA),"Datos de entrada");
	m_pFrame->Show(true);
	return true;
}


/****************************************************************************/
/*				MiVentana:   Implementacion									*/
/****************************************************************************/

MiVentana::MiVentana(wxWindow *parent, wxWindowID id) : AGVentana(parent,id)
{
	int i;
	///Sizer para alinear los controles
	m_pSizer=new wxFlexGridSizer(4,4,5,5);
	m_pTextArchivo = new wxTextCtrl(this,-1,"",wxDefaultPosition,wxSize(300,-1));
	m_pButtonAceptar = new wxButton(this,BUTTON_ACEPTAR,"Aceptar");
	m_pButtonArchivo = new wxButton(this,BUTTON_ARCHIVO,"Archivo");

	//los controles se adicionan al sizer
	for(i=0;i<5;i++)
	{
		m_pSizer->Add(20,20);
	}
	m_pSizer->Add(new wxStaticText(this,-1,"Archivo donde se encuentran los datos"));
	m_pSizer->Add(m_pTextArchivo);
	m_pSizer->Add(m_pButtonArchivo);
	for(i=0;i<6;i++)
	{
		m_pSizer->Add(20,20);
	}
	m_pSizer->Add(m_pButtonAceptar);

	//Estas tres lineas siempre deberian ir al final
	m_pSizer->Layout();
	SetSizer(m_pSizer);
	SetAutoLayout(TRUE);
}

BEGIN_EVENT_TABLE(MiVentana, AGVentana)
	EVT_BUTTON(BUTTON_ACEPTAR, MiVentana::OnAceptar)
	EVT_BUTTON(BUTTON_ARCHIVO, MiVentana::OnArchivo)
END_EVENT_TABLE()




void MiVentana::OnArchivo(wxCommandEvent &event)
{
	wxFileDialog *dlg = new wxFileDialog(this, "Abre un archivo", "", "", "Archivos de texto(*.txt)|*.txt", wxOPEN, wxDefaultPosition);
	if ( dlg->ShowModal() == wxID_OK )
	{
		m_pTextArchivo->SetValue(dlg->GetPath());
	}
	dlg->Destroy();
}


void MiVentana::ObtenerValores()
{
	GNumMaq = ObtenerEntero(0,0);
	GNumIndiv = ObtenerEntero(0,1);
	GNumGeneraciones = ObtenerEntero(0,2);
	GProbMutacion = ObtenerReal(0,3);

	CodEquipo = new wxString[GNumMaq];
	Codigos = new int [GNumMaq][4];
	int i,j;
	for(i=0;i<GNumMaq;i++)
	{
		CodEquipo[i] = ObtenerCad(i+2,0);
	}
	
	for(i=0;i<GNumMaq;i++)
	{
		for(j=0;j<3;j++)
		{
			Codigos[i][j] = ObtenerEntero(i+2,j+1);
		}
		Codigos[i][3]= Codigos[i][0]*Codigos[i][1]; //prioridad
	}
}

//!Función que devuelve una cadena y recibe la linea y la columna, donde se encuentra el valor*/
wxString MiVentana::ObtenerCad(int fila, int col)
{
	wxString cad;
	//La separación entre columnas es tab
	char tab=9;
	int pos,i;
	cad = m_pArchEntrada->GetLine(fila);
	pos=cad.Freq(tab);	
	for(i=1;i<=col;i++)
	{
		cad=cad.AfterFirst(tab);
	}
	cad=cad.BeforeFirst(tab);
	cad=cad.Trim(true);
	cad=cad.Trim(false);
	return(cad);
}

long MiVentana::ObtenerEntero(int fila, int col)
{
	wxString Cad;
	long valor;
	Cad = ObtenerCad(fila, col);
	Cad.ToLong(&valor);
	return valor;
}
               
double MiVentana::ObtenerReal(int fila, int col)
{
	wxString Cad;
	double valor;
	Cad = ObtenerCad(fila, col);
	Cad.ToDouble(&valor);
	return valor;
}


void MiVentana::OnAceptar(wxCommandEvent &event)
{
	///AQUI SE REALIZA EL PROCESO DEL ALGORITMO GENÉTICO

	//Leer datos de entrada
	m_pArchEntrada = new wxTextFile;
	m_pArchEntrada->Open(m_pTextArchivo->GetValue());
	ObtenerValores();	
	m_pArchEntrada->Close();
	delete m_pArchEntrada;
	/*Obtención del frame para comunicarle al algoritmo donde debe mostrar los resultados*/
	AGFrame *pframe = (AGFrame*)(ObtenerFrame());
	pframe->SetPagina(ID_CONSOLA);

	//creacion del archivo de salida. (es borrado en caso de ya existir)
	FILE* pArchIndividuos = fopen("Individuos.txt", "wt");
	if(pArchIndividuos) fclose(pArchIndividuos);

	//medicion del tiempo de proceso
	time_t   inicio, fin;
	time( &inicio );

	//creación y ajustes del algoritmo genético
	AGMantenimiento *MiAg = new AGMantenimiento(pframe);
	// Utilizacion Paso a Paso del Algoritmo
	int t=0;
	
	MiAg->iniciarOptimizacion();
	do
	{
		MiAg->iterarOptimizacion();
		
		if(t%5==0)
		{
			wxString Cad;
			Cad << "\n\nGeneración  " << t << "\n";
			pframe->m_pTextCtrl->AppendText(Cad);
			Individuo Ind = *MiAg->m_pMejorEnEstaGeneracion;
			MiAg->mostrar(Ind);
			MiAg->guardar(Ind, pArchIndividuos, t);
		}
		

		//Prestar el mando a la aplicación para que no se "cuelgue"
		EJECUTAR_EVENTOS;	
		t++;
	}while(!MiAg->finalizar());

	//Mostrar y guardar los resultados
	MiAg->mostrarMedidas();
	MiAg->guardarProgramacion();
	time( &fin );

	FILE* ArchGeneral = fopen("General.txt", "at");
	if(ArchGeneral)
	{
		fprintf(ArchGeneral, "\nMANTENIMIENTO. N.Individuos =%5d, N.Generaciones =%5d, N.Maquinas =%3d. \n", GNumIndiv, GNumGeneraciones, GNumMaq );
		fprintf(ArchGeneral, " Mejor F.O.: %f\tGeneración mejor F.O: %d.\tProb.Mutacion: %f\tTiempo de proceso: %f\n", MiAg->m_pMejorEnLaHistoria->objetivo(), (int)MiAg->m_GeneracionDelMejorEnLaHistoria, GProbMutacion, difftime( fin, inicio ));
		fclose(ArchGeneral);
	}

	//Liberar memoria
	delete MiAg;	

	pframe->m_pTextCtrl->AppendText("FINALIZADO");
}