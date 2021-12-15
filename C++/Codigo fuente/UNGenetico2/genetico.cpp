#ifndef __GENETICO_CPP
#define __GENETICO_CPP

#include "genetico.h"


/*****************************************************
				FUNCIONES GLOBALES
*****************************************************/

/*!Retorna el entero mas cercano a \a x
\param x Número del que se desea encontar su valor entero más cercano
\return Número entero más cercano a \a x*/
long redondear(double x)
{	
	double ent;
	if(  fabs( modf(x,&ent) ) < 0.5  )
		return (long)ent;
	else if(x>0)
		return (long)ceil(x);
	else
		return (long)floor(x);
}

/*****************************************************
			INDIVIDUO (Implementacion)
*****************************************************/


/*!Inicializa las propiedades básicas del individuo
\param pAG Apuntador al objeto AlgoritmoGenetico al que pertenece.*/
Individuo::Individuo(AlgoritmoGenetico *pAG) 
{
	m_pAG=pAG;
	m_pGenoma = new Arreglo<Gen>;
	m_Probabilidad = 0.0;
	m_pPareja = NULL;
	m_objetivoActualizado = false;
}

//Constructor por copia de otro individuo
/*!Construye el nuevo objeto copiando idénticamente las propiedades 
de otro objeto de la clase Individuo.
\param origen Objeto del que se hace copia.*/
Individuo::Individuo(const Individuo& origen)
{
	m_pGenoma = new Arreglo<Gen>;
	operator = (origen);
}



/*!Copia idénticamente las propiedades de otro objeto 
de la clase Individuo.
\param origen Objeto del que se hace copia
\return Referencia al individuo*/
const Individuo& Individuo:: operator = (const Individuo& origen)
{
	m_pAG=origen.m_pAG;
	m_Objetivo = origen.m_Objetivo;
	m_pPareja = origen.m_pPareja;
	m_Probabilidad = origen.m_Probabilidad;
	m_objetivoActualizado = origen.m_objetivoActualizado;	//Si el otro está actualizado, este también lo estará.

	int i, tam=origen.m_pGenoma->getSize();
	int miTam=m_pGenoma->getSize();

	m_pGenoma->asignarMemoria(tam);
	for(i=0; i<tam && i<miTam; i++)
	{	
		m_pGenoma->getObj(i).copiar( origen.m_pGenoma->getObj(i) );
	}
	for(i=miTam; i<tam; i++)
	{
		Gen* pNuevo = origen.m_pGenoma->getObj(i).crearCopia();
		m_pGenoma->Adicionar(pNuevo);
	}
	m_pGenoma->Truncar(tam);
	return *this;
}

/*!Destruye todos sus componentes*/
Individuo::~Individuo()
{
	if(m_pGenoma) delete m_pGenoma;
}



/*!Hace que cada uno de los genes presentes en el genoma se genere aleatoriamente 
invocando su miembro \a generarAleatorio().*/
void Individuo::generarAleatorio()
{
	int tam=m_pGenoma->getSize();
	for(int i=0;i<tam;i++) m_pGenoma->getObj(i).generarAleatorio();
	m_objetivoActualizado = false;
}


/*!Recalcula la función objetivo en caso de estar desactualizada
decodificando la información genética y pasando a través de la función \a objetivo() 
definida en la clase AlgoritmoGenetico a la que pertenece.
\param actualizarAG indica si se debe decoficar la información genética actualizando las variables correspondientes que pertenecen al algoritmo genético
\return Valor de la función de evaluación del individuo*/
double Individuo::objetivo(bool actualizarAG)
{
	//si la funcion objetivo no está actualizada o si actualizarAG=true, decodificamos la informacion del individuo
	if(!m_objetivoActualizado || actualizarAG)
		m_pAG->codificacion(this, ESTADO_DECODIFICAR);

	//si la funcion objetivo no está actualizada la actualizamos.
	if(!m_objetivoActualizado)
	{		
		m_Objetivo = m_pAG->objetivo();
		m_objetivoActualizado = true;	
	}
	return m_Objetivo;
}


/*!Hace que cada uno de los genes presentes en el individuo pase por el operador 
de mutación respectivo*/
void Individuo::mutar()
{	
	int tamGenoma = getTamGenoma();
	for(int i=0; i<tamGenoma; i++)
	{
		m_pAG->m_pListaOperadorMutacion->getObj(i).mutar( &m_pGenoma->getObj(i) );
	}
	m_objetivoActualizado = false;
}


/*****************************************************
			POBLACION (Implementacion)
*****************************************************/


/*!Inicializa el arreglo que contendrá a los individuos de la población 
\param nIndividuos Número inicial de individuos de la población.
\param crearAleatorios Si si valor es \a true, los individuos se crearán aleatoriamente. De lo contrario se crearán con sus valores iniciales establecidos.*/
Poblacion::Poblacion(AlgoritmoGenetico* pAG, int nIndividuos, bool crearAleatorios)
{
	m_pGeneracion = new Arreglo<Individuo>;
	m_pAG = pAG;
	setTam(nIndividuos, false, crearAleatorios);
}

/*!Construye el nuevo objeto copiando idénticamente las propiedades 
de otro objeto de la clase Poblacion.
\param origen Objeto del que se hace copia.*/
Poblacion::Poblacion(const Poblacion& origen)
{
	m_pAG = origen.m_pAG;
	m_pGeneracion = new Arreglo<Individuo>(*origen.m_pGeneracion);
}

/*!Copia idénticamente las propiedades de otro objeto 
de la clase Poblacion.
\param origen Objeto del que se hace copia
\return Referencia a la poblacion*/
const Poblacion& Poblacion::operator = (const Poblacion& origen)
{
	m_pAG = origen.m_pAG;
	//la asignación directa se puede realizar ya que individuo no es clase abstracta
	*m_pGeneracion = *origen.m_pGeneracion;
	return *this;
}

/*!Elimina todos los objetos creados dinámicamente*/
Poblacion::~Poblacion()
{
	if(m_pGeneracion) delete m_pGeneracion;
}


/*!Si el nuevo tamaño es menor que el anterior, se eliminan los individuos restantes. Si 
es mayor, los nuevos individuos son creados aleatoriamente dependiendo del valor de 
\a crearAleatorios
\param nuevoTam Nuevo tamaño del arreglo. 
\param crearAleatorios Si su valor es \a true se crean individuos aleatorios. Si es \a false se crean individuos con valores iniciales preestablecidos
\param remplazar Si su valor es \a true, los individuos existentes en la población son reemplazados por nuevos individuos.
\return Tamaño asignado o -1 en caso de error.*/
int Poblacion::setTam(int nuevoTam, bool remplazar, bool crearAleatorios)
{
	if( nuevoTam<1 ) return -1; //no tiene sentido una poblacion sin individuos.
	if(remplazar) m_pGeneracion->FlushDestroy();
	int i, old = m_pGeneracion->getSize();
	if( nuevoTam==old ) return old;	//no hay nada que cambiar

	m_pGeneracion->asignarMemoria(nuevoTam);
	for(i=old; i<nuevoTam; i++)
	{
		Individuo *pNuevo = new Individuo(*m_pAG->m_pModelo);
		if(crearAleatorios) pNuevo->generarAleatorio();
		m_pGeneracion->Adicionar(pNuevo);
	}
	m_pGeneracion->Truncar(nuevoTam);
	return nuevoTam;
}


/*!	
\param ind Apuntador al individuo a insertar
  \param pos Posición en la que será insertado el individuo. Si no se especifica, el individuo se ubicará en la ultima posición.
\return Posición en la que fué ubicado el individuo.*/
int Poblacion::InsertarIndividuo(Individuo* ind, int pos)
{
	int tam = getTam();
	if(pos==-1 || pos>tam) pos=tam;
	else if(pos < 0) pos=0;
	m_pGeneracion->Insertar(ind, pos);
	return pos;
}

/*!Hace que cada uno de los individuos presentes en población realice 
el proceso de mutación.*/
void Poblacion::mutar()
{	
	int tam = getTam();
	for(int i=0; i<tam; i++)
	{
		getIndividuo(i).mutar();
	}
}


/*! 
\param Maximizar Si su valor es \a true, ordena a los individuos en forma descendente. De lo contrario los ordena de forma ascedente.*/
void Poblacion::ordenar(bool Maximizar)
{
	int tam=m_pGeneracion->getSize();
	if(tam<=0){return;}

	bool cambio=true;
	if(Maximizar)
		while(cambio)
		{
			cambio=false;
			for(int j=0; j<2; j++)
				for(int i=j; i<tam-1; i+=2)
					if( m_pGeneracion->getObj(i).objetivo() < m_pGeneracion->getObj(i+1).objetivo() )
						cambio = m_pGeneracion->IntercambiarPos(i, i+1);
		}
	else
		while(cambio)
		{
			cambio=false;
			for(int j=0; j<2; j++)
				for(int i=j; i<tam-1; i+=2)
					if( m_pGeneracion->getObj(i).objetivo() > m_pGeneracion->getObj(i+1).objetivo() )
						cambio = m_pGeneracion->IntercambiarPos(i, i+1);						
		}

#if defined(UN_DEBUG)	
	for(int k=0; k<tam; k++)
		if(Maximizar)
			unASSERT( m_pGeneracion->getObj(k).objetivo() >= m_pGeneracion->getObj(k+1).objetivo() );
		else
			unASSERT( m_pGeneracion->getObj(k).objetivo() <= m_pGeneracion->getObj(k+1).objetivo() );
#endif
}






/*************************************************************
			AlgoritmoGenetico (Implementacion)
**************************************************************/


#if defined(USAR_VENTANA)

	
	/*!Inicializa todos los apuntadores de la clase en posiciones nulas
	\param pFrame Apuntador a la ventana de la aplicación*/
	AlgoritmoGenetico::AlgoritmoGenetico(AGFrame *pFrame)
	{		
		inicializarApuntadores();		
		m_pFrame=pFrame;
	}

	/*!Almacena la ventana actualmente utilizada
	\param pFrame Apuntador a la ventana de la aplicación*/
	inline void AlgoritmoGenetico::SetFrame(AGFrame* pFrame)
	{
		m_pFrame=pFrame;
	}

#else

	/*!Inicializa todos los apuntadores de la clase en posiciones nulas*/
	AlgoritmoGenetico::AlgoritmoGenetico()
	{		
		inicializarApuntadores();
	}

#endif


/*!Es invocada por los constructores de la clase y por \a inicializarVariables()*/
void AlgoritmoGenetico::inicializarApuntadores()
{
	m_pModelo = NULL;
	m_pPoblacionActual = NULL;
	m_pMejorEnEstaGeneracion = NULL;
	m_pPeorEnEstaGeneracion = NULL;
	m_pMejorEnLaHistoria = NULL;
	m_pListaOperadorAdaptacion = NULL;
	m_pListaOperadorFinalizacion = NULL;
	m_pListaOperadorMutacion = NULL;
	m_pListaOperadorCruce = NULL;
	m_pOpProbabilidad = NULL;
	m_pOpSeleccion = NULL;
	m_pOpParejas = NULL;
	m_pOpReproduccion = NULL;
}


/*!Inicializa los apuntadores en posiciones nulas, inicializa las variables 
propias de cada iteración y establece los parámetros por defecto. Invoca la función 
\a inicializarParametros() en la cual usted puede cambiar los parámetros por defecto.
Ver parámetros establecidos por defecto en <a href="tablas.html#tabalg">Valores por
defecto de la clase AlgoritmoGenetico</a>. 
Es invocada por \a iniciarOptimizacion().*/
void AlgoritmoGenetico::inicializarVariables()
{	
	inicializarApuntadores();

	//parámetros por defecto
	m_GeneracionMaxima = 100;
	m_TamanoPoblacion = 100;
	m_IndicadorUsarAdaptacion = true;
	m_IndicadorInicializarPoblacionAleatoria=true;
	m_IndicadorMaximizar = false;
	m_IndicadorArchivo = true;
	m_IntervaloSalvar = 1;
	sprintf(m_NombreArchivo, "salidas.txt");
	m_IndicadorMostrar = false;
	m_IndicadorMostrarMejorEnHistoria = true;
	m_IndicadorMostrarGeneracionMejorHistorico = true;
	m_IndicadorMostrarMejorEnGeneracion = true;
	m_IndicadorMostrarPeorEnGeneracion = true;
	m_IndicadorMostrarMedia = true;
	m_IndicadorMostrarDesviacion = true;
	m_IndicadorMostrarOnLine = true;
	m_IndicadorMostrarOffLine = true;

	
	//el usuario puede cambiar los parámetros por defecto
	inicializarParametros();
	if(m_TamanoPoblacion<1) m_TamanoPoblacion=1;

#if defined(USAR_VENTANA)
	m_pFrame->m_NomArch.Empty();
	m_pFrame->m_NomArch << m_NombreArchivo;
	m_pFrame->m_pPagina = (AGVentana*)(m_pFrame->ObtenerPagina(ID_CONSOLA));
	m_pFrame->m_pTextCtrl = m_pFrame->m_pPagina->m_pTxtConsola;
#endif
}



/*!Realiza las siguientes acciones:
- Crea los arreglos contenedores de los Operadores de mutacion, cruce, 
adaptación y finalización.
- Invoca la función \a definirOperadores() la cual puede ser definida para utilizar 
operadores diferentes a los definidos por defecto.
- Verifica si fueron creados los operadores correctamente, en caso contrario crea  
los operadores por defecto.

Los operadores por defecto se pueden ver en <a href="tablas.html#oprdef">Operadores por
defecto de UNGenético 2.0</a>.
Es invocada por \a iniciarOptimizacion()
*/
void AlgoritmoGenetico::crearOperadores()
{
	m_pListaOperadorMutacion = new Arreglo<OperadorMutacion>;
	m_pListaOperadorCruce = new Arreglo<OperadorCruce>;
	m_pListaOperadorAdaptacion = new Arreglo<OperadorAdaptacion>;
	m_pListaOperadorFinalizacion = new Arreglo<OperadorFinalizacion>;

	//se permite al usuario definir sus operadores
	definirOperadores();
	
	//se comprueba que se hayan definido los operadores correctamente.
	if(!m_pOpProbabilidad) m_pOpProbabilidad = new OperadorProbabilidadLineal();
	if(!m_pOpSeleccion) m_pOpSeleccion = new OperadorSeleccionEstocasticaRemplazo();
	if(!m_pOpParejas) m_pOpParejas = new OperadorParejasAdyacentes();
	if(!m_pOpReproduccion) m_pOpReproduccion = new OperadorReproduccionMejorPadreMejorHijo();
	
	if(m_pListaOperadorAdaptacion->getSize()==0)
	{
		OperadorAdaptacion *OA = new OperadorAdaptacionElitismo();
		m_pListaOperadorAdaptacion->Adicionar(OA);
	}
	
	int tamGenoma = m_pModelo->getTamGenoma();
	int tamArreglo = m_pListaOperadorMutacion->getSize();
	if(tamGenoma > tamArreglo)
	{
		OperadorMutacion *OM;
		for(int i=tamArreglo; i<tamGenoma; i++)
		{
			OM = m_pModelo->getGen(i).operadorMutacionDefecto();
			m_pListaOperadorMutacion->Adicionar(OM);
		}
	}
	tamArreglo = m_pListaOperadorCruce->getSize();
	if(tamGenoma > tamArreglo)
	{
		OperadorCruce *OC;
		for(int i=tamArreglo; i<tamGenoma; i++)
		{
			OC = m_pModelo->getGen(i).operadorCruceDefecto();
			m_pListaOperadorCruce->Adicionar(OC);	
		}
	}		
}


/*!El proceso completo de optimización se realiza comenzando por la inicialización
y ejecutando iteraciones del algoritmo hasta que se llegue al final lo cual es 
indicado por el criterio de finalización especificado para el algoritmo.
El proceso completo de optimización comprende las siguientes instrucciones:
<pre>
iniciarOptimizacion();
do
{
	iterarOptimizacion();
}while(!finalizar());
</pre>*/
void AlgoritmoGenetico::optimizar()
{
	iniciarOptimizacion();
	do
	{
		iterarOptimizacion();
	}while(!finalizar());	
}


/*!Inicializa todas las variables y parámetros, crea 
el individuo modelo y los operadores a utilizar en el algoritmo.
Si el algoritmo está en mitad de proceso, éste es finalizado y 
preparado para comenzar de nuevo.
Es invocada por \a optimizar().
*/
void AlgoritmoGenetico::iniciarOptimizacion()
{
	//en caso de estar en medio de otro proceso de optimizacion es necesario finalizarlo.
	finalizarOptimizacion();
	
	inicializarVariables();	
	srand( (unsigned int) time(NULL));
	
	//inicializar variables
	m_Generacion = 0;
	m_GeneracionDelMejorEnLaHistoria = 0;
	m_Media = 0.0; 
	m_MedidaOnLine = 0.0;
	m_MedidaOffLine = 0.0;
	m_MedidaOnLineAnterior = 0.0;
	m_MedidaOffLineAnterior = 0.0;
	m_Desviacion = 0.0;
	
	m_pModelo = new Individuo(this);
	codificacion(m_pModelo, ESTADO_CREAR);	
	crearOperadores();

	m_pMejorEnEstaGeneracion = new Individuo(*m_pModelo);
	m_pPeorEnEstaGeneracion = new Individuo(*m_pModelo);
	m_pMejorEnLaHistoria = new Individuo(*m_pModelo);
}


/*!Si es la primera iteración genera una nueva población del tamaño definido
por \a m_TamanoPoblacion. Una iteración contiene las 
siguientes instrucciones:
- asignarProbabilidad();
- seleccionar();
- asignarParejas();
- reproducir();
- mutar();
- adaptacion();
- actualizarMedidas();

Salva las medidas en archivo y las muestra en pantalla, dependiendo 
de los valores de m_IndicadorArchivo, m_IndicadorMostrar y m_IntervaloSalvar,
invocando las funciones
- salvar()
- mostrarMedidas() 

Es invocada por \a optimizar()*/
void AlgoritmoGenetico::iterarOptimizacion()
{
	if(m_Generacion==0)
	{
		m_pPoblacionActual = new Poblacion(this, m_TamanoPoblacion, m_IndicadorInicializarPoblacionAleatoria);	
	}
	else
	{
		asignarProbabilidad();
		seleccionar();
		asignarParejas();
		reproducir();
		mutar();
		adaptacion();  //es importante invocar adaptacion() antes de actualizarMedidas()
	}
	actualizarMedidas();

	bool condicion= (m_Generacion==0)||(m_Generacion%m_IntervaloSalvar==0);
	if(m_IndicadorArchivo && condicion)
		salvar();
	if(m_IndicadorMostrar && condicion)
		mostrarMedidas();

	m_Generacion++;
}


/*!Invoca los operadores de finalización del algoritmo genético en el mismo orden 
en que se adicionaron. Si alguno indica que el algoritmo debe finalizar, retorna 
true. También retorna true siempre que se alcance el número máximo de iteraciones
especificado por \a m_GeneracionMaxima. Es invocada por \a optimizar().
\return \a true si el algoritmo debe finalizar. \a false en caso contrario.*/
bool AlgoritmoGenetico::finalizar()
{	
	if(m_Generacion>=m_GeneracionMaxima)
		return true; //sin importar cual sea el criterio de finalizacion.
	
	bool parar = false;
	int i=0, tam = m_pListaOperadorFinalizacion->getSize();
	while(i<tam && !parar)
	{
		parar = m_pListaOperadorFinalizacion->getObj(i++).finalizar(*this);
	}
	return parar;
}


/*!Elimina todos los objetos creados en el algoritmo.
Es invocada por \a iniciarOptimizacion() y por el destructor de la clase.*/
void AlgoritmoGenetico::finalizarOptimizacion()
{
	if(m_pPoblacionActual)	
		delete m_pPoblacionActual;	
	if(m_pModelo) 
		delete m_pModelo;
	if(m_pListaOperadorAdaptacion) 
		delete m_pListaOperadorAdaptacion;
	if(m_pListaOperadorFinalizacion) 
		delete m_pListaOperadorFinalizacion;
	if(m_pListaOperadorMutacion) 
		delete m_pListaOperadorMutacion;
	if(m_pListaOperadorCruce) 
		delete m_pListaOperadorCruce;
	if(m_pOpProbabilidad) 
		delete m_pOpProbabilidad;
	if(m_pOpSeleccion) 
		delete m_pOpSeleccion;
	if(m_pOpParejas) 
		delete m_pOpParejas;
	if(m_pOpReproduccion) 
		delete m_pOpReproduccion;	
	if(m_pMejorEnEstaGeneracion) 
		delete m_pMejorEnEstaGeneracion;
	if(m_pPeorEnEstaGeneracion) 
		delete m_pPeorEnEstaGeneracion;
	if(m_pMejorEnLaHistoria) 
		delete m_pMejorEnLaHistoria;	
}

/*!Invoca la función \a asignarProbabilidad del operador de probabilidad del algoritmo.
Es invocada por \a iterarOptimizacion()*/
inline void AlgoritmoGenetico::asignarProbabilidad()
{
	if(m_pOpProbabilidad) 
		m_pOpProbabilidad->asignarProbabilidad(*m_pPoblacionActual, m_IndicadorMaximizar);
}

/*!Invoca la función \a seleccionar del operador de selección del algoritmo.
Es invocada por \a iterarOptimizacion()*/
inline void AlgoritmoGenetico::seleccionar()
{
	if(m_pOpSeleccion) 
		m_pOpSeleccion->seleccionar(*m_pPoblacionActual);
}

/*!Invoca la función \a asignarParejas del operador de asignación de parejas del algoritmo.
Es invocada por \a iterarOptimizacion()*/
inline void AlgoritmoGenetico::asignarParejas()
{
	if(m_pOpParejas) 
		m_pOpParejas->asignarParejas(*m_pPoblacionActual);
}

/*!Invoca la función \a reproducir del operador de reproducción del algoritmo.
Es invocada por \a iterarOptimizacion()*/
inline void AlgoritmoGenetico::reproducir()
{
	if(m_pOpReproduccion) 
		m_pOpReproduccion->reproducir(*m_pPoblacionActual, m_IndicadorMaximizar);
}

/*!Invoca la función \a mutar de la población actual.
Es invocada por \a iterarOptimizacion()*/
inline void AlgoritmoGenetico::mutar()
{
	m_pPoblacionActual->mutar();
}


/*!Sólo se realiza adaptación del algoritmo si <i>m_IndicadorUsarAdaptacion = true</i>.
Se realiza invocando la función \a adaptacion de los operadores de adaptación 
del algoritmo en el mismo orden en que se adicionaron en la función 
\a definirOperadores().
Es invocada por \a iterarOptimizacion()*/
void AlgoritmoGenetico::adaptacion()
{	
	if(m_IndicadorUsarAdaptacion)
	{		
		int tam=m_pListaOperadorAdaptacion->getSize();
		for(int i=0; i<tam; i++)
			m_pListaOperadorAdaptacion->getObj(i).adaptacion(this);
	}
}


/*!Es invocada por iterarOptimizacion(). Actualiza los siguientes miembros:
	- m_Media
	- m_MedidaOnLine 
	- m_MedidaOffLine 
	- m_Desviacion
	- m_pMejorEnEstaGeneracion
	- m_pPeorEnEstaGeneracion 
	- m_pMejorEnLaHistoria
	- m_GeneracionDelMejorEnLaHistoria
	- m_MedidaOffLineAnterior
	- m_MedidaOnLineAnterior
*/
void AlgoritmoGenetico::actualizarMedidas()
{
	int i, nIndividuos = m_pPoblacionActual->getTam();

	//Encontrar mejor y peor funciones objetivo en la generacion actual
	double mejor, peor, SumaObj=0.0;
	int PosMejor=0, PosPeor=0;
	mejor = peor = m_pPoblacionActual->getIndividuo(0).objetivo();
	for(i=0; i < nIndividuos; i++)
	{
		double obj= m_pPoblacionActual->getIndividuo(i).objetivo();
		SumaObj+=obj;
		if(  (m_IndicadorMaximizar && (obj>mejor) ) || (!m_IndicadorMaximizar && (obj<mejor))  )
		{
			mejor=obj;
			PosMejor=i;
		}
		if( (m_IndicadorMaximizar && (obj<peor) )  || (!m_IndicadorMaximizar && (obj>peor))  )
		{
			peor=obj;
			PosPeor=i;
		}
	}
	//calcular media actual
	m_Media = SumaObj/(double)nIndividuos;

	//actualizar el mejor individuo de la generacion
	*m_pMejorEnEstaGeneracion = m_pPoblacionActual->getIndividuo(PosMejor);
	*m_pPeorEnEstaGeneracion = m_pPoblacionActual->getIndividuo(PosPeor);

	//actualizar el mejor individuo de la historia
	double valMejorHistoria = m_pMejorEnLaHistoria->objetivo();
	if( (m_Generacion==0)||( m_IndicadorMaximizar&&(mejor>valMejorHistoria))||( !m_IndicadorMaximizar &&(mejor<valMejorHistoria)))
	{
		*m_pMejorEnLaHistoria = *m_pMejorEnEstaGeneracion;
		m_GeneracionDelMejorEnLaHistoria = m_Generacion;
	}

	//Actualizar medidas Offline y Online
	static double AcumuladoOffLine=0.0;
	static int TotalIndividuos=0;
	static double AcumuladoOnLine=0.0;
	
	if(m_Generacion==0)
	{
		AcumuladoOffLine=0.0;
		TotalIndividuos=0;
		AcumuladoOnLine=0.0;
	}
	m_MedidaOffLineAnterior = m_MedidaOffLine;
	AcumuladoOffLine += m_pMejorEnLaHistoria->objetivo();
	m_MedidaOffLine = AcumuladoOffLine/((double)m_Generacion+1.0);
	
	m_MedidaOnLineAnterior = m_MedidaOnLine;
	AcumuladoOnLine+=SumaObj;
	TotalIndividuos+=nIndividuos;
	m_MedidaOnLine = AcumuladoOnLine/(double)TotalIndividuos;
	
	//Calcular desviación
	double acumDesv=0;
	for(i=0;i < nIndividuos;i++)
	{
		double valor = m_pPoblacionActual->getIndividuo(i).objetivo();
		acumDesv+= pow(valor-m_Media,2.0);
	}
	m_Desviacion=sqrt(acumDesv / nIndividuos);
}


/*!Esta función es invocada por \a iterarOptimizacion() solamente si 
<i>m_IndicadorMostrar = true</i>. Los resultados que se muestran dependen de 
los indicadores especificados. Se muestran en el siguiente orden.
	  
<table border="1" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#FFFFFF" width="100%" id="AutoNumber1">
  <tr>
    <td width="50%"><i><b>Valor Mostrado</b></i></td>
    <td width="50%"><i><b>Depende de</b></i></td>
  </tr>
  <tr>
    <td width="50%">Generación</td>
    <td width="50%">Siempre es mostrado</td>
  </tr>
  <tr>
    <td width="50%">Función objetivo del mejor en la historia</td>
    <td width="50%">m_IndicadorMostrarMejorEnHistoria</td>
  </tr>
  <tr>
    <td width="50%">Generación del Mejor en la historia</td>
    <td width="50%">m_IndicadorMostrarGeneracionMejorHistorico</td>
  </tr>
  <tr>
    <td width="50%">Mejor Función Objetivo en la Generación Actual</td>
    <td width="50%">m_IndicadorMostrarMejorEnGeneracion</td>
  </tr>
  <tr>
    <td width="50%">Peor Función Objetivo en la Generación Actual</td>
    <td width="50%">m_IndicadorMostrarPeorEnGeneracion</td>
  </tr>
  <tr>
    <td width="50%">Media Aritmética de la Función Objetivo</td>
    <td width="50%">m_IndicadorMostrarMedia</td>
  </tr>
  <tr>
    <td width="50%">Desviación Estándar de la Función Objetivo</td>
    <td width="50%">m_IndicadorMostrarDesviacion</td>
  </tr>
  <tr>
    <td width="50%">Medida OnLine Actual</td>
    <td width="50%">m_IndicadorMostrarOnLine</td>
  </tr>
  <tr>
    <td width="50%">Medida OffLine Actual</td>
    <td width="50%">m_IndicadorMostrarOffLine</td>
  </tr>
</table>
 
Cada valor es mostrado si su respectivo indicador tiene valor \a true*/
void AlgoritmoGenetico::mostrarMedidas()
{
#if defined(USAR_VENTANA)
	wxString SALIDA;
#else
	#define SALIDA cout
#endif
	
	SALIDA << "\n\n\t\t\tESTADISTICAS GENERALES\n\n";
		SALIDA << "Número Total de Generaciones:   \t\t\t" << m_Generacion << "\n\n";
	if(m_IndicadorMostrarMejorEnHistoria)
		SALIDA << "Mejor Funcion Objetivo en la Historia:\t\t\t" << m_pMejorEnLaHistoria->objetivo() << "\n\n";
	if(m_IndicadorMostrarGeneracionMejorHistorico)
		SALIDA << "Generacion del Mejor Individuo en la Historia:\t\t" << m_GeneracionDelMejorEnLaHistoria << "\n\n";
	if(m_IndicadorMostrarMejorEnGeneracion)
		SALIDA << "Mejor Funcion Objetivo en la Generacion Actual:\t\t" << m_pMejorEnEstaGeneracion->objetivo() << "\n\n";
	if(m_IndicadorMostrarPeorEnGeneracion)
		SALIDA << "Peor Función Objetivo en la Generacion Actual:\t\t" << m_pPeorEnEstaGeneracion->objetivo() << "\n\n";
	if(m_IndicadorMostrarMedia)
		SALIDA << "Media Aritmetica de la Funcion Objetivo:  \t\t" << m_Media << "\n\n";
	if(m_IndicadorMostrarDesviacion)
		SALIDA << "Desviacion Estandar de la Funcion Objetivo:\t\t" << m_Desviacion << "\n\n";
	if(m_IndicadorMostrarOnLine)
		SALIDA << "Medida OnLine Actual:   \t\t\t\t" << m_MedidaOnLine << "\n\n";
	if(m_IndicadorMostrarOffLine)
		SALIDA << "Medida OffLine Actual:   \t\t\t\t" << m_MedidaOffLine << "\n\n";

#if defined(USAR_VENTANA)
	AGVentana *pagina = (AGVentana*)(m_pFrame->ObtenerPagina(ID_CONSOLA));
	pagina->m_pTxtConsola->AppendText(SALIDA);
#else
	#undef SALIDA
#endif
}



/*!Esta función es invocada por \a iterarOptimizacion() solamente si 
<i>m_IndicadorArchivo = true</i>. El archivo de salida es el especificado 
por \a m_NombreArchivo.
Los resultados salvados dependen de los indicadores especificados. Se guardan 
en el siguiente orden.
	  
	<table border="1" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#FFFFFF" width="100%" id="AutoNumber1">
  <tr>
    <td width="50%"><i><b>Valor Guardado</b></i></td>
    <td width="50%"><i><b>Depende de</b></i></td>
  </tr>
  <tr>
    <td width="50%">Generación</td>
    <td width="50%">Siempre es mostrado</td>
  </tr>
  <tr>
    <td width="50%">Función objetivo del mejor en la historia</td>
    <td width="50%">m_IndicadorMostrarMejorEnHistoria</td>
  </tr>
  <tr>
    <td width="50%">Generación del Mejor en la historia</td>
    <td width="50%">m_IndicadorMostrarGeneracionMejorHistorico</td>
  </tr>
  <tr>
    <td width="50%">Mejor Función Objetivo en la Generación Actual</td>
    <td width="50%">m_IndicadorMostrarMejorEnGeneracion</td>
  </tr>
  <tr>
    <td width="50%">Peor Función Objetivo en la Generación Actual</td>
    <td width="50%">m_IndicadorMostrarPeorEnGeneracion</td>
  </tr>
  <tr>
    <td width="50%">Media Aritmética de la Función Objetivo</td>
    <td width="50%">m_IndicadorMostrarMedia</td>
  </tr>
  <tr>
    <td width="50%">Desviación Estándar de la Función Objetivo</td>
    <td width="50%">m_IndicadorMostrarDesviacion</td>
  </tr>
  <tr>
    <td width="50%">Medida OnLine Actual</td>
    <td width="50%">m_IndicadorMostrarOnLine</td>
  </tr>
  <tr>
    <td width="50%">Medida OffLine Actual</td>
    <td width="50%">m_IndicadorMostrarOffLine</td>
  </tr>
</table>
 
Cada valor es guardado si su respectivo indicador tiene valor \a true
*/
void AlgoritmoGenetico::salvar()
{
	FILE *pArch;
	if(m_Generacion==0)
	{
		pArch = fopen(m_NombreArchivo, "wt");
		if(!pArch) return;
		fprintf(pArch,"Columnas: {\n");
		fprintf(pArch," Generacion\n");
		if(m_IndicadorMostrarMejorEnHistoria)
			fprintf(pArch," Mejor en la historia\n");
		if(m_IndicadorMostrarGeneracionMejorHistorico)
			fprintf(pArch," Generacion del mejor en la historia\n");
		if(m_IndicadorMostrarMejorEnGeneracion)
			fprintf(pArch," Mejor en generacion actual\n");
		if(m_IndicadorMostrarPeorEnGeneracion)
			fprintf(pArch," Peor en generacion actual\n");
		if(m_IndicadorMostrarMedia)
			fprintf(pArch," Media\n");
		if(m_IndicadorMostrarDesviacion)
			fprintf(pArch," Desviacion estandar\n");
		if(m_IndicadorMostrarOnLine)
			fprintf(pArch," Medida OnLine\n");
		if(m_IndicadorMostrarOffLine)
			fprintf(pArch," MedidaOffLine\n");
		fprintf(pArch,"}\n");
		fclose(pArch);
	}

	pArch=fopen(m_NombreArchivo, "at");
	if(!pArch) return;
	fprintf(pArch,"%5d\t", m_Generacion);
	if(m_IndicadorMostrarMejorEnHistoria)
		fprintf(pArch,"%13f\t", m_pMejorEnLaHistoria->objetivo());
	if(m_IndicadorMostrarGeneracionMejorHistorico)
		fprintf(pArch,"%5d\t", m_GeneracionDelMejorEnLaHistoria);
	if(m_IndicadorMostrarMejorEnGeneracion)
		fprintf(pArch,"%13f\t", m_pMejorEnEstaGeneracion->objetivo());
	if(m_IndicadorMostrarPeorEnGeneracion)
		fprintf(pArch,"%13f\t", m_pPeorEnEstaGeneracion->objetivo());
	if(m_IndicadorMostrarMedia)
		fprintf(pArch,"%13f\t", m_Media);
	if(m_IndicadorMostrarDesviacion)
		fprintf(pArch,"%13f\t", m_Desviacion);
	if(m_IndicadorMostrarOnLine)
		fprintf(pArch,"%13f\t", m_MedidaOnLine);
	if(m_IndicadorMostrarOffLine)
		fprintf(pArch,"%13f\t", m_MedidaOffLine);

	fprintf(pArch,"\n");
	fclose(pArch);
}


//////////////////////////////OPERADORES (Implementacion de Funciones)///////////////////////////////////


//Método para efectuar el proceso de adaptacion
void OperadorAdaptacionElitismo::adaptacion(AlgoritmoGenetico *pAG)
{
	int tamPob = pAG->m_pPoblacionActual->getTam();
	bool condicion;
	for(int i=0; i<(1.0+tamPob*0.001); i++)
	{		
		double posAzar = ((double)rand()/(double)(RAND_MAX+1))*(double)(tamPob-1);
		
		if(pAG->m_IndicadorMaximizar)
			condicion = pAG->m_pMejorEnLaHistoria->objetivo() > pAG->m_pPoblacionActual->getIndividuo((int)posAzar).objetivo();
		else
			condicion = pAG->m_pMejorEnLaHistoria->objetivo() < pAG->m_pPoblacionActual->getIndividuo((int)posAzar).objetivo();

		if(condicion)
		{
			Individuo *temp = new Individuo(*pAG->m_pMejorEnLaHistoria);
			temp = pAG->m_pPoblacionActual->remplazarIndividuo(temp, (int)posAzar );
			delete temp;
		}
	}
}

/*!Inicializa el valor correspondiente al tipo de adaptación que será implementado e invoca a los métodos que 
establecen sus parámetros.
\param pAG Apuntador al algoritmo genético sobre el que opera.
\param TipoAdaptacion Indica cuál tipo de adaptación para la probabilidad de mutación se va a implementar. Por defecto se implementa la  ADAPTACION_PROBMUTACION_EXPONENCIAL.*/
OperadorAdaptacionProbMutacion::OperadorAdaptacionProbMutacion(AlgoritmoGenetico *pAG, int TipoAdaptacion)
{
	if(TipoAdaptacion!=ADAPTACION_PROBMUTACION_OFFLINE && TipoAdaptacion!=ADAPTACION_PROBMUTACION_EXPONENCIAL)
		TipoAdaptacion=ADAPTACION_PROBMUTACION_EXPONENCIAL;
	m_tipoAdaptacion=TipoAdaptacion;
	//establecer los parametros por defecto
	m_T = pAG->m_GeneracionMaxima/2 + 1;
	setParamsOffline();
	setParamsExponencial();
}

/*!
\param MaxProb Valor máximo de probabilidad de mutación para asignar a un gen, se restringe al intervalo [0, 1]. Por defecto es igual a 0.5.
\param FactorVariacion Valor que determina el factor de variación permitido para la medida offline. Por defecto es igual a 0.001.
\param Escalon Valor en que se aumenta la probabilidad de mutación a un gen en cada variación. Por defecto es igual a 0.1.
\param MaxCont Número máximo de generaciones que pueden pasar sin que varíe la probabilidad de mutación cuando la medida offline no varía en un factor mayor a \a m_factorVariacion. Por defecto es igual a 10.*/
void OperadorAdaptacionProbMutacion::setParamsOffline(double MaxProb, double FactorVariacion, double Escalon, int MaxCont)
{
	m_maxProb = restringir(MaxProb, 0.0, 1.0);
	m_factorVariacion = FactorVariacion;
	m_escalon = Escalon;
	m_maxCont = MaxCont;
}

/*!
\param MaxProb Valor máximo de probabilidad de mutación para asignar a un gen, se restringe al intervalo [0, 1]. Por defecto es igual a 0.5.
\param T Valor positivo determina la atenuación de la curva exponencial.*/
void OperadorAdaptacionProbMutacion::setParamsExponencial(double MaxProb, double T)
{
	m_maxProb = restringir(MaxProb, 0.0, 1.0);
	if(T>0) m_T = T;
}

//Método para efectuar el proceso de adaptacion para la probabilidad de mutación 
void OperadorAdaptacionProbMutacion::adaptacion(AlgoritmoGenetico *pAG)
{
	switch(m_tipoAdaptacion)
	{
	case ADAPTACION_PROBMUTACION_OFFLINE:
		{
			static int contador=0;
			double dif = fabs(pAG->m_MedidaOffLine - pAG->m_MedidaOffLineAnterior);
			if (dif <= fabs(pAG->m_MedidaOffLine)*m_factorVariacion)
				contador++;
			else
				contador=0;
			if(contador >= m_maxCont)
			{
				int tam = pAG->m_pListaOperadorMutacion->getSize();
				for(int i=0;i<tam;i++)
				{
					double nuevaProb = pAG->m_pListaOperadorMutacion->getObj(i).ObtenerProbabilidadMutacion() + m_escalon;
					if(nuevaProb > m_maxProb) nuevaProb=m_maxProb;
					pAG->m_pListaOperadorMutacion->getObj(i).AsignarProbabilidadMutacion( nuevaProb );
				}
				contador=0; 
			}
		}
		break;

	case ADAPTACION_PROBMUTACION_EXPONENCIAL:	
		{
			double NuevaProb = m_maxProb * exp(  -(double)pAG->m_Generacion / m_T  );
			int tam=pAG->m_pListaOperadorMutacion->getSize();
			for(int i=0;i<tam;i++)
			{			
				//(*pAG->m_pListaOperadorMutacion)[i]->AsignarProbabilidadMutacion(NuevaProb);
				pAG->m_pListaOperadorMutacion->getObj(i).AsignarProbabilidadMutacion(NuevaProb);
			}
		}
		break;
	}
}



/*!Inicializa los tamaños inicial y final de la población para el algoritmo
\param pAG Apuntador al algoritmo genético sobre el que opera.
\param NumIndivInicio Número inicial de individuos en la población, debe ser positivo. Por defecto es igual al número de individuos del algoritmo genético.
\param NumIndivFin Número final de individuos en la población, Por defecto es igual a la cuarta parte del número incial de individuos del algoritmo genético.*/
OperadorAdaptacionNumIndividuos::OperadorAdaptacionNumIndividuos(AlgoritmoGenetico *pAG, int NumIndivInicio, int NumIndivFin)
{	
	if(NumIndivInicio <= 0) NumIndivInicio = pAG->m_TamanoPoblacion;
	m_nIndivInicio = NumIndivInicio;

	if(NumIndivFin <= 0) NumIndivFin = m_nIndivInicio/4 + 1;
	else if(NumIndivFin > m_nIndivInicio) NumIndivFin = m_nIndivInicio;
	m_nIndivFin = NumIndivFin;
}


//Método para efectuar el proceso de adaptacion
void OperadorAdaptacionNumIndividuos::adaptacion(AlgoritmoGenetico* pAG)
{	
	//Varía LINEALMENTE el número de individuos entre m_nIndivInicio y m_nIndicFin
	int NuevoTamPob = (int)(m_nIndivInicio - ((m_nIndivInicio - m_nIndivFin)/(double)(pAG->m_GeneracionMaxima))* pAG->m_Generacion);
	if(NuevoTamPob < pAG->m_TamanoPoblacion)
	{	
		pAG->m_pPoblacionActual->setTam(NuevoTamPob);
		pAG->m_TamanoPoblacion = NuevoTamPob;
	}
}



/*!\param Pob Referencia a la población sobre la que opera 
\param Maximizar Su valor sirve como referencia para determinar qué individuo es mejor a otro dependiendo de su función de evaluación.*/
void OperadorProbabilidadProporcional::asignarProbabilidad(Poblacion& Pob, bool Maximizar)
{
	int i, tam = Pob.getTam();	
	if(tam<1) return;	//no hay individuos para asignar probabilidad
	
	//encontrar minima funcion objetivo de la poblacion o 1.0
	double *pObj = new double[tam];
	double min = 1.0;

	for(i=0;i<tam;i++)
	{
		pObj[i] = Pob.getIndividuo(i).objetivo();
		if( pObj[i] < min)
		{
			min = pObj[i];
		}
	}

	// Se suma (1-min) a todoas las funciones objetivo. Así aseguramos que todos sean positivos y >= 1
	for(i=0;i<tam;i++)
	{
		pObj[i] += (1.0 - min);
#if defined(UN_DEBUG)
		wxASSERT( pObj[i] >= 1.0 );
#endif
	}

	//se encuentra el denominador para asignar la probabilidad	
	double denominador=0.0;
	for(i=0;i<tam;i++)
	{
		if(Maximizar)
		{
			denominador += pObj[i];
		}
		else
		{
			denominador += 1.0/pObj[i]; //estamos seguros de no tener división por cero.
		}
	}
#if defined(UN_DEBUG)
	wxASSERT(denominador>0.0);
#endif

	//se asigna la probabilidad de supervivencia a cada individuo
	double probabilidad;
	for(i=0;i<tam;i++)
	{
		if(Maximizar)
		{
			probabilidad = pObj[i]/denominador;			
		}
		else
		{
			probabilidad = (1.0/pObj[i])/denominador;
		}		
		Pob.getIndividuo(i).asignarProbabilidad(probabilidad);
	}	
	delete [] pObj;
}


/*!\param Pob Referencia a la población sobre la que opera 
\param Maximizar Su valor sirve como referencia para determinar qué individuo es mejor a otro dependiendo de su función de evaluación.*/
void OperadorProbabilidadLineal::asignarProbabilidad(Poblacion& Pob, bool Maximizar)
{
	Pob.ordenar(Maximizar);
	
	int tam = Pob.getTam();	
	double nmax = 2-m_nmin;
	
	for(int i=0;i<tam;i++)
	{
		double probabilidad = (nmax-((nmax-m_nmin)*((i+1)/double(tam+1))))/double(tam);		
		Pob.getIndividuo(i).asignarProbabilidad(probabilidad);
	}
}


/*!\param Pob Referencia a la población sobre la que opera 
\param Maximizar No es utilizado. Existe por compatibilidad*/
void OperadorProbabilidadHomogenea::asignarProbabilidad(Poblacion &Pob, bool Maximizar)
{
	int tam = Pob.getTam();
	for(int i=0;i<tam;i++)
	{
		Pob.getIndividuo(i).asignarProbabilidad(1.0/((double)tam));
	}
}

/*!\param pPob Referencia a la población sobre la que opera.*/
void OperadorSeleccionEstocasticaRemplazo::seleccionar(Poblacion& pPob)
{
	int i, j, tam = pPob.getTam();
	
	//asignar angulo a cada individuo
	double suma=0.0;
	double *angulo=new double[tam];
	for(i=0;i<tam;i++)
	{
		suma += pPob.getIndividuo(i).getProbabilidad();
		angulo[i]=suma;
	}

#if defined(UN_DEBUG)
	wxASSERT(suma-1 >= -1e-10  &&  suma-1<=1e-10);
#endif

	//se crea la nueva poblacion y se "juega a la ruleta", introduciendo cada individuo seleccionado.
	double azar;	
	Poblacion *pNuevaPob = new Poblacion(pPob);

	for(i=0;i<tam;i++)
	{		
		azar = suma*((double)rand()/((double)RAND_MAX+1.0)); //se multiplica por suma. este puede ser diferente de 1.0
#if defined(UN_DEBUG)
		wxASSERT( azar < suma); //azar no debe ser igual a suma
#endif

		for(j=0;j<tam;j++)
		{
			if(azar < angulo[j])
			{				
				Individuo *ind = new Individuo(pNuevaPob->getIndividuo(j));
				ind = pPob.remplazarIndividuo(ind, i);
				delete ind;
				j=tam;
			}
		}
	}		
	delete [] angulo;
	delete pNuevaPob;
}


/*!\param Pob Referencia a la población sobre la que opera.*/
void OperadorParejasAleatorias::asignarParejas(Poblacion& Pob)
{
	int i, pareja, tam = Pob.getTam();
	
	//eliminar parejas actuales
	for(i=0;i<tam;i++)
	{
		Pob.getIndividuo(i).asignarPareja(NULL); 
	}	
	
	double azar;
	for(i=0;i<tam-1;i++)
	{
		if( Pob.getIndividuo(i).getPareja()==NULL)
		{
			do
			{
				azar = (double)rand()/((double)RAND_MAX+1.0);
				pareja = (int)( i+1.0 + azar*(tam-i-1.0) );
#if defined(UN_DEBUG)
				wxASSERT( pareja < tam && pareja > i);
#endif
			}while(Pob.getIndividuo(pareja).getPareja()!=NULL );
			Pob.getIndividuo(i).asignarPareja( &Pob.getIndividuo(pareja) );
			Pob.getIndividuo(pareja).asignarPareja( &Pob.getIndividuo(i) );
		}
	}
}


/*!\param Pob Referencia a la población sobre la que opera.*/
void OperadorParejasAdyacentes::asignarParejas(Poblacion& Pob)
{
	int i, tam = Pob.getTam();

	//eliminar parejas actuales
	for(i=0;i<tam;i++)
	{
		Pob.getIndividuo(i).asignarPareja(NULL);
	}
	
	for(i=0;i<tam-1;i=i+2)
	{
		Pob.getIndividuo(i).asignarPareja( &Pob.getIndividuo(i+1) );
		Pob.getIndividuo(i+1).asignarPareja( &Pob.getIndividuo(i) );
	}
}



/*!\param Pob Referencia a la población sobre la que opera.*/
void OperadorParejasExtremos::asignarParejas(Poblacion& Pob)
{
	int i, tam = Pob.getTam();

	//eliminar parejas actuales
	for(i=0;i<tam;i++)
	{
		Pob.getIndividuo(i).asignarPareja(NULL);
	}

	for(i=0; i < tam/2; i++)
	{
		Pob.getIndividuo(i).asignarPareja( &Pob.getIndividuo(tam-1-i) );
		Pob.getIndividuo(tam-1-i).asignarPareja( &Pob.getIndividuo(i) );
	}
}




#pragma warning(push, 3)	//Para evitar  "warning(level 4) C4100" ocasionada por parámetros de funciones no referenciados



/*!\param Pob Referencia a la población sobre la que opera
\param Maximizar No se utiliza. Existe por compatibilidad*/
void OperadorReproduccionCruceSimple::reproducir(Poblacion& Pob, bool Maximizar)
{
	int i,j,puntoCruce, tamPob = Pob.getTam();
	AlgoritmoGenetico *pAG = Pob.GetAG();
	int tamGenoma = pAG->m_pModelo->getTamGenoma();
	double azar;
	Individuo *pPadre,*pMadre;
	for(i=0;i<tamPob;i++)
	{
		if( Pob.getIndividuo(i).getPareja()!=NULL)
		{
			pPadre = &Pob.getIndividuo(i);
			pMadre = Pob.getIndividuo(i).getPareja();
			azar = (double)rand()/((double)RAND_MAX+1.0);
			puntoCruce = (int)(azar*tamGenoma);
			for(j=puntoCruce;j<tamGenoma;j++)
			{
				Gen *pGen = pMadre->getGen(j).crearCopia();
				pGen = pPadre->remplazarGen( pGen, j );
				pGen = pMadre->remplazarGen( pGen, j );
				delete pGen;
			}
			pPadre->asignarPareja(NULL);
			pMadre->asignarPareja(NULL);	
		}
	}
}
#pragma warning(pop)

/*!\param Pob Referencia a la población sobre la que opera
\param Maximizar Su valor sirve como referencia para determinar qué individuo es mejor a otro dependiendo de su función de evaluación.*/
void OperadorReproduccionDosPadresDosHijos::reproducir(Poblacion& Pob, bool Maximizar)
{
	AlgoritmoGenetico *pAG = Pob.GetAG();
	int i, j, tamGenoma = pAG->m_pModelo->getTamGenoma();
	int tamPob = Pob.getTam();
	Individuo *pMejor,*pPeor,*pInd;
	OperadorCruce *OC;

	bool condicion;	
	for(i=0;i<tamPob;i++)
	{
		pInd = &Pob.getIndividuo(i);
		if(pInd->getPareja())
		{
			condicion= pInd->objetivo() > pInd->getPareja()->objetivo();
			if((Maximizar && condicion) || (!Maximizar && !condicion))
			{
				pMejor=pInd;
				pPeor=pInd->getPareja();
			}
			else
			{
				pMejor=pInd->getPareja();
				pPeor=pInd;
			}						
			for(j=0;j<tamGenoma;j++)
			{
				Arreglo<Gen>* Hijos= new Arreglo<Gen>;
				OC = &pAG->m_pListaOperadorCruce->getObj(j);			
				OC->cruzarGenes( &pMejor->getGen(j) , &pPeor->getGen(j), Hijos, 2, i);
				
				Gen *temp = pMejor->remplazarGen( &Hijos->getObj(0) , j);
				delete temp;
				temp = pPeor->remplazarGen( &Hijos->getObj(1) , j);
				delete temp;
				Hijos->FlushDetach();
				delete Hijos;
			}
			pMejor->asignarPareja(NULL);
			pPeor->asignarPareja(NULL);
		}		
	}
}

/*!\param Pob Referencia a la población sobre la que opera
\param Maximizar Su valor sirve como referencia para determinar qué individuo es mejor a otro dependiendo de su función de evaluación.*/
void OperadorReproduccionMejorPadreMejorHijo::reproducir(Poblacion& Pob, bool Maximizar)
{
	AlgoritmoGenetico *pAG = Pob.GetAG();
	int i, j, tamGenoma = pAG->m_pModelo->getTamGenoma();
	int tamPob = Pob.getTam();
	Individuo *pMejor, *pPeor, *pHijo1, *pHijo2,*pInd;
	OperadorCruce *OC;
	bool condicion;
	for(i=0;i<tamPob;i++)
	{
		pInd = &Pob.getIndividuo(i);
		if(pInd->getPareja())
		{
			condicion= pInd->objetivo() > pInd->getPareja()->objetivo();
			if((Maximizar && condicion) || (!Maximizar && !condicion))
			{
				pMejor = pInd;
				pPeor = pInd->getPareja();
			}
			else
			{
				pMejor = pInd->getPareja();
				pPeor = pInd;
			}
			pHijo1 = new Individuo(*pMejor);
			pHijo2 = new Individuo(*pMejor);
			for(j=0;j<tamGenoma;j++)
			{
				Arreglo<Gen>* Hijos= new Arreglo<Gen>;
				OC = &pAG->m_pListaOperadorCruce->getObj(j);
				OC->cruzarGenes( &pMejor->getGen(j) , &pPeor->getGen(j), Hijos, 2, i);
				
				Gen *temp = pHijo1->remplazarGen( &Hijos->getObj(0), j);
				delete temp;
				temp = pHijo2->remplazarGen( &Hijos->getObj(1), j);
				delete temp;
				Hijos->FlushDetach(); //antes de eliminar se deben desligar los elementos
				delete Hijos;
			}
			if( (Maximizar &&  pHijo1->objetivo() > pHijo2->objetivo())  ||  (!Maximizar && pHijo1->objetivo() < pHijo2->objetivo()) )
				*pPeor = *pHijo1;
			else
				*pPeor = *pHijo2;

			pPeor->asignarPareja(NULL);
			delete pHijo2;
			delete pHijo1;			
			pMejor->asignarPareja(NULL);			
		}
	}
}


/*!\param Pob Referencia a la población sobre la que opera
\param Maximizar Su valor sirve como referencia para determinar qué individuo es mejor a otro dependiendo de su función de evaluación.*/
void OperadorReproduccionMejoresEntrePadresEHijos::reproducir(Poblacion& Pob, bool Maximizar)
{
	AlgoritmoGenetico *pAG = Pob.GetAG();
	int i, j, tamGenoma = pAG->m_pModelo->getTamGenoma();
	int tamPob = Pob.getTam();
	Individuo *pMejor, *pPeor, *pHijo1, *pHijo2, *pInd;
	OperadorCruce *OC;
	bool condicion;
	for(i=0;i<tamPob;i++)
	{
		pInd = &Pob.getIndividuo(i);
		if(pInd->getPareja())
		{
			condicion= (pInd->objetivo() > pInd->getPareja()->objetivo());			
			if((Maximizar && condicion) || (!Maximizar && !condicion))
			{
				pMejor = new Individuo(*pInd);
				pPeor = new Individuo(*pInd->getPareja());
			}
			else
			{
				pMejor = new Individuo(*pInd->getPareja());
				pPeor = new Individuo(*pInd);
			}
			pHijo1 = new Individuo(*pMejor);
			pHijo2 = new Individuo(*pMejor);
			for(j=0;j<tamGenoma;j++)
			{
				Arreglo<Gen>* Hijos = new Arreglo<Gen>;
				OC = &pAG->m_pListaOperadorCruce->getObj(j);		
				OC->cruzarGenes( &pMejor->getGen(j) , &pPeor->getGen(j) , Hijos, 2, i);
				
				Gen *pTemp = pHijo1->remplazarGen( &Hijos->getObj(0), j);
				delete pTemp;
				pTemp = pHijo2->remplazarGen( &Hijos->getObj(1), j);
				delete pTemp;
				Hijos->FlushDetach();
				delete Hijos;
			}
			Individuo *pIndiv[4]={pMejor,pPeor,pHijo1,pHijo2};
			double obj[4];
			for(j=0;j<4;j++)
			{
				if(Maximizar)
					obj[j]=pIndiv[j]->objetivo();
				else
					obj[j]=-(pIndiv[j]->objetivo());
			}
			//obtener posiciones de los dos mejores entre padres e hijos
			int p1=0, p2=1;
			for(j=1;j<4;j++)
			{
				if(obj[j]>obj[p1])
				{
					p2=p1;
					p1=j;
				}
				else if(obj[j]>obj[p2])
				{
					p2=j;
				}
			}

#if defined(UN_DEBUG)
			if(Maximizar)
				wxASSERT( pIndiv[p1]->objetivo() >= pIndiv[p2]->objetivo());
			else
				wxASSERT( pIndiv[p1]->objetivo() <= pIndiv[p2]->objetivo());
#endif

			*(pInd->getPareja()) = *(pIndiv[p2]);
			pInd->getPareja()->asignarPareja(NULL);
			*pInd = *(pIndiv[p1]);
			pInd->asignarPareja(NULL);
			for(j=0;j<4;j++)
			{				
				delete pIndiv[j];
			}		
		}
	}
}

 
#endif		//__GENETICO_CPP
