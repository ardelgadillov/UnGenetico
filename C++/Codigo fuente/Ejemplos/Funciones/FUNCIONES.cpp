
#include "FUNCIONES.h"


/********************************************************************************/
/*				IMPLEMENTACION DE  AGFunciones							*/
/********************************************************************************/

void AGFunciones::inicializarParametros()
{
	MiVentana *m_pVentanaEntrada = (MiVentana*)(m_pFrame->ObtenerPagina(ID_ENTRADA));
	m_TamanoPoblacion = m_pVentanaEntrada->m_SpinTamPob->GetValue();
	m_CriterioFinalizacion = m_pVentanaEntrada->m_ChoiceFinalizacion->GetSelection();
	m_GeneracionMaxima = m_pVentanaEntrada->m_SpinGenMax->GetValue();
}


void AGFunciones::codificacion(Individuo* pInd, int estado)
{
	ADICIONAR_GENARREGLO_REAL(pInd, 0, x, Dimension, Dimension, Emin, Emax, 0  )
}


void AGFunciones::definirOperadores()
{
	DEFINIR_OPERADOR_PROBABILIDAD( OperadorProbabilidadLineal )		//OperadorProbabilidadHomogenea(), OperadorProbabilidadLineal(),
	DEFINIR_OPERADOR_SELECCION( OperadorSeleccionEstocasticaRemplazo )
	DEFINIR_OPERADOR_PAREJAS( OperadorParejasAleatorias )		//OperadorParejasExtremos(), OperadorParejasAleatorias()
	DEFINIR_OPERADOR_REPRODUCCION( OperadorReproduccionMejoresEntrePadresEHijos )		//OperadorReproduccionMejorPadreMejorHijo(),	 OperadorReproduccionCruceSimple(),  OperadorReproduccionDosPadresDosHijos()
	ADICIONAR_OPERADOR_ADAPTACION(	OperadorAdaptacionElitismo	)	//OperadorAdaptacionElitismo(this)	OperadorAdaptacionProbMutacion(this, ADAPTACION_PROBMUTACION_EXPONENCIAL)		OperadorAdatacionNumIndividuos(this)	OperadorAdaptacionProbMutacion(this)

	if(m_CriterioFinalizacion==1)	ADICIONAR_OPERADOR_FINALIZACION( OperadorFinalizacionOnline )
	else if(m_CriterioFinalizacion==2) ADICIONAR_OPERADOR_FINALIZACION( OperadorFinalizacionOffline )

	ADICIONAR_OPERADOR_MUTACION( OperadorMutacionArregloReal(0.01) )
}


//Funcion Objetivo según la función seleccionada
double AGFunciones::objetivo()
{
	double res=0.0;
	int i,j;
	switch (func)
	{
		case ESFERICO:
			for(i=0;i<Dimension;i++)
			{
				res += pow(x[i],2.0);
			}
			break;
		case ROSENBROCK:
			for(i=0;i<Dimension-1;i++)
			{
				res += 100*pow((x[i+1]-pow(x[i],2.0)),2.0)+pow((x[i]-1),2.0);
			}
			break;
		case SCHWEFEL:
			double SumaSCH;
			for(i=0;i<Dimension;i++)
			{
				SumaSCH=0.0;
				for(j=0;j<=i;j++)
				{
					SumaSCH += x[j];
				}
				res += pow(SumaSCH,2.0);
			}
			break;
		case SCHWEFEL_DOS:
			for(i=0;i<Dimension;i++)
			{
				res += sin(sqrt(fabs(x[i]))) * x[i];
			}
			res =  -res + 418.9828872722*Dimension;
			break;
		case RASTRIGIN:
			for(i=0;i<Dimension;i++)
			{
				res += pow( x[i], 2.0) - aRAS*cos(2*PI* x[i]);
			}
			res += aRAS*Dimension;
			break;
		case GRIEWANGK:
			{
				double SumaGRI=0.0, MultiGRI=1.0;
				for(i=0;i<Dimension;i++)
				{
					SumaGRI += pow( x[i], 2.0);
					MultiGRI *= cos( x[i]/sqrt(i+1) );
				}
				res = (SumaGRI/dGRI)-MultiGRI+1;
			}
			break;
		case ACKLEY:
			{
				double SumaACK=0.0, SumaACKCos=0.0;			
				double a=aACK;
				double b=0.2;
				double c=2*PI;
				for(i=0;i<Dimension;i++)
				{
					SumaACK += pow( x[i],2.0);
					SumaACKCos += cos(c * x[i]);
				}
				SumaACK = a*exp(-b*sqrt(SumaACK/(double)Dimension));
				SumaACKCos = exp(SumaACKCos/(double)Dimension);
				res = -SumaACK-SumaACKCos+a+exp(1);
			}
			break;
		case MICHALEWICKZ:
			for(i=0;i<Dimension;i++)
			{
				res += (sin(x[i])) * pow(sin((((i+1)*pow( x[i], 2.0)))/PI), 2.0*mMIC);
			}
			res=-res;
			break;
		case KATSUURA:
			double SumaKAT=0.0;
			res=1.0;
			int Beta=25;
			for(i=0;i<Dimension;i++)
			{
				for(j=1;j<=Beta;j++)
				{
					SumaKAT += fabs( ( pow(2.0,j)*x[i]-fabs(pow(2.0,i+1)*x[i]) )/pow(2.0,j));
				}
			res *= (1+(i+1)*SumaKAT);
			}
			break;
	}
	return (res);
}


void AGFunciones::mostrar(Individuo& Ind, int generacion)
{
	wxString Cad;
	Cad.Empty();
	Cad << "\n\n\nGeneración  " << generacion;
	Cad << "\n\nFunción Objetivo:\t\t\t\t\t\t" << Ind.objetivo(true) << "\n";
	Cad << "Medida OnLine Actual:\t\t\t\t\t" << m_MedidaOnLine << "\n";
	Cad << "Medida OffLine Actual:\t\t\t\t\t" << m_MedidaOffLine << "\nValores actuales:\n";
	for(int i=0;i<Dimension;i++)
	{
		Cad << x[i] << "  ";
	}	
	m_pFrame->m_pTextCtrl->AppendText(Cad);
}


void AGFunciones::GuardarValores(Individuo& Ind, int generacion)
{
	codificacion(&Ind, ESTADO_DECODIFICAR);
	int i=1;
	if(generacion==1)
	{
		pValores=fopen("Valores.txt","wt");
		if(!pValores) return;
		fprintf(pValores,"Columnas: {\n");
		fprintf(pValores," %d. Generacion\n", i);
		for(i=1;i<=Dimension;i++)
		{
			fprintf(pValores,"Variable %d\n", i);
		}
		fprintf(pValores,"}\n");
		fclose(pValores);
	}
	pValores=fopen("Valores.txt","at");
	if(!pValores) return;
	fprintf(pValores,"%i \t",generacion);
	for(i=0;i<Dimension;i++)
	{
		fprintf(pValores,"%f \t", x[i] );
	}
	fprintf(pValores,"\n");
	fclose(pValores);
}

void AGFunciones::GuardarIteraciones(Individuo& Ind)
{
	pValores=fopen("iteraciones.txt","at");
	if(!pValores) return;
	codificacion(&Ind, ESTADO_DECODIFICAR);	
	fprintf(pValores,"%d \t",m_TamanoPoblacion);
	fprintf(pValores,"%d \t",m_Generacion);
	fprintf(pValores,"%f \t",Ind.objetivo());
	fprintf(pValores,"\n");
	fclose(pValores);
}


/********************************************************************************/
/*			IMPLEMENTACION DE LA VENTANA Y LA APLICACION						*/
/********************************************************************************/

//!Se crea el frame para la aplicación
bool AGApp::OnInit()
{
	//Se debe crear el frame
	m_pFrame = new AGFrame("Funciones de prueba. UNgenético 2.0", 50, 50, 700, 500);
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
	m_pSizer=new wxFlexGridSizer(20,3,5,5);

	//Se crean los controles
	wxString list[]= {""};
	m_ChoiceFuncion = new wxChoice(this,CHOICE_FUNCION,wxDefaultPosition,wxSize(300,-1), 0, list);
	m_ChoiceFuncion->Append("Modelo Esférico");
	m_ChoiceFuncion->Append("Función de ROSENBROCK Generalizada");
	m_ChoiceFuncion->Append("Función de SCHWEFEL 1.2");
	m_ChoiceFuncion->Append("Función de SCHWEFEL");
	m_ChoiceFuncion->Append("Función de RASTRIGIN Generalizada");
	m_ChoiceFuncion->Append("Función de GRIEWANGK");
	m_ChoiceFuncion->Append("Función de ACKLEY");
	m_ChoiceFuncion->Append("Función de MICHALEWICKZ");
	m_ChoiceFuncion->Append("Función de KATSUURA");
	m_ChoiceFuncion->SetSelection(0);
	m_ChoiceFinalizacion = new wxChoice(this,-1,wxDefaultPosition,wxSize(300,-1), 0, list);
	m_ChoiceFinalizacion->Append("Numero maximo de Generaciones");
	m_ChoiceFinalizacion->Append("Variacion menor a 0.001 en la medida OnLine");
	m_ChoiceFinalizacion->Append("Variacion menor a 0.001 en la medida OffLine");
	m_ChoiceFinalizacion->SetSelection(0);
	m_SpinGenMax = new wxSpinCtrl(this,-1,"1000",wxDefaultPosition,wxSize(100,-1),wxSP_ARROW_KEYS,1,10000);
	m_SpinDimension = new wxSpinCtrl(this,-1,"20",wxDefaultPosition,wxSize(100,-1),wxSP_ARROW_KEYS,1,100);
	m_SpinMax = new wxSpinCtrl(this,-1,"5",wxDefaultPosition,wxSize(100,-1),wxSP_ARROW_KEYS,-100,100);
	m_SpinMin = new wxSpinCtrl(this,-1,"-5",wxDefaultPosition,wxSize(100,-1),wxSP_ARROW_KEYS,-100,100);
	m_SpinTamPob = new wxSpinCtrl(this,-1,"1000",wxDefaultPosition,wxSize(100,-1),wxSP_ARROW_KEYS,2,10000);
	m_ButtonAceptar = new wxButton(this,BUTTON_ACEPTAR,"Aceptar");

	m_SpinaRAS = new wxSpinCtrl(this,-1,"10",wxDefaultPosition,wxSize(100,-1),wxSP_ARROW_KEYS,1,100);
	m_SpindGRI = new wxSpinCtrl(this,-1,"400",wxDefaultPosition,wxSize(100,-1),wxSP_ARROW_KEYS,1,1000);
	m_SpinaACK = new wxSpinCtrl(this,-1,"20",wxDefaultPosition,wxSize(100,-1),wxSP_ARROW_KEYS,1,100);
	m_SpinmMIC = new wxSpinCtrl(this,-1,"10",wxDefaultPosition,wxSize(100,-1),wxSP_ARROW_KEYS,1,100);
	Desabilitar();

	//los controles se adicionan al sizer
	for(i=0;i<4;i++)
	{
		m_pSizer->Add(10,10);
	}
	m_pSizer->Add(new wxStaticText(this,-1,"Escoja la función"));
	m_pSizer->Add(m_ChoiceFuncion);
	m_pSizer->Add(10,10);
	m_pSizer->Add(new wxStaticText(this,-1,"Escoja la dimensión del problema"));
	m_pSizer->Add(m_SpinDimension);
	m_pSizer->Add(10,10);
	m_pSizer->Add(new wxStaticText(this,-1,"Escoja el valor máximo de las variables"));
	m_pSizer->Add(m_SpinMax);
	m_pSizer->Add(10,10);
	m_pSizer->Add(new wxStaticText(this,-1,"Escoja el valor mínimo de las variables"));
	m_pSizer->Add(m_SpinMin);

	m_pSizer->Add(10,10);
	m_pSizer->Add(new wxStaticText(this,-1,"Valor de 'a' en la función de RASTRIGIN"));
	m_pSizer->Add(m_SpinaRAS);
	m_pSizer->Add(10,10);
	m_pSizer->Add(new wxStaticText(this,-1,"Valor de 'd' en la función de GRIEWANGK"));
	m_pSizer->Add(m_SpindGRI);
	m_pSizer->Add(10,10);
	m_pSizer->Add(new wxStaticText(this,-1,"Valor de 'a' en la función de ACKLEY"));
	m_pSizer->Add(m_SpinaACK);
	m_pSizer->Add(10,10);
	m_pSizer->Add(new wxStaticText(this,-1,"Valor de 'm' en la función de MICHALEWICKZ"));
	m_pSizer->Add(m_SpinmMIC);


	for(i=0;i<4;i++)
	{
		m_pSizer->Add(20,20);
	}
	m_pSizer->Add(new wxStaticText(this,-1,"Escoja el criterio de finalización del algoritmo"));
	m_pSizer->Add(m_ChoiceFinalizacion);
	m_pSizer->Add(10,10);
	m_pSizer->Add(new wxStaticText(this,-1,"Escoja el número maximo de generaciones"));
	m_pSizer->Add(m_SpinGenMax);
	m_pSizer->Add(10,10);
	m_pSizer->Add(new wxStaticText(this,-1,"Escoja el número de individuos de cada generación"));
	m_pSizer->Add(m_SpinTamPob);
	for(i=0;i<5;i++)
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
	EVT_CHOICE(CHOICE_FUNCION, MiVentana::OnFuncion)
END_EVENT_TABLE()


void MiVentana::OnFuncion(wxCommandEvent &event)
{
	Desabilitar();
	switch (m_ChoiceFuncion->GetSelection()+1)
	{
		case RASTRIGIN:
			m_SpinaRAS->Enable(true);
			break;
		case GRIEWANGK:
			m_SpindGRI->Enable(true);
			break;
		case ACKLEY:
			m_SpinaACK->Enable(true);
			break;
		case MICHALEWICKZ:
			m_SpinmMIC->Enable(true);
			break;
	}
}

void MiVentana::Desabilitar()
{
	m_SpinaRAS->Enable(false);
	m_SpindGRI->Enable(false);
	m_SpinaACK->Enable(false);
	m_SpinmMIC->Enable(false);
}


/*ESTA FUNCIÓN ES INVOCADA AL PRESIONAL EL BOTON ACEPTAR.
  CONTIENE TODA LA EJECUCION DEL ALGORITMO GENETICO*/
void MiVentana::OnAceptar(wxCommandEvent &event)
{
	////////Ejecucion del algortimo
	AGFrame *pFrame = (AGFrame*)(ObtenerFrame());
	pFrame->SetPagina(ID_CONSOLA);
	AGFunciones *MiAg = new AGFunciones(pFrame);


	MiAg->Dimension = m_SpinDimension->GetValue();
	MiAg->Emax = m_SpinMax->GetValue();
	MiAg->Emin = m_SpinMin->GetValue();
	MiAg->func = m_ChoiceFuncion->GetSelection()+1;

	MiAg->aRAS = m_SpinaRAS->GetValue();
	MiAg->dGRI = m_SpindGRI->GetValue();
	MiAg->aACK = m_SpinaACK->GetValue();
	MiAg->mMIC = m_SpinmMIC->GetValue();

	// Utilizacion Paso a Paso del Algoritmo
	int t=0;
	MiAg->iniciarOptimizacion();
	do
	{
		MiAg->iterarOptimizacion();
		MiAg->mostrar(*MiAg->m_pMejorEnEstaGeneracion,t++ );
		//Guardamos los pValores del Mejor en la historia.
		MiAg->GuardarValores(*MiAg->m_pMejorEnEstaGeneracion,t);
		EJECUTAR_EVENTOS
	}while(!MiAg->finalizar());
	MiAg->mostrarMedidas();
	MiAg->GuardarIteraciones(*MiAg->m_pMejorEnEstaGeneracion);

	delete MiAg;
}