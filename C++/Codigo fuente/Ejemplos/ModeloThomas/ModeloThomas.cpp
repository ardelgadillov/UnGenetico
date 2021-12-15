#define USAR_VENTANA

#include "UNGenetico.h"

#define ID_ENTRADA 6000

long n;
double *Qfinal,*Qreal, *P, *ET;

class MiVentana : public AGVentana
{
public:
	MiVentana(wxWindow *parent, wxWindowID id = -1);
	///Controles utilizados en la nueva página
	wxTextCtrl *m_pTextArchivo;
	wxButton *m_ButtonAceptar;
	wxButton *m_ButtonArchivo;
	wxTextFile *m_pArchEntrada;

	void OnAceptar(wxCommandEvent &event);
	void OnArchivo(wxCommandEvent &event);

	double ObtenerY(int n, int col);
	void ObtenerValores();

	DECLARE_EVENT_TABLE()

	enum botones
	{
		BUTTON_ACEPTAR=5000,
		BUTTON_ARCHIVO
	};

};

DECLARAR_ALGORITMO(ModeloThomas)
	void inicializarParametros();
	void definirOperadores();

	void mostrar(Individuo& Ind, int generacion);
	void GuardarParametros(Individuo& Ind, int generacion);
	void GuardarCaudales(Individuo& Ind, int generacion);

	double a,b,c,d,swo,sgo;
	FILE *caudal;
	FILE *par;

FIN_DECLARAR_ALGORITMO

void ModeloThomas::inicializarParametros()
{
	m_TamanoPoblacion = 300;
	m_GeneracionMaxima = 1000;	
}

void ModeloThomas::definirOperadores()
{
	DEFINIR_OPERADOR_PROBABILIDAD( OperadorProbabilidadLineal() ) //OperadorProbabilidadHomogenea(),OperadorProbabilidadLineal(),OperadorProbabilidadProporcional()
	DEFINIR_OPERADOR_SELECCION( OperadorSeleccionEstocasticaRemplazo() )
	DEFINIR_OPERADOR_PAREJAS( OperadorParejasAdyacentes )//OperadorParejasExtremos(), OperadorParejasAleatoria()
	DEFINIR_OPERADOR_REPRODUCCION(OperadorReproduccionMejoresEntrePadresEHijos() )//OperadorReproduccionMejorPadreMejorHijo(),OperadorReproduccionCruceSimple(),OperadorReproduccionDosPadresDosHijos()
	ADICIONAR_OPERADOR_ADAPTACION(	OperadorAdaptacionElitismo	)	//OperadorAdaptacionElitismo()	OperadorAdaptacionProbMutacion(ADAPTACION_PROBMUTACION_EXPONENCIAL)		OperadorAdatacionNumIndividuos()	OperadorAdaptacionProbMutacion()
	ADICIONAR_OPERADOR_FINALIZACION( OperadorFinalizacionOffline )

	for(int i=0;i<=5;i++)
	{
		ADICIONAR_OPERADOR_MUTACION(  OperadorMutacionRealUniforme(0.07) )//OperadorMutacionRealUniforme, OperadorMutacionRealMuhlenbein , OperadorMutacionRealNoUniforme(this)
		ADICIONAR_OPERADOR_CRUCE( OperadorCruceRealIntermedioExtendido() )//OperadorCruceRealLinealBGA, OperadorCruceRealPlano, OperadorCruceRealAritmetico(0.4), OperadorCruceRealBLX(0.3),OperadorCruceRealLineal, OperadorCruceRealDiscreto,OperadorCruceRealHeuristico, OperadorCruceRealLinealBGA(this)
	}
}

void ModeloThomas::codificacion(Individuo* pInd, int estado)
{
	ADICIONAR_GENREAL(pInd, 0, a,   0.0001,   1.0,   0.5)
	ADICIONAR_GENREAL(pInd, 1, b,   10.0,  350.0, 150.0)
	ADICIONAR_GENREAL(pInd, 2, c,   0.0,   0.9,   0.5)
	ADICIONAR_GENREAL(pInd, 3, d,   0.001, 1.0,   0.5)
	ADICIONAR_GENREAL(pInd, 4, swo, 0.0,   500.0, 250.0)
	ADICIONAR_GENREAL(pInd, 5, sgo, 0.0,   500.0, 250.0)
}

//Funcion Objetivo
double ModeloThomas::objetivo()
{
	int i;
	double *W,*F,*Y,*Ro,*Rg,*Qg,*Sw,*Sg;
	W= new double[n];
	F= new double[n];
	Y= new double[n];
	Ro= new double[n];
	Rg= new double[n];
	Qg= new double[n];
	Sw= new double[n+1];
	Sg= new double[n+1];
	double FO;
	Sw[0]=swo;
	Sg[0]=sgo;
	for(i=0;i<n;i++)
	{
		W[i]=P[i]+Sw[i];
		F[i]=(W[i]+b)/(2*a);
		Y[i]=F[i]-sqrt(pow(F[i],2)-W[i]*(b/a));
		Sw[i+1]=Y[i]*exp(-ET[i]/b);
		Ro[i]=(1-c)*(W[i]-Y[i]);
		Rg[i]=c*(W[i]-Y[i]);
		Sg[i+1]=(Rg[i]+Sg[i])/(d+1);
		Qg[i]=d*Sg[i+1];
		Qfinal[i]=Ro[i]+Qg[i];
	}
	delete [] W;
	delete [] F;
	delete [] Y;
	delete [] Ro;
	delete [] Rg;
	delete [] Qg;
	delete [] Sw;
	delete [] Sg;
	FO=0;
	for(i=0;i<n;i++)
	{
		FO+=pow((Qreal[i]-Qfinal[i]),2);		
	}
	FO=sqrt(FO)/n;
	return(FO);	
}

void ModeloThomas::mostrar(Individuo& Ind, int generacion)
{
	codificacion(&Ind, ESTADO_DECODIFICAR);
	wxString Cad;
	Cad.Empty();
	Cad << "\nGeneración  " << generacion << "\n";
	Cad << a << "  " << b << "  " << c << "  " << d << "  " << swo << "  " <<sgo;
	Cad << "\nFunción Objetivo: " << Ind.objetivo() << "\n";
	m_pFrame->m_pTextCtrl->AppendText(Cad);
}

void ModeloThomas::GuardarParametros(Individuo& Ind, int generacion)
{	
	if(generacion==1)
	{
		par=fopen("parametros.txt","wt");
		if(!par) return;
		fprintf(par,"Columnas: {\n");
		fprintf(par,"Generacion\n");
		fprintf(par,"a\n");
		fprintf(par,"b\n");
		fprintf(par,"c\n");
		fprintf(par,"d\n");
		fprintf(par,"Sw0\n");
		fprintf(par,"Sg0\n");
		fprintf(par,"}\n");
		fclose(par);
	}
	par=fopen("parametros.txt","at");
	if(!par) return;
	codificacion(&Ind, ESTADO_DECODIFICAR);
	fprintf(par,"%i \t",generacion);
	fprintf(par,"%f \t",a);
	fprintf(par,"%f \t",b);
	fprintf(par,"%f \t",c);
	fprintf(par,"%f \t",d);
	fprintf(par,"%f \t",swo);
	fprintf(par,"%f \t",sgo);
	fprintf(par,"\n");
	fclose(par);
}

void ModeloThomas::GuardarCaudales(Individuo& Ind, int generacion)
{	
	int i=1;
	if(generacion==1)
	{
		caudal=fopen("caudales.txt","wt");
		if(!caudal) return;
		fprintf(caudal,"Columnas: {\n");
		fprintf(caudal," %d. Generacion\n", i);
		for(i=1;i<=n;i++)
		{
				fprintf(caudal,"Caudal %d\n", i);
		}
		fprintf(caudal,"}\n");
		fclose(caudal);
	}	
	caudal=fopen("caudales.txt","at");
	if(!caudal) return;
	codificacion(&Ind, ESTADO_DECODIFICAR);
	fprintf(caudal,"%i \t",generacion);
	for(i=0;i<n;i++)
	{
		fprintf(caudal,"%f \t",Qfinal[i]);
	}
	fprintf(caudal,"\n");
	fclose(caudal);
}


DECLARAR_APLICACION(AGApp)

//!Se crea el frame para la aplicación
bool AGApp::OnInit()
{
	//Se debe crear un frame
	m_pFrame = new AGFrame("Modelo de Thomas con UNGenetico 2.0", 50, 50, 700, 500);
	SetTopWindow(m_pFrame);
	//Si el frame se muestra aca se muestran cada salida pero se demora mas
	m_pFrame->AgregarPagina(new MiVentana(m_pFrame->m_pNotebook,ID_ENTRADA),"Datos de entrada");
	m_pFrame->Show(true);
	return true;
}

MiVentana::MiVentana(wxWindow *parent, wxWindowID id) : AGVentana(parent,id)
{
	int i;
	///Sizer para alinear los controles
	m_pSizer=new wxFlexGridSizer(4,4,5,5);
	m_pTextArchivo = new wxTextCtrl(this,-1,"",wxDefaultPosition,wxSize(300,-1));
	m_ButtonAceptar = new wxButton(this,BUTTON_ACEPTAR,"Aceptar");
	m_ButtonArchivo = new wxButton(this,BUTTON_ARCHIVO,"Archivo");

	//los controles se adicionan al sizer
	for(i=0;i<5;i++)
	{
		m_pSizer->Add(20,20);
	}
	m_pSizer->Add(new wxStaticText(this,-1,"Archivo donde se encuentran los datos"));
	m_pSizer->Add(m_pTextArchivo);
	m_pSizer->Add(m_ButtonArchivo);
	for(i=0;i<6;i++)
	{
		m_pSizer->Add(20,20);
	}
	m_pSizer->Add(m_ButtonAceptar);

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

//!Función que devuelve un valor y recibe la linea y la columna, donde se encuentra el valor*/
double MiVentana::ObtenerY(int fila, int col)
{
	wxString cad;
	//La separación entre columnas es tab
	char tab='\t';
	int pos,i;
	double y;
	cad=m_pArchEntrada->GetLine(fila);
	pos=cad.Freq(tab);
	//prueba si la columna existe
	if (col>pos+1)
	{
		wxLogMessage("Columna Inexistente");
		return 0;
	}
	//Escoge la cadena de la columna
	for(i=1;i<=col;i++)
	{
		cad=cad.AfterFirst(tab);
	}
	cad=cad.BeforeFirst(tab);
	cad=cad.Trim(true);
	cad=cad.Trim(false);

	if (!cad.ToDouble(&y))
	{
		wxLogMessage("Debe contener solo caracteres númericos");
		return 0;
	}
	return(y);
}

void MiVentana::ObtenerValores()
{
	wxString linea = m_pArchEntrada->GetLine(0);
	linea.ToLong(&n);
	Qfinal = new double[n];
	Qreal = new double[n];
	P = new double[n];
	ET = new double[n];
	for(int i=0;i<n;i++)
	{
		Qfinal[i] = 0;
		Qreal[i] = ObtenerY(1,i);
		P[i] = ObtenerY(2,i);
		ET[i] = ObtenerY(3,i);
	}
	wxString Qf,Qr,Pw,E;
	for(i=0;i<n;i++)
	{
		Qf << Qfinal[i] << "  ";
		Qr << Qreal[i] << "  ";
		Pw << P[i] << "  ";
		E << ET[i] << "  ";
	}
}
 
void MiVentana::OnAceptar(wxCommandEvent &event)
{
	//Leer datos de entrada
	m_pArchEntrada = new wxTextFile;
	m_pArchEntrada->Open(m_pTextArchivo->GetValue());
	ObtenerValores();	
	m_pArchEntrada->Close();
	delete m_pArchEntrada;

	////////Ejecucion del algoritmo
	AGFrame *pframe = (AGFrame*)(ObtenerFrame());
	pframe->SetPagina(ID_CONSOLA);
	ModeloThomas *MiAg = new ModeloThomas(pframe);
	
	// Utilizacion Paso a Paso del Algoritmo
	int t=0;
	MiAg->iniciarOptimizacion();
	do
	{
		MiAg->iterarOptimizacion();
		if((t%1)==0)
		{
			MiAg->mostrar(*MiAg->m_pMejorEnEstaGeneracion,t);
		}
		t++;
		//Guardar parámetros usará las variables del ultimo individuo decodificado. Es decir de mejor en esta generacion
		MiAg->GuardarParametros(*MiAg->m_pMejorEnEstaGeneracion, t);
		MiAg->GuardarCaudales(*MiAg->m_pMejorEnEstaGeneracion, t);
		EJECUTAR_EVENTOS
	}while(!MiAg->finalizar());
	MiAg->mostrar(*MiAg->m_pMejorEnEstaGeneracion,t);
	MiAg->mostrarMedidas();
	wxString Cad;
	Cad << "\n\n\t\t\tQreal\t\tQfinal\n";
	for(int i=0;i<n;i++)
	{
		Cad << "Caudal  " << i+1 << "\t\t" << Qreal[i] << "\t\t" << Qfinal[i] << "\n";
	}
	pframe->m_pTextCtrl->AppendText(Cad);
	delete MiAg;

}
