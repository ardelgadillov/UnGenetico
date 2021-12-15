#ifndef __GENREAL_H
#define __GENREAL_H

#include"genetico.h"

/************************************************************
                      GEN REAL (double)
************************************************************/

//!Clase derivada de la clase Gen especializada en un gen de tipo real
/*!Se utiliza para representar y almacenar la informaci�n gen�tica de variables
cuya codificaci�n se realiza con n�meros reales.*/
class GenReal:public Gen
{
public:
	//!Constructor por copia
	/*!Construye el nuevo objeto copiando id�nticamente las propiedades 
	de otro objeto de la clase GenReal.
	\param origen Objeto del que se hace copia.*/
	GenReal(const GenReal& origen)
	{
		operator=(origen);
	}

    //!Constructor por defecto
    GenReal(double Min=0.0, double Max=1.0);

    //!Constructor con valor inicial
    GenReal(double Min, double Max, double ValorInicial);

	//!Crea una copia exacta del gen
	/*!
	\return Apuntador a un nuevo objeto GenReal id�ntico al actual.*/
	Gen *crearCopia() const 
	{
		return (new GenReal(*this));
	}

	//!Asigna nuevos valores al gen copiando las propiedades de otro gen
	/*!
	\param origen Objeto del que se hace copia*/
	void copiar(const Gen& origen)
	{
		operator=(*((const GenReal*)(&origen))); 
	}

    //!Operador de asignaci�n a partir de un dato real.
	/*!Asigna al gen el valor de un dato de tipo real. 
	\param nuevoValor N�mero real que se asigna al gen
	\return Referencia al gen.*/
    const GenReal& operator=(const double& nuevoValor)
	{
		setVal(nuevoValor);
		return *this;
	}
	
	//!Conversi�n impl�cita a double
	/*!Convierte un objeto GenReal en un dato de tipo double 
	utilizando su valor actual. Permite hacer operaciones del estilo \f$(double)gen1+(double)gen2\f$.
	\return Valor actual del gen.*/
    operator const double() const 
	{
		return m_Valor; 
	}

	//!Destructor
    ~GenReal(){}

	//!Ajusta el valor actual del dato almacenado por el gen. Retorna el valor asignado.
	/*!
	\param nuevoValor Valor a asignar al gen. Es restringido al rango [m_Minimo, m_Maximo].
	\return Valor asignado.*/
	double setVal(double nuevoValor)
	{	
		return m_Valor = restringir(nuevoValor, m_Minimo, m_Maximo);
	}


	//!Retorna el valor actual del dato almacenado en el gen
	/*!
	\return Valor actual del gen */
	double getVal() const
	{
		return m_Valor;
	}
	
	//!Retorna el valor m�ximo que puede tomar el gen
	/*!
	\return Valor m�ximo permitido para el gen. */
	double getMax() const
	{
		return m_Maximo;
	}

	//!Retorna el valor m�nimo que puede tomar el gen
	/*!
	\return Valor m�nimo permitido para el gen. */
	double getMin() const
	{
		return m_Minimo;
	}

	
    //!Genera un valor aleatorio para el dato almacenado en el gen
	/*!Asigna al gen un nuevo valor real aleatorio en el rango establecido*/
    void generarAleatorio()
	{
		double azar = (double)rand()/(double)RAND_MAX;
		m_Valor = m_Minimo + azar * (m_Maximo - m_Minimo);	
	}


    //!Retorna un objeto correspondiente al operador de mutaci�n establecido por defecto para genes enteros
    inline OperadorMutacion *operadorMutacionDefecto() const;

    //!Retorna un objeto correspondiente al operador de cruce establecido por defecto para genes enteros
    inline OperadorCruce *operadorCruceDefecto() const;

protected:
	
	//!Almacena la Informaci�n del gen
    double m_Valor;

    //!M�nimo valor que puede tomar el gen. Solo puede modificarse en el constructor
    double m_Minimo;

    //!M�ximo valor que puede tomar el gen. Solo puede modificarse en el constructor
    double m_Maximo;
};


/******************************************************************
                     Operadores de Mutaci�n
******************************************************************/

//!Clase derivada de la clase OperadorMutacion empleada en genes reales
/*!Efect�a una mutaci�n uniforme sobre un gen de tipo real*/
class OperadorMutacionRealUniforme:public OperadorMutacion
{
public:
	//!Constructor
	/*!Inicializa el valor de la probabilidad de mutaci�n.
	\param ProbabilidadMutacion Establece el valor inicial para la probabilidad de mutaci�n propia del operador. Por defecto es 0.1.*/
    OperadorMutacionRealUniforme(double ProbabilidadMutacion=0.1):OperadorMutacion(ProbabilidadMutacion){}
	
	//!Destructor
    ~OperadorMutacionRealUniforme(){}
    //!Ejecuta una mutaci�n uniforme sobre un objeto de la clase GenReal
    void mutarGen(Gen *pGen);
};

//!Clase derivada de la clase OperadorMutacion empleada en genes reales
/*!Efect�a una mutaci�n no uniforme sobre un gen de tipo real*/
class OperadorMutacionRealNoUniforme:public OperadorMutacion
{
public:
	//!Constructor
	/*!Inicializa el valor de la probabilidad de mutaci�n y el valor de b.
	\param ProbabilidadMutacion Valor inicial para la probabilidad de mutaci�n propia del operador. Por defecto es 0.1
	\param b Valor que determina el grado de dependencia con el numero de generaciones. Por defecto es 0.5 */
    OperadorMutacionRealNoUniforme(AlgoritmoGenetico *pAG, double ProbabilidadMutacion=0.1, double b=0.5):OperadorMutacion(ProbabilidadMutacion)
    {
		m_pAG = pAG;
		setB(b);
    }
	
	//!Cambia el valor del par�metro \a b
	/*!\param b Valor a asignar al par�metro \a b.*/ 
	void setB(double b)
	{
		m_b = b;
	}
	//!Retorna el valor del par�metro \a b
	double getB()
	{
		return m_b;
	}
	//!Destructor
	~OperadorMutacionRealNoUniforme(){}
	
	//!Ejecuta una mutaci�n no uniforme sobre un objeto de la clase GenReal
	void mutarGen(Gen *g);

private:
	
	//!Par�metro que determina el grado de dependencia con el numero de generaciones. 
	double m_b;
	
	//!Apuntador a un objeto de la clase AlgoritmoGenetico
	AlgoritmoGenetico *m_pAG;
};

//!Clase derivada de la clase OperadorMutacion empleada en genes reales
/*!Efect�a una mutaci�n Muhlenbein sobre un gen de de tipo real*/
class OperadorMutacionRealMuhlenbein:public OperadorMutacion
{
public:
	//!Constructor
	/*!Inicializa el valor de la probabilidad de mutaci�n y el valor del factor que define el rango de mutaci�n.
	\param ProbabilidadMutacion Valor inicial para la probabilidad de mutaci�n propia del operador. Por defecto es 0.1.
	\param Factor Valor que define el rango de mutaci�n. Por defecto es 0.1.*/
	OperadorMutacionRealMuhlenbein(double ProbabilidadMutacion=0.1, double Factor=0.1):OperadorMutacion(ProbabilidadMutacion)
	{		
		setFactor(Factor);
	}	
	//!Cambia el valor del par�metro \a Factor
	/*!\param Factor Valor que define el rango de mutaci�n.*/
	void setFactor(double Factor)
	{
		m_Factor=Factor;
	}
	//!Retorna el valor del par�metro Factor
	double getFactor()
	{
		return m_Factor;
	}
	//!Destructor 
	~OperadorMutacionRealMuhlenbein(){}  
	
	//!Ejecuta una mutaci�n Muhlenbein sobre un objeto de la clase GenReal
	void mutarGen(Gen *pGen);

private:
	
	//!Valor que define el rango de mutaci�n. 
	double m_Factor;
};

/*******************************************************************
                        Operadores de Cruce
*******************************************************************/

//!Clase derivada de la clase OperadorCruce empleada en genes reales
/*!Efect�a un cruce plano entre dos genes de tipo real*/
class OperadorCruceRealPlano : public OperadorCruce
{
public:
	//!Constructor
    OperadorCruceRealPlano(){}
	//!Destructor
    ~OperadorCruceRealPlano(){}
	//!Ejecuta un cruce plano sobre genes reales
    void cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice=-1);
};

//!Clase derivada de la clase OperadorCruce empleada en genes reales
/*!Efect�a un cruce aritm�tico entre dos genes de tipo real*/
class OperadorCruceRealAritmetico:public OperadorCruce
{
public:
	/*!Inicializa el valor del par�metro \a Lambda.
	\param Lambda Valor que pondera cada uno de los genes padres. Por defecto es igual a 0.7.*/
	OperadorCruceRealAritmetico(double Lambda=0.7)
	{
		setLambda(Lambda);
	}
	//!Cambia el valor del par�metro \a Lambda comprobando los l�mites
	/*!\param Lambda Valor a asignar al par�metro \a Lambda.*/
	void setLambda(double Lambda)
	{
		m_Lambda = restringir(Lambda, 0.0, 1.0);
	}
	//!Retorna el valor del par�metro \a Lambda
	double getLambda()
	{
		return m_Lambda;
	}
	//!Destructor 
	~OperadorCruceRealAritmetico(){}
	//!Ejecuta un cruce aritm�tico sobre genes reales.
	void cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice=-1);
private:
	//!Par�metro que pondera cada uno de los genes padres.
	double m_Lambda;
};

//!Clase derivada de la clase OperadorCruce empleada en genes reales
/*!Efect�a un cruce BLX - \f$\alpha\f$ entre dos genes de tipo real*/
class OperadorCruceRealBLX:public OperadorCruce
{
public:
	//!Constructor
	/*!Inicializa el valor de \a Alfa
	\param Alfa Establece la amplitud del intervalo de definici�n. Por defecto es igual a 0.3.*/
	OperadorCruceRealBLX(double Alfa=0.3)
	{
		setAlfa(Alfa);
	}
	//!Cambia el valor del par�metro \a Alfa comprobando los l�mites
	/*!\param Alfa Valor a asignar al par�metro \a Alfa*/
	void setAlfa(double Alfa)
	{
		m_Alfa = restringir(Alfa, 0.0, 1.0);
	}
	//!Retorna el valor del par�metro \a Alfa
	double getAlfa()
	{
		return m_Alfa;
	}
	//!Destructor 
	~OperadorCruceRealBLX(){}  
	//!Ejecuta el cruce BLX - \f$\alpha\f$ sobre genes reales.
	void cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice=-1);
private:
	//!Establece la amplitud del intervalo de definici�n.
	double m_Alfa;
};

//!Clase derivada de la clase OperadorCruce empleada en genes reales
/*!Efect�a un cruce lineal entre dos genes de tipo real*/
class OperadorCruceRealLineal:public OperadorCruce
{
public:
	//!Constructor 
	OperadorCruceRealLineal(){}
	//!Destructor 
	~OperadorCruceRealLineal(){}
	//!Ejecuta un cruce lineal sobre genes reales
	void cruzarGenes(const Gen *madre, const Gen *padre, Arreglo<Gen> *hijos, int numHijos, int indice=-1);
};

//!Clase derivada de la clase OperadorCruce empleada en genes reales
/*!Efect�a un cruce discreto entre dos genes de tipo real*/
class OperadorCruceRealDiscreto:public OperadorCruce
{
public:
  //!Constructor 
  OperadorCruceRealDiscreto(){}
  //!Destructor 
  ~OperadorCruceRealDiscreto(){}
  //!Ejecuta un cruce discreto sobre genes reales
  void cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice=-1);
};

//!Clase derivada de la clase OperadorCruce empleada en genes reales
/*!Efect�a el cruce extendido intermedio entre dos genes de tipo real*/
class OperadorCruceRealIntermedioExtendido:public OperadorCruce
{
public:
	//!Constructor
	OperadorCruceRealIntermedioExtendido(){}
	//!Destructor
	~OperadorCruceRealIntermedioExtendido(){}
	//!Ejecuta un cruce intermedio extendido sobre genes reales
	void cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice=-1);
};

//!Clase derivada de la clase OperadorCruce empleada en genes reales
/*!Efect�a un cruce heur�stico entre dos genes de tipo real*/
class OperadorCruceRealHeuristico:public OperadorCruce
{
public:
	//!Constructor
	OperadorCruceRealHeuristico(){}
	//!Destructor
	~OperadorCruceRealHeuristico(){}
	//!Ejecuta un cruce heur�stico sobre genes reales
	void cruzarGenes(const Gen *pMejor, const Gen *pPeor, Arreglo<Gen> *pHijos, int numHijos, int indice=-1);
};


//!Clase derivada de la clase OperadorCruce empleada en genes reales
/*!Efect�a un cruce BGA linealentre genes de tipo real*/
class OperadorCruceRealLinealBGA:public OperadorCruce
{
public:
	//!Constructor
	OperadorCruceRealLinealBGA(AlgoritmoGenetico *pAG){m_pAG=pAG;}
	//!Destructor
	~OperadorCruceRealLinealBGA(){}
	//!Ejecuta el cruce BGA lineal sobre genes reales
	void cruzarGenes(const Gen *madre, const Gen *padre, Arreglo<Gen> *hijos, int numHijos, int indice);
private:
	//!Apuntador al objeto AlgoritmoGenetico en el que opera
	AlgoritmoGenetico *m_pAG;
};



/****************************************************************************/
/*		DEFINICION DE MACROS PARA USO DE GENREAL				
/****************************************************************************/

//!Macro que adiciona un gen de tipo real al individuo
/*Usa la macro __ADICIONAR_GEN con el prop�sito particular de insertar
un objeto GenReal en una posici�n fija del individuo apuntado por \a pIndividuo.
Este objeto estar� asociado con una de las variables del sistema a optimizar
de la cual se establecen sus valores m�nimo, m�ximo e inicial.

\param pIndividuo Apuntador al individuo donde se adiciona el gen.
\param pos Posici�n en la que se adiciona el gen.
\param var Nombre de la variable del sistema a optimizar asociada con el gen adicionado.
\param valMin Valor m�nimo que puede tomar el gen.
\param valMax Valor m�ximo que puede tomar el gen.
\param valInicial Valor inicial que toma el gen.*/
#define ADICIONAR_GENREAL(pIndividuo, pos, var, valMin, valMax, valInicial)	\
	__ADICIONAR_GEN(pIndividuo, pos, var, double, GenReal,(valMin, valMax, valInicial))


#endif	//__GENREAL_H